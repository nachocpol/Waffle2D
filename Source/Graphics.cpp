#include "Graphics.h"
#include "Window.h"
#include "FileUtils.h"
#include "MathUtils.h"
#include "Sprite.h"
#include "Image.h"
#include "Logging.h"

#include <cstdio>

#include "glad/glad.h"
#include "glad/glad_wgl.h"

#include <windows.h>

using namespace Waffle;

void GLAPIENTRY GLDebugCallback(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam)
{
	if (type == GL_DEBUG_TYPE_ERROR)
	{
		ERR("GL CALLBACK: message = %s\n", message);
	}
	else
	{
		WARN("GL CALLBACK: message = %s\n", message);
	}
}

// Utility method to create a shader from a source file.
static bool CreateShader(const char* path, GLenum type, unsigned int& shaderID)
{
	long shaderSize = 0;
	shaderID = 0;
	if (File::ReadFile(path, nullptr, shaderSize))
	{
		char* shaderSrc = (char*)malloc(shaderSize);
		bool succ = false;
		if (File::ReadFile(path, shaderSrc, shaderSize))
		{
			shaderSrc[shaderSize-1] = 0;
			shaderID = glCreateShader(type);
			glShaderSource(shaderID, 1, &shaderSrc, (const GLint*)&shaderSize);
			glCompileShader(shaderID);
			int result = 0;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
			if (!result)
			{
				char info[512];
				glGetShaderInfoLog(shaderID, 512, nullptr, info);
				ERR("Shader compilation failed! \n %s \n %s", path, info);
			}
			else
			{
				succ = true;
			}
		}
		free(shaderSrc);
		return succ;
	}
	return false;
}

Graphics::Graphics()
	: m_initialized(false)
	, m_window(nullptr)
	, m_windowClosed(false)
	, m_renderContext(nullptr)
	, m_spritePipeline(0)
	, m_viewportDirty(false)
	, m_whiteImage(nullptr)
	, m_view(0.0f, 0.0f)
{
}

Graphics::~Graphics()
{
}

Graphics& Graphics::Get()
{
	static Graphics* kGraphics = nullptr;
	if (!kGraphics)
	{
		kGraphics = new Graphics();
	}
	return *kGraphics;
}

unsigned int kDummyVAO = 0;

bool Graphics::Init()
{
	if (m_initialized)
	{
		return true;
	}

	// Init window:
	m_window = new Window;
	if (!m_window->Init("Waffle2D"))
	{
		return false;
	}

	// Init dummy GL context:
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,		// Flags
		PFD_TYPE_RGBA,													// The kind of framebuffer. RGBA or palette.
		32,																// Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,																// Number of bits for the depthbuffer
		8,																// Number of bits for the stencilbuffer
		0,																// Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	HDC handleToDevice = GetDC((HWND)m_window->GetHandle());
	int pfId = ChoosePixelFormat(handleToDevice, &pfd);
	if (!SetPixelFormat(handleToDevice, pfId, &pfd))
	{
		return false;
	}

	HGLRC dummyContext = wglCreateContext(handleToDevice);
	if (!wglMakeCurrent(handleToDevice, dummyContext))
	{
		return false;
	}

	gladLoadGL();
	gladLoadWGL(handleToDevice);

	int contextAttribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB,4,
		WGL_CONTEXT_MINOR_VERSION_ARB,5,
		WGL_CONTEXT_FLAGS_ARB , WGL_CONTEXT_DEBUG_BIT_ARB,
		0,
	};
	m_renderContext = wglCreateContextAttribsARB(handleToDevice, 0, contextAttribs);
	if (!m_renderContext)
	{
		ERR("Failed to create the opengl context using wglCreateContextAttribsARB \n ");
		return false;
	}
	
	if (!wglDeleteContext(dummyContext))
	{
		ERR("Failed to delete dummy context \n");
	}

	wglMakeCurrent(handleToDevice, (HGLRC)m_renderContext);

	// Setup debugging, should be enabled by default as we are already passing debug flag during context creation:
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GLDebugCallback, nullptr);

	INFO("OpenGl version: %s\n", glGetString(GL_VERSION));
	INFO(" Vendor: %s\n", glGetString(GL_VENDOR));
	INFO(" Renderer name: %s\n", glGetString(GL_RENDERER));
	INFO(" GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	m_width = m_window->GetWidth();
	m_height = m_window->GetHeight();
	INFO("Render size: %ix%i\n", m_width, m_height);

	if (!InitResources())
	{
		return false;
	}

	// Generic state:
	glGenVertexArrays(1, &kDummyVAO);
	glBindVertexArray(kDummyVAO);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_STENCIL_TEST);
	glViewport(0, 0, m_width, m_height);

	// Blending:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	return true;
}

void Graphics::OnResize(int w, int h)
{
	if (w != m_width || h != m_height)
	{
		m_width = w;
		m_height = h;
		m_viewportDirty = true;
		INFO("Resizing to: %ix%i\n", m_width, m_height);
	}
}

bool Graphics::Closed() const
{
	return m_windowClosed;
}

void Waffle::Graphics::BeginFrame()
{
	m_windowClosed = !m_window->Update();
}

void Graphics::Flush()
{
	HDC deviceHandle = GetDC((HWND)m_window->GetHandle());
	SwapBuffers(deviceHandle);
}

void Graphics::ClearScreen(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::DrawSprite(Sprite* sprite)
{
	if (!sprite)
	{
		return;
	}

	if (m_viewportDirty)
	{
		glViewport(0, 0, m_width, m_height);
	}

	// Simple orthographic projection:
	float w = (float)m_width;
	float h = (float)m_height;
	Vec2 projection = Vec2(1.0f / (w * 0.5f), 1.0f / (h * 0.5f));

	// Sprite transformation:
	const Vec2 size = sprite->GetSize();
	Transform transform = sprite->GetTransform();

	// Apply view (this could be done on the shader....):
	if (!sprite->GetIsUI())
	{
		transform.Position.X -= m_view.X;
		transform.Position.Y -= m_view.Y;
	}

	// Apply sprite size:
	transform.Scale.X *= size.X;
	transform.Scale.Y *= size.Y;

	Mat3 worldTransform = transform.AsMatrix();

	glBindVertexArray(m_spriteMesh.ID);
	glUseProgram(m_spritePipeline);
	{
		// Transforms:
		glUniform2fv(glGetUniformLocation(m_spritePipeline, "uProjection"), 1, &projection.X);
		glUniformMatrix3fv(glGetUniformLocation(m_spritePipeline, "uTransform"), 1, GL_FALSE, worldTransform.Data[0]);

		// Tint:
		Color tint = sprite->GetTint();
		glUniform4fv(glGetUniformLocation(m_spritePipeline, "uTint"), 1, &tint.R);

		// Scale and bias
		Vec2 imgScale = sprite->GetImageScale();
		Vec2 imgBias = sprite->GetImageBias();
		float scaleBias[4] = { imgScale.X, imgScale.Y, imgBias.X, imgBias.Y };
		glUniform4fv(glGetUniformLocation(m_spritePipeline, "uScaleBias"), 1, scaleBias);

		// Bind image:
		glActiveTexture(GL_TEXTURE0);
		const Image* img = sprite->GetImage();
		if (img)
		{
			glBindTexture(GL_TEXTURE_2D, img->m_imageID);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, m_whiteImage->m_imageID);
		}
		glUniform1i(glGetUniformLocation(m_spritePipeline, "uImage"), 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glBindVertexArray(kDummyVAO); // Unbind sprite VAO to avoid messing it.
}

void Graphics::SetView(Vec2 view)
{
	m_view = view;
}

Vec2 Graphics::GetView() const
{
	return m_view;
}

Vec2 Graphics::GetCurViewport()
{
	return Vec2((float)m_width, (float)m_height);
}

bool Graphics::InitResources()
{
	// Setup shaders:
	m_spritePipeline = glCreateProgram();
	unsigned int spriteVtxShader = 0;
	if (!CreateShader("shaders:Sprite.vert", GL_VERTEX_SHADER, spriteVtxShader))
	{
		return false;
	}
	unsigned int spriteFragShader = 0;
	if (!CreateShader("shaders:Sprite.frag", GL_FRAGMENT_SHADER, spriteFragShader))
	{
		return false;
	}

	glAttachShader(m_spritePipeline, spriteVtxShader);
	glAttachShader(m_spritePipeline, spriteFragShader);
	glLinkProgram(m_spritePipeline);
	int res = 0;
	glGetProgramiv(m_spritePipeline, GL_LINK_STATUS, &res);
	if (!res)
	{
		char info[512];
		glGetProgramInfoLog(m_spritePipeline, 512, nullptr, info);
		ERR("Failed to link the program! \n %s", info);
	}
	glDeleteShader(spriteFragShader);
	glDeleteShader(spriteVtxShader);

	if (!res)
	{
		return false;
	}

	// Quad VAO
	struct Vertex
	{
		Vertex() 
		{}
		Vertex(Vec2 p, Vec2 tc)
			: Position(p)
			, TexCoord(tc)
		{}
		Vec2 Position;
		Vec2 TexCoord;
	}verts[6];

	float sz = 0.5f;
	verts[0] = { { sz, sz}, {1.0f, 1.0f} };
	verts[1] = { { sz,-sz}, {1.0f, 0.0f} };
	verts[2] = { {-sz,-sz}, {0.0f, 0.0f} };

	verts[3] = { {-sz, sz}, {0.0f, 1.0f} };
	verts[4] = { { sz, sz}, {1.0f, 1.0f} };
	verts[5] = { {-sz,-sz}, {0.0f, 0.0f} };

	glGenVertexArrays(1, &m_spriteMesh.ID);
	glBindVertexArray(m_spriteMesh.ID);
	{
		int vtxSize = sizeof(Vertex);
		glGenBuffers(1, &m_spriteMesh.VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_spriteMesh.VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, vtxSize * 6, verts, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vtxSize, (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vtxSize, (void*)(sizeof(Vec2)));
		glEnableVertexAttribArray(1);

		//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, vtxSize, (void*)(sizeof(Vec2) * 2));
		//glEnableVertexAttribArray(2);
	}
	glBindVertexArray(kDummyVAO);

	m_whiteImage = Image::CreateFromFile("data:Img/DefaultWhite.png");
	if (!m_whiteImage)
	{
		return false;
	}

	return true;
}

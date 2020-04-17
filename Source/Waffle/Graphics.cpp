#include "Graphics.h"
#include "Window.h"
#include "FileUtils.h"
#include "MathUtils.h"
#include "Sprite.h"
#include "Image.h"

#include <cstdio>

#include "glad/glad.h"

#include <windows.h>

using namespace Waffle;

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
				printf("Shader compilation failed! \n %s \n %s", path, info);
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

	// Init GL context:
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
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

	m_renderContext = wglCreateContext(handleToDevice);
	if (!wglMakeCurrent(handleToDevice, (HGLRC)m_renderContext))
	{
		return false;
	}

	gladLoadGL();
	printf("OpenGl version:%s\n", glGetString(GL_VERSION));

	m_width = m_window->GetWidth();
	m_height = m_window->GetHeight();
	printf("Render size: %ix%i\n", m_width, m_height);

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

	return true;
}

void Graphics::OnResize(int w, int h)
{
	if (w != m_width || h != m_height)
	{
		m_width = w;
		m_height = h;
		m_viewportDirty = true;
		printf("Resizing to: %ix%i\n", m_width, m_height);
	}
}

bool Graphics::Closed() const
{
	return m_windowClosed;
}

void Graphics::Flush()
{
	m_windowClosed = !m_window->Update();
	SwapBuffers(GetDC((HWND)m_window->GetHandle()));
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

bool Graphics::InitResources()
{
	// Setup shaders:
	m_spritePipeline = glCreateProgram();
	unsigned int spriteVtxShader = 0;
	if (!CreateShader("../../Source/Waffle/Shaders/Sprite.vert", GL_VERTEX_SHADER, spriteVtxShader))
	{
		return false;
	}
	unsigned int spriteFragShader = 0;
	if (!CreateShader("../../Source/Waffle/Shaders/Sprite.frag", GL_FRAGMENT_SHADER, spriteFragShader))
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
		printf("Failed to link the program! \n %s", info);
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
		Vec2 Position;
		Vec2 TexCoord;
		Color Color0;
	}verts[6];

	float sz = 0.5f;
	verts[0] = { { sz, sz}, {1.0f, 1.0f}, {1.0f,1.0f,1.0f,1.0f} };
	verts[1] = { { sz,-sz}, {1.0f, 0.0f}, {1.0f,1.0f,1.0f,1.0f} };
	verts[2] = { {-sz,-sz}, {0.0f, 0.0f}, {1.0f,1.0f,1.0f,1.0f} };

	verts[3] = { {-sz, sz}, {0.0f, 1.0f}, {1.0f,1.0f,1.0f,1.0f} };
	verts[4] = { { sz, sz}, {1.0f, 1.0f}, {1.0f,1.0f,1.0f,1.0f} };
	verts[5] = { {-sz,-sz}, {0.0f, 0.0f}, {1.0f,1.0f,1.0f,1.0f} };

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
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, vtxSize, (void*)(sizeof(Vec2) * 2));
		glEnableVertexAttribArray(2);
	}
	glBindVertexArray(kDummyVAO);

	m_whiteImage = Image::CreateFromFile("../../Data/Waffle/Img/DefaultWhite.png");
	if (!m_whiteImage)
	{
		return false;
	}

	return true;
}

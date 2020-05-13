#include "Graphics/Graphics.h"
#include "Graphics/MathUtils.h"
#include "Graphics/Sprite.h"
#include "Graphics/Image.h"
#include "Graphics/Font.h"
#include "FileUtils.h"
#include "Window.h"
#include "Logging.h"
#include "Time.h"

#include <cstdio>

#include "glad/glad.h"
#include "glad/glad_wgl.h"

#include <windows.h>

using namespace Waffle;

void GLAPIENTRY GLDebugCallback(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam)
{
	if (type == GL_DEBUG_TYPE_OTHER)
	{
		return;
	}
	if (type == GL_DEBUG_TYPE_ERROR)
	{
		ERR("GL CALLBACK: message = %s", message);
	}
	else
	{
		WARN("GL CALLBACK: message = %s", message);
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
	: m_Initialized(false)
	, m_Window(nullptr)
	, m_WindowClosed(false)
	, m_RenderContext(nullptr)
	, m_SpritePipeline(0)
	, m_ViewportDirty(false)
	, m_WhiteImage(nullptr)
	, m_View(0.0f, 0.0f)
	, m_RenderScale(1.0f)
	, m_BatchManager(this)
{
}

Waffle::Graphics::Graphics(const Graphics& other)
	: m_BatchManager(nullptr)
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
	if (m_Initialized)
	{
		return true;
	}

	// Init window:
	m_Window = new Window;
	if (!m_Window->Init("Waffle2D"))
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

	HDC handleToDevice = GetDC((HWND)m_Window->GetHandle());
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

	// Check if we can use the context extension:
	if (GLAD_WGL_ARB_create_context)
	{
		int contextAttribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB,4,
			WGL_CONTEXT_MINOR_VERSION_ARB,5,
			WGL_CONTEXT_FLAGS_ARB , WGL_CONTEXT_DEBUG_BIT_ARB,
			0,
		};

		m_RenderContext = wglCreateContextAttribsARB(handleToDevice, 0, contextAttribs);
		if (!m_RenderContext)
		{
			ERR("Failed to create the opengl context using wglCreateContextAttribsARB \n ");
			return false;
		}
		if (!wglDeleteContext(dummyContext))
		{
			ERR("Failed to delete dummy context \n");
		}

		wglMakeCurrent(handleToDevice, (HGLRC)m_RenderContext);
	
		// Setup debugging, should be enabled by default as we are already passing debug flag during context creation:
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(GLDebugCallback, nullptr);
	}
	else
	{
		INFO("The WGL_ARB_create_context extension is not availiable. ");
		m_RenderContext = dummyContext;
	}
	
	INFO("OpenGl version: %s", glGetString(GL_VERSION));
	INFO(" Vendor: %s", glGetString(GL_VENDOR));
	INFO(" Renderer name: %s", glGetString(GL_RENDERER));
	INFO(" GLSL version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	m_Width = m_Window->GetWidth();
	m_Height = m_Window->GetHeight();
	INFO("Render size: %ix%i", m_Width, m_Height);

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
	glViewport(0, 0, m_Width, m_Height);

	// Blending:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	return true;
}

void Graphics::OnResize(int w, int h)
{
	if (w != m_Width || h != m_Height)
	{
		m_Width = w;
		m_Height = h;
		m_ViewportDirty = true;
		INFO("Resizing to: %ix%i", m_Width, m_Height);
	}
}

bool Graphics::Closed() const
{
	return m_WindowClosed;
}

void Waffle::Graphics::BeginFrame()
{
	m_WindowClosed = !m_Window->Update();
}

void Graphics::Flush()
{
	m_BatchManager.Flush();
	m_BatchManager.SwapBuffer();

	HDC deviceHandle = GetDC((HWND)m_Window->GetHandle());
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

	if (m_ViewportDirty)
	{
		glViewport(0, 0, m_Width, m_Height);
	}

	// Simple orthographic projection:
	float w = (float)m_Width  * m_RenderScale; 
	float h = (float)m_Height * m_RenderScale;
	Vec2 projection = Vec2(1.0f / (w * 0.5f), 1.0f / (h * 0.5f));

	// Sprite transformation:
	const Vec2 size = sprite->GetSize();
	Transform transform = sprite->GetTransform();

	// Apply view (this could be done on the shader....):
	if (!sprite->GetIsUI())
	{
		transform.Position.X -= m_View.X;
		transform.Position.Y -= m_View.Y;
	}

	// Apply sprite size:
	transform.Scale.X *= size.X;
	transform.Scale.Y *= size.Y;

	Mat3 worldTransform = transform.AsMatrix();

	DrawCallInfo dc;
	dc.ImageID = sprite->GetImage() ? sprite->GetImage()->m_ImageID : 0;
	Vec2 scale = sprite->GetImageScale();
	Vec2 bias   = sprite->GetImageBias();
	dc.ImageScaleBias[0] = scale.X;
	dc.ImageScaleBias[1] = scale.Y;
	dc.ImageScaleBias[2] = bias.X;
	dc.ImageScaleBias[3] = bias.Y;
	dc.Tint = sprite->GetTint();
	dc.WorldTransform = worldTransform;
	dc.Projection = projection;
	dc.IsFont = false;
		
	m_BatchManager.Push(dc);
}

void Graphics::DrawTextString(const char* text, Font* font, Vec2 position, Color color)
{
	int numChars = (int)strlen(text);

	const Image* fontImage = font->GetFontImage();
	float fontWidth = (float)fontImage->GetWidth();
	float fontHeight = (float)fontImage->GetHeight();

	// Simple orthographic projection:
	float w = (float)m_Width  * m_RenderScale;
	float h = (float)m_Height * m_RenderScale;
	Vec2 projection = Vec2(1.0f / (w * 0.5f), 1.0f / (h * 0.5f));

	DrawCallInfo dc;
	dc.ImageID = fontImage->m_ImageID;
	dc.Projection = projection;
	dc.IsFont = true;
	dc.Tint = color;

	Transform transform;
	transform.Position = position;

	float curX = 0.0f;

	for(int charIdx = 0; charIdx < numChars;++charIdx)
	{
		const CharacterData& charData = font->GetCharacterData(text[charIdx]);

		transform.Scale = Vec2(charData.X1 - charData.X0, charData.Y1 - charData.Y0);

		// Scale bias:
		dc.ImageScaleBias[0] = (transform.Scale.X) / fontWidth;
		dc.ImageScaleBias[1] = (transform.Scale.Y) / fontHeight;
		dc.ImageScaleBias[2] = charData.X0 / fontWidth;
		dc.ImageScaleBias[3] = charData.Y0 / fontHeight;
		
		transform.Scale.Y *= -1.0f; // Ug... flip it.

		Mat3 worldMtx = transform.AsMatrix();
		dc.WorldTransform = worldMtx;
		
		SubmitDrawCall(dc);

		transform.Position.X += charData.XAdvance;
	}
}

void Graphics::SetView(Vec2 view)
{
	m_View = view;
}

Vec2 Graphics::GetView() const
{
	return m_View;
}

Vec2 Graphics::GetCurViewport()
{
	return Vec2((float)m_Width, (float)m_Height);
}

void Graphics::SetRenderScale(float scale)
{
	m_RenderScale = scale;
}

float Graphics::GetRenderScale() const
{
	return m_RenderScale;
}

bool Graphics::InitResources()
{
	// Setup pipelines:
	m_SpritePipeline = CreatePipeline("shaders:Sprite.vert", "shaders:Sprite.frag");
	if (!m_SpritePipeline)
	{
		return false;
	}

	m_SpriteInstancedPipeline = CreatePipeline("shaders:SpriteInstanced.vert", "shaders:SpriteInstanced.frag");
	if (!m_SpriteInstancedPipeline)
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

	glGenVertexArrays(1, &m_SpriteMesh.ID);
	glBindVertexArray(m_SpriteMesh.ID);
	{
		int vtxSize = sizeof(Vertex);
		glGenBuffers(1, &m_SpriteMesh.VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_SpriteMesh.VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, vtxSize * 6, verts, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vtxSize, (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vtxSize, (void*)(sizeof(Vec2)));
		glEnableVertexAttribArray(1);
	}
	glBindVertexArray(kDummyVAO);

	m_WhiteImage = Image::CreateFromFile("data:Img/DefaultWhite.png");
	if (!m_WhiteImage)
	{
		return false;
	}

	return true;
}

void Graphics::SubmitDrawCall(const DrawCallInfo& drawCall)
{
	glBindVertexArray(m_SpriteMesh.ID);
	glUseProgram(m_SpritePipeline);
	{
		// Transforms:
		glUniform2fv(glGetUniformLocation(m_SpritePipeline, "uProjection"), 1, &drawCall.Projection.X);
		glUniformMatrix3fv(glGetUniformLocation(m_SpritePipeline, "uTransform"), 1, GL_FALSE, drawCall.WorldTransform.Data[0]);

		// Tint:
		glUniform4fv(glGetUniformLocation(m_SpritePipeline, "uTint"), 1, &drawCall.Tint.R);

		// Scale and bias
		glUniform4fv(glGetUniformLocation(m_SpritePipeline, "uScaleBias"), 1, drawCall.ImageScaleBias);

		// Font
		glUniform1i(glGetUniformLocation(m_SpritePipeline, "uFontRender"), drawCall.IsFont ? 1 : 0);

		// Bind image:
		glActiveTexture(GL_TEXTURE0);
		if (drawCall.ImageID != 0)
		{
			glBindTexture(GL_TEXTURE_2D, drawCall.ImageID);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, m_WhiteImage->m_ImageID);
		}
		glUniform1i(glGetUniformLocation(m_SpritePipeline, "uImage"), 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glBindVertexArray(kDummyVAO); // Unbind sprite VAO to avoid messing it.
}

unsigned int Graphics::CreatePipeline(const char* vs, const char* ps)
{
	unsigned int pipeline = glCreateProgram();

	unsigned int vertexShader = 0;
	if (!CreateShader(vs, GL_VERTEX_SHADER, vertexShader))
	{
		return 0;
	}
	unsigned int pixelShader = 0;
	if (!CreateShader(ps, GL_FRAGMENT_SHADER, pixelShader))
	{
		return 0;
	}

	glAttachShader(pipeline, vertexShader);
	glAttachShader(pipeline, pixelShader);

	glBindAttribLocation(pipeline, 0, "aPosition");
	glBindAttribLocation(pipeline, 1, "aTexCoord");

	glLinkProgram(pipeline);
	int res = 0;
	glGetProgramiv(pipeline, GL_LINK_STATUS, &res);
	if (!res)
	{
		char info[512];
		glGetProgramInfoLog(pipeline, 512, nullptr, info);
		ERR("Failed to link the program! \n %s", info);
	}
	glDeleteShader(pixelShader);
	glDeleteShader(vertexShader);

	if (!res)
	{
		return 0;
	}

	return pipeline;
}

Graphics::BatchManager::BatchManager(Graphics* graphics)
	: m_Graphics(graphics)
	, m_BatchIndex(0)
	, m_ItemCount(0)
	, m_BufferIndex(0)
{
	m_Batch.resize(DrawsPerBatch);
	m_BatchBuffers[0] = nullptr;
	m_BatchBuffers[1] = nullptr;
}

void Graphics::BatchManager::Push(const DrawCallInfo& dc)
{
	// Check if we reached max number of batches:
	if (m_BatchIndex >= (MaxBatches - 1))
	{
		ERR("Ran out of  batches for this frame");
		m_Graphics->SubmitDrawCall(dc);
		return;
	}

	// Pass through:
	if (!Enabled)
	{
		m_Graphics->SubmitDrawCall(dc);
		return;
	}

	// Check if we need to flush:
	bool flushed = false;
	if (m_ItemCount != 0)
	{
		if (!((dc.ImageID == m_CurItem.ImageID) && (dc.IsFont == m_CurItem.IsFont) && (dc.Projection == m_CurItem.Projection)))
		{
			flushed = true;
			Flush();
		}
	}
	
	// Cache current item:
	if(flushed || m_ItemCount == 0)
	{
		m_CurItem = dc;
	}

	// Batch it:
	memcpy(m_Batch[m_ItemCount].Transform[0], &dc.WorldTransform.Row0.X, sizeof(float) * 3);
	memcpy(m_Batch[m_ItemCount].Transform[1], &dc.WorldTransform.Row1.X, sizeof(float) * 3);
	memcpy(m_Batch[m_ItemCount].Transform[2], &dc.WorldTransform.Row2.X, sizeof(float) * 3);
	memcpy(m_Batch[m_ItemCount].ScaleBias, dc.ImageScaleBias, sizeof(float) * 4);
	memcpy(m_Batch[m_ItemCount].Tint, &dc.Tint.R, sizeof(float) * 4);
	++m_ItemCount;
	
	if (m_ItemCount >= DrawsPerBatch)
	{
		// Also flush if reached max draws for the batch:
		Flush();
	}
}

void Graphics::BatchManager::Flush()
{
	// Nothing to do...
	if (m_ItemCount <= 0)
	{
		return;
	}

	if (!m_BatchBuffers[0])
	{
		Timer setupBatch;
		setupBatch.Start();

		m_BatchBuffers[0] = new unsigned int[MaxBatches];
		m_BatchBuffers[1] = new unsigned int[MaxBatches];

		glGenBuffers(MaxBatches, m_BatchBuffers[0]);
		glGenBuffers(MaxBatches, m_BatchBuffers[1]);

		for (int idx = 0; idx < 2; ++idx)
		{
			for (int b = 0; b < MaxBatches; ++b)
			{
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BatchBuffers[idx][b]);
				glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PerBatchData) * DrawsPerBatch, NULL, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			}
		}

		INFO("Creating all the batch resources took:%fms", setupBatch.Stop());
		size_t totalMem = 2 * MaxBatches * DrawsPerBatch * sizeof(PerBatchData);
		INFO("Total GPU mem for batch buffers:%i", totalMem);
	}

	// Copy data into batch GPU buffer:
	// NOTE: not sure wtf is OpenGL doing, the 1st map of the frame takes 13ms? Is that like doing one sync every frame, and thats it?
	// as it stands, should be doing any wait sync.
	// https://www.gamedev.net/forums/topic/666461-map-buffer-range-super-slow/
	// https://www.khronos.org/opengl/wiki/GLAPI/glMapBufferRange
	// https://www.khronos.org/opengl/wiki/Buffer_Object_Streaming
	//Timer flushTimer;
	//flushTimer.Start();
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BatchBuffers[m_BufferIndex][m_BatchIndex]);
		PerBatchData* pData = (PerBatchData*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(PerBatchData) * m_ItemCount, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
		memcpy(pData, m_Batch.data(), sizeof(PerBatchData) * m_ItemCount);
   		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}
	//INFO("Flush took:%f", flushTimer.Stop());

	// Submit draw call:
	{
		Vec2 projection = m_CurItem.Projection;
		int font = m_CurItem.IsFont ? 1 : 0;
		int imgID = m_CurItem.ImageID;

		glBindVertexArray(m_Graphics->m_SpriteMesh.ID);
		glUseProgram(m_Graphics->m_SpriteInstancedPipeline);
		{
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_BatchBuffers[m_BufferIndex][m_BatchIndex]);

			// Transforms:
			glUniform2fv(glGetUniformLocation(m_Graphics->m_SpriteInstancedPipeline, "uProjection"), 1, &projection.X);

			// Font
			glUniform1i(glGetUniformLocation(m_Graphics->m_SpriteInstancedPipeline, "uFontRender"), font);

			// Bind image:
			glActiveTexture(GL_TEXTURE0);
			if (imgID != 0)
			{
				glBindTexture(GL_TEXTURE_2D, imgID);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, m_Graphics->m_WhiteImage->m_ImageID);
			}
			glUniform1i(glGetUniformLocation(m_Graphics->m_SpriteInstancedPipeline, "uImage"), 0);

			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_ItemCount);
		}
		glBindVertexArray(kDummyVAO); // Unbind sprite VAO to avoid messing it.
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // Unbind batch SSBO
	}

	m_ItemCount = 0;
	m_BatchIndex++; // Increase batch index
	m_CurItem = {}; // Reset item
}

void Graphics::BatchManager::SwapBuffer()
{
	m_BufferIndex = !m_BufferIndex;
	m_BatchIndex = 0;
}

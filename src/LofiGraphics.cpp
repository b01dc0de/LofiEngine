#include "LofiGraphics.h"
#include "Common.h"

#include "linmath.h"

namespace Lofi
{

	const vxcolor Vertices[] =
	{
		vxcolor{{-0.6f, -0.4f, 0.0f}, { 1.0f, 0.0f, 0.0f}},
		vxcolor{{ 0.6f, -0.4f, 0.0f}, { 0.0f, 1.0f, 0.0f}},
		vxcolor{{ 0.0f,  0.6f, 0.0f}, { 0.0f, 0.0f, 1.0f}},
	};

	GLchar* LoadShaderSource(const char* Filename)
	{
		GLchar* Result = nullptr;

		FILE* ShaderFile = nullptr;
		fopen_s(&ShaderFile, Filename, "rb");
		if (ShaderFile)
		{
			fseek(ShaderFile, 0, SEEK_END);
			size_t FileSize = ftell(ShaderFile);
			fseek(ShaderFile, 0, SEEK_SET);

			Result = new GLchar[FileSize + 1];
			fread(Result, sizeof(GLchar), FileSize, ShaderFile);
			Result[FileSize] = 0x00;
			fclose(ShaderFile);
		}

		return Result;
	}

	struct
	{
		GLuint vertex_buffer = 0;
		GLuint vertex_shader = 0;
		GLuint fragment_shader = 0;
		GLuint gfx_pipeline = 0;
		GLint mvp_location = 0;
		GLint vpos_location = 0;
		GLint vcol_location = 0;
		GLuint vertex_array = 0;
	} GraphicsState;

	void Graphics::Init()
	{
		glGenBuffers(1, &GraphicsState.vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, GraphicsState.vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

		GLchar* vshader_source = LoadShaderSource("src/glsl/vxcolor_v.glsl");
		GLchar* fshader_source = LoadShaderSource("src/glsl/vxcolor_f.glsl");

		GraphicsState.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(GraphicsState.vertex_shader, 1, &vshader_source, nullptr);
		glCompileShader(GraphicsState.vertex_shader);

		GraphicsState.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(GraphicsState.fragment_shader, 1, &fshader_source, nullptr);
		glCompileShader(GraphicsState.fragment_shader);

		delete[] vshader_source;
		delete[] fshader_source;

		GraphicsState.gfx_pipeline = glCreateProgram();
		glAttachShader(GraphicsState.gfx_pipeline, GraphicsState.vertex_shader);
		glAttachShader(GraphicsState.gfx_pipeline, GraphicsState.fragment_shader);
		glLinkProgram(GraphicsState.gfx_pipeline);

		GraphicsState.mvp_location = glGetUniformLocation(GraphicsState.gfx_pipeline, "MVP");
		GraphicsState.vpos_location = glGetAttribLocation(GraphicsState.gfx_pipeline, "vPos");
		GraphicsState.vcol_location = glGetAttribLocation(GraphicsState.gfx_pipeline, "vCol");

		glGenVertexArrays(1, &GraphicsState.vertex_array);
		glBindVertexArray(GraphicsState.vertex_array);
		glEnableVertexAttribArray(GraphicsState.vpos_location);
		glVertexAttribPointer(GraphicsState.vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(vxcolor), (void*)offsetof(vxcolor, pos));
		glEnableVertexAttribArray(GraphicsState.vcol_location);
		glVertexAttribPointer(GraphicsState.vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vxcolor), (void*)offsetof(vxcolor, col));
	}

	float GetAspectRatio(float Width, float Height)
	{
		float Result = 1.0f;
		if (Width > 0.0f && Height > 0.0f)
		{
			Result = Width / Height;
		}
		return Result;
	}

	void Graphics::Draw(GLFWwindow* InWindow)
	{
		if (!InWindow) { return; }

		int Width = 0.0f, Height = 0.0f;
		glfwGetFramebufferSize(InWindow, &Width, &Height);
		float AspectRatio = GetAspectRatio(Width, Height);

		glViewport(0, 0, Width, Height);
		glClear(GL_COLOR_BUFFER_BIT);

		mat4x4 m, p, mvp;
		mat4x4_identity(m);
		mat4x4_ortho(p, -AspectRatio, AspectRatio, -1.f, 1.f, 1.f, -1.f);
		mat4x4_mul(mvp, p, m);

		glUseProgram(GraphicsState.gfx_pipeline);
		glUniformMatrix4fv(GraphicsState.mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);
		glBindVertexArray(GraphicsState.vertex_array);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(InWindow);
	}

	void Graphics::Terminate()
	{

	}
}
#include "LofiGraphics.h"
#include "Common.h"

#include "HandmadeMath.h"

namespace Lofi
{
	const vxcolor TriangleVertices[] =
	{
		vxcolor{{-0.6f, -0.4f, 0.0f}, { 1.0f, 0.0f, 0.0f}},
		vxcolor{{ 0.6f, -0.4f, 0.0f}, { 0.0f, 1.0f, 0.0f}},
		vxcolor{{ 0.0f,  0.6f, 0.0f}, { 0.0f, 0.0f, 1.0f}},
	};

	constexpr float fCubeUnit = 0.25f;
	const v3f Color_LightGray{0.75f, 0.75f, 0.75f};
	const v3f Color_DarkGray{ 0.25f, 0.25f, 0.25f };
	const v3f Color_White{ 1.f, 1.f, 1.f };
	const vxcolor CubeVertices[] =
	{
		// Front
		vxcolor{{fCubeUnit, fCubeUnit, -fCubeUnit}, {1.0f, 0.0f, 0.0f}},
		vxcolor{{-fCubeUnit, fCubeUnit, -fCubeUnit}, {0.0f, 1.0f, 0.0f}},
		vxcolor{{fCubeUnit, -fCubeUnit, -fCubeUnit}, {0.0f, 0.0f, 1.0f}},
		vxcolor{{-fCubeUnit, -fCubeUnit, -fCubeUnit}, Color_White},
		// Back
		vxcolor{{fCubeUnit, fCubeUnit, fCubeUnit}, {0.0f, 1.0f, 1.0f}},
		vxcolor{{-fCubeUnit, fCubeUnit, fCubeUnit}, {1.0f, 0.0f, 1.0f}},
		vxcolor{{fCubeUnit, -fCubeUnit, fCubeUnit}, {1.0f, 1.0f, 0.0f}},
		vxcolor{{-fCubeUnit, -fCubeUnit, fCubeUnit}, Color_White},
	};

	/*
						   ^
						   |  ^
						   | /
						   |/
					<------|------X
						  /|
						 / |
						/  |
					   Z   Y
	*/

	/*
		All cube faces (front-facing):

		  Front:               Back:
				0-------1           5-------4
				|       |           |       |
				|       |           |       |
				2-------3           7-------6

		    Top:             Bottom:
				4-------5           6-------7
				|       |           |       |
				|       |           |       |
				0-------1           2-------3

		   Left:              Right:
				4-------0           1-------5
				|       |           |       |
				|       |           |       |
				6-------2           3-------7
	*/
	// DEV_NOTE: Assuming CCW is front-facing
	const GLuint CubeIndices[] =
	{
		// Front
		2, 1, 0,
		2, 3, 1,
		// Back
		7, 4, 5,
		7, 6, 4,
		// Top
		0, 5, 4,
		0, 1, 5,
		// Bottom
		2, 7, 6,
		2, 3, 7,
		// Left
		6, 0, 4,
		6, 2, 0,
		// Right
		3, 5, 1,
		3, 7, 5,
	};

	struct ShaderFileSource
	{
		GLchar* Contents = nullptr;

		operator GLchar* ()
		{
			return Contents;
		}
		GLchar** operator&()
		{
			return &Contents;
		}
		bool IsValid()
		{
			return nullptr != Contents;
		}
		ShaderFileSource(const char* Filename)
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

			if (Result) { Contents = Result; }
		}
		~ShaderFileSource()
		{
			if (Contents) { delete[] Contents; }
		}
	};

	struct
	{
		GLuint tri_vertex_buffer = 0;
		GLuint tri_vertex_array = 0;

		GLuint cube_vertex_buffer = 0;
		GLuint cube_vertex_array = 0;

		GLuint vertex_shader = 0;
		GLuint fragment_shader = 0;
		GLuint gfx_pipeline = 0;

		GLint mvp_location = 0;
		GLint vpos_location = 0;
		GLint vcol_location = 0;
	} GraphicsState;

	void Graphics::Init()
	{
		ShaderFileSource vshader_src{ "src/glsl/vxcolor_v.glsl" };
		ShaderFileSource fshader_src{ "src/glsl/vxcolor_f.glsl" };

		if (!(vshader_src.IsValid() && fshader_src.IsValid()))
		{
			return;
		}

		GraphicsState.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(GraphicsState.vertex_shader, 1, &vshader_src, nullptr);
		glCompileShader(GraphicsState.vertex_shader);

		GraphicsState.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(GraphicsState.fragment_shader, 1, &fshader_src, nullptr);
		glCompileShader(GraphicsState.fragment_shader);

		GraphicsState.gfx_pipeline = glCreateProgram();
		glAttachShader(GraphicsState.gfx_pipeline, GraphicsState.vertex_shader);
		glAttachShader(GraphicsState.gfx_pipeline, GraphicsState.fragment_shader);
		glLinkProgram(GraphicsState.gfx_pipeline);

		GraphicsState.mvp_location = glGetUniformLocation(GraphicsState.gfx_pipeline, "MVP");
		GraphicsState.vpos_location = glGetAttribLocation(GraphicsState.gfx_pipeline, "vPos");
		GraphicsState.vcol_location = glGetAttribLocation(GraphicsState.gfx_pipeline, "vCol");

		//glGenBuffers(1, &GraphicsState.tri_vertex_buffer);
		//glBindBuffer(GL_ARRAY_BUFFER, GraphicsState.tri_vertex_buffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);

		//glGenVertexArrays(1, &GraphicsState.tri_vertex_array);
		//glBindVertexArray(GraphicsState.tri_vertex_array);

		{ // Cube
			glGenBuffers(1, &GraphicsState.cube_vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, GraphicsState.cube_vertex_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

			glGenVertexArrays(1, &GraphicsState.cube_vertex_array);
			glBindVertexArray(GraphicsState.cube_vertex_array);
		}

		glEnableVertexAttribArray(GraphicsState.vpos_location);
		glVertexAttribPointer(GraphicsState.vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(vxcolor), (void*)offsetof(vxcolor, pos));
		glEnableVertexAttribArray(GraphicsState.vcol_location);
		glVertexAttribPointer(GraphicsState.vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vxcolor), (void*)offsetof(vxcolor, col));

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS); // default: GL_LESS
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// HMM_Mat4 HMM_Orthographic_RH_NO(float Left, float Right, float Bottom, float Top, float Near, float Far)
		HMM_Mat4 mvp_ortho = HMM_Orthographic_RH_NO(-AspectRatio, AspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
		// HMM_Mat4 HMM_LookAt_RH(HMM_Vec3 Eye, HMM_Vec3 Center, HMM_Vec3 Up)
		const HMM_Vec3 GlobalUp{ 0.f, 1.f, 0.f };
		const HMM_Vec3 Origin{ 0.f, 0.f, 0.f };
		HMM_Mat4 mvp_lookat = HMM_LookAt_RH(HMM_Vec3{0.15f, 0.15f, -0.5f}, Origin, GlobalUp);

		const GLfloat* mvp = (const GLfloat*)&mvp_lookat;
		static bool bUseOrtho = false;
		if (bUseOrtho) { mvp = (const GLfloat*)&mvp_ortho; }
		
		glUseProgram(GraphicsState.gfx_pipeline);
		glUniformMatrix4fv(GraphicsState.mvp_location, 1, GL_FALSE, mvp);

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
		if (bUseOrtho)
		{
			glBindVertexArray(GraphicsState.tri_vertex_array);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		else
		{
			glBindVertexArray(GraphicsState.cube_vertex_array);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, CubeIndices);
		}

		glfwSwapBuffers(InWindow);
	}

	void Graphics::Terminate()
	{
	}
}
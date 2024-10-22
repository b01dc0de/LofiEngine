#include "LofiGraphics.h"
#include "Common.h"

namespace Lofi
{
    const vxcolor TriangleVerts[] =
    {
        vxcolor{{-0.6f, -0.4f, 0.0f}, { 1.0f, 0.0f, 0.0f}},
        vxcolor{{ 0.6f, -0.4f, 0.0f}, { 0.0f, 1.0f, 0.0f}},
        vxcolor{{ 0.0f,  0.6f, 0.0f}, { 0.0f, 0.0f, 1.0f}},
    };

    constexpr float fCubeUnit = 0.5f;
    const v3f Color_LightGray{ 0.75f, 0.75f, 0.75f };
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

    /* REFERENCE: Right-handed axes
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

    /* REFERENCE: All cube faces (front-facing):

          Front:               Back:
                0-------1           5-------4
                |       |           |       |
                |       |           |       |
                2-------3           7-------6

            Top:             Bottom:
                4-------5           7-------6
                |       |           |       |
                |       |           |       |
                0-------1           3-------2

           Left:              Right:
                4-------0           1-------5
                |       |           |       |
                |       |           |       |
                6-------2           3-------7
    */
    // DEV_NOTE: CCW is front-facing
    const GLuint CubeInds[] =
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
        3, 6, 7,
        3, 2, 6,
        // Left
        6, 0, 4,
        6, 2, 0,
        // Right
        3, 5, 1,
        3, 7, 5,
    };

    const vxtex Reference_TexCubeVerts[] =
    {
        // Front
        vxtex{{fCubeUnit, fCubeUnit, -fCubeUnit}, {0.0f, 0.0f}},
        vxtex{{-fCubeUnit, fCubeUnit, -fCubeUnit}, {1.0f, 0.0f}},
        vxtex{{fCubeUnit, -fCubeUnit, -fCubeUnit}, {0.0f, 1.0f}},
        vxtex{{-fCubeUnit, -fCubeUnit, -fCubeUnit}, {1.0f, 1.0f}},
        // Back
        vxtex{{fCubeUnit, fCubeUnit, fCubeUnit}, {0.0f, 0.0f}},
        vxtex{{-fCubeUnit, fCubeUnit, fCubeUnit}, {1.0f, 0.0f}},
        vxtex{{fCubeUnit, -fCubeUnit, fCubeUnit}, {0.0f, 1.0f}},
        vxtex{{-fCubeUnit, -fCubeUnit, fCubeUnit}, {1.0f, 1.0f}},
    };

    // UV-textured cube
    const vxtex TexCubeVerts[] =
    {
        // Front
        vxtex{{fCubeUnit, fCubeUnit, -fCubeUnit}, {0.0f, 0.0f}},
        vxtex{{-fCubeUnit, fCubeUnit, -fCubeUnit}, {1.0f, 0.0f}},
        vxtex{{fCubeUnit, -fCubeUnit, -fCubeUnit}, {0.0f, 1.0f}},
        vxtex{{-fCubeUnit, -fCubeUnit, -fCubeUnit}, {1.0f, 1.0f}},
        // Back
        vxtex{{fCubeUnit, fCubeUnit, fCubeUnit}, {0.0f, 0.0f}},
        vxtex{{-fCubeUnit, fCubeUnit, fCubeUnit}, {1.0f, 0.0f}},
        vxtex{{fCubeUnit, -fCubeUnit, fCubeUnit}, {0.0f, 1.0f}},
        vxtex{{-fCubeUnit, -fCubeUnit, fCubeUnit}, {1.0f, 1.0f}},
        // Top
        vxtex{{fCubeUnit, fCubeUnit, fCubeUnit}, {0.0f, 0.0f}},
        vxtex{{-fCubeUnit, fCubeUnit, fCubeUnit}, {1.0f, 0.0f}},
        vxtex{{fCubeUnit, fCubeUnit, -fCubeUnit}, {0.0f, 1.0f}},
        vxtex{{-fCubeUnit, fCubeUnit, -fCubeUnit}, {1.0f, 1.0f}},
        // Bottom
        vxtex{{fCubeUnit, -fCubeUnit, -fCubeUnit}, {0.0f, 0.0f}},
        vxtex{{-fCubeUnit, -fCubeUnit, -fCubeUnit}, {1.0f, 0.0f}},
        vxtex{{fCubeUnit, -fCubeUnit, fCubeUnit}, {0.0f, 1.0f}},
        vxtex{{-fCubeUnit, -fCubeUnit, fCubeUnit}, {1.0f, 1.0f}},
        // Left
        vxtex{{fCubeUnit, fCubeUnit, -fCubeUnit}, {0.0f, 0.0f}},
        vxtex{{fCubeUnit, fCubeUnit, fCubeUnit}, {1.0f, 0.0f}},
        vxtex{{fCubeUnit, -fCubeUnit, -fCubeUnit}, {0.0f, 1.0f}},
        vxtex{{fCubeUnit, -fCubeUnit, fCubeUnit}, {1.0f, 1.0f}},
        // Right
        vxtex{{-fCubeUnit, fCubeUnit, -fCubeUnit}, {0.0f, 0.0f}},
        vxtex{{-fCubeUnit, fCubeUnit, fCubeUnit}, {1.0f, 0.0f}},
        vxtex{{-fCubeUnit, -fCubeUnit, -fCubeUnit}, {0.0f, 1.0f}},
        vxtex{{-fCubeUnit, -fCubeUnit, fCubeUnit}, {1.0f, 1.0f}},
    };
    /*
        0---1
        |   |
        2---3

        + Offset: (FaceN * 4)
    */
    const GLuint TexCubeInds[] =
    {
        // Front : 0
        2, 1, 0,
        2, 3, 1,
        // Back : 4
        7, 4, 5, //6, 5, 4,
        7, 6, 4, //6, 7, 5,
        // Top : 8
        10, 9, 8,
        10, 11, 9,
        // Bottom : 12
        14, 13, 12,
        14, 15, 13,
        // Left : 16
        18, 16, 17,
        18, 17, 19,
        // Right : 20
        22, 21, 20,
        22, 23, 21,
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

        GLuint vxcolor_vertex_shader = 0;
        GLuint vxcolor_fragment_shader = 0;
        GLuint vxcolor_gfx_pipeline = 0;
        GLint vxcolor_mvp_location = 0;
        GLint vxcolor_vpos_location = 0;
        GLint vxcolor_vcol_location = 0;

        GLuint texcube_vertex_buffer = 0;
        GLuint texcube_vertex_array = 0;

        GLuint vxtex_vshader = 0;
        GLuint vxtex_fshader = 0;
        GLuint vxtex_pipeline = 0;
        GLint vxtex_mvp_location = 0;
        GLint vxtex_vpos_location = 0;
        GLint vxtex_vuv_location = 0;

        GLuint test_texture = 0;
    } GraphicsState;

    struct
    {
        int Width = 0;
        int Height = 0;
        int nrChannels = 0;
    } ImageState;

    void Graphics::Init()
    {
        { // Init vertex buffers
            { // Triangle
                //glGenBuffers(1, &GraphicsState.tri_vertex_buffer);
                //glBindBuffer(GL_ARRAY_BUFFER, GraphicsState.tri_vertex_buffer);
                //glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);

                //glGenVertexArrays(1, &GraphicsState.tri_vertex_array);
                //glBindVertexArray(GraphicsState.tri_vertex_array);
            }

            { // Cube
                glGenBuffers(1, &GraphicsState.cube_vertex_buffer);
                glBindBuffer(GL_ARRAY_BUFFER, GraphicsState.cube_vertex_buffer);
                glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

                glGenVertexArrays(1, &GraphicsState.cube_vertex_array);
                glBindVertexArray(GraphicsState.cube_vertex_array);
            }
        }

        ShaderFileSource vxcolor_vshader_src{ "src/glsl/vxcolor_v.glsl" };
        ShaderFileSource vxcolor_fshader_src{ "src/glsl/vxcolor_f.glsl" };
        if (!(vxcolor_vshader_src.IsValid() && vxcolor_fshader_src.IsValid())) { return; }
        else
        {
            GraphicsState.vxcolor_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(GraphicsState.vxcolor_vertex_shader, 1, &vxcolor_vshader_src, nullptr);
            glCompileShader(GraphicsState.vxcolor_vertex_shader);

            GraphicsState.vxcolor_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(GraphicsState.vxcolor_fragment_shader, 1, &vxcolor_fshader_src, nullptr);
            glCompileShader(GraphicsState.vxcolor_fragment_shader);

            GraphicsState.vxcolor_gfx_pipeline = glCreateProgram();
            glAttachShader(GraphicsState.vxcolor_gfx_pipeline, GraphicsState.vxcolor_vertex_shader);
            glAttachShader(GraphicsState.vxcolor_gfx_pipeline, GraphicsState.vxcolor_fragment_shader);
            glLinkProgram(GraphicsState.vxcolor_gfx_pipeline);

            GraphicsState.vxcolor_mvp_location = glGetUniformLocation(GraphicsState.vxcolor_gfx_pipeline, "MVP");
            GraphicsState.vxcolor_vpos_location = glGetAttribLocation(GraphicsState.vxcolor_gfx_pipeline, "vPos");
            GraphicsState.vxcolor_vcol_location = glGetAttribLocation(GraphicsState.vxcolor_gfx_pipeline, "vCol");

            glEnableVertexAttribArray(GraphicsState.vxcolor_vpos_location);
            glVertexAttribPointer(GraphicsState.vxcolor_vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(vxcolor), (void*)offsetof(vxcolor, pos));
            glEnableVertexAttribArray(GraphicsState.vxcolor_vcol_location);
            glVertexAttribPointer(GraphicsState.vxcolor_vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vxcolor), (void*)offsetof(vxcolor, col));
        }

        { // TexCube
            glGenBuffers(1, &GraphicsState.texcube_vertex_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, GraphicsState.texcube_vertex_buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(TexCubeVerts), TexCubeVerts, GL_STATIC_DRAW);
            //glBufferData(GL_ARRAY_BUFFER, sizeof(Reference_TexCubeVerts), Reference_TexCubeVerts, GL_STATIC_DRAW);

            glGenVertexArrays(1, &GraphicsState.texcube_vertex_array);
            glBindVertexArray(GraphicsState.texcube_vertex_array);
        }

        ShaderFileSource vxtex_vshader_src{ "src/glsl/vxtex_v.glsl" };
        ShaderFileSource vxtex_fshader_src{ "src/glsl/vxtex_f.glsl" };
        if (!(vxtex_vshader_src.IsValid() && vxtex_fshader_src.IsValid())) { return; }
        else
        {
            GraphicsState.vxtex_vshader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(GraphicsState.vxtex_vshader, 1, &vxtex_vshader_src, nullptr);
            glCompileShader(GraphicsState.vxtex_vshader);

            GraphicsState.vxtex_fshader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(GraphicsState.vxtex_fshader, 1, &vxtex_fshader_src, nullptr);
            glCompileShader(GraphicsState.vxtex_fshader);

            GraphicsState.vxtex_pipeline = glCreateProgram();
            glAttachShader(GraphicsState.vxtex_pipeline, GraphicsState.vxtex_vshader);
            glAttachShader(GraphicsState.vxtex_pipeline, GraphicsState.vxtex_fshader);
            glLinkProgram(GraphicsState.vxtex_pipeline);

            GraphicsState.vxtex_mvp_location = glGetUniformLocation(GraphicsState.vxtex_pipeline, "MVP");
            GraphicsState.vxtex_vpos_location = glGetAttribLocation(GraphicsState.vxtex_pipeline, "vPos");
            GraphicsState.vxtex_vuv_location = glGetAttribLocation(GraphicsState.vxtex_pipeline, "vUV");

            glEnableVertexAttribArray(GraphicsState.vxtex_vpos_location);
            glVertexAttribPointer(GraphicsState.vxtex_vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(vxtex), (void*)offsetof(vxtex, pos));
            glEnableVertexAttribArray(GraphicsState.vxtex_vuv_location);
            glVertexAttribPointer(GraphicsState.vxtex_vuv_location, 2, GL_FLOAT, GL_FALSE, sizeof(vxtex), (void*)offsetof(vxtex, uv));
        }

        { // Load test_texture
            unsigned char* TestTextureData = stbi_load("assets/feels.jpg", &ImageState.Width, &ImageState.Height, &ImageState.nrChannels, 0);

            if (TestTextureData)
            {
                glGenTextures(1, &GraphicsState.test_texture);
                glBindTexture(GL_TEXTURE_2D, GraphicsState.test_texture);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImageState.Width, ImageState.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, TestTextureData);
                glGenerateMipmap(GL_TEXTURE_2D);

                stbi_image_free(TestTextureData);
            }
        }

        { // Global GL settings
            // Depth testing:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS); // default: GL_LESS

            // Face culling:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
        }
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
        const HMM_Vec3 CameraPos{ 0.15f, 0.15f, -0.15f };
        HMM_Mat4 mvp_lookat = HMM_LookAt_RH(CameraPos, Origin, GlobalUp);

        const GLfloat* mvp = (const GLfloat*)&mvp_lookat;
        static bool bUseOrtho = false;
        if (bUseOrtho) { mvp = (const GLfloat*)&mvp_ortho; }

        if (bUseOrtho)
        {
            glUseProgram(GraphicsState.vxcolor_gfx_pipeline);
            glUniformMatrix4fv(GraphicsState.vxcolor_mvp_location, 1, GL_FALSE, mvp);

            glBindVertexArray(GraphicsState.tri_vertex_array);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        else
        {
            const bool bUseTexCube = true;
            if (bUseTexCube)
            {
                glUseProgram(GraphicsState.vxtex_pipeline);
                glUniformMatrix4fv(GraphicsState.vxtex_mvp_location, 1, GL_FALSE, mvp);

                glBindVertexArray(GraphicsState.texcube_vertex_array);
                glBindTexture(GL_TEXTURE_2D, GraphicsState.test_texture);
                //glDrawElements(GL_TRIANGLES, ARRAY_SIZE(CubeInds), GL_UNSIGNED_INT, CubeInds);
                glDrawElements(GL_TRIANGLES, ARRAY_SIZE(TexCubeInds), GL_UNSIGNED_INT, TexCubeInds);
            }
            else
            {
                glUseProgram(GraphicsState.vxcolor_gfx_pipeline);
                glUniformMatrix4fv(GraphicsState.vxcolor_mvp_location, 1, GL_FALSE, mvp);

                glBindVertexArray(GraphicsState.cube_vertex_array);
                glDrawElements(GL_TRIANGLES, ARRAY_SIZE(CubeInds), GL_UNSIGNED_INT, CubeInds);
            }
        }

        glfwSwapBuffers(InWindow);
    }

    void Graphics::Terminate()
    {
    }
}
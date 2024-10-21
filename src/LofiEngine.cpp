#include "LofiEngine.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
//#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cstdio>

void HandleError(int ErrorNo, const char* ErrorDesc)
{
	printf("ERROR: %d, %s\n", ErrorNo, ErrorDesc);
}

void HandleKeyInput(GLFWwindow* InWindow, int InKey, int ScanCode, int Action, int Modifers)
{
	switch (InKey)
	{
		case GLFW_KEY_ESCAPE:
		{
			glfwSetWindowShouldClose(InWindow, GLFW_TRUE);
		} break;
		default:
		{ } break;
	}
}

static const int AppWidth = 640;
static const int AppHeight = 480;
static GLFWwindow* AppWindow = nullptr;

int MainGLFW() { return 0; }

int main()
{
	constexpr int SuccessRetval = 0;
	constexpr int ErrorRetval = -1;

	printf("LofiEngine -- Init\n");

	if (!glfwInit())
	{
		printf("glfwInit FAILED!\n");
		return ErrorRetval;
	}

	// glfwInit SUCCESS
	{
		glfwSetErrorCallback(HandleError);

		GLFWwindow* NewWindow = glfwCreateWindow(AppWidth, AppHeight, "LofiEngine", nullptr, nullptr);
		if (!NewWindow)
		{
			printf("glfwCreateWindow FAILED!\n");
			return ErrorRetval;
		}
		else
		{
			// Init
			{
				AppWindow = NewWindow;

				glfwSetKeyCallback(AppWindow, HandleKeyInput);

				glfwMakeContextCurrent(AppWindow);
				gladLoadGL(glfwGetProcAddress);
				glfwSwapInterval(1);
			}

			// MainLoop
			{
				bool bRunning = true;
				while (bRunning)
				{
					glClear(GL_COLOR_BUFFER_BIT);

					glfwSwapBuffers(AppWindow);
					glfwPollEvents();

					if (glfwWindowShouldClose(AppWindow))
					{
						bRunning = false;
					}
				}
			}

			// Terminate
			{
				glfwDestroyWindow(AppWindow);
			}
		}

		glfwTerminate();
	}

	return SuccessRetval;
}
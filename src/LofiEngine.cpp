#include "LofiEngine.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cstdio>

#define LOGF(...) printf(__VA_ARGS__)

namespace Lofi
{
	struct AppState
	{
		const int AppWidth = 640;
		const int AppHeight = 480;
		GLFWwindow* AppWindow = nullptr;
	};
	AppState GlobalState;

	void HandleError(int ErrorNo, const char* ErrorDesc)
	{
		LOGF("ERROR: %d, %s\n", ErrorNo, ErrorDesc);
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
			{} break;
		}
	}

	constexpr int SuccessRetval = 0;
	constexpr int ErrorRetval = -1;

	bool EngineInit()
	{
		LOGF("LofiEngine -- Init\n");

		if (!glfwInit()) { return false; }

		glfwSetErrorCallback(HandleError);

		GLFWwindow* NewWindow = glfwCreateWindow(GlobalState.AppWidth, GlobalState.AppHeight, "LofiEngine", nullptr, nullptr);
		if (!NewWindow) { LOGF("glfwCreateWindow FAILED!\n"); return false; }

		GlobalState.AppWindow = NewWindow;

		glfwSetKeyCallback(GlobalState.AppWindow, HandleKeyInput);

		glfwMakeContextCurrent(GlobalState.AppWindow);
		gladLoadGL(glfwGetProcAddress);
		glfwSwapInterval(1);

		return true;
	}

	bool EngineMainLoop()
	{
		bool bRunning = true;
		while (bRunning)
		{
			glClear(GL_COLOR_BUFFER_BIT);

			glfwSwapBuffers(GlobalState.AppWindow);
			glfwPollEvents();

			if (glfwWindowShouldClose(GlobalState.AppWindow))
			{
				bRunning = false;
			}
		}
		
		return true;
	}

	bool EngineTerminate()
	{
		if (GlobalState.AppWindow)
		{
			glfwDestroyWindow(GlobalState.AppWindow);
		}

		glfwTerminate();

		return true;
	}

	int Main(int argc, const char* argv[])
	{
		bool Result = true;
		Result &= EngineInit();
		Result &= EngineMainLoop();
		Result &= EngineTerminate();
		return Result ? SuccessRetval : ErrorRetval;
	}
}

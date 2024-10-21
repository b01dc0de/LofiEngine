#ifndef LOFIGRAPHICS_H
#define LOFIGRAPHICS_H

#include "Common.h"

namespace Lofi
{
	struct v2f
	{
		float x;
		float y;
	};

	struct v3f
	{
		float x;
		float y;
		float z;
	};

	struct v4f
	{
		float x;
		float y;
		float z;
		float w;
	};

	struct vxcolor
	{
		v3f pos;
		v3f col;
	};

	struct Graphics
	{
		static void Init();
		static void Draw(GLFWwindow* InWindow);
		static void Terminate();
	};
}

#endif // LOFIGRAPHICS_H
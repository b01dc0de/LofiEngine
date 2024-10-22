#ifndef LOFIGRAPHICS_H
#define LOFIGRAPHICS_H

#include "Common.h"

namespace Lofi
{
    using v2f = HMM_Vec2;
    using v3f = HMM_Vec3;
    using v4f = HMM_Vec4;
    using m4f = HMM_Mat4;

    struct vxcolor
    {
        v3f pos;
        v3f col;
    };

    struct vxtex
    {
        v3f pos;
        v2f uv;
    };

    struct Graphics
    {
        static void Init();
        static void Draw(GLFWwindow* InWindow);
        static void Terminate();
    };
}

#endif // LOFIGRAPHICS_H
#ifndef COMMON_H
#define COMMON_H

/*-----BEGIN LIBRARIES-----*/
// GLAD / GLFW
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
// STB
#include <stb/stb_image.h>
// Handmade Math
#include <HandmadeMath/HandmadeMath.h>
/*----- END  LIBRARIES-----*/
// Standard Library
#include <cstdio>

#define LOGF(...) printf(__VA_ARGS__)
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#endif // COMMON_H
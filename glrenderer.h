#pragma once

#include <Windows.h>
#include <gl\GL.h>
#include "RendererStructs.h"

extern unsigned char *video_Buffer;
extern unsigned int textureBytesPerPixel;
extern t_DisplayMode currentDisplayMode;
extern bool do3d;

void SetPixelFormatAndCreateContext(HWND hWnd);
void AllocatePixelBuffer(unsigned int width, unsigned int height, int bytesPerPixel);
void DrawPixelBuffer();
void VideoSwapBuffers();
void ResetVideo();

int AllocateTexture(t_AllocateTextureQuery t);
int AddTexture(t_AddTexture input);
int RemoveTexture(unsigned int* textureTag);
void Render3d(t_Render3dInput input);
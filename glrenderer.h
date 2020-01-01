#pragma once

#include <Windows.h>
#include <gl\GL.h>
#include "RendererStructs.h"

extern unsigned char *videoBuffer;
extern t_DisplayMode currentDisplayMode;
extern bool rendering3d;

void SetPixelFormatAndCreateContext(HWND hWnd);
void AllocatePixelBuffer(unsigned int width, unsigned int height, int bytesPerPixel);
void DrawPixelBuffer();
void VideoSwapBuffers();
void ResetVideo();

int AllocateTexture(t_AllocateTextureInOut t);
int AddTexture(t_AddTextureInput input);
int RemoveTexture(unsigned int* textureTag);
void Render3d(t_Render3dInput input);
void OpenRenderer();
void CloseRenderer();
#pragma once

#include <Windows.h>
#include <gl\GL.h>
#include "RendererStructs.h"

extern unsigned short *video_Buffer;
extern t_DisplayMode currentDisplayMode;

void SetPixelFormatAndCreateContext(HWND hWnd);
void AllocatePixelBuffer(unsigned int width, unsigned int height);
void DrawPixelBuffer();
void VideoSwapBuffers();
void ResetVideo();
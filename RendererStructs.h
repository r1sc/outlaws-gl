#pragma once

typedef struct
{
	unsigned int width;
	unsigned int height;
	unsigned int bpp;
	unsigned char unk[24];
} t_SetDisplayModeArgs;


typedef struct {
	int action;
	void *unk;
} t_RasterizerArgs;

#pragma pack(push, 1)
typedef struct  {
	unsigned int width;
	unsigned int height;
	unsigned int bpp;
	int unk1;
	char str[20];
} t_DisplayMode;
#pragma pack(pop)

typedef struct {
	unsigned int width;
	unsigned int height;
	unsigned int bpp;
} t_AllocateArgs;

#pragma pack(push, 1)
typedef struct
{
	void* pixelData;
	int dword4 = 1024;
	int height;
	int dwordC = 2;
	int dword10 = 0;
	int dword14 = 0;
	unsigned int bufferWidth;
	unsigned int bufferHeight;
} t_LockBufferArgs;
#pragma pack(pop)

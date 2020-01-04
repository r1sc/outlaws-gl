#include <stdio.h>
#include <Windows.h>

#include "settings.h"

unsigned int windowResolutionWidth = 640;
unsigned int windowResolutionHeight = 480;
bool fullscreen = false;
bool linearInterpolation = true;

void ReadSettings() {	
	FILE* f = fopen("lecogl.conf", "r");
	if (!f) {
		return;
	}

	while (1) {
		char c = fgetc(f);
		ungetc(c, f);
		if (c == ';')
			fscanf(f, "%*[^\n]\n"); // skip line
		else {
			char key[100] = "\0";
			int value;
			int result = fscanf(f, "%s %d\n", key, &value);
			if (result == EOF)
				break;
			else if (result < 2)
				break;
			else if(strcmp(key, "windowResolutionWidth") == 0) {
				windowResolutionWidth = value;
			}
			else if (strcmp(key, "windowResolutionHeight") == 0) {
				windowResolutionHeight = value;
			}
			else if (strcmp(key, "fullscreen") == 0) {
				fullscreen = value == 1;
			}
			else if (strcmp(key, "linearInterpolation") == 0) {
				linearInterpolation = value == 1;
			}
		}
	}

	fclose(f);
}
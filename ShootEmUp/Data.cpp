#include "Data.h"

int Clip_ValuesA[100][2],
	Clip_ValuesB[100][2];

void InitClipValues() {
	int i;

	i = 0;
	Clip_ValuesA[i][0] = 0; // X
	Clip_ValuesA[i][1] = 0; // Y
	Clip_ValuesB[i][0] = 90; // WIDTH
	Clip_ValuesB[i][1] = 90; // HEIGHT

	i = 1;
	Clip_ValuesA[i][0] = 94;
	Clip_ValuesA[i][1] = 0;
	Clip_ValuesB[i][0] = 53;
	Clip_ValuesB[i][1] = 72;

	i = 2;
	Clip_ValuesA[i][0] = 151;
	Clip_ValuesA[i][1] = 0;
	Clip_ValuesB[i][0] = 72;
	Clip_ValuesB[i][1] = 51;

	i = 3;
	Clip_ValuesA[i][0] = 236;
	Clip_ValuesA[i][1] = 0;
	Clip_ValuesB[i][0] = 139;
	Clip_ValuesB[i][1] = 149;
}

void SpriteData::Init(int w, int h, int rows, int columns, int total) {
	int x = 0;
	AnimationMax = total;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			Clips[i*rows+j] = new SDL_Rect();
			Clips[i*rows+j]->x = j*w;
			Clips[i*rows+j]->y = i*h;
			Clips[i*rows+j]->w = w;
			Clips[i*rows+j]->h = h;
			x++;
			if (x >= total)
				return;
		}
	}
}
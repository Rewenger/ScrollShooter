#include "Data.h"

int Clip_ValuesA[100][2],
	Clip_ValuesB[100][2];

void InitClipValues() {
	int i;

	i = 0;
	Clip_ValuesA[i][0] = 0;
	Clip_ValuesA[i][1] = 0;
	Clip_ValuesB[i][0] = 90;
	Clip_ValuesB[i][1] = 90;
}
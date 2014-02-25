#ifndef __LEADERBOARD_H__
#define __LEADERBOARD_H__

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include "Leaderboard.h"

class Leaderboard {
private:
		char *filename;
		int recordcount;
public:
		Leaderboard(char* name, int records);
		void Update();
		void Save();
		//----------------------------
		int StatValue[11][2];
		char* PlayerName[512];
};

#endif
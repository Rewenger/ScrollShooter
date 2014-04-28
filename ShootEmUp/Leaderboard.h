#ifndef __LEADERBOARD_H__
#define __LEADERBOARD_H__

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <exception>

class Leaderboard {
public:
		Leaderboard(char* name, int records);
		void Update();
		void Save();
		//----------------------------
		char *filename;
		int recordcount;
		int StatValue[11][2];
		char* PlayerName[512];
};

#endif
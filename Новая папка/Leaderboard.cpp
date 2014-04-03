#include "Leaderboard.h"

using namespace std;

Leaderboard::Leaderboard(char* name, int records) {
	filename = name;
	recordcount = records;
}

void Leaderboard::Update() {
	char retStr[512] = {0};
    char* defStr     = "NULL";

	char* secName = new char[50];
    char* keyName = new char[50];
	char* keyVal = new char[50];
    char* fileName = filename;
 
	secName  = "PlayerName";
	for (int i = 1; i <= recordcount; i++) {
		sprintf(keyVal, "%d", i);
		strcpy(keyName, "p");
		strcat(keyName, keyVal);
		GetPrivateProfileStringA(secName, keyName, defStr, retStr, 512, fileName);
		PlayerName[i-1] = new char[512];
		strcpy(PlayerName[i-1], retStr);
		//printf("name = %s \n", PlayerName[i-1]);
	}
	
	secName  = "Score1";
	for (int i = 1; i <= recordcount; i++) {
		sprintf(keyVal, "%d", i);
		strcpy(keyName, "v");
		strcat(keyName, keyVal);
		GetPrivateProfileStringA(secName, keyName, defStr, retStr, 512, fileName);
		StatValue[i-1][0] = atoi(retStr);
	}

	secName  = "Score2";
	for (int i = 1; i <= recordcount; i++) {
		sprintf(keyVal, "%d", i);
		strcpy(keyName, "v");
		strcat(keyName, keyVal);
		GetPrivateProfileStringA(secName, keyName, defStr, retStr, 512, fileName);
		StatValue[i-1][1] = atoi(retStr);
	}
}

void Leaderboard::Save() {
	char* secName = new char[50];
    char* keyName = new char[50];
	char* keyVal = new char[50];
	char* dataVal = new char[50];
    char* fileName = filename;

	secName  = "PlayerName";
	for (int i = 1; i <= recordcount; i++) {
		sprintf(keyVal, "%d", i);
		strcpy(keyName, "p");
		strcat(keyName, keyVal);
		WritePrivateProfileStringA(secName, keyName, PlayerName[i-1], fileName);
	}

	secName  = "Score1";
	for (int i = 1; i <= recordcount; i++) {
		sprintf(keyVal, "%d", i);
		strcpy(keyName, "v");
		strcat(keyName, keyVal);
		sprintf(dataVal, "%d", StatValue[i-1][0]);
		WritePrivateProfileStringA(secName, keyName, dataVal, fileName);
	}

	secName  = "Score2";
	for (int i = 1; i <= recordcount; i++) {
		sprintf(keyVal, "%d", i);
		strcpy(keyName, "v");
		strcat(keyName, keyVal);
		sprintf(dataVal, "%d", StatValue[i-1][1]);
		WritePrivateProfileStringA(secName, keyName, dataVal, fileName);
	}
}
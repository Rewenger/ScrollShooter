#include "Leaderboard.h"

using namespace std;

Leaderboard::Leaderboard(char* name, int records) {
	if (name == NULL)
		throw exception();
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
		sprintf_s(keyVal, sizeof(keyVal), "%d", i);
		strcpy_s(keyName, sizeof(keyName), "p");
		strcat_s(keyName, sizeof(keyName), keyVal);
		GetPrivateProfileStringA(secName, keyName, defStr, retStr, 512, fileName);
		PlayerName[i-1] = new char[513];
		strcpy_s(PlayerName[i-1], 15, retStr);
		//printf("name = %s \n", PlayerName[i-1]);
	}
	
	secName  = "Score1";
	for (int i = 1; i <= recordcount; i++) {
		sprintf_s(keyVal, sizeof(keyVal), "%d", i);
		strcpy_s(keyName, sizeof(keyName), "v");
		strcat_s(keyName, sizeof(keyName), keyVal);
		GetPrivateProfileStringA(secName, keyName, defStr, retStr, 512, fileName);
		StatValue[i-1][0] = atoi(retStr);
	}

	secName  = "Score2";
	for (int i = 1; i <= recordcount; i++) {
		sprintf_s(keyVal, sizeof(keyVal), "%d", i);
		strcpy_s(keyName, sizeof(keyName), "v");
		strcat_s(keyName, sizeof(keyName), keyVal);
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
		sprintf_s(keyVal, sizeof(keyVal), "%d", i);
		strcpy_s(keyName, sizeof(keyName), "p");
		strcat_s(keyName, sizeof(keyName), keyVal);
		WritePrivateProfileStringA(secName, keyName, PlayerName[i-1], fileName);
	}

	secName  = "Score1";
	for (int i = 1; i <= recordcount; i++) {
		sprintf_s(keyVal, sizeof(keyVal), "%d", i);
		strcpy_s(keyName, sizeof(keyName), "v");
		strcat_s(keyName, sizeof(keyName), keyVal);
		sprintf_s(dataVal, sizeof(dataVal), "%d", StatValue[i-1][0]);
		WritePrivateProfileStringA(secName, keyName, dataVal, fileName);
	}

	secName  = "Score2";
	for (int i = 1; i <= recordcount; i++) {
		sprintf_s(keyVal, sizeof(keyVal), "%d", i);
		strcpy_s(keyName, sizeof(keyName), "v");
		strcat_s(keyName, sizeof(keyName), keyVal);
		sprintf_s(dataVal, sizeof(dataVal), "%d", StatValue[i-1][1]);
		WritePrivateProfileStringA(secName, keyName, dataVal, fileName);
	}
}
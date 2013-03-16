#ifndef _GMMANAGER_H_
#define _GMMANAGER_H_

#include <windows.h>

struct GMFeaturesList {
	char Name[11];
	BYTE ConnectMessage;
	BYTE Item;
	BYTE ChangeZen;
	BYTE ChangePKLevel;
	BYTE GMoveALL;
	BYTE SendGG;
	BYTE Reloads;
	BYTE BanCommands;
};

class CGMManager {
public:
	void LoadConfigs();
	int GMListNumber(char * Name);

	GMFeaturesList GMFeatures[20];
	int GMCount;
};

extern CGMManager GMManager;

#endif
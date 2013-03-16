#ifndef NEWSSYSTEM_H
 #define NEWSSYSTEM_H

class CNewsSystem {
public:
	CNewsSystem();
	~CNewsSystem(void);
	void NewsLoad();
	void NewsCore();
	void ReloadConfigs();
	int IsEnable;
private:
	int News_Step;
	int News_Range;
	int News_ID;
	unsigned int News_Seconds;
	char NewsName[30][256];
	int NewsCount;

	//Configs
	int NewsTime;
	int NewsMultiplier;
};

extern CNewsSystem NewsSystem;

#endif //~NEWSSYSTEM_H
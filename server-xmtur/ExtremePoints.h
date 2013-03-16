#include "User.h"
struct CExtremePoints {

	CExtremePoints();
	~CExtremePoints();

	void LoadStepsList();
	void GetExtremePoints(int aIndex);
	void SaveExtremePoints(int aIndex);
	int AddPointsPerHour();
	BOOL AddPointsPerMonster(int aIndex, int MonsterID);

	int IsEnable;
	int AddPoints_Minutes;

	int StartHour[10];
	int EndHour[10];
	int Points[512];
	int DefaultPoints;
	int StepsCount;
	int Type;
	int Monsters[512];
};

void ExtremePoints_CheckCore(void * lpParam);

extern CExtremePoints ExtremePoints;
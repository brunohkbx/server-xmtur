#include "User.h"
#define MAX_ITEMS 100
#define MAX_BC_LEVEL 8 //0 to 7

struct BLOODCASTLE_ITEMBAG {
	BYTE Index;
	int ID;
	BYTE Level;
	BYTE Luck;
	BYTE Skill;
	BYTE Durability;
	BYTE Option;
	BYTE Excellent;
	BYTE Ancient;
	BYTE Count;
};

struct CBloodCastleBag {

public:
	void StartProcess();
	void LoadItemList();
	void ItemDrop(LPOBJ gObj);

	BLOODCASTLE_ITEMBAG Items[MAX_BC_LEVEL][MAX_ITEMS];

	int IsBloodCastle_ItemManager;
	int ItemsCount[MAX_BC_LEVEL];
};

extern CBloodCastleBag BloodCastleBag;
int GetExcellentOption();
void BloodCastle_ItemDrop(LPOBJ gObj);
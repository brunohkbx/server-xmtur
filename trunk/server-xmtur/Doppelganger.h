
#define DOPPELGANGER_MAP_RANGE(mapnumber) ( ((mapnumber) < MAP_INDEX_DOPPELGANGER1)?FALSE:((mapnumber) > MAP_INDEX_DOPPELGANGER4 )?FALSE:TRUE )

struct MONSTERS_INFO
{
	int ID;
	int X;
	int Y;
};

struct CHEST_ITEMBAG
{
	int Type;
	int Index;
	BYTE Level;
	BYTE Option;
	BYTE Luck;
	BYTE Skill;
	BYTE Duration;
	BYTE Excellent;
};

static const struct Doppelganger_Entrance_Position{
	int X1;
	int Y1;
	int X2;
	int Y2;
} Doppelganger_Entrance[4] = {
	195, 34, 199, 40,
	134, 78, 139, 84,
	106, 65, 111, 71,
	93, 23, 97, 25
};

static const struct Doppelganger_Boss_Position{
	int Boss1X;
	int Boss1Y;
	int Boss2X;
	int Boss2Y;
	int Boss3X;
	int Boss3Y;
} Doppelganger_Boss_Position[4] = {
	198, 51, 219, 75, 220, 84,
	137, 96, 116, 139, 113, 158,
	109, 92, 109, 112, 109, 128,
	93, 50, 62, 55, 49, 80,
};


enum DOPPELGANGER_STATES {
	NONE = 0,
	PREPARING = 1,
	RUNNING = 2,
	FINISHED = 3,
	STANDBY = 4,
};

enum DOPPELGANGER_RESULTS {
	SUCCESS = 0,
	PLAYER_DEAD = 1,
	FAILED = 2,
};

class CDoppelganger
{
public:
	//Common Functions
	void StartProcess(BOOL Reload);
		
	//NPC
	void Lugard(int aIndex);
	void EnterDoppelganger(int aIndex,int Invitation);
	
	//Checks
	int GetPosition(int aIndex);
	void CheckPartyMembers();
	void CheckPartyStatus();

	//Data Send
	void ChangeMapAttr(BOOL State, int aIndex);
	void SendTimeInfo(int Type, int aIndex);
	void SendTrianglesPosition(int aIndex);

	//Event States
	void State_STANDBY();
	void State_PREPARING();
	void State_RUNNING();
	void State_FINISHED();

	//Monster Functions
	void MonstersProcess();
	void MoveProc(LPOBJ lpObj);
	void ClearMonsters();
	void SetMonsters();
	void LoadMonsters();
	void CheckMonsterPassed();
	void AddMonster(int aIndex, int Class, int X, int Y);
	void CreateIceWalker();

	//Boss Functions
	void InterimRewardChest(int X, int Y);
	void FinalRewardChest(int X, int Y);
	void OpenChest(int aIndex, int X, int Y, int ChestType);
	
	//Monsters Information
	
	MONSTERS_INFO Monster[4][255];
	//int MonstersCount[4];
	int MonstersCount;
	int MonsterPassed;
	int CurrentMonster[255];

	int SiverChestIndex[2];
	int GoldenChestIndex;
	int Larvas[5];
	int MonsterBoss[3];

	//Others
	int Timer;
	int State;
	int Status;
	int EventLevel;
	int PartyNumber;

	//Configs
	int OnlyInParty;
	int EventMap;
	int StandByTime;
	int PieceofSpeculumDropRate;

	//Start Time
	DWORD dwTickCount; //Set Start Time....

	//StandBy Timer!
	DWORD StandbyTimer;
	
	//Players Information
	int Player[5];
	int PlayersCount;

	//Rewards Structures & Functions
	CHEST_ITEMBAG GoldenChest[100]; 
	CHEST_ITEMBAG SilverChest[100];
	int SilverChest_DropRate; //Larva Rate or Drop Rate

	//Monsters Position
	int PosX[4];
	int PosY[4];
	int MPosX[4];
	int MPosY[4];

	void LoadItemBag();
	int TotalItems[2]; //0: Silver Chest - 1: Golden Chest
};

extern CDoppelganger Doppelganger;

//Protocols


//0xC3, 0x0A, 0x30, 0x23	
//Double Goer NPC Click
struct PMSG_DGOER_EVENT_INFO // 0x23
{
	BYTE C;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE Minutes;
};

//Double Goer Timer Init
struct PMSG_DGOER_TIMER_INIT // 0x10
{
	BYTE C;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;	
	BYTE Timer;
};

//Double Goer Set Monster Count
struct PMSG_DGOER_MONSTER_COUNT // 0x14
{
	BYTE C;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE Total;
	BYTE Killed;
};

//Double Goer Set Player Triangle Position  
struct PMSG_DGOER_TRIANG_POS // 0x0F
{
	BYTE C;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE Pos;
};

//Double Goer Set Red Triangle Position
//0xC1 0x06 0xBF 0x11 0x00 0x12
struct PMSG_DGOER_RTRIANG_POS // 0x11
{
	BYTE C;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE Unk;
	BYTE Pos;
};

//Double Goer End, Timer Stop ( 0x00 - Success, 0x01 - Fail)
//C1 05 BF 13 XX
struct PMSG_DGOER_END
{
	BYTE C;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE Status;
};

struct PMSG_DGOER_USER_PLACE {
	WORD Index;
	BYTE Visible;
	BYTE Distance;
};

//Double Goer Update Bar(Triangles)
//0xC1, 0x1C, 0xBF, 0x12, 0x00, 0x00, 0x02, 0x00, 0x64, 0x2F, 0x01, 0x0E, 0x64, 0x2F, 0x02 , 0x08 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00
struct PMSG_DGOER_UPDATE_BAR
{
	BYTE C;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	
	//Time
	WORD Timer;
	WORD Players;
	PMSG_DGOER_USER_PLACE Player[5];
};



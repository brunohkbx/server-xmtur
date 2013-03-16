
#define IMPERIAL_MAP_RANGE(mapnumber) ( ((mapnumber) < MAP_INDEX_IMPERIALFORT1)?FALSE:((mapnumber) > MAP_INDEX_IMPERIALFORT4 )?FALSE:TRUE )

//0xC1, 0xXX, 0xF7, 0x02
struct PMSG_FORT_ENTER_RESULT
{
	PBMSG_HEAD2 h;
	BYTE Result;
	BYTE Day;
	BYTE State;
	BYTE Unk;
	WORD Index;
	WORD EntryTime;
};

//0xC1, 0x06, 0xBF, 0x09, 0x01, 0x02
//Result : 2 - Open, 3 - Close
struct PMSG_FORT_OPENCLOSE_GATE 
{
	BYTE C;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE Status;
	BYTE Result;
};

//0xC1, 0x0D, 0xF7, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28	
//Empire Guardian Timer Init
//Status : 01 - StandBy, 02 - Kill Mobs, 03 - Empty, 00 - Standby
struct PMSG_FORT_TIMER // 0x04
{
	PBMSG_HEAD2 h;
	BYTE Status;
	BYTE Unk1;
	BYTE Unk2;
	BYTE Unk3;
	BYTE Unk4;
	BYTE TimerL;
	BYTE TimerH;
	BYTE Unk5;
	DWORD Monster;
};

//0xC1, 0x0D, 0xF7, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
struct PMSG_FORT_END_MESSAGE
{
	BYTE C;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE Type;
	BYTE Unk1[3];
	unsigned long Experience;
	BYTE Unk2;
};

enum ImperialGuardian_State {
	IMPERIAL_NONE = 0,
	IMPERIAL_STANDBY = 1,
	IMPERIAL_GATEKILL = 2,
	IMPERIAL_FINISHED = 3,
	IMPERIAL_FAILED = 4,
	IMPERIAL_START = 5,
};


enum Door_State {
	DOOR_CLOSED = 0,
	DOOR_OPEN = 1,
};

struct IMPERIAL_MONSTERS
{
	int ID;
	int X;
	int Y;
	int Stage;
};

#define MAX_ITEMS 100

struct GAION_ITEMBAG {
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

struct CImperialGuardian {
	void StartProcess(BOOL Reload);

	void EnterEvent(int aIndex);

	void GiveUserDamage(int aIndex, int iDamage);
	void CheckTraps();
	void CheckPassGates(int aIndex, WORD Gate);
	void CheckGates();
	void OpenCloseGate(BYTE Status);

	void SendStage();
	void SendTimer();
	void MovePlayers();
	void SuccessMessage(int GainExp);
	void FailMessage();
	void MessageSend(char* Msg, int Type);
	void PDataSend(unsigned char * aRecv, int Size);
	void SendTimeInfo(int Type);
	void CheckPartyStatus();
	void SetState(int SetState);

	void DropSemicronPiece(int aIndex, int Class, int X, int Y);

	//Doors Functions
	void SetDoors();
	void CreateDoor(int GateType, int X, int Y, int Direction);
	int Door[10];
	int TotalDoors;

	//Player Info
	int Player[5];
	int DoorState[5];
	int PlayersCount;
	int PartyNumber;

	//Event Variables
	BYTE State;
	int Timer;
	int Zone;
	int ChangeZone;
	int EventDay;
	int EventMap;
	BYTE Step;
	DWORD StartTickCount;
	DWORD MoveTickCount;

	int PieceOfPaperDropRate;

	//Stage Information
	int BattleStatus; //1: STANDBY - 2: BATTLE!

	//Monsters
	void LoadMonsters();
	void SetMonsters();
	void ClearMonsters();

	IMPERIAL_MONSTERS Monsters[7][255];
	int SetedMonsters[255];
	int MonstersCount[7];
	int TotalMonsters; //Total Monsters Loaded
	int CurrentMonsters; //Killed Monsters
	int TrapsDamageBase;
	int TrapsDamagePlus;

	//Gaion ItemBag
	GAION_ITEMBAG Items[MAX_ITEMS];
	int GaionItemCount;

	//Drop Info
	int ItemDrop;
	int MoneyDrop;
	int MaxItemsDrop;

	void GaionItemDrop(int aIndex, int CordX, int CordY);
	void LoadGaionItemBag();
};

extern CImperialGuardian ImperialGuardian;
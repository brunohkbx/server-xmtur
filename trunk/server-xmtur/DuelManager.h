#define MAX_DUEL_ROOMS 4
#define MAX_DUEL_SPECS 10

struct PMSG_DUEL_ROOMSTATUS // Complemento de 0x06
{
	BYTE Name1[10];
	BYTE Name2[10];
	BYTE DuelRunning;
	BYTE DuelOpen;
};

struct PMSG_DUEL_STATUS // 0x06
{
	BYTE H;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	PMSG_DUEL_ROOMSTATUS RoomStatus[MAX_DUEL_ROOMS];
};

struct PMSG_DUEL_CANCEL // 0x03
{
	BYTE H;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE Unk1;
};

struct PMSG_DUEL_ANSWER_START
{
	BYTE H;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE bDuelOK;
	BYTE NumberH;
	BYTE NumberL;
	char szName[10];
};

struct PMSG_DUEL_START // 0xAA
{
	BYTE H;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;	
	BYTE bDuelStart;	//3
	BYTE NumberH;	//4
	BYTE NumberL;	//5
	char szName[10];	//6
};

struct PMSG_DUEL_FINISH // 0x0C
{
	BYTE H;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE szWinner[10];
	BYTE szLooser[10];
};

struct PMSG_DUEL_SPEC_ADD // 0x08
{
	BYTE H;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE szName[10];
};

struct PMSG_DUEL_WATCH_REQ // 0x07
{
	BYTE H;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE Room;
};


struct PMSG_DUEL_SCORE // 0x04
{
	BYTE H;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE NumberH1;	
	BYTE NumberL1;	
	BYTE NumberH2;	
	BYTE NumberL2;	
	BYTE btDuelScore1;	
	BYTE btDuelScore2;	
};

struct PMSG_DUEL_LIFEBAR_REFILL // 0x05
{
	BYTE H;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE Player1H;
	BYTE Player1L;
	BYTE Player2H;
	BYTE Player2L;
	BYTE Life1;
	BYTE Life2;
	BYTE Shield1;
	BYTE Shield2;
};


struct PMSG_DUEL_LIFEBAR_NAME // 0x07 00 
{
	BYTE H;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	WORD Type;
	BYTE Name1[10];
	BYTE Name2[10];
	BYTE Player1H;
	BYTE Player1L;
	BYTE Player2H;
	BYTE Player2L;
};

struct PMSG_DUEL_LIFEBAR_INIT // 0x0D
{
	BYTE H;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE Unk1;
};

struct PMSG_DUEL_SPEC_LIST // 0x0B
{
	BYTE H;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE btCount;
	BYTE szName[MAX_DUEL_SPECS][10];
};


struct PMSG_DUEL_QUESTION_START // 0x01
{
	BYTE H;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE NumberH;
	BYTE NumberL;
	BYTE szName[10];
};

struct DUEL_ROOM
{
	BOOL Free;
	BOOL Waiting;

	int Dueler1;
	BYTE Points1;
	int Dueler2;
	BYTE Points2;

	int Spectator[MAX_DUEL_SPECS];
	DWORD dwTickCount;
	DWORD dwWaitTime;
	DWORD dwMoveTime;
	BOOL DuelFinish;
};

struct CDuelManager {
public:
	CDuelManager();
	~CDuelManager(void);

	void Core(int aIndex, unsigned char* aRecv);
	void DuelRequest(int aIndex,PMSG_DUEL_QUESTION_START* lpMsg);
	void DuelResponse(int aIndex,PMSG_DUEL_ANSWER_START* lpMsg);
	void DuelWatchRequest(int aIndex,PMSG_DUEL_WATCH_REQ* lpMsg);
	void DuelRemoveRequest(int aIndex);
	void DuelWinner(int Room);
	void RequestDuelNPC(int aIndex);
	void StartProcess();
	void Run();
	///------
	
	void SendSpectatorList(int aIndex, int Room);
	void DuelIntefaceStatus(int aIndex, int Room);
	void DuelIntefaceInit(int aIndex, int Room);
	char ReserveDuelRoom(int aIndex, int aTarget);
	char ReserveWatchPlace(int Room);
	void CheckRoomStatus(int Room);
	void UpdateDuelScore(int Room);
	void MoveDuelers(int Room);
	void RoomReset(int Room);
	void FinishDuel(int aIndex);
	BOOL IsSpectator(int Room, int aIndex);

	int IsEnable;
	int DuelCost;
	int DuelMinLevel;
	int EnableBuff;
	int EnablePrize;

	DUEL_ROOM DuelRoom[MAX_DUEL_ROOMS];
};

extern CDuelManager DuelManager;

void DuelManager_CheckCore(void * lpParam);
void DuelManager_WarpProcess(void * lpParam);
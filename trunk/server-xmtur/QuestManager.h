struct QUEST_DATA
{
	WORD Dialog;
	WORD Quest;
};

struct QUEST_DATA_BYTE
{
	BYTE DialogL;
	BYTE DialogH;
	BYTE QuestL;
	BYTE QuestH;
};

struct QUEST_DETELE_REQ
{
	BYTE H; //C1
	BYTE Size;
	BYTE Headcode; //F6
	BYTE Subcode; //0F
	WORD QuestNumber;
	WORD QuestType;
};	

struct PMSG_QUESTLIST_PHANTOM //Send QuestList to Shadow Phantom NPC!
{
	BYTE H; //C1
	BYTE Size;
	BYTE Headcode; //F6
	BYTE Subcode; //0A
	WORD NPCType;
	BYTE CountL;
	BYTE CountH;
	QUEST_DATA Quest[MAX_ACCEPT_QUEST];
};


struct PMSG_QUESTLIST_TKEY //Send QuestList to "T" Key
{
	BYTE H; //C1
	BYTE Size;
	BYTE Headcode; //F6
	BYTE Subcode; //0A
	BYTE Amount;
	QUEST_DATA_BYTE Quest[MAX_ACCEPT_QUEST];
};


struct PMSG_QUEST_DIALOG_1 //Accept or Deny Dialogs!
{
	BYTE H; //C1
	BYTE Size;
	BYTE Headcode; //F6
	BYTE Subcode; //0B
	QUEST_DATA Quest;
	BYTE Unk;
};

struct QUEST_REQ {
	BYTE TypeL;
	BYTE TypeH;
	BYTE Unk[2];
	BYTE ClassL;
	BYTE ClassH;
	BYTE CountL;
	BYTE CountH;
	BYTE Unk1[2];
	BYTE TotalL;
	BYTE TotalH;
	BYTE ItemInfo[12];
};

struct QUEST_REW {
	BYTE TypeL;
	BYTE TypeH;
	BYTE Unk[2];
	BYTE ClassL;
	BYTE ClassH;
	BYTE Count[4];
	BYTE ItemInfo[12];
};

struct PMSG_QUEST_ACCEPTQUEST
{
	BYTE H; //C1
	BYTE Size;
	BYTE Headcode; //F6
	BYTE Subcode; //0C
	QUEST_DATA Quest;
	BYTE RequirementsCount;
	BYTE RewardsCount;
	BYTE Position;
	QUEST_REQ Requirements[5];
	QUEST_REW Rewards[5];
};

struct PMSG_QUEST_FINISH
{
	BYTE H;
	BYTE Size;
	BYTE Head;
	BYTE Sub;
	QUEST_DATA Quest;
	BYTE bAnswer;	// 0 : Nothing | 1 : Close | 2 : Reached Zen Limit | 3 : Full Inv
};

struct Requirements {

	char Type;

	WORD ItemType; //Used for SkillType and MonsterNumber
	WORD ItemIndex;
	DWORD Count; //Used KillCount and ItemCount

	int Level;
	int Option;
	int Luck;
	int Skill;
	int Durability;
	int Excellent;
	int Ancient;
};

struct Rewards {

	char Type;

	WORD ItemType;
	WORD ItemIndex;
	DWORD Count;

	int Level;
	int Option;
	int Luck;
	int Skill;
	int Durability;
	int Excellent;
	int Ancient;
};

struct QuestInfo {
	int QuestNumber;
	int QuestType;
	int AllowedNPC;
	int AcceptDialog;
	int RejectDialog;
	char Name[50];
	int MinLevel;
	int MaxLevel;
	int Class[6]; //For 6 Class
	Requirements Requirements[6];
	Rewards Rewards[6];
	int RequirementsCount;
	int RewardsCount;
};
///////////////////////////////////////

struct PMSG_QUEST_GETINFO // 0x01
{
	BYTE H;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	WORD QuestNumber;
	WORD QuestType;
	BYTE Result;
};


#define MAX_QUESTS 500

struct CQuestManager {
public:
	void StartProcess();
	void QuestProtocolCore(int aIndex,unsigned char* aRecv);
	void LoadQuestList();
	void SendQuestList(int aIndex, int NPC);
	void SendAcceptDenyDialog(int aIndex, PMSG_QUEST_GETINFO* lpMsg);
	void SendQuestInformation(int aIndex, PMSG_QUEST_GETINFO* lpMsg);
	void GetQuestList(int aIndex);
	void AcceptDenyQuest(int aIndex, PMSG_QUEST_GETINFO* lpMsg);
	void DeleteQuest(int aIndex, QUEST_DETELE_REQ * lpMsg);
	void FinishQuest(int aIndex, PMSG_QUEST_GETINFO* lpMsg);

	BOOL AllowedClass(int aIndex, int QuestID);
	int GetQuestID(int QuestType, int QuestNumber);
	int GetQuestPlace(int aIndex,int QuestType, int QuestNumber);
	BOOL QuestAlreadyAcepted(int aIndex, int QuestType, int QuestNumber);
	int RemoveRequirements(int aIndex,int ItemType,int ItemIndex,int Durability,int Level,int Luck,int Skill,int Option,int Excellent,int Ancient);
	int GetItemsCount(int aIndex,int ItemType,int ItemIndex,int Durability,int Level,int Luck,int Skill,int Option,int Excellent,int Ancient);
	int GetEmptyPlace(int aIndex);
	void SendQuestToPlayer(int aIndex);

	int Count;
	QuestInfo QuestList[MAX_QUESTS];
};

extern CQuestManager QuestManager;
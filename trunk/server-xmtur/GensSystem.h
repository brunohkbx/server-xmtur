struct PMSG_GENS_JOIN	// C1 06 F8 02 01 00
{
	BYTE C;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE Type;
	BYTE Sub;
};

struct PMSG_GENS_LEAVE	// C1 05 F8 04 01
{
	BYTE C;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	BYTE Type;
};

struct PMSG_GENS_INFO	// 0xC1,0x18,0xF8,0x07,0x01,0x00,0x00,0x00,0x6E,0x01,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE8,0x03,0x00,0x00
{
	BYTE C;
	BYTE Size;
	BYTE Headcode;
	BYTE Subcode;
	int GensType;
	int Ranking;
	int Symbol;
	int Contribution;
	int NextContribution;

	BYTE bInfluence;
	int iGensRanking;
	int iGensClass;
	int iContributePoint;
	int iNextContributePoint;

};

struct PMSG_GENS_VIEWPORT
{
	BYTE H; //C1
	BYTE SizeL;
	BYTE SizeH;
	BYTE Headcode; //F8
	BYTE SubCode; //5
	BYTE Count; //5
	BYTE GensType; //6
	BYTE IndexHI; //7
	BYTE IndexLO; //8
	char Unk1[5]; //9
	BYTE Rank;	//14
	char Unk2[6];//15
};

struct PWMSG_GENS_VIEWPORT
{
	BYTE C;		// 0xC2
	BYTE SizeH;
	BYTE SizeL;
	BYTE Head;	// 0xF8
	BYTE Sub;	// 0x05
	BYTE Count;
	BYTE Family;
	BYTE IndexH;
	BYTE IndexL;
	BYTE bNot1[5];
	BYTE Symbol;
	BYTE bNot2[3];
	BYTE bUnk;
	BYTE bNot3[3];
};

struct PMSG_GENS_CONTRIBUTION
{
	BYTE H;
	BYTE Size;
	BYTE Head;
	BYTE Sub;	// 0x12
	char Name[11];
	int Contrib;
};

struct PMSG_GENS_REWARD
{
	PBMSG_HEAD2 h;
	BYTE Gens;
};

class CGensSystem {

public:
	CGensSystem();
	void GetGensInformation(int aIndex);
	void SendGensInfo(int aIndex);
	void SaveGensInfo(int aIndex);
	void EnterGensFamily(int aIndex, int GensType);
	void LeaveGensFamily(int aIndex);
	void GetGensSymbol(int aIndex);
	void GetGensRankingReward(PMSG_GENS_REWARD *aRecv, int aIndex);
	void AddContribution(int aIndex, int Points);
	void SendGensViewport(int aIndex);
	int GetLevelDifference(short Level1, short Level2);
	BOOL GetContributionPoints(int aIndex, int aTargetIndex);
	void ExecuteRankUpdate();
	//ProtocolCore
	void GensProtocolCore(int aIndex, unsigned char* aRecv);
};

extern CGensSystem GensSystem;
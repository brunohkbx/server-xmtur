struct MAPS_INFO{
	int Map;
	int RequiredZen;
	int LevelMax;
	int ResetMax;
};
	
class CPartySystem {
public:
	void WarpToParty(int aIndex, int PartyNumber);
	BOOL CheckLevelResetsDif(int aIndex, int LeaderIndex);
	void StartProcess();
	void LoadConfig();
	MAPS_INFO Maps[64];

	int Enabled;

	int TracePartyMinLevel;
	int CreatePartyDiferenceLevel;
	int CreatePartyDiferenceResets;
	int MapCount;
};

extern CPartySystem PartySystem;

int GetPartyLeader(int PartyNumber);

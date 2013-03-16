struct CCommands {

public:
	void SendPost(int aIndex, LPCSTR Message);
	void AddStats(int aIndex, LPCSTR AddPoints, DWORD Type);
	void DropItem(int aIndex, LPCSTR Message);
	void GMove(int aIndex, LPCSTR Message);
	void GMoveALL(int aIndex, LPCSTR Message);
	void ChangePKLevel(int aIndex, LPCSTR Message);
	void ChangeZen(int aIndex, LPCSTR Message);
	void SendGG(int aIndex, char* Message);
	void PKClear(int aIndex);
	void ServerInfo(int aIndex);
	void Online(int aIndex);
	void Reload(int aIndex, LPCSTR Type);
	void SendFireworks(int aIndex, LPCSTR Message);
	void GetPlayerInfo(int aIndex, LPCSTR Message);
	void Spawn(int aIndex, LPCSTR Message);
	void ChangeSkin(int aIndex, LPCSTR Message);
	void ChangeVault(int aIndex, LPCSTR Message);

	//Ban Commands
	void BanCharacter(int aIndex, LPCSTR Message);
	void UnBanCharacter(int aIndex, LPCSTR Message);
	void BanAccount(int aIndex, LPCSTR Message);
	void UnBanAccount(int aIndex, LPCSTR Message);

	
	BOOL Manager(int aIndex, PMSG_CHATDATA * lpMsg);
};

extern CCommands Commands;
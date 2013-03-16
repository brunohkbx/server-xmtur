
struct MOVES_LIST {
	BYTE Index;
	char Name[20];
	short Level;
	int Money;
	int GensMap;
	int Gate;
};

struct CWarpSystem {
	//Functions
	void LoadMoves();
	int GetWarpIndex(int MoveIndex);
	void MovePlayer(int aIndex, int MoveIndex);

	//Variables
	MOVES_LIST Moves[50];
	int Count;
};

extern CWarpSystem WarpSystem;
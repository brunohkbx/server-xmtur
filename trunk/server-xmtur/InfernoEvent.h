#define MAX_ITEMS 100

struct INFERNOEVENT_ITEMBAG {
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

struct CInfernoEvent {

public:
	void LoadItemList();
	void DropItem(int aIndex);

	INFERNOEVENT_ITEMBAG Items[MAX_ITEMS];

	int ItemsCount;
};

extern CInfernoEvent InfernoEvent;
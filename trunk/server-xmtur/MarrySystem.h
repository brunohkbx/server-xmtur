#define MARRYSYSTEM_FILE ".\\MarrySystem.xtr"

class CMarrySystem {

public:
	void StartProcess(int Reload);
	void ProposeMarry(int aIndex, LPSTR Message);
	void AcceptMarry(int aIndex);
	void GetMarryInfo(int aIndex);
	void TraceMarry(int aIndex);
	BOOL IsInEventMap(int aIndex);
	void SendDivorce(int aIndex);
	void DivorceProcess(int aIndex, int Force);
	void SendMarryText(int aIndex);
	void MarryInfo(int aIndex);
	BOOL CheckMarryShop(int aIndex, int ShopIndex);

	//General
	int IsEnable;
	int MapNumber;
	int CordX_1;
	int CordY_1;
	int CordX_2;
	int CordY_2;

	int MinLevel;
	int MinResets; 

	int EnableTraceMarry;
	int MarryPlusExperience;

	int ForceDivorceMoney;

	//Shop for Marry
	int IsShopEnable;
	int Shop_MapNumber;
	int Shop_CordX;
	int Shop_CordY;

};

extern CMarrySystem MarrySystem;
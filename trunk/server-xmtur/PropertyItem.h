#include "item.h"
#include "user.h"

struct PROPERTY_ITEM
{
	LPOBJ	lpObj;
	char	AccountID[11];
	int		ItemSerial;
	int		ItemID;
	time_t	ExpirationDate;
};

struct PMSG_PROPERTYITEMINFO
{
	PBMSG_HEAD2		h;
	short			ItemID;
	short			Pos;
	int				Time;
};

class CPropertyItem
{
public:
	CPropertyItem(void);
	virtual ~CPropertyItem(void);

	bool MakePropertyItem(LPOBJ lpObj, CItem * Item, int seconds);
	void SetPropertyItem(CItem * Item);
	void LoadPropertyItem(int aIndex, int ItemID, int Serial, int Time);
	void SendPropertyInfo(LPOBJ lpObj, int ItemID, int Serial, int pos);
	bool IsPropertyItem(LPOBJ lpObj, int ItemID, int Serial);
	bool IsPropertyItem(int ItemID, int Serial);
	void SavePropertyItem(LPOBJ lpObj);
	void ErasePlayerItems(LPOBJ lpObj);

	void Run();

private:

	std::vector<PROPERTY_ITEM>::iterator FindPropertyItem(int ItemID, int serial, LPOBJ lpObj);

	std::vector<PROPERTY_ITEM> m_vtPropertyItems;

};

extern CPropertyItem	g_PropItems;
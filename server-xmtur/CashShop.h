// CashShop.h: interface for the CCashShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASHSHOP_H__B57A9271_3D21_40E0_B132_82F87B122CB4__INCLUDED_)
#define AFX_CASHSHOP_H__B57A9271_3D21_40E0_B132_82F87B122CB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "PCSProtocol.h"
#include "item.h"
#include "protocol.h"
#include "wsShopServerCli.h"


#define MAX_CASH_SHOP_CATEGORY	4
#define MAX_SHOP_ITEM			500	//135
#define MAX_PACK_ITEM			200
#define MAX_CASH_SHOP_PROTOCOL	15
#define MAX_CASH_SHOP_PROTOCOL_DATA	9
#define MAX_CASH_ITEM_BRANCH	100

#pragma pack(1)

struct CASHSHOP_ITEM_STATUS
{
	BYTE btItemType;	// 0
	WORD wItemIndex;	// 1
	BYTE btItemLevel;	// 3
	BYTE btDurability;	// 4
	BYTE btSkillOption;	// 5
	BYTE btLuckOption;	// 6
	BYTE btAddOption;	// 7
	BYTE btExOption;	// 8
	BYTE btItemInfo[MAX_ITEM_INFO];	// 9
};

struct INGAMESHOP_ITEMLIST
{
	int Category;
	int Place;
	int PackageID;
	int ProductIndex;
	int ProductOption;
	WORD ItemType;
	WORD ItemIndex;
	BYTE BuyType; 
	int Price;
	BYTE ItemCategory;
	int Durability;
	BYTE X;
	BYTE Y;
	BYTE PackageItem;
	char* Name[250];
};

struct INGAMESHOP_PACKAGELIST
{
	BYTE	Guid;
	BYTE	PackageID;
	BYTE	ItemSeqNum;
	WORD	dwItemGroup;
	WORD	dwItemType;
	BYTE	btItemLevel;
	BYTE	btItemDurability;
	BYTE	btItemSkill;
	BYTE	btItemLuck;
	BYTE	btItemOption;
	BYTE	btItemExOption;
	BYTE	btItemSetOption;
	BYTE	btItemType;
	DWORD	dwItemPeriodTime;
	WORD	wUniqueID1;
	WORD	wUniqueID2;
	WORD	wUniqueID3;
	BYTE	btX;
	BYTE	btY;
};

struct PMSG_SEND_CASHSHOP_VERS
{
	BYTE H;
	BYTE Size;
	BYTE Head;
	BYTE Sub;
	WORD Vers_Main;
	WORD Vers_Sub;
	WORD Vers_Rev;
};

struct PMSG_REQ_INGAMESHOPINIT
{
	PBMSG_HEAD2 h;
};

struct PMSG_ANS_INGAMESHOP_POINT
{
	PBMSG_HEAD2 h;
	BYTE result;
	double WCoinC[2];
	double WCoinP[2];
	double Goblin;
};

struct PMSG_ANS_INGAMESHOP_ITEMCOUNT
{
	PBMSG_HEAD2 h;
	short OwnInv[2];
	short GiftInv[2];
};
#pragma pack (1)
struct PMSG_ANS_INGAMESHOP_ITEM
{
	PBMSG_HEAD2	h;
	int	UniqueCode;
	int AuthCode;
	int UniqueValue1;
	int UniqueValue2;
	int UniqueValue3;
	int Unk1;
	int Unk2;
	BYTE Unk3;
};

struct PMSG_REQ_INGAMESHOPOPEN
{
	PBMSG_HEAD2 head;
	BYTE btShopOpenType;	// 4
};

struct PMSG_REQ_INGAMESHOP_INVENTORY
{
	PBMSG_HEAD2	h;
	int	Result;
	BYTE InventoryType;
	BYTE Unk;
};

struct PMSG_REQ_INGAMESHOP_ITEMBUY
{
	PBMSG_HEAD2	h;
	int ItemIndex;
	int Category;
	int ItemOpt;
	short ItemID;
	int ItemSubIndex;
};

struct PMSG_ANS_INGAMESHOP_ITEMBUY
{
	PBMSG_HEAD2 h;
	BYTE Result;
	int Unknown;
};

struct PMSG_REQ_INGAMESHOP_ITEMUSE
{
	PBMSG_HEAD2	h;
	int UniqueCode;
	int AuthCode;
	WORD ItemID;
	BYTE Unk;
};

struct PMSG_ANS_INGAMESHOP_ITEMUSE
{
	PBMSG_HEAD2	h;
	BYTE result;
};



#pragma pack ()

struct PMSG_REQ_CASHITEMLIST
{
	PBMSG_HEAD2 head;
	BYTE btCategoryIndex;	// 4
	BYTE btPageIndex;	// 5
	BYTE btPageLoaded;	// 6
};


struct PMSG_REQ_CASHITEM_BUY
{
	PBMSG_HEAD2 head;
	DWORD dwItemPriceGuid;	// 4
	BYTE btCategoryIndex;	// 8
};


struct CASHSHOP_ITEMLIST_TO_PROTOCOL
{
	DWORD dwItemGuid;	// 0
	BYTE btCategoryCode;	// 4
	BYTE btItemDuration;	// 5
	BYTE btItemSaleRatio;	// 6
	WORD wItemPrice;	// 7
	BYTE btSpecialOption;	// 9
	BYTE btItemInfo[MAX_ITEM_INFO];	// A
	DWORD dwItemUsePeriod;	// 11
};


struct PMSG_ANS_CASHITEMLIST
{
	PBMSG_HEAD2 head;	// C1:F5:06
	BYTE btCategoryIndex;	// 4
	BYTE btPageIndex;	// 5
	BYTE btItemCount;	//	// 6
	CASHSHOP_ITEMLIST_TO_PROTOCOL ItemInfo[MAX_CASH_SHOP_PROTOCOL_DATA];	// 7
};


struct CASHSHOP_BRANCH_ITEMLIST
{
	DWORD dwItemGuid;	// 0
	int iBranchType;	// 4
};


#pragma pack()

class CCashShop
{

public:

  	CCashShop();
	virtual ~CCashShop();

	void Initialize();
	void Load(LPSTR pchFilename);
	void LoadPackages(LPSTR pchFilename);
	void LoadShopOption(LPSTR pchFilename);
	void CashShopOptioNReload();
	BOOL IsGetAmountFromShopItem(int iItemCode);
	INGAMESHOP_ITEMLIST* SearchItemList(int ItemIndex, int ItemSubIndex, int Category, int ItemID, int ItemOpt);
	INGAMESHOP_ITEMLIST* SearchItemList(int UniqueID1, int UniqueID2, int UniqueID3);
	INGAMESHOP_ITEMLIST* SearchItemFromInventory(LPOBJ lpObj, int UniqueCode, int AuthCode);
	BOOL AddUser(LPOBJ lpObj);
	BOOL DeleteUser(LPOBJ lpObj);
	LPOBJ SearchUser(int iUserGuid);
	BOOL CGCashShopOpen(LPOBJ lpObj, PMSG_REQ_INGAMESHOPOPEN* lpMsg);
	BOOL CGCashPoint(LPOBJ lpObj);
	void CGCashItemBuy(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_ITEMBUY* lpMsg);
	void GCCashPoint(LPOBJ lpObj);
	void GCCashItemBuyResult(LPOBJ lpObj, BYTE btResult);
	BOOL CGCashInventoryItemCount(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_INVENTORY* lpMsg);
	void GCCashInventoryItemCount(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_INVENTORY* lpMsg);
	BOOL GCCashItemInventory(LPOBJ lpObj, int ItemCount, int InventoryType);
	void CGCashInventoryItemUse(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_ITEMUSE* lpMsg);
	bool DeleteItemFromInventory(LPOBJ lpObj, int UniqueCode, int AuthCode);
	BOOL GiveBuyItemToInventory(LPOBJ lpObj, INGAMESHOP_ITEMLIST* lpItemInfo, BYTE Inventory);
	BOOL GiveBuyItemToInventory(LPOBJ lpObj, INGAMESHOP_PACKAGELIST * lpPackInfo, BYTE Inventory);
	BOOL CheckPeriodItemUsed(LPOBJ lpObj, int iItemCode);
	BOOL CheckPeriodItem(int iItemCode);
	int	 GetItemCountInShopInventory(LPOBJ lpObj, int InventoryType);
	bool InsertItemToInventory(LPOBJ lpObj, int UniqueCode, int AuthCode, int UniqueID1, int UniqueID2, int UniqueID3, BYTE Inventory);
	bool UsePackage(LPOBJ lpObj, int PackageID);
	LPBYTE GetItemList(int iCategory, int iPageCount);

private:

	int iCashItemCount;	// 4
	int iPackItemCount;
	BOOL bCashItemListReload;	// C
	std::map<int,OBJECTSTRUCT *> MapUserObject;	// 10

	INGAMESHOP_ITEMLIST ShopItemList[MAX_SHOP_ITEM];	// 40
	INGAMESHOP_PACKAGELIST PackItemList[MAX_PACK_ITEM];
};

extern BOOL g_bUseMoveMapBound;
extern CCashShop g_CashShop;
static BYTE InGameShopKey1[10] = { 0xC1, 0x0A, 0xD2, 0x0C, 0x00, 0x02, 0xDB, 0x07, 0x06, 0x00 };
static BYTE InGameShopKey2[10] = { 0xC1, 0x0A, 0xD2, 0x15, 0x47, 0x02, 0xDA, 0x07, 0x05, 0x00 };
BOOL IsCashItem(int iItemCode);

#endif // !defined(AFX_CASHSHOP_H__B57A9271_3D21_40E0_B132_82F87B122CB4__INCLUDED_)

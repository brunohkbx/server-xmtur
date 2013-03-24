#include "stdafx.h"
#include "CashShop.h"
#include "CashItemPeriodSystem.h"
#include "wsShopServerCli.h"
#include "GameServer.h"
#include "GameMain.h"
#include "LogProc.h"
#include "DSProtocol.h"
#include "Readscript.h"
#include "Winutil.h"

BOOL g_bUseCashShop = FALSE;
BOOL g_bUseLotteryEvent = FALSE;
BOOL g_bUseMoveMapBound = FALSE;

CCashShop g_CashShop;

CCashShop::CCashShop(){
	return;
}

CCashShop::~CCashShop(){
	return;
}

void CCashShop::Initialize()
{
	this->iCashItemCount = 0;
	this->bCashItemListReload = FALSE;
	this->MapUserObject.clear();

	memset(&this->ShopItemList,0,sizeof(this->ShopItemList));
	memset(&this->PackItemList,0,sizeof(this->PackItemList));
}


void CCashShop::CashShopOptioNReload() 
{
	this->bCashItemListReload = TRUE;
	this->Load(gDirPath.GetNewPath("InGameShop_Products.txt"));
	this->LoadPackages(gDirPath.GetNewPath("InGameShop_Packages.txt"));
	this->bCashItemListReload = FALSE;
}

void CCashShop::Load(LPSTR pchFilename)
{
	SMDToken Token;

	SMDFile = fopen(pchFilename, "r");

	if(SMDFile == NULL){
		MsgBox("[InGameShop][Products] List load Failed. [%s]", pchFilename);
		return;
	}

	this->Initialize();

	int iType = 0;
	INGAMESHOP_ITEMLIST ItemStatus;
	BOOL bResult = 0;
	this->iCashItemCount = 0;
	memset(&this->ShopItemList,0,sizeof(this->ShopItemList));

	while(TRUE)
	{
		Token = (SMDToken)GetToken();
		if(Token == END) break;

		iType = TokenNumber;

		while(TRUE)
		{
			Token = (SMDToken)GetToken();

			if(Token == END) break;
		
			if(iType == 1){

				if(!strcmp("end", TokenString)) break;
	
				this->ShopItemList[iCashItemCount].Category = TokenNumber;

				Token = (SMDToken)GetToken();
				this->ShopItemList[iCashItemCount].Place = TokenNumber;

				Token = (SMDToken)GetToken();
				this->ShopItemList[iCashItemCount].PackageID = TokenNumber;

				Token = (SMDToken)GetToken();
				this->ShopItemList[iCashItemCount].ProductIndex = TokenNumber;

				Token = (SMDToken)GetToken();
				this->ShopItemList[iCashItemCount].ProductOption = TokenNumber;

				Token = (SMDToken)GetToken();
				this->ShopItemList[iCashItemCount].ItemType = TokenNumber;
				
				Token = (SMDToken)GetToken();
				this->ShopItemList[iCashItemCount].ItemIndex = TokenNumber;
				
				Token = (SMDToken)GetToken();
				this->ShopItemList[iCashItemCount].BuyType = TokenNumber;

				Token = (SMDToken)GetToken();
				this->ShopItemList[iCashItemCount].Price = TokenNumber;

				Token = (SMDToken)GetToken();
				this->ShopItemList[iCashItemCount].ItemCategory = TokenNumber;

				Token = (SMDToken)GetToken();
				this->ShopItemList[iCashItemCount].Durability = TokenNumber;

				Token = (SMDToken)GetToken();
				this->ShopItemList[iCashItemCount].X = TokenNumber;

				Token = (SMDToken)GetToken();
				this->ShopItemList[iCashItemCount].Y = TokenNumber;
				
				Token = (SMDToken)GetToken();
				strcpy((char*)this->ShopItemList[iCashItemCount].Name, TokenString);

				LogAddTD("[InGameShop][Load Cash Item List](%s) Add List - (%d/%d) X:%d, Y:%d, Price:%d",
					this->ShopItemList[iCashItemCount].Name,
					this->ShopItemList[iCashItemCount].ItemType,
					this->ShopItemList[iCashItemCount].ItemIndex,
					this->ShopItemList[iCashItemCount].X, this->ShopItemList[iCashItemCount].Y, 
					this->ShopItemList[iCashItemCount].Price);

				this->iCashItemCount++;
			}
		}
	}

	fclose(SMDFile);

	LogAddTD("[InGameShop][Load Item List] - Complete! - Total: %d", this->iCashItemCount);
}

void CCashShop::LoadPackages(LPSTR pchFilename)
{
	SMDToken Token;

	SMDFile = fopen(pchFilename, "r");

	if(SMDFile == NULL){
		MsgBox("[InGameShop][PackageList] File not Found: %s", pchFilename);
		return;
	}

	int iType = 0;
	INGAMESHOP_PACKAGELIST PackStatus;
	BOOL bResult = 0;

	while(TRUE)
	{
		Token = (SMDToken)GetToken();

		if(Token == END) break;

		iType = TokenNumber;

		while(TRUE)
		{
			Token = (SMDToken)GetToken();

			if(Token == END) break;
		
			if(iType == 1)
			{
				if(!strcmp("end", TokenString)) break;
				//FALTA TERMINAR AUN!! PARTE DE PACKAGES (PAQUETES DE VARIOS ITEMSSSS)
				LogAddTD("[InGameShop][Package Item List] Add Package Item: Pack: %d, Seq: %d", 0,0);
			}
		}
	}

	LogAddTD("[InGameShop][Package Load List] Complete! - Total:%d", this->iPackItemCount);
	fclose(SMDFile);
}


BOOL CCashShop::IsGetAmountFromShopItem(int iItemCode) //NOT IN USE
{
	return FALSE;
}

BOOL CCashShop::AddUser(LPOBJ lpObj)
{
	if ( this->SearchUser(lpObj->DBNumber) )
		return FALSE;

	PMSG_REQ_INGAMESHOPINIT pInit;
	PHeadSubSetB((LPBYTE)&pInit, 0xD2, 0x00, sizeof(pInit));
	DataSend(lpObj->m_Index, (LPBYTE)&pInit, sizeof(pInit));
	DataSend(lpObj->m_Index, (LPBYTE)&InGameShopKey1, 10);
	DataSend(lpObj->m_Index, (LPBYTE)&InGameShopKey2, 10);
	this->MapUserObject.insert(std::pair<int, LPOBJ>(lpObj->DBNumber, lpObj));
	GDReqInGameShopPoint(lpObj->m_Index);
	GDReqInGameShopItemList(lpObj->m_Index);
	return TRUE;
}

BOOL CCashShop::DeleteUser(LPOBJ lpObj)
{
	std::map<int, OBJECTSTRUCT *>::iterator Iter;

	Iter = this->MapUserObject.find(lpObj->DBNumber);
	if(Iter == this->MapUserObject.end()) return FALSE;

	this->MapUserObject.erase(Iter);

	memset(&lpObj->ShopInventory,0,sizeof(lpObj->ShopInventory));
	memset(&lpObj->GiftInventory,0,sizeof(lpObj->GiftInventory));
	return TRUE;
}


LPOBJ CCashShop::SearchUser(int iUserGuid){

	LPOBJ lpReturnObj = NULL;

	std::map<int, OBJECTSTRUCT *>::iterator Iter;
	Iter = this->MapUserObject.find(iUserGuid);

	if(Iter == this->MapUserObject.end()) return FALSE;
	lpReturnObj = Iter->second;

	return lpReturnObj;
}

struct PMSG_ANS_CASHSHOPOPEN
{
	PBMSG_HEAD2 head;	// C1:D2:02
	BYTE btResult;	// 4
};

BOOL CCashShop::CGCashShopOpen(LPOBJ lpObj, PMSG_REQ_INGAMESHOPOPEN * lpMsg)
{
	BYTE btResult = 0;

	if(this->bCashItemListReload == TRUE) btResult = 6;

	if(!gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID)) return FALSE;
	
	if(lpObj->Connected <= PLAYER_LOGGED || lpObj->CloseCount != -1) return FALSE;

	if(lpMsg->btShopOpenType == 0){

		if(lpObj->m_IfState.use > 0){
			btResult = 8;
		}

		if(btResult == 0){
			lpObj->m_IfState.use = 1;
			lpObj->m_IfState.type = 18;
			lpObj->m_IfState.state = 1;
			btResult = 1;
		}

	} else if(lpMsg->btShopOpenType == 1){
		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.type = 0;
		lpObj->m_IfState.state = 0;
		btResult = 0;
	}

	PMSG_ANS_CASHSHOPOPEN pMsg;

	pMsg.btResult = btResult;
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x02, sizeof(PMSG_ANS_CASHSHOPOPEN));
	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.head.size);

	return TRUE;
}

BOOL CCashShop::CGCashInventoryItemCount(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_INVENTORY *lpMsg)
{
	if(!gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID)) return FALSE;
	if(lpObj->Connected !=PLAYER_PLAYING && lpObj->Type != OBJ_USER) return FALSE;

	this->GCCashInventoryItemCount(lpObj, lpMsg);
	return true;
}


BOOL CCashShop::CGCashPoint(LPOBJ lpObj)
{
	if(!gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID)) return FALSE;
	
	if(lpObj->Connected !=PLAYER_PLAYING && lpObj->Type != OBJ_USER) return FALSE;

	BYTE InitCashShop[4] = { 0xC1,0x04,0xD2,0x00 };
	DataSend(lpObj->m_Index,(LPBYTE)&InitCashShop,4);
	
	PMSG_SEND_CASHSHOP_VERS pMsg = { 0xC1,sizeof(pMsg),0xD2,0x0C }; //Script
	pMsg.Vers_Main = 512;
	pMsg.Vers_Sub = 2013;
	pMsg.Vers_Rev = 022;

	DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.Size);

	pMsg.Sub = 0x15; //Banner
	pMsg.Vers_Main = 583;
	pMsg.Vers_Sub = 2012;
	pMsg.Vers_Rev = 007;

	DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.Size);

	this->GCCashPoint(lpObj);

	return TRUE;
}

void CCashShop::GCCashPoint(LPOBJ lpObj)
{
	if(!gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID)) return;
	
	PMSG_ANS_INGAMESHOP_POINT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x01, sizeof(PMSG_ANS_INGAMESHOP_POINT));
	pMsg.Goblin = lpObj->m_GoblinPoint;
	pMsg.WCoinC[0] = lpObj->m_WCoinC;
	pMsg.WCoinC[1] = lpObj->m_WCoinC;
	pMsg.WCoinP[0] = lpObj->m_WCoinP;
	pMsg.WCoinP[1] = lpObj->m_WCoinP;

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

int CCashShop::GetItemCountInShopInventory(LPOBJ lpObj, int InventoryType)
{
	int Count = 0;
	for(int X=0; X < 30;++X){
		if(InventoryType == 0x53){
			if(lpObj->ShopInventory[X].UniqueCode != 0){
				++Count;
			}
		} else if(InventoryType == 0x47){
			if(lpObj->GiftInventory[X].UniqueCode != 0){
				++Count;
			}
		}
	}

	return Count;
}

void CCashShop::GCCashInventoryItemCount(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_INVENTORY *lpMsg)
{
	if(!gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID)) return;

	int ItemCount = this->GetItemCountInShopInventory(lpObj, lpMsg->InventoryType);

	PMSG_ANS_INGAMESHOP_ITEMCOUNT pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x06, sizeof(pMsg));

	if(ItemCount > 0){
		pMsg.OwnInv[0] = ItemCount+1;
		pMsg.OwnInv[1] = ItemCount+1;
		pMsg.GiftInv[0] = 1;
		pMsg.GiftInv[1] = 1;
	}

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	this->GCCashItemInventory(lpObj, ItemCount, lpMsg->InventoryType);
}

BOOL CCashShop::GCCashItemInventory(LPOBJ lpObj, int ItemCount, int InventoryType)
{
	PMSG_ANS_INGAMESHOP_ITEM pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x0D, sizeof(pMsg));
	pMsg.Unk3 = 0x50;
	int Count = 0;

	for(int X=0; X < 30;++X)
	{
		if(InventoryType == 0x53 && lpObj->ShopInventory[X].UniqueCode != 0 && lpObj->ShopInventory[X].AuthCode != 0){
			pMsg.UniqueCode = lpObj->ShopInventory[X].UniqueCode;
			pMsg.AuthCode = lpObj->ShopInventory[X].AuthCode;
			pMsg.UniqueValue1 = lpObj->ShopInventory[X].ItemCode1;
			pMsg.UniqueValue2 = lpObj->ShopInventory[X].ItemCode2;
			pMsg.UniqueValue3 = lpObj->ShopInventory[X].ItemCode3;
			++Count;
		} else if(InventoryType == 0x47 && lpObj->GiftInventory[X].UniqueCode != 0 && lpObj->GiftInventory[X].AuthCode != 0){
			pMsg.UniqueCode = lpObj->GiftInventory[X].UniqueCode;
			pMsg.AuthCode = lpObj->GiftInventory[X].AuthCode;
			pMsg.UniqueValue1 = lpObj->GiftInventory[X].ItemCode1;
			pMsg.UniqueValue2 = lpObj->GiftInventory[X].ItemCode2;
			pMsg.UniqueValue3 = lpObj->GiftInventory[X].ItemCode3;
			++Count;
		} else {
			continue;
		}

		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		if(Count >= ItemCount) break;
	}
	return TRUE;
}

bool CCashShop::InsertItemToInventory(LPOBJ lpObj, int UniqueCode, int AuthCode, int UniqueID1, int UniqueID2, int UniqueID3, BYTE Inventory)
{
	for(int X=0;X < 30;++X)
	{
		if(Inventory == 1){
			if(lpObj->ShopInventory[X].UniqueCode == 0 && lpObj->ShopInventory[X].AuthCode == 0){
				lpObj->ShopInventory[X].UniqueCode = UniqueCode;
				lpObj->ShopInventory[X].AuthCode = AuthCode;
				lpObj->ShopInventory[X].ItemCode1 = UniqueID1;
				lpObj->ShopInventory[X].ItemCode2 = UniqueID2;
				lpObj->ShopInventory[X].ItemCode3 = UniqueID3;
				LogAddTD("[InGameShop][%s] Add Item - UniqueID: %d, Auth: %d, Package: %d, Index: %d, Option: %d",
					lpObj->AccountID, UniqueCode, AuthCode, UniqueID1, UniqueID2, UniqueID3);
				return TRUE;
			}
		} else if(Inventory == 2){
			if(lpObj->GiftInventory[X].UniqueCode == 0 && lpObj->GiftInventory[X].AuthCode == 0){
				lpObj->GiftInventory[X].UniqueCode = UniqueCode;
				lpObj->GiftInventory[X].AuthCode = AuthCode;
				lpObj->GiftInventory[X].ItemCode1 = UniqueID1;
				lpObj->GiftInventory[X].ItemCode2 = UniqueID2;
				lpObj->GiftInventory[X].ItemCode3 = UniqueID3;
				LogAddTD("[InGameShop][%s] Add Gift - UniqueID: %d, Auth: %d, Package: %d, Index: %d, Option: %d",
					lpObj->AccountID, UniqueCode, AuthCode, UniqueID1, UniqueID2, UniqueID3);
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CCashShop::CGCashInventoryItemUse(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_ITEMUSE *lpMsg)
{
	PMSG_ANS_INGAMESHOP_ITEMUSE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x0B, sizeof(pMsg));

	if(!gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID)) return;

	if(lpObj->Connected !=PLAYER_PLAYING && lpObj->Type != OBJ_USER) return;

	INGAMESHOP_ITEMLIST * lpItem = this->SearchItemFromInventory(lpObj, lpMsg->UniqueCode, lpMsg->AuthCode);

	if(lpItem == NULL){
		pMsg.result = 1;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[InGameShop][%s][%s] Item is not Valid %d %d",lpObj->AccountID,lpObj->Name,lpMsg->UniqueCode,lpMsg->AuthCode);
		return;
	}

	if(::CheckInventoryEmptySpace(lpObj,lpItem->X,lpItem->Y) == FALSE){
		pMsg.result = 1;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[InGameShop][%s][%s] Not enough Space for Item %d %d",lpObj->AccountID,lpObj->Name,lpMsg->UniqueCode,lpMsg->AuthCode);
		return;
	}

	if(this->DeleteItemFromInventory(lpObj, lpMsg->UniqueCode, lpMsg->AuthCode) == TRUE)
	{
		if(lpItem->ItemCategory == 1){
			if(this->CheckPeriodItem(lpMsg->ItemID)){
				g_CashItemPeriodSystem.GDReqPeriodItemInsert(lpObj, lpMsg->ItemID, lpItem->Durability);
			}
		} else if(lpItem->ItemCategory == 2){
			ItemSerialCreateSend(lpObj->m_Index, 236, NULL, NULL, lpMsg->ItemID, 0, 0, 0, 0, 0, lpItem->Durability, 0, 0);
		} else {
			ItemSerialCreateSend(lpObj->m_Index, 235, NULL, NULL, lpMsg->ItemID, 0, lpItem->Durability, 0, 0, 0, lpObj->m_Index, 0, 0);
		}

		pMsg.result = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		this->GCCashPoint(lpObj);
		GDReqInGameShopItemListSave(lpObj->m_Index);
		GDReqInGameShopPointSave(lpObj->m_Index);
		LogAddTD("[InGameShop][%s][%s] Used Item Code: %d, Auth: %d, Item ID: %d", lpObj->AccountID, lpObj->Name, lpMsg->UniqueCode, lpMsg->AuthCode, lpMsg->ItemID);
	}
	return;
}


INGAMESHOP_ITEMLIST * CCashShop::SearchItemFromInventory(LPOBJ lpObj, int UniqueCode, int AuthCode)
{
	INGAMESHOP_ITEMLIST * lpItem = NULL;
	for(int X=0; X < 30;++X){
		if(lpObj->ShopInventory[X].UniqueCode == UniqueCode 
		&& lpObj->ShopInventory[X].AuthCode == AuthCode){

			lpItem = this->SearchItemList(lpObj->ShopInventory[X].ItemCode1, 
			lpObj->ShopInventory[X].ItemCode2, lpObj->ShopInventory[X].ItemCode3);
		} else if(lpObj->GiftInventory[X].UniqueCode == UniqueCode 
		&& lpObj->GiftInventory[X].AuthCode == AuthCode){

			lpItem = this->SearchItemList(lpObj->GiftInventory[X].ItemCode1,
			lpObj->GiftInventory[X].ItemCode2, lpObj->GiftInventory[X].ItemCode3);
		}
	}
	return lpItem;
}

bool CCashShop::DeleteItemFromInventory(LPOBJ lpObj, int UniqueCode, int AuthCode)
{
	for(int X=0; X < 30;++X){

		if(lpObj->ShopInventory[X].UniqueCode == UniqueCode && lpObj->ShopInventory[X].AuthCode == AuthCode){
			lpObj->ShopInventory[X].AuthCode = 0;
			lpObj->ShopInventory[X].UniqueCode = 0;
			lpObj->ShopInventory[X].ItemCode1 = 0;
			lpObj->ShopInventory[X].ItemCode2 = 0;
			lpObj->ShopInventory[X].ItemCode3 = 0;

			memcpy(&lpObj->ShopInventory[X],&lpObj->ShopInventory[X+1],sizeof(lpObj->ShopInventory));

			PMSG_REQ_INGAMESHOP_INVENTORY pList = {0};
			pList.InventoryType = 0x53;

			this->GCCashInventoryItemCount(lpObj, (PMSG_REQ_INGAMESHOP_INVENTORY *)&pList);
			return TRUE;
		} else if(lpObj->GiftInventory[X].UniqueCode == UniqueCode && lpObj->GiftInventory[X].AuthCode == AuthCode){
			lpObj->GiftInventory[X].AuthCode = 0;
			lpObj->GiftInventory[X].UniqueCode = 0;
			lpObj->GiftInventory[X].ItemCode1 = 0;
			lpObj->GiftInventory[X].ItemCode2 = 0;
			lpObj->GiftInventory[X].ItemCode3 = 0;
			PMSG_REQ_INGAMESHOP_INVENTORY pList = {0};
			pList.InventoryType = 0x47;
			
			memcpy(&lpObj->GiftInventory[X],&lpObj->GiftInventory[X+1],sizeof(lpObj->GiftInventory));

			this->GCCashInventoryItemCount(lpObj, (PMSG_REQ_INGAMESHOP_INVENTORY *)&pList);
			return TRUE;
		}
	}
	return FALSE;
}

void CCashShop::CGCashItemBuy(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_ITEMBUY *lpMsg)
{
	int iCategoryIndex = 0;
	BYTE btPosition = 0;
	BOOL bItemEmptySpace = FALSE;
	int iItemCode = 0;
	BYTE buyFlag = FALSE;
	INGAMESHOP_ITEMLIST* lpCashItemInfo = NULL;

	if(this->bCashItemListReload == TRUE) return;

	if(!gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID)) return;

	if(lpObj->Connected !=PLAYER_PLAYING && lpObj->Type != OBJ_USER) return;

	lpCashItemInfo = this->SearchItemList(lpMsg->ItemIndex, lpMsg->ItemSubIndex, lpMsg->Category, lpMsg->ItemID, lpMsg->ItemOpt);

	if(lpCashItemInfo == NULL){
		LogAddC(2,"[InGameShop][%s][%s] Unknown Item: Index:%d Sub:%d Category: %d ItemID: %d Opt: %d", 
			lpObj->AccountID, lpObj->Name, lpMsg->ItemIndex, lpMsg->ItemSubIndex, lpMsg->Category, lpMsg->ItemID,lpMsg->ItemOpt);
		return;
	}


	switch(lpCashItemInfo->BuyType)
	{
		case 0:
			if(lpObj->m_WCoinC >= lpCashItemInfo->Price){
				lpObj->m_WCoinC -= lpCashItemInfo->Price;
				buyFlag = TRUE;
			}
		break;
		case 1:
			if(lpObj->m_WCoinP >= lpCashItemInfo->Price){
				lpObj->m_WCoinP -= lpCashItemInfo->Price;
				buyFlag = TRUE;
			}
		break;

		case 2:
			if(lpObj->m_GoblinPoint >= lpCashItemInfo->Price){
				lpObj->m_GoblinPoint -= lpCashItemInfo->Price;
				buyFlag = TRUE;
			}
		break;
			
	}
	PMSG_ANS_INGAMESHOP_ITEMBUY pMsg = {-1};
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x03, sizeof(pMsg));

	if(buyFlag == TRUE)
	{
		if(lpCashItemInfo->PackageItem != 0){
			this->UsePackage(lpObj, lpCashItemInfo->PackageItem);
	
			pMsg.Result = 0;

			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

			PMSG_REQ_INGAMESHOP_INVENTORY pList = {0};
			pList.InventoryType = 0x53;

			this->GCCashInventoryItemCount(lpObj, (PMSG_REQ_INGAMESHOP_INVENTORY *)&pList);
			this->GCCashPoint(lpObj);
		}
		else if(this->GiveBuyItemToInventory(lpObj,lpCashItemInfo,1) == true)
		{
			LogAddTD("[InGameShop][%s][%s] Item Purchased Successfuly",lpObj->AccountID,lpObj->Name);

			pMsg.Result = 0;

			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

			PMSG_REQ_INGAMESHOP_INVENTORY pList = {0};
			pList.InventoryType = 0x53;

			this->GCCashInventoryItemCount(lpObj, (PMSG_REQ_INGAMESHOP_INVENTORY *)&pList);
			this->GCCashPoint(lpObj);
		}

		GDReqInGameShopItemListSave(lpObj->m_Index);
		GDReqInGameShopPointSave(lpObj->m_Index);
	} else {
		pMsg.Result = 1;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

}

bool CCashShop::UsePackage(LPOBJ lpObj, int PackageID)
{
	for(int X=0; X < this->iPackItemCount; ++X)
	{
		/*if(Iter->second->PackageID == PackageID){
			this->GiveBuyItemToInventory(lpObj, Iter->second, 1);
		}*/
	}
	return true;
}

BOOL CCashShop::CheckPeriodItemUsed(LPOBJ lpObj, int iItemCode){ //NOT IN USE
	return FALSE;
}

BOOL CCashShop::CheckPeriodItem(int iItemCode){
	if(iItemCode == ITEMGET(13,43) || 
	iItemCode == ITEMGET(13,44) || 
	iItemCode == ITEMGET(13,45) ||
	iItemCode == ITEMGET(13,54) ||
	iItemCode == ITEMGET(13,59) ||
	iItemCode == ITEMGET(13,62) ||
	iItemCode == ITEMGET(13,63) ||
	iItemCode == ITEMGET(13,93) ||
	iItemCode == ITEMGET(13,94) ||
	iItemCode == ITEMGET(13,95) ||
	iItemCode == ITEMGET(13,96) ||
	iItemCode == ITEMGET(13,104) ||
	iItemCode == ITEMGET(13,105) ||
	iItemCode == ITEMGET(14,72) ||
	iItemCode == ITEMGET(14,73) ||
	iItemCode == ITEMGET(14,74) ||
	iItemCode == ITEMGET(14,75) ||
	iItemCode == ITEMGET(14,76) ||
	iItemCode == ITEMGET(14,77)){
		return TRUE;
	}

	LogAddC(2,"[InGameShop] Buff Period Item Invalid %d",iItemCode);
	return FALSE;
}

INGAMESHOP_ITEMLIST * CCashShop::SearchItemList(int ItemIndex,int ItemSubIndex,int Category,int ItemID, int ItemOpt)
{
	INGAMESHOP_ITEMLIST* lpItemInfo = NULL;

	for(int X=0; X < this->iCashItemCount;++X)
	{
		if(ItemIndex == this->ShopItemList[X].PackageID
		&& Category == this->ShopItemList[X].Category 
		&& ItemID == ItemGetNumberMake(this->ShopItemList[X].ItemType, this->ShopItemList[X].ItemIndex) 
		&& ItemOpt == this->ShopItemList[X].ProductOption){
			lpItemInfo = &this->ShopItemList[X];
			break;
		}
	}

	return lpItemInfo;
}

INGAMESHOP_ITEMLIST * CCashShop::SearchItemList(int UniqueID1, int UniqueID2, int UniqueID3)
{
	INGAMESHOP_ITEMLIST* lpItemInfo = NULL;

	for(int X=0; X < this->iCashItemCount;++X)
	{
		if(UniqueID1 == this->ShopItemList[X].PackageID 
		&& UniqueID2 == this->ShopItemList[X].ProductIndex  
		&& UniqueID3 == this->ShopItemList[X].ProductOption){
			lpItemInfo = &this->ShopItemList[X];
			break;
		}
	}

	return lpItemInfo;
}

BOOL CCashShop::GiveBuyItemToInventory(LPOBJ lpObj, INGAMESHOP_ITEMLIST *lpItemInfo, BYTE Inventory)
{
	if(Inventory == 1)
	{
		for(int i=0;i<30;++i)
		{
			if(lpObj->ShopInventory[i].UniqueCode == 0 && lpObj->ShopInventory[i].AuthCode == 0)
			{
				lpObj->ShopInventory[i].UniqueCode = rand()%10000+4400/200;
				lpObj->ShopInventory[i].AuthCode = rand()%250000+1700/12;
				lpObj->ShopInventory[i].ItemCode1 = lpItemInfo->PackageID;
				lpObj->ShopInventory[i].ItemCode2 = lpItemInfo->ProductIndex;
				lpObj->ShopInventory[i].ItemCode3 = lpItemInfo->ProductOption;
				LogAddTD("[InGameShop][%s][%s] Item Added to Shop Inventory [UID:%d, Auth:%d, C1:%d, C2:%d, C3:%d]", lpObj->AccountID, lpObj->Name,
					lpObj->ShopInventory[i].UniqueCode, lpObj->ShopInventory[i].AuthCode,
					lpObj->ShopInventory[i].ItemCode1, lpObj->ShopInventory[i].ItemCode2, lpObj->ShopInventory[i].ItemCode3);
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CCashShop::GiveBuyItemToInventory(LPOBJ lpObj, INGAMESHOP_PACKAGELIST *lpItemInfo, BYTE Inventory)
{
	if(Inventory == 1)
	{
		for(int i=0;i<30;++i)
		{
			if(lpObj->ShopInventory[i].UniqueCode == 0 && lpObj->ShopInventory[i].AuthCode == 0)
			{
				lpObj->ShopInventory[i].UniqueCode = rand()%10000+4400/200;
				lpObj->ShopInventory[i].AuthCode = rand()%250000+1700/12;
				lpObj->ShopInventory[i].ItemCode1 = lpItemInfo->wUniqueID1;
				lpObj->ShopInventory[i].ItemCode2 = lpItemInfo->wUniqueID2;
				lpObj->ShopInventory[i].ItemCode3 = lpItemInfo->wUniqueID3;
				LogAddTD("[InGameShop][%s][%s] Package Added to Shop Inventory [UID:%d, Auth:%d, C1:%d, C2:%d, C3:%d]", lpObj->AccountID, lpObj->Name,
					lpObj->ShopInventory[i].UniqueCode, lpObj->ShopInventory[i].AuthCode,
					lpObj->ShopInventory[i].ItemCode1, lpObj->ShopInventory[i].ItemCode2, lpObj->ShopInventory[i].ItemCode3);
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL IsCashItem(int iItemCode) //GS-CS Weird Compilation
{
	switch ( iItemCode )
	{
		case ITEMGET(13,43): return TRUE;
		case ITEMGET(13,44): return TRUE;
		case ITEMGET(13,45): return TRUE;
		case ITEMGET(13,46): return TRUE;
		case ITEMGET(13,47): return TRUE;
		case ITEMGET(13,48): return TRUE;
		case ITEMGET(13,52): return TRUE;
		case ITEMGET(13,53): return TRUE;
		case ITEMGET(13,54): return TRUE;
		case ITEMGET(13,55): return TRUE;
		case ITEMGET(13,56): return TRUE;
		case ITEMGET(13,57): return TRUE;
		case ITEMGET(13,58): return TRUE;
		case ITEMGET(14,53): return TRUE;
		case ITEMGET(14,54): return TRUE;
		case ITEMGET(14,58): return TRUE;
		case ITEMGET(14,59): return TRUE;
		case ITEMGET(14,60): return TRUE;
		case ITEMGET(14,61): return TRUE;
		case ITEMGET(14,62): return TRUE;
		case ITEMGET(14,70): return TRUE;
		case ITEMGET(14,71): return TRUE;
		case ITEMGET(14,72): return TRUE;
		case ITEMGET(14,73): return TRUE;
		case ITEMGET(14,74): return TRUE;
		case ITEMGET(14,75): return TRUE;
		case ITEMGET(14,76): return TRUE;
		case ITEMGET(14,77): return TRUE;
		case ITEMGET(14,78): return TRUE;
		case ITEMGET(14,79): return TRUE;
		case ITEMGET(14,80): return TRUE;
		case ITEMGET(14,81): return TRUE;
		case ITEMGET(14,82): return TRUE;

	}

	return FALSE;
}		

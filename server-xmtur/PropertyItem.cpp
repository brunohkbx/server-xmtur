#include "StdAfx.h"
#include "PropertyItem.h"
#include "logproc.h"
#include "DSProtocol.h"
#include "DBSockMng.h"
#include "Winutil.h"

CPropertyItem	g_PropItems;

CPropertyItem::CPropertyItem(void)
{
}


CPropertyItem::~CPropertyItem(void)
{
}

bool CPropertyItem::MakePropertyItem(LPOBJ lpObj, CItem * Item, int seconds)
{
	PROPERTY_ITEM ItemStruct;

	ItemStruct.lpObj = lpObj;
	
	time_t Time;
	Time = time(NULL);
	Time = Time += seconds;

	Item->m_ExpirationItem = 1;

	ItemStruct.ExpirationDate = Time;
	ItemStruct.ItemID = Item->m_Type;
	ItemStruct.ItemSerial = Item->m_Number;
	memcpy(ItemStruct.AccountID, lpObj->AccountID, 11);

	this->m_vtPropertyItems.push_back(ItemStruct);

	LogAddTD("[Expiration Items] (%s)(%s) Set Property Item: %d Serial: %d Time: %d seconds", lpObj->AccountID, lpObj->Name, Item->m_Type, Item->m_Number, seconds);

	return true;
}

std::vector<PROPERTY_ITEM>::iterator CPropertyItem::FindPropertyItem(int serial, int ItemID, LPOBJ lpObj)
{
	std::vector<PROPERTY_ITEM>::iterator It;

	for(It = this->m_vtPropertyItems.begin(); It != this->m_vtPropertyItems.end(); ++It)
	{
		if(It->lpObj == lpObj && ItemID == It->ItemID && serial == It->ItemSerial)
		{
			return It;
		}
	}
	It = this->m_vtPropertyItems.end();
	return It;
}

void CPropertyItem::SendPropertyInfo(LPOBJ lpObj, int ItemID, int Serial, int pos)
{
	PMSG_PROPERTYITEMINFO pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x12, sizeof(pMsg));

	std::vector<PROPERTY_ITEM>::iterator It = this->FindPropertyItem(Serial,ItemID,lpObj);
	if(It == this->m_vtPropertyItems.end())
	{
		LogAddC(2, "[Expiration Items] (%s)(%s) Error: item not exist", lpObj->AccountID, lpObj->Name);
		return;
	}

	pMsg.ItemID = ItemID;
	pMsg.Pos = pos;
	pMsg.Time = It->ExpirationDate;

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

void CPropertyItem::LoadPropertyItem(int aIndex, int ItemID, int Serial, int Time)
{
	PROPERTY_ITEM ItemStruct;

	ItemStruct.ItemSerial = Serial;
	ItemStruct.ItemID = ItemID;
	ItemStruct.lpObj = &gObj[aIndex];
	ItemStruct.ExpirationDate = Time;
	memcpy(ItemStruct.AccountID, gObj[aIndex].AccountID, 11);

	this->m_vtPropertyItems.push_back(ItemStruct);

	LogAddTD("[Expiration Items] (Account %s) Loaded Item: ID:%d SERIAL:%d", gObj[aIndex].AccountID, ItemID, Serial);
}

bool CPropertyItem::IsPropertyItem(LPOBJ lpObj, int ItemID, int Serial)
{
	if(Serial == 0)
	{
		return false;
	}
	std::vector<PROPERTY_ITEM>::iterator It;

	for(It = this->m_vtPropertyItems.begin(); It != this->m_vtPropertyItems.end(); ++It)
	{
		if(It->ItemID == ItemID && It->ItemSerial == Serial && It->lpObj == lpObj)
		{
			return true;
		}
	}
	return false;
}

bool CPropertyItem::IsPropertyItem(int ItemID, int Serial)
{
	if(Serial == 0)
	{
		return false;
	}
	std::vector<PROPERTY_ITEM>::iterator It;

	for(It = this->m_vtPropertyItems.begin(); It != this->m_vtPropertyItems.end(); ++It)
	{
		if(It->ItemID == ItemID && It->ItemSerial == Serial)
		{
			return true;
		}
	}
	return false;
}

void CPropertyItem::SavePropertyItem(LPOBJ lpObj)
{
	char Buff[1024] = {0};
	int PacketSize = 0;

	ISHOP_PITEM_SAVE * pInfo = (ISHOP_PITEM_SAVE *)(Buff);
	ISHOP_PITEM_INFO * pItem = (ISHOP_PITEM_INFO *)(Buff+sizeof(ISHOP_PITEM_SAVE));
	
	std::vector<PROPERTY_ITEM>::iterator It;

	for(It = this->m_vtPropertyItems.begin(); It != this->m_vtPropertyItems.end(); ++It)
	{
		if(It->lpObj == lpObj)
		{
			pItem = (ISHOP_PITEM_INFO *)(Buff+sizeof(ISHOP_PITEM_SAVE)+pInfo->Count*sizeof(ISHOP_PITEM_INFO));

			pItem->ItemID = It->ItemID;
			pItem->Serial = It->ItemSerial;
			pItem->Time = It->ExpirationDate;
			++pInfo->Count;
		}
	}
	PacketSize = sizeof(ISHOP_PITEM_SAVE)+pInfo->Count*sizeof(ISHOP_PITEM_INFO);

	pInfo->aIndex = lpObj->m_Index;
	memcpy(pInfo->AccountID, lpObj->AccountID, 11);
	pInfo->h.c = 0xC2;
	pInfo->h.headcode = 0xD4;
	pInfo->h.sizeH = HIBYTE(PacketSize);
	pInfo->h.sizeL = LOBYTE(PacketSize);

	cDBSMng.Send((PCHAR)Buff, PacketSize);
}

void CPropertyItem::Run()
{
	time_t Time = time(NULL);
	for(int i = 0;i<this->m_vtPropertyItems.size();++i)
	{
		if(this->m_vtPropertyItems[i].lpObj->Connected < PLAYER_LOGGED)
		{
			this->m_vtPropertyItems.erase(this->m_vtPropertyItems.begin()+i);
		}
		else if(this->m_vtPropertyItems[i].lpObj->Connected == PLAYER_PLAYING)
		{
			if(this->m_vtPropertyItems[i].ExpirationDate < Time)
			{
				for(int n=0;n<76;++n)
				{
					if(this->m_vtPropertyItems[i].lpObj->pInventory[n].m_Type == this->m_vtPropertyItems[i].ItemID && this->m_vtPropertyItems[i].lpObj->pInventory[n].m_Number == this->m_vtPropertyItems[i].ItemSerial)
					{
						gObjInventoryDeleteItem(this->m_vtPropertyItems[i].lpObj->m_Index, n);
						GCInventoryItemDeleteSend(this->m_vtPropertyItems[i].lpObj->m_Index, n, 1);
						this->m_vtPropertyItems.erase(this->m_vtPropertyItems.begin()+i);
					}
				}
			}
		}
	}
}

void CPropertyItem::ErasePlayerItems(LPOBJ lpObj)
{
	for(int i=0;i<this->m_vtPropertyItems.size();++i)
	{
		if(strcmp(this->m_vtPropertyItems[i].AccountID, lpObj->AccountID) == 0)
		{
			this->m_vtPropertyItems.erase(this->m_vtPropertyItems.begin()+i);
		}
	}
}
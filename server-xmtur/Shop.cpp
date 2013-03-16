//GameServer 1.00.77 JPN - Completed
//GameServer 1.00.90 JPN - Completed
#include "Stdafx.h"
#include "GameServer.h"
#include "GameServerAuth.h"
#include "Shop.h"
#include "LogProc.h"
#include "..\include\readscript.h"
#include "GameMain.h"
#include "..\common\WzMemScript.h"

CShop ShopC[MAX_SHOP];

CShop::CShop()
{
	return;
}


CShop::~CShop()
{
	return;
}

void CShop::Init()
{
	this->SendItemDataLen = 0;
	this->ItemCount = 0;
	memset(this->ShopInventoryMap, 0, sizeof(this->ShopInventoryMap));
}

BOOL CShop::InsertItem(int type, int index, int level, int dur, int op1, int op2 ,int op3)
{
	int itemp;
	int width;
	int height;
	int x;
	int y;
	int blank;

	blank = -1;
	itemp = type * MAX_SUBTYPE_ITEMS + index;
	
	if ( itemp < 0 )
	{
		MsgBox(lMsg.Get(MSGGET(2, 53)), __FILE__, __LINE__, type, index);
		return false;
	}
	
	ItemGetSize(itemp, width, height);

	if ( width < 0 || height < 0 )
	{
		MsgBox(lMsg.Get(MSGGET(2, 54)), __FILE__, __LINE__);
		return FALSE;
	}

	for ( y=0;y<15;y++)
	{
		for ( x=0;x<8;x++)
		{
			if ( this->ShopInventoryMap[x + y*8] == 0 )
			{
				blank = this->InentoryMapCheck(x, y, width, height);

				if ( blank >= 0 )
				{
					goto skiploop;
				}
			}
		}
	}

	if ( blank < 0 )
	{
		MsgBox("error-L2 : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

skiploop:

	this->m_item[blank].m_Level = level;

	if ( dur == 0 )
	{
		dur = ItemGetDurability(ITEMGET(type, index), level, 0, 0);
	}

	this->m_item[blank].m_Durability = dur;
	this->m_item[blank].Convert(itemp, op1, op2, op3, 0, 0, 0, NULL, 0xFF, CURRENT_DB_VERSION);
	this->m_item[blank].Value();

	if(g_SocketOption.CheckItemType(itemp) == 1)//Season 4.5 addon
	{
		g_SocketOption.ClearSlotOption(&this->m_item[blank],g_iShopBuySocketItemSlotCount);
	}

	this->SendItemData[this->SendItemDataLen] = blank;
	this->SendItemDataLen++;
	ItemByteConvert((LPBYTE)&this->SendItemData[this->SendItemDataLen], this->m_item[blank]);
	this->SendItemDataLen += MAX_ITEM_INFO;
	this->ItemCount++;

	return TRUE;
}

int CShop::InentoryMapCheck(int sx, int sy, int width, int height)
{
	int x;
	int y;
	int blank = 0;

	if ( (sx+width) > 8 )
		return -1;

	if ( (sy+height) > 15 )
		return -1;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			if ( this->ShopInventoryMap[( sy + y) * 8 + (sx + x)] )
			{
				blank++;
				break;
			}
		}
	}

	if ( blank == 0 )
	{
		for(y=0;y<height;y++)
		{
			for(x=0;x<width;x++)
			{
				this->ShopInventoryMap[( sy + y) * 8 + (sx + x)] = 1;
			}
		}

		return (sx + sy*8);
	}

	return -1;
}

BOOL CShop::LoadShopItem(char* filename )
{
	int Token;
	int type;
	int index;
	int level;
	int dur;
	int op1;
	int op2;
	int op3;

	this->Init();

	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("Shop data load error %s", filename);
		return FALSE;
	}

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		if ( Token == 1 )
		{
			type = TokenNumber;

			Token = GetToken();
			index = TokenNumber;

			Token = GetToken();
			level = TokenNumber;

			Token = GetToken();
			dur = TokenNumber;

			Token = GetToken();
			op1 = TokenNumber;

			Token = GetToken();
			op2 = TokenNumber;

			Token = GetToken();
			op3 = TokenNumber;

			if (this->InsertItem(type, index, level, dur, op1, op2, op3) == FALSE )
			{
				MsgBox("error-L3 : %s %s %d", filename, __FILE__, __LINE__);
			}
		}
	}

	fclose(SMDFile);
	return true;
}

BOOL CShop::LoadShopItem(int ShopNumber)
{
	gGameServerAuth.RequestData(ShopNumber);
	int DataBufferSize = gGameServerAuth.GetDataBufferSize();
	char* DataBuffer = gGameServerAuth.GetDataBuffer();

	CWzMemScript WzMemScript;
	int Token;
	int type;
	int index;
	int level;
	int dur;
	int op1;
	int op2;
	int op3;

	WzMemScript.SetBuffer(DataBuffer, DataBufferSize);

	this->Init();

	
	while ( true )
	{
		Token = WzMemScript.GetToken();

		if ( Token == 2 )
		{
			break;
		}

		if ( Token == 1 )
		{
			type = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			index = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			level = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			dur = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			op1 = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			op2 = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			op3 = WzMemScript.GetNumber();

			if (this->InsertItem(type, index, level, dur, op1, op2, op3) == FALSE )
			{
				MsgBox("error-L3 : Shop %d", ShopNumber);
			}
		}
	}

	return TRUE;
}

BOOL ShopDataLoad() //Season 3.0 identical Season 2.5 identical gs-cs 56
{
	ShopC[0].LoadShopItem(11);
	ShopC[1].LoadShopItem(12);
	ShopC[2].LoadShopItem(13);
	ShopC[3].LoadShopItem(14);
	ShopC[4].LoadShopItem(15);
	ShopC[5].LoadShopItem(16);
	ShopC[6].LoadShopItem(17);
	ShopC[7].LoadShopItem(18);
	ShopC[8].LoadShopItem(19);
	ShopC[9].LoadShopItem(20);
	ShopC[10].LoadShopItem(21);
	ShopC[11].LoadShopItem(22);
	ShopC[12].LoadShopItem(23);

	//season 2.5 add-on
	ShopC[13].LoadShopItem(30); //WzAG 0x1E
	ShopC[14].LoadShopItem(31); //WzAG 0x1F

	//season 3.0 add-on
	ShopC[15].LoadShopItem(32); //WzAG 0x20
	ShopC[16].LoadShopItem(33); //WzAG 0x21
	ShopC[17].LoadShopItem(34); //WzAG 0x22
	
	ShopC[18].LoadShopItem(35); //WzAG 0x22
	ShopC[19].LoadShopItem(36); //WzAG 0x22
	ShopC[20].LoadShopItem(37); //WzAG 0x22
	//season 4.5 add-on
	///

	LogAdd(lMsg.Get(MSGGET(1, 209)));
	return TRUE;
}
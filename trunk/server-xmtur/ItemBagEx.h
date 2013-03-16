#ifndef ITEMBAGEX_H
#define ITEMBAGEX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemBag.h"
#include "MapClass.h"
#include "User.h"
#include "Item.h"

class CItemBagDropMapInfo
{

public:

	CItemBagDropMapInfo()
	{
		this->Init();
	};

	void Init()
	{
		this->m_bIsDrop = false;
		this->m_MinMonsterLevel = 0;
		this->m_MaxMonsterLevel = 0;
	};

	BYTE m_bIsDrop;
	BYTE m_MinMonsterLevel;
	BYTE m_MaxMonsterLevel;

};

#define MAX_ITEMBAG		150

class CItemBagAttrEx
{
public:
	CItemBagAttrEx()
	{
		m_type = 0;
		m_index = 0;
		m_minLevel = 0;
		m_maxLevel = 0;
		m_isskill = 0;
		m_isluck = 0;
		m_isoption = 0;
		m_isexitem = 0;
	}

	BYTE m_type;
	BYTE m_index;
	BYTE m_minLevel;
	BYTE m_maxLevel;
	BYTE m_isskill;
	BYTE m_isluck;
	BYTE m_isoption;
	BYTE m_isexitem;
};



class CItemBagEx
{

public:

	CItemBagEx();
	virtual ~CItemBagEx();

	void Init(LPSTR name);
	void LoadItem(LPSTR script_file);
	int DropEventItem(int aIndex);
	int DropItem(int aIndex);
	int DropRedRibbonBoxEventItem(int aIndex);
	int DropGreenRibbonBoxEventItem(int aIndex);
	int DropBlueRibbonBoxEventItem(int aIndex);
	int DropPinkChocolateBoxEventItem(int aIndex);
	int DropRedChocolateBoxEventItem(int aIndex);
	int DropBlueChocolateBoxEventItem(int aIndex);
	int DropLightPurpleCandyBoxEventItem(int aIndex);
	int DropVermilionCandyBoxEventItem(int aIndex);
	int DropDeepBlueCandyBoxEventItem(int aIndex);
	int DropKundunEventItem(int aIndex, BYTE btMapNumber,BYTE cX, BYTE cY);
	int DropCrywolfDarkElfItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	int DropCrywolfBossMonsterItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	int DropKanturuMayaHandItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	int DropKanturuNightmareItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);
	int DropHallowinEventItem(LPOBJ lpObj);
	int DropCastleHuntZoneBossReward(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY);

	BYTE GetMinLevel(int n);
	BYTE GetMaxLevel(int n);
	int PickItem(CItem& objItem, int& nItemIndex);

private:

	int GetBagCount(){return this->m_iBagObjectCount;}
	int IsEnableEventItemDrop(int aIndex);
	BYTE GetLevel(int n);
private:

	BOOL m_bLoad;	// 4
	char m_sEventName[255];	// 8
	int m_iEventItemType;	// 108
	int m_iEventItemLevel;	// 10C
	int m_iDropZen;	// 110
	int m_iEventItemDropRate;	// 114
	int m_iItemDropRate;	// 118
	int m_iExItemDropRate;	// 11C
	int m_iBagObjectCount;	// 120
	CItemBagDropMapInfo DropMapInfo[MAX_NUMBER_MAP];	// 124
	CItemBagAttrEx BagObject[MAX_ITEMBAG_ATTR];	// 19C
};

#endif
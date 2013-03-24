#include "Stdafx.h"
#include "AcceptIp.h"
#include "wsJoinServerCli.h"
#include "GameMain.h"
#include "GameServer.h"
#include "DirPath.h"
#include "LogProc.h"
#include "gameserverauth.h"
#include "DirPath.h"
#include "DSProtocol.h"
#include "CLoginCount.h"
#include "MapClass.h"
#include "wsGameServer.h"
#include "EledoradoEvent.h"
#include "WhisperCash.h"
#include "SProtocol.h"
#include "ChaosCastle.h"
#include "DevilSquare.h"
#include "Gate.h"
#include "User.h"
#include "Event.h"
#include "PacketCheckSum.h"
#include "DragonEvent.h"
#include "AttackEvent.h"
#include "SendHackLog.h"
#include "SimpleModulus.h"
#include "MapServerManager.h"
#include "GGSvr.h"
#include "QuestInfo.h"
#include "SkillHitBox.h"
#include "EventManagement.h"
#include "EDSProtocol.h"
#include "MonsterItemMng.h"
#include "ConMember.h"
#include "GMMng.h"
#include "BattleSoccerManager.h"
#include "GuildClass.h"
#include "MoveCommand.h"
#include "MixSystem.h"
#include "BloodCastle.h"
#include "Shop.h"
#include "ItemAddOption.h"
#include "CrywolfSync.h"
#include "Kanturu.h"
#include "KanturuMonsterMng.h"
#include "SkillAdditionInfo.h"
#include "TMonsterSkillElement.h"
#include "TMonsterSkillUnit.h"
#include "TMonsterSkillManager.h"
#include "TMonsterAIElement.h"
#include "TMonsterAIAutomata.h"
#include "TMonsterAIUnit.h"
#include "TMonsterAIRule.h"
#include "TMonsterAIGroup.h"
#include "CashShop.h"
#include "CashItemPeriodSystem.h"
#include "CashLotterySystem.h"
#include "ProbabilityItemBag.h"
#include "IllusionTempleEvent.h"
#include "BuffEffect.h"
#include "MasterLevelSystem.h"
#include "MasterSkillSystem.h"
#include "ChaosCard.h"
#include "Raklion.h"
#include "PCBangPointSystem.h"
#include "XmasAttackEvent.h"
#include "QuestManager.h"
#include "Doppelganger.h"
#include "ImperialGuardian.h"
#include "WarpSystem.h"
#include "DuelManager.h"
#include "ExtremePoints.h"
#include "BloodCastleBag.h"

#include "MarrySystem.h"
#include "NewsSystem.h"
#include "PartySystem.h"
#include "SafeMapSystem.h"
#include "GMManager.h"
#include "InfernoEvent.h"
#include "GamblingSystem.h"

#include "CrywolfAltar.h"
#include "Winutil.h"
#include "SetItemOption.h"
#include "ClassDef.h"
#include "Prodef.h"

CConfigs Configs;

int MapMinUserLevel[MAX_NUMBER_MAP] = { 0, 20, 15, 10, 80, 0, 0, 60, 130, 0, 160, 10, 36, 80, 130, 170, 210, 310, 15, 30, 100, 160, 220, 280, 15, 50, 120, 180, 240, 300, 10, 10, 0};

BOOL JoinServerConnected;
BOOL DataServerConnected;
BOOL GameServerCreated;
BOOL DevilSquareEventConnect;
BOOL IsDevilSquareEventConnected;
BOOL EventChipServerConnect;
BOOL IsEventChipServerConnected;
BOOL GSInfoSendFlag;

CDragonEvent * DragonEvent;
CAttackEvent * AttackEvent;
CItemBag * LuckboxItemBag;
CItemBag * Mon55;
CItemBag * Mon53;
CItemBagEx * StarOfXMasItemBag;
CItemBag * FireCrackerItemBag;
CItemBag * HeartOfLoveItemBag;
CItemBag * GoldMedalItemBag;
CItemBag * SilverMedalItemBag;
CItemBag * EventChipItemBag;
CProbabilityItemBag * GoldGoblenItemBag;
CProbabilityItemBag * TitanItemBag;
CProbabilityItemBag * GoldDerconItemBag;
CProbabilityItemBag * DevilLizardKingItemBag;
CProbabilityItemBag * KanturItemBag;
CItemBag * RingEventItemBag;
CItemBag * FriendShipItemBag;
CItemBag * DarkLordHeartItemBag;
CItemBagEx * KundunEventItemBag;
CItemBagEx * HiddenTreasureBoxItemBag;
CItemBagEx * RedRibbonBoxEventItemBag;
CItemBagEx * GreenRibbonBoxEventItemBag;
CItemBagEx * BlueRibbonBoxEventItemBag;
CItemBagEx * PinkChocolateBoxEventItemBag;
CItemBagEx * RedChocolateBoxEventItemBag;
CItemBagEx * BlueChocolateBoxEventItemBag;
CItemBagEx * LightPurpleCandyBoxEventItemBag;
CItemBagEx * VermilionCandyBoxEventItemBag;
CItemBagEx * DeepBlueCandyBoxEventItemBag;
CItemBagEx * CrywolfDarkElfItemBag;
CItemBagEx * CrywolfBossMonsterItemBag;
CItemBagEx * CastleItemMixItemBag;
CItemBagEx * CastleHuntZoneBossItemBag;
CItemBagEx * KanturuMayaHandItemBag;
CItemBagEx * KanturuNightmareItemBag;
CItemBagEx * HallowinDayEventItemBag;
CItemBag * RingOfHeroBoxItemBag;
CProbabilityItemBag * NewYearLuckyPouchItemBag;
CProbabilityItemBag * GMPresentBoxItemBag;
CProbabilityItemBag * IllusionTemple1ItemBag;
CProbabilityItemBag * IllusionTemple2ItemBag;
CProbabilityItemBag * IllusionTemple3ItemBag;
CProbabilityItemBag * MoonHarvestItemBag;
CProbabilityItemBag * MoonHarvestItemBag2;
CProbabilityItemBag * CherryBlossom1;
CProbabilityItemBag * CherryBlossom2;
CProbabilityItemBag * CherryBlossom3;
CProbabilityItemBag * CherryBlossom4;
CProbabilityItemBag * PCBangGageGreenBox;
CProbabilityItemBag * PCBangGageRedBox;
CProbabilityItemBag * PCBangGagePurpleBox;
CProbabilityItemBag * ReservedBox;
CProbabilityItemBag * RaklionSelupanItemBag;
CProbabilityItemBag * XMasEventA;
CProbabilityItemBag * XMasEventB;
CProbabilityItemBag * XMasEventC;
CProbabilityItemBag * LuckyCoin10;
CProbabilityItemBag * LuckyCoin20;
CProbabilityItemBag * LuckyCoin30;


CProbabilityItemBag * MedussaItemBag;

CProbabilityItemBag * SilverBoxItemBag;
CProbabilityItemBag * GoldenBoxItemBag;

CGamblingItemBag * pGamblingSystemItemBag=NULL;	


CwsGameServer cGameServer;	// line : 213GameServer

wsJoinServerCli cJoinServer;	// line : 214 Join Server
wsJoinServerCli cDataServer;	// line : 215 DataServer
wsJoinServerCli cExDB;	// line : 239 Extra DataBase Server
wsJoinServerCli cRankingServer;	// line : 244 Ranking Server
wsJoinServerCli cEventServer; // line : 246 EVENT_MU2003

WzUdp CSSocket;	// line 256

CDirPath gDirPath;	// line : 248 Directory Path
MapClass MapC[MAX_NUMBER_MAP];	// line 249	// Map Manager

CMonsterAttr gMAttr;	// line 250
CMonsterSetBase gMSetBase;	// line 251
classdef DCInfo;	// line 252:

CWhisperCash WhisperCash;	// line 253
PartyClass gParty;	// line 254
CDbSave gDbSave;	// line 255
CGuildClass Guild;	// line 265

CMsg lMsg;	// line 324
NSerialCheck gNSerialCheck[OBJMAX];	// line 326
CLoginCount gLCount[3];	// line 329

CSimpleModulus g_SimpleModulusCS;	// line 751
CSimpleModulus g_SimpleModulusSC;	// line 752

void CheckSumFileLoad(char* szCheckSum);

void gSetDate()
{
	tm *today;
	time_t ltime;

	time(&ltime);
	today=localtime(&ltime);

	today->tm_year=today->tm_year+1900;
	Configs.Year=today->tm_year;
}

BOOL gJoomin15Check(char* szJN)
{
	int tyear=1900;
	char szTyear[3]="";
	
	if ( szJN[6] == 51 || szJN[6] == 52)
	{
		tyear = 2000;
	}

	memcpy(&szTyear[0], szJN, sizeof(szTyear)-1);

	tyear=tyear+atoi(&szTyear[0]);
	if ( (Configs.Year-15 ) < tyear )
	{
		return 0;
	}
	return 1;
}

BOOL gJoominCheck(char* szJN, int iLimitAge)
{

	if ( iLimitAge <0 )
	{
		return 1;
	}

	int tyear=1900;
	char szTyear[3]="";

	if ( szJN[6] == 51 || szJN[6] == 52)
	{
		tyear = 2000;
	}

	memcpy(&szTyear[0], szJN, sizeof(szTyear)-1);

	tyear=tyear+atoi(&szTyear[0]);
	if ( (Configs.Year-iLimitAge ) < tyear )
	{
		return 0;
	}
	
	return 1;
}	




void CConfigs::LoadConfigs(){

	WIN32_FIND_DATAA wFD;
	if(FindFirstFileA(CONFIG_FILE,&wFD)==INVALID_HANDLE_VALUE){
		MessageBoxA(0,"Failed to open XTR Configuration File","Error",MB_OK);
		ExitProcess(0);
	}

	//Load all Configs
	this->PostEnable = GetPrivateProfileInt("Post","IsEnable",1,CONFIG_FILE);
	this->PostLevel = GetPrivateProfileInt("Post","Level",50,CONFIG_FILE);
	this->PostCost = GetPrivateProfileInt("Post","Cost",100000,CONFIG_FILE);
	this->PostColor = GetPrivateProfileInt("Post","Color",1,CONFIG_FILE);

	this->AddStatsEnable = GetPrivateProfileInt("AddStats","IsEnable",1,CONFIG_FILE);
	this->AddStatsMaxPoints = GetPrivateProfileInt("AddStats","MaxPoints",32000,CONFIG_FILE);
	this->AddStatsLevel = GetPrivateProfileInt("AddStats","Level",50,CONFIG_FILE);
	this->AddStatsCost = GetPrivateProfileInt("AddStats","Cost",100000,CONFIG_FILE);

	this->ItemDropEnable = GetPrivateProfileInt("ItemDrop","IsEnable",1,CONFIG_FILE);

	this->PKClearEnable = GetPrivateProfileInt("PKClear","IsEnable",1,CONFIG_FILE);
	this->PKClearCost = GetPrivateProfileInt("PKClear","Cost",1000000,CONFIG_FILE);
	this->PKClearLevel = GetPrivateProfileInt("PKClear","Level",100,CONFIG_FILE);
	this->PKClearMap = GetPrivateProfileInt("PKClear","MapNumber",1,CONFIG_FILE);

	this->MultiVaultEnable = GetPrivateProfileInt("MultiVault","IsEnable",1,CONFIG_FILE);
	this->MultiVaultMax = GetPrivateProfileInt("MultiVault","MaxVaults",3,CONFIG_FILE);
	this->MultiVaultFloodTime = GetPrivateProfileInt("MultiVault","FloodTime",5,CONFIG_FILE);

	//Summoner Creation
	this->SummonerCreation_WOCard = GetPrivateProfileInt("Common","Enable_SummonerCreation",1,CONFIG_FILE);

	//MasterLevel AddZen
	this->ML_AddZen = GetPrivateProfileInt("Mastering","ML_AddZen",50,CONFIG_FILE);

	//Payment Manager
	this->Payment_IsEnable = GetPrivateProfileInt("PaymentSystem","IsEnable",1,CONFIG_FILE);
	this->Payment_ExtraExperience = GetPrivateProfileInt("PaymentSystem","ExtraExperience_Percent",100,CONFIG_FILE);


	//Dark Wizard
	this->MaxStr[0] = GetPrivateProfileInt("MaxStats","DarkWizard_MaxStrength",32000,CONFIG_FILE);
	this->MaxAgi[0] = GetPrivateProfileInt("MaxStats","DarkWizard_MaxAgility",32000,CONFIG_FILE);
	this->MaxVit[0] = GetPrivateProfileInt("MaxStats","DarkWizard_MaxVitality",32000,CONFIG_FILE);
	this->MaxEne[0] = GetPrivateProfileInt("MaxStats","DarkWizard_MaxEnergy",32000,CONFIG_FILE);

	//Dark Knight
	this->MaxStr[1] = GetPrivateProfileInt("MaxStats","DarkKnight_MaxStrength",32000,CONFIG_FILE);
	this->MaxAgi[1] = GetPrivateProfileInt("MaxStats","DarkKnight_MaxAgility",32000,CONFIG_FILE);
	this->MaxVit[1] = GetPrivateProfileInt("MaxStats","DarkKnight_MaxVitality",32000,CONFIG_FILE);
	this->MaxEne[1] = GetPrivateProfileInt("MaxStats","DarkKnight_MaxEnergy",32000,CONFIG_FILE);

	//Fairy Elf
	this->MaxStr[2] = GetPrivateProfileInt("MaxStats","FairyElf_MaxStrength",32000,CONFIG_FILE);
	this->MaxAgi[2] = GetPrivateProfileInt("MaxStats","FairyElf_MaxAgility",32000,CONFIG_FILE);
	this->MaxVit[2] = GetPrivateProfileInt("MaxStats","FairyElf_MaxVitality",32000,CONFIG_FILE);
	this->MaxEne[2] = GetPrivateProfileInt("MaxStats","FairyElf_MaxEnergy",32000,CONFIG_FILE);

	//Magic Gladiator
	this->MaxStr[3] = GetPrivateProfileInt("MaxStats","MagicGladiator_MaxStrength",32000,CONFIG_FILE);
	this->MaxAgi[3] = GetPrivateProfileInt("MaxStats","MagicGladiator_MaxAgility",32000,CONFIG_FILE);
	this->MaxVit[3] = GetPrivateProfileInt("MaxStats","MagicGladiator_MaxVitality",32000,CONFIG_FILE);
	this->MaxEne[3] = GetPrivateProfileInt("MaxStats","MagicGladiator_MaxEnergy",32000,CONFIG_FILE);

	//Dark Lord
	this->MaxStr[4] = GetPrivateProfileInt("MaxStats","DarkLord_MaxStrength",32000,CONFIG_FILE);
	this->MaxAgi[4] = GetPrivateProfileInt("MaxStats","DarkLord_MaxAgility",32000,CONFIG_FILE);
	this->MaxVit[4] = GetPrivateProfileInt("MaxStats","DarkLord_MaxVitality",32000,CONFIG_FILE);
	this->MaxEne[4] = GetPrivateProfileInt("MaxStats","DarkLord_MaxEnergy",32000,CONFIG_FILE);
	this->MaxCom = GetPrivateProfileInt("MaxStats","DarkLord_MaxCommand",32000,CONFIG_FILE);

	//Summoner
	this->MaxStr[5] = GetPrivateProfileInt("MaxStats","Summoner_MaxStrength",32000,CONFIG_FILE);
	this->MaxAgi[5] = GetPrivateProfileInt("MaxStats","Summoner_MaxAgility",32000,CONFIG_FILE);
	this->MaxVit[5] = GetPrivateProfileInt("MaxStats","Summoner_MaxVitality",32000,CONFIG_FILE);
	this->MaxEne[5] = GetPrivateProfileInt("MaxStats","Summoner_MaxEnergy",32000,CONFIG_FILE);

	GetPrivateProfileString("Common","ConnectMessage","Welcome to MU Continent",this->ConnectMessage,sizeof(this->ConnectMessage),CONFIG_FILE);

	this->DisableCheckSum = GetPrivateProfileInt("Fixes","DisableCheckSum",1,CONFIG_FILE);

	//Items
	this->EnableExcellentWithAncient = GetPrivateProfileInt("Items","EnableExcellentWithAncient",0,CONFIG_FILE);
	this->EnableExcellentWithSocket = GetPrivateProfileInt("Items","EnableExcellentWithSocket",0,CONFIG_FILE);
	this->JewelOfLifeMaxOption = GetPrivateProfileInt("Fixes","RemoveOptionsLimit",0,CONFIG_FILE);
	this->RemovePotionLimit = GetPrivateProfileInt("Fixes","RemovePotionLimit",0,CONFIG_FILE);
	this->RemovePersonalID = GetPrivateProfileInt("Fixes","RemovePersonalID",0,CONFIG_FILE);
	this->FixMarlonQuestRemove = GetPrivateProfileInt("Fixes","RemoveMarlonQuestReset",0,CONFIG_FILE);
	this->FixPStoreLevel5Remove = GetPrivateProfileInt("Fixes","RemovePShopItemsCheckLeve5",0,CONFIG_FILE);
	this->FixSerial0Items = GetPrivateProfileInt("Fixes","FixSerial0ItemsCheck",0,CONFIG_FILE);
	this->FixRemovePKPartySystem = GetPrivateProfileInt("Fixes","RemoveWebZenPKPartySystem",0,CONFIG_FILE);
	this->FixMasterLevelEvents = GetPrivateProfileInt("Fixes","RemoveMastersEventsLimit",0,CONFIG_FILE);
	this->EnablePKPlayersInEvents = GetPrivateProfileInt("Fixes","RemovePKEventsCheck",0,CONFIG_FILE);
	this->EnableSiegeSkills = GetPrivateProfileInt("Fixes","EnableSiegeSkills",0,CONFIG_FILE);
	this->RemoveAntiHack = GetPrivateProfileInt("Fixes","RemoveAntiHacks",1,CONFIG_FILE);
	this->RemoveCastleMixLimit = GetPrivateProfileInt("CastleMix","RemoveMixLimit",0,CONFIG_FILE);
	this->RemovePartyZenPercent = GetPrivateProfileInt("Fixes","RemovePartyZenPercent",0,CONFIG_FILE);
	this->RemovePKItemDrop = GetPrivateProfileInt("Fixes","RemovePKItemDrop",0,CONFIG_FILE);

	//Customs Configs
	//Events
	this->BC_Group = GetPrivateProfileInt("BloodCastle","BloodCastle_Group",12,CONFIG_FILE);
	this->BC_ID = GetPrivateProfileInt("BloodCastle","BloodCastle_ID",15,CONFIG_FILE);
	this->BC_Level = GetPrivateProfileInt("BloodCastle","BloodCastle_Level",0,CONFIG_FILE);
	this->BC_Skill = GetPrivateProfileInt("BloodCastle","BloodCastle_Skill",0,CONFIG_FILE);
	this->BC_Luck = GetPrivateProfileInt("BloodCastle","BloodCastle_Luck",0,CONFIG_FILE);
	this->BC_Dur = GetPrivateProfileInt("BloodCastle","BloodCastle_Durability",0,CONFIG_FILE);
	this->BC_Opt = GetPrivateProfileInt("BloodCastle","BloodCastle_Option",0,CONFIG_FILE);
	this->BC_Exc = GetPrivateProfileInt("BloodCastle","BloodCastle_Excellent",0,CONFIG_FILE);
	this->BC_Anc = GetPrivateProfileInt("BloodCastle","BloodCastle_Ancient",0,CONFIG_FILE);
	
	this->IT_Group = GetPrivateProfileInt("IllusionTemple","IllusionTemple_Group",12,CONFIG_FILE);
	this->IT_ID = GetPrivateProfileInt("IllusionTemple","IllusionTemple_ID",15,CONFIG_FILE);
	this->IT_Level = GetPrivateProfileInt("IllusionTemple","IllusionTemple_Level",0,CONFIG_FILE);
	this->IT_Skill = GetPrivateProfileInt("IllusionTemple","IllusionTemple_Skill",0,CONFIG_FILE);
	this->IT_Luck = GetPrivateProfileInt("IllusionTemple","IllusionTemple_Luck",0,CONFIG_FILE);
	this->IT_Dur = GetPrivateProfileInt("IllusionTemple","IllusionTemple_Durability",0,CONFIG_FILE);
	this->IT_Opt = GetPrivateProfileInt("IllusionTemple","IllusionTemple_Option",0,CONFIG_FILE);
	this->IT_Exc = GetPrivateProfileInt("IllusionTemple","IllusionTemple_Excellent",0,CONFIG_FILE);
	this->IT_Anc = GetPrivateProfileInt("IllusionTemple","IllusionTemple_Ancient",0,CONFIG_FILE);
	this->IT_MinPlayers = GetPrivateProfileInt("IllusionTemple","IllusionTemple_MinPlayers",4,CONFIG_FILE);
	
	this->WW_Group = GetPrivateProfileInt("WhiteWizard","WhiteWizard_Group",12,CONFIG_FILE);
	this->WW_ID = GetPrivateProfileInt("WhiteWizard","WhiteWizard_ID",15,CONFIG_FILE);
	this->WW_Level = GetPrivateProfileInt("WhiteWizard","WhiteWizard_Level",0,CONFIG_FILE);
	this->WW_Skill = GetPrivateProfileInt("WhiteWizard","WhiteWizard_Skill",0,CONFIG_FILE);
	this->WW_Luck = GetPrivateProfileInt("WhiteWizard","WhiteWizard_Luck",0,CONFIG_FILE);
	this->WW_Dur = GetPrivateProfileInt("WhiteWizard","WhiteWizard_Durability",1,CONFIG_FILE);
	this->WW_Opt = GetPrivateProfileInt("WhiteWizard","WhiteWizard_Option",0,CONFIG_FILE);
	this->WW_Exc = GetPrivateProfileInt("WhiteWizard","WhiteWizard_Excellent",0,CONFIG_FILE);
	this->WW_Anc = GetPrivateProfileInt("WhiteWizard","WhiteWizard_Ancient",0,CONFIG_FILE);

	this->GDragonBoxCount = GetPrivateProfileInt("GoldenInvasion","GoldenDragon_BoxCountDrop",5,CONFIG_FILE);

	this->CCDropGroup1 = GetPrivateProfileInt("ChaosCastle","ChaosCastle_Prize1_Group",12,CONFIG_FILE);
	this->CCDropID1 = GetPrivateProfileInt("ChaosCastle","ChaosCastle_Prize1_ID",15,CONFIG_FILE);
	this->CCDropGroup2 = GetPrivateProfileInt("ChaosCastle","ChaosCastle_Prize2_Group",12,CONFIG_FILE);
	this->CCDropID2 = GetPrivateProfileInt("ChaosCastle","ChaosCastle_Prize2_ID",15,CONFIG_FILE);
	this->CCDropGroup3 = GetPrivateProfileInt("ChaosCastle","ChaosCastle_Prize3_Group",12,CONFIG_FILE);
	this->CCDropID3 = GetPrivateProfileInt("ChaosCastle","ChaosCastle_Prize3_ID",15,CONFIG_FILE);
	this->CCDropGroup4 = GetPrivateProfileInt("ChaosCastle","ChaosCastle_Prize4_Group",12,CONFIG_FILE);
	this->CCDropID4 = GetPrivateProfileInt("ChaosCastle","ChaosCastle_Prize4_ID",15,CONFIG_FILE);
	this->CCMinPlayers = GetPrivateProfileInt("ChaosCastle","ChaosCastle_MinPlayers",2,CONFIG_FILE);
	this->CCItem1 = ITEMGET(this->CCDropGroup1,this->CCDropID1);
	this->CCItem2 = ITEMGET(this->CCDropGroup2,this->CCDropID2);
	this->CCItem3 = ITEMGET(this->CCDropGroup2,this->CCDropID3);
	this->CCItem4 = ITEMGET(this->CCDropGroup4,this->CCDropID4);

	//Pets
	this->DarkHorseMixRate = GetPrivateProfileInt("PetsMix","DarkHorseMix_SuccessRate",50,CONFIG_FILE);
	this->DarkHorseMixCost = GetPrivateProfileInt("PetsMix","DarkHorse_Price",500000,CONFIG_FILE);
	this->DarkRavenMixRate = GetPrivateProfileInt("PetsMix","DarkRavenMix_SuccessRate",80,CONFIG_FILE);
	this->DarkRavenMixCost = GetPrivateProfileInt("PetsMix","DarkRaven_Price",2500000,CONFIG_FILE);
	this->DinorantMixRate  = GetPrivateProfileInt("PetsMix","DinorantMix_SuccessRate",60,CONFIG_FILE);
	this->FruitMixRate = GetPrivateProfileInt("FruitMix","FruitMix_SuccessRate",60,CONFIG_FILE);

	//Chaos Machine Mix's
	this->SoulPotionMixRate = GetPrivateProfileInt("PotionsMix","SoulPotion_SuccessRate",100,CONFIG_FILE);
	this->SoulPotionMixCost = GetPrivateProfileInt("PotionsMix","SoulPotion_Cost",100000,CONFIG_FILE);
	this->BlessPotionMixRate= GetPrivateProfileInt("PotionsMix","BlessPotion_SuccessRate",100,CONFIG_FILE);
	this->BlessPotionMixCost= GetPrivateProfileInt("PotionsMix","BlessPotion_Cost",50000,CONFIG_FILE);
	this->WingsLvl2MixRate = GetPrivateProfileInt("WingsMix","Level2Wings_SuccessRate",25,CONFIG_FILE);
	this->WingsLvl3MixRate  = GetPrivateProfileInt("WingsMix","Level3Wings_SuccessRate",70,CONFIG_FILE);
	this->CondorMixRate = GetPrivateProfileInt("WingsMix","CondorMix_SuccessRate",70,CONFIG_FILE);

	//Plus Items
	this->DefaultMixRate = GetPrivateProfileInt("PlusItemsMixs","Default_PlusMixRate",45,CONFIG_FILE);
	this->DefaultMixRateWLuck  = GetPrivateProfileInt("PlusItemsMixs","PlusMixWLuck_AddRate",20,CONFIG_FILE);
	this->DefaultMixMaxRate= GetPrivateProfileInt("PlusItemsMixs","PlusMix_MaxRate",75,CONFIG_FILE);
	this->Items10MixRate  = GetPrivateProfileInt("PlusItemsMixs","PlusMix+10_Normal_SuccesRate",60,CONFIG_FILE);
	this->Items10SpecialRate= GetPrivateProfileInt("PlusItemsMixs","PlusMix+10_Special_SuccesRate",60,CONFIG_FILE);
	this->Items10WingsRate = GetPrivateProfileInt("PlusItemsMixs","PlusMix+10_Wings_SuccesRate",50,CONFIG_FILE);
	this->Items11MixRate = GetPrivateProfileInt("PlusItemsMixs","PlusMix+11_+12_Normal_SuccesRate",60,CONFIG_FILE);
	this->Items11SpecialRate = GetPrivateProfileInt("PlusItemsMixs","PlusMix+11_+12_Special_SuccesRate",60,CONFIG_FILE);
	this->Items11WingsRate = GetPrivateProfileInt("PlusItemsMixs","PlusMix+11_+12_Wings_SuccesRate",50,CONFIG_FILE);
	this->Items13MixRate= GetPrivateProfileInt("PlusItemsMixs","PlusMix+13_Normal_SuccesRate",60,CONFIG_FILE);
	this->Items13SpecialRate= GetPrivateProfileInt("PlusItemsMixs","PlusMix+13_Special_SuccesRate",60,CONFIG_FILE);
	this->Items13WingsRate = GetPrivateProfileInt("PlusItemsMixs","PlusMix+13_Wings_SuccesRate",50,CONFIG_FILE);
	this->Items14MixRate = GetPrivateProfileInt("PlusItemsMixs","PlusMix+14_SuccessRate",50,CONFIG_FILE);
	this->Items15MixRate = GetPrivateProfileInt("PlusItemsMixs","PlusMix+15_SuccessRate",50,CONFIG_FILE);

	//Kundun Configs
	this->KundunAncientDrop = GetPrivateProfileInt("Kundun","KundunMonster_AncientDropRate",2500,CONFIG_FILE);
	this->KundunItemDrop = GetPrivateProfileInt("Kundun","KundunMonster_ItemDropCount",3,CONFIG_FILE);

	//Great Golden Dercon
	this->GreatGoldenDragonCountDrop = GetPrivateProfileInt("GoldenInvasion","GreatGoldenDragon_BoxCountDrop",3,CONFIG_FILE);

	//Raklion Item Drop
	this->SelupanItemDrop = GetPrivateProfileInt("Raklion","Selupan_ItemCount_Drop",10,CONFIG_FILE);

	//Castle Siege Mix
	this->CastleMixRate = GetPrivateProfileInt("CastleMix","CastleMix_SuccessRate",100,CONFIG_FILE);

	//Kalima Gate
	this->KalimaGateDissapearTime = GetPrivateProfileInt("Kalima","Gate_DissapearTime",2,CONFIG_FILE);

	//LevelPoints
	this->DLPoints = GetPrivateProfileInt("LevelUpPoints","DarkLord_LvLPoints",7,CONFIG_FILE);
	this->MGPoints = GetPrivateProfileInt("LevelUpPoints","MagicGladiator_LvLPoints",7,CONFIG_FILE);
	this->NormalPoints = GetPrivateProfileInt("LevelUpPoints","Normal_LvLPoints",5,CONFIG_FILE);
	this->MarlonPoints = GetPrivateProfileInt("LevelUpPoints","Marlon_PlusLvLPoints",5,CONFIG_FILE);
	this->BCDLMGPoints = GetPrivateProfileInt("LevelUpPoints","BloodCastle_DL_MG_LvLPoints",7,CONFIG_FILE);
	this->BCNormalPoints= GetPrivateProfileInt("LevelUpPoints","BloodCastle_Normal_LvLPoints",5,CONFIG_FILE);
	this->BCMarlonPoints = GetPrivateProfileInt("LevelUpPoints","BloodCastle_Marlon_PlusLvLPoints",5,CONFIG_FILE);
	this->CCDLMGPoints = GetPrivateProfileInt("LevelUpPoints","ChaosCastle_DL_MG_LvLPoints",7,CONFIG_FILE);
	this->CCNormalPoints  = GetPrivateProfileInt("LevelUpPoints","ChaosCastle_Normal_LvLPoints",5,CONFIG_FILE);
	this->CCMarlonPoints = GetPrivateProfileInt("LevelUpPoints","ChaosCastle_Marlon_PlusLvLPoints",5,CONFIG_FILE);
	this->ITDLMGPoints = GetPrivateProfileInt("LevelUpPoints","IllusionTemple_DL_MG_LvLPoints",7,CONFIG_FILE);
	this->ITNormalPoints = GetPrivateProfileInt("LevelUpPoints","IllusionTemple_Normal_LvLPoints",5,CONFIG_FILE);
	this->ITMarlonPoints = GetPrivateProfileInt("LevelUpPoints","IllusionTemple_Marlon_PlusLvLPoints",5,CONFIG_FILE);
	this->CWDLMGPoints = GetPrivateProfileInt("LevelUpPoints","Crywolf_DL_MG_LvLPoints",7,CONFIG_FILE);
	this->CWNormalPoints = GetPrivateProfileInt("LevelUpPoints","Crywolf_Normal_LvLPoints",5,CONFIG_FILE);
	this->CWMarlonPoints = GetPrivateProfileInt("LevelUpPoints","Crywolf_Marlon_PlusLvLPoints",5,CONFIG_FILE);

	//Summon Monsters
	this->Summon1 = GetPrivateProfileInt("SummonMonsters","Summon_Globlin",26,CONFIG_FILE);
	this->Summon2 = GetPrivateProfileInt("SummonMonsters","Summon_StoneGolem",32,CONFIG_FILE);
	this->Summon3 = GetPrivateProfileInt("SummonMonsters","Summon_Asassin",20,CONFIG_FILE);
	this->Summon4 = GetPrivateProfileInt("SummonMonsters","Summon_EliteYeti",20,CONFIG_FILE);
	this->Summon5 = GetPrivateProfileInt("SummonMonsters","Summon_DarkKnight",10,CONFIG_FILE);
	this->Summon6 = GetPrivateProfileInt("SummonMonsters","Summon_Bali",150,CONFIG_FILE);
	this->Summon7 = GetPrivateProfileInt("SummonMonsters","Summon_Soldier",151,CONFIG_FILE);

	//Transform Rings
	this->Ring1 = GetPrivateProfileInt("TransformationRings","Ring_BudgeDragon",2,CONFIG_FILE);
	this->Ring2 = GetPrivateProfileInt("TransformationRings","Ring_Giant",7,CONFIG_FILE);
	this->Ring3 = GetPrivateProfileInt("TransformationRings","Ring_Skeleton",14,CONFIG_FILE);
	this->Ring4 = GetPrivateProfileInt("TransformationRings","Ring_ThunderLich",8,CONFIG_FILE);
	this->Ring5 = GetPrivateProfileInt("TransformationRings","Ring_PoisonBull",9,CONFIG_FILE);
	this->Ring6 = GetPrivateProfileInt("TransformationRings","Ring_DeathCow",41,CONFIG_FILE);

	//Jewel Prices
	this->SoulPrice  = GetPrivateProfileInt("ItemsPrice","JoBlessPrice",9000000,CONFIG_FILE);
	this->BlessPrice = GetPrivateProfileInt("ItemsPrice","JoSoulPrice",6000000,CONFIG_FILE);
	this->ChaosPrice = GetPrivateProfileInt("ItemsPrice","JoChaosPrice",810000,CONFIG_FILE);
	this->CreationPrice = GetPrivateProfileInt("ItemsPrice","JoLifePrice",30000000,CONFIG_FILE);
	this->LifePrice = GetPrivateProfileInt("ItemsPrice","JoCreationPrice",45000000,CONFIG_FILE);
	this->FruitPrice = GetPrivateProfileInt("ItemsPrice","FruitsPrice",3000000,CONFIG_FILE);
	this->MonarchPrice = GetPrivateProfileInt("ItemsPrice","CrestOfMonarchPrice",300000,CONFIG_FILE);
	this->FeatherPrice = GetPrivateProfileInt("ItemsPrice","BlueFeatherPrice",180000,CONFIG_FILE);

	//Jewel Rate
	this->SoulRate = GetPrivateProfileInt("JewelsSuccess","JoSNormalSuccessRate",50,CONFIG_FILE);
	this->SoulSpecialRate = GetPrivateProfileInt("JewelsSuccess","JoSSpecialSuccessRate",50,CONFIG_FILE);
	this->SoulSocketRate = GetPrivateProfileInt("JewelsSuccess","JoSSocketSuccessRate",60,CONFIG_FILE);
	this->SoulLuckRate = GetPrivateProfileInt("JewelsSuccess","JoSWLuckAddSuccessRate",20,CONFIG_FILE);
	this->LifeRate = GetPrivateProfileInt("JewelsSuccess","JoLifeSuccessRate",50,CONFIG_FILE);

	//Skill Level
	this->Skill1 = GetPrivateProfileInt("SkillsLevel","TwistingSlash_MinLevel",80,CONFIG_FILE);
	this->Skill2 = GetPrivateProfileInt("SkillsLevel","Ragefull_MinLevel",160,CONFIG_FILE);
	this->Skill3 = GetPrivateProfileInt("SkillsLevel","DeathStab_MinLevel",170,CONFIG_FILE);
	this->Skill4 = GetPrivateProfileInt("SkillsLevel","Impale_MinLevel",28,CONFIG_FILE);
	this->Skill5 = GetPrivateProfileInt("SkillsLevel","InnerStrenght_MinLevel",120,CONFIG_FILE);
	this->Skill6 = GetPrivateProfileInt("SkillsLevel","Penetration_MinLevel",130,CONFIG_FILE);

	//ManaShield
	this->MS_Agility = GetPrivateProfileInt("ManaShield","ManaShield_Agility",30,CONFIG_FILE);
	this->MS_Energy = GetPrivateProfileInt("ManaShield","ManaShield_Energy",200,CONFIG_FILE);
	this->MS_Divisor = GetPrivateProfileInt("ManaShield","ManaShield_Divisor",30,CONFIG_FILE);
	this->ML_MS_Agility = GetPrivateProfileInt("Master_ManaShield","Master_ManaShield_Agility",30,CONFIG_FILE);
	this->ML_MS_Energy = GetPrivateProfileInt("Master_ManaShield","Master_ManaShield_Energy",200,CONFIG_FILE);
	this->ML_MS_Divisor = GetPrivateProfileInt("Master_ManaShield","Master_ManaShield_Divisor",30,CONFIG_FILE);

	//Reflect Summoner
	this->ReflectRate = GetPrivateProfileInt("Reflect","Reflect_Percent_Base",42,CONFIG_FILE);
	this->ReflectDivisor = GetPrivateProfileInt("Reflect","Reflect_Percent_Divisor",30,CONFIG_FILE);

	//Berserker
	this->Berserker_EnergyDivisor = GetPrivateProfileInt("Berserker","Berserker_Attack_EnergyDivisor",20,CONFIG_FILE);
	this->Berserker_EnergyAdd = GetPrivateProfileInt("Berserker","Berserker_Attack_EnergyAdd",30,CONFIG_FILE);
	this->Berserker_ManaRate= GetPrivateProfileInt("Berserker","Berserker_ManaRemovePercent",30,CONFIG_FILE);
	this->Berserker_Timer = GetPrivateProfileInt("Berserker","Berserker_Time_Divisor",60,CONFIG_FILE);

	//Inner Shield
	this->Inner_Base = GetPrivateProfileInt("InnerStrength","InnerStrength_Base",12,CONFIG_FILE);
	this->Inner_Energy = GetPrivateProfileInt("InnerStrength","InnerStrength_Energy",100,CONFIG_FILE);
	this->Inner_Vitality = GetPrivateProfileInt("InnerStrength","InnerStrength_Vitality",20,CONFIG_FILE);
	this->Inner_Timer = GetPrivateProfileInt("InnerStrength","InnerStrength_Timer",60,CONFIG_FILE);
	this->Inner_Timer_Divisor = GetPrivateProfileInt("InnerStrength","InnerStrength_Timer_Divisor",10,CONFIG_FILE);

	this->ML_Inner_Base = GetPrivateProfileInt("InnerStrength","Master_InnerStrength_Base",12,CONFIG_FILE);
	this->ML_Inner_Energy= GetPrivateProfileInt("Master_InnerStrength","Master_InnerStrength_Energy",100,CONFIG_FILE);
	this->ML_Inner_Vitality = GetPrivateProfileInt("Master_InnerStrength","Master_InnerStrength_Vitality",20,CONFIG_FILE);
	this->ML_Inner_Timer= GetPrivateProfileInt("Master_InnerStrength","Master_InnerStrength_Timer",60,CONFIG_FILE);
	this->ML_Inner_Timer_Divisor = GetPrivateProfileInt("InnerStrength","Master_InnerStrength_Timer_Divisor",10,CONFIG_FILE);

	//ItemDrop
	this->ExcDropRate = GetPrivateProfileInt("ItemDropRate","Excellent_DropRate",2000,CONFIG_FILE);
	this->ExcLuckRate = GetPrivateProfileInt("ItemDropRate","Excellent_WLuckRate",1,CONFIG_FILE);
	this->ExcSkilRate = GetPrivateProfileInt("ItemDropRate","Excellent_WSkillRate",100,CONFIG_FILE);
	this->NormalLuckRate = GetPrivateProfileInt("ItemDropRate","Normal_DropWLuckRate",6,CONFIG_FILE);
	this->NormalSkillRate = GetPrivateProfileInt("ItemDropRate","Normal_DropWSkillRate",2,CONFIG_FILE);

	//Party Exp
	this->PartyExp_03 = GetPrivateProfileInt("PartyExp","PartyExp_03",230,CONFIG_FILE);
	this->PartyExp_04 = GetPrivateProfileInt("PartyExp","PartyExp_04",270,CONFIG_FILE);
	this->PartyExp_05 = GetPrivateProfileInt("PartyExp","PartyExp_05",300,CONFIG_FILE);
	this->PartyExp_Default = GetPrivateProfileInt("PartyExp","PartyExp_Default",120,CONFIG_FILE);
	this->SetPartyExp_02 = GetPrivateProfileInt("PartyExp","SetPartyExp_02",160,CONFIG_FILE);
	this->SetPartyExp_03 = GetPrivateProfileInt("PartyExp","SetPartyExp_03",180,CONFIG_FILE);
	this->SetPartyExp_04 = GetPrivateProfileInt("PartyExp","SetPartyExp_04",200,CONFIG_FILE);
	this->SetPartyExp_05 = GetPrivateProfileInt("PartyExp","SetPartyExp_05",220,CONFIG_FILE);
	this->SetPartyExp_Default = GetPrivateProfileInt("PartyExp","SetPartyExp_Default",150,CONFIG_FILE);

	this->AttackDamageMin_Bow_StrDexDiv_Elf = GetPrivateProfileInt("FairyElf","DamageMin_Bow_StrDexDiv",7,".\\CalCharacter.xtr");
	this->AttackDamageMax_Bow_StrDexDiv_Elf = GetPrivateProfileInt("FairyElf","DamageMax_Bow_StrDexDiv",7,".\\CalCharacter.xtr");
	this->AttackDamageMin_NoBow_DexDiv_Elf = GetPrivateProfileInt("FairyElf","DamageMin_NoBow_DexDiv",7,".\\CalCharacter.xtr");
	this->AttackDamageMax_NoBow_DexDiv_Elf = GetPrivateProfileInt("FairyElf","DamageMax_NoBow_DexDiv",4,".\\CalCharacter.xtr");
	this->AttackDamageMin_NoBow_StrDiv_Elf = GetPrivateProfileInt("FairyElf","DamageMin_NoBow_StrDiv",14,".\\CalCharacter.xtr");
	this->AttackDamageMax_NoBow_StrDiv_Elf = GetPrivateProfileInt("FairyElf","DamageMax_NoBow_StrDiv",8,".\\CalCharacter.xtr");
	this->AttackDamageMin_DexStrDiv_Elf = GetPrivateProfileInt("FairyElf","DamageMin_DexStrDiv",7,".\\CalCharacter.xtr");
	this->AttackDamageMax_DexStrDiv_Elf = GetPrivateProfileInt("FairyElf","DamageMax_DexStrDiv",4,".\\CalCharacter.xtr");

	this->AttackDamageMin_StrDiv_DK = GetPrivateProfileInt("DarkKnight","DamageMin_StrDiv",6,".\\CalCharacter.xtr");
	this->AttackDamageMax_StrDiv_DK = GetPrivateProfileInt("DarkKnight","DamageMax_StrDiv",4,".\\CalCharacter.xtr");

	this->AttackDamageMin_StrDiv_MG = GetPrivateProfileInt("MagicGladiator","DamageMin_StrDiv",6,".\\CalCharacter.xtr");
	this->AttackDamageMax_StrDiv_MG = GetPrivateProfileInt("MagicGladiator","DamageMax_StrDiv",4,".\\CalCharacter.xtr");
	this->AttackDamageMin_EneDiv_MG = GetPrivateProfileInt("MagicGladiator","DamageMin_EneDiv",12,".\\CalCharacter.xtr"); 
	this->AttackDamageMax_EneDiv_MG = GetPrivateProfileInt("MagicGladiator","DamageMax_EneDiv",8,".\\CalCharacter.xtr");

	this->AttackDamageMin_StrDiv_DL = GetPrivateProfileInt("DarkLord","DamageMin_StrDiv",7,".\\CalCharacter.xtr");
	this->AttackDamageMax_StrDiv_DL = GetPrivateProfileInt("DarkLord","DamageMax_StrDiv",5,".\\CalCharacter.xtr");
	this->AttackDamageMin_EneDiv_DL= GetPrivateProfileInt("DarkLord","DamageMin_EneDiv",14,".\\CalCharacter.xtr");
	this->AttackDamageMax_EneDiv_DL = GetPrivateProfileInt("DarkLord","DamageMax_EneDiv",10,".\\CalCharacter.xtr");

	this->AttackDamageMin_StrDiv_SUM = GetPrivateProfileInt("Summoner","DamageMin_StrDiv",7,".\\CalCharacter.xtr");
	this->AttackDamageMax_StrDiv_SUM = GetPrivateProfileInt("Summoner","DamageMax_StrDiv",4,".\\CalCharacter.xtr");

	this->AttackDamageMin_StrDiv_DW = GetPrivateProfileInt("DarkWizard","DamageMin_StrDiv",6,".\\CalCharacter.xtr");
	this->AttackDamageMax_StrDiv_DW = GetPrivateProfileInt("DarkWizard","DamageMax_StrDiv",4,".\\CalCharacter.xtr");

	this->AttackSpeed_Elf1 = GetPrivateProfileInt("AttackSpeed","AttackSpeed_Elf",50,".\\CalCharacter.xtr");
	this->AttackSpeed_DK_MG1 = GetPrivateProfileInt("AttackSpeed","AttackSpeed_DK_MG",15,".\\CalCharacter.xtr");
	this->AttackSpeed_DL1 = GetPrivateProfileInt("AttackSpeed","AttackSpeed_DL",10,".\\CalCharacter.xtr");
	this->AttackSpeed_Sum1 = GetPrivateProfileInt("AttackSpeed","AttackSpeed_SUM",20,".\\CalCharacter.xtr");
	this->AttackSpeed_DW1 = GetPrivateProfileInt("AttackSpeed","AttackSpeed_DW",20,".\\CalCharacter.xtr");

	this->AttackSpeed_Elf2 = GetPrivateProfileInt("MagicSpeed","MagicSpeed_Elf",50,".\\CalCharacter.xtr");
	this->AttackSpeed_DK_MG2 = GetPrivateProfileInt("MagicSpeed","MagicSpeed_DK_MG",20,".\\CalCharacter.xtr");
	this->AttackSpeed_DL2 = GetPrivateProfileInt("MagicSpeed","MagicSpeed_DL",10,".\\CalCharacter.xtr");
	this->AttackSpeed_Sum2 = GetPrivateProfileInt("MagicSpeed","MagicSpeed_SUM",20,".\\CalCharacter.xtr");
	this->AttackSpeed_DW2 = GetPrivateProfileInt("MagicSpeed","MagicSpeed_DW",10,".\\CalCharacter.xtr");

	this->SuccessfulBlocking_Elf = GetPrivateProfileInt("SuccessfulBlocking","SuccessfulBlocking_Elf",4,".\\CalCharacter.xtr");
	this->SuccessfulBlocking_DW_DK_MG = GetPrivateProfileInt("SuccessfulBlocking","SuccessfulBlocking_DW_DK_MG",7,".\\CalCharacter.xtr");
	this->SuccessfulBlocking_DL = GetPrivateProfileInt("SuccessfulBlocking","SuccessfulBlocking_DL",4,".\\CalCharacter.xtr");
	this->SuccessfulBlocking_Sum = GetPrivateProfileInt("SuccessfulBlocking","SuccessfulBlocking_SUM",3,".\\CalCharacter.xtr");

	this->Defense_Elf = GetPrivateProfileInt("Defense","Defense_Elf",10,".\\CalCharacter.xtr");
	this->Defense_DK = GetPrivateProfileInt("Defense","Defense_DK",3,".\\CalCharacter.xtr");
	this->Defense_DL = GetPrivateProfileInt("Defense","Defense_DL",7,".\\CalCharacter.xtr");
	this->Defense_Sum = GetPrivateProfileInt("Defense","Defense_SUM",3,".\\CalCharacter.xtr");
	this->Defense_DW_MG = GetPrivateProfileInt("Defense","Defense_DW_MG",4,".\\CalCharacter.xtr");
}

void CConfigs::GameMainInit(HWND hWnd)
{

	int n;
	int DataBufferSize;
	char* DataBuffer;
	int LevelOver_N;
	
	srand(time(NULL));
	ReadServerInfo();
	LogInit(TRUE);
	gSetDate();

	gDirPath.SetFirstPath(FIRST_PATH);
	gDirPath.SetFirstPath(FINAL_PATH);
	
	this->ServerType=GetPrivateProfileInt("GameServerInfo","ServerType",0, gDirPath.GetNewPath("commonserver.cfg") );
	this->Partition=GetPrivateProfileInt("GameServerInfo","Partition",0, gDirPath.GetNewPath("commonserver.cfg") );
	this->Language=GetPrivateProfileInt("GameServerInfo","Language",0, gDirPath.GetNewPath("commonserver.cfg") );
	
	gGameServerAuth.Init();
	gGameServerAuth.SetInfo(this->Language,this->Partition,0,this->szGameServerVersion,this->szServerName,this->ServerType,GameServerAuthCallBackFunc);

	DragonEvent = new CDragonEvent;
	
	if(DragonEvent == 0){
		MsgBox("Failed to Create CDragonEvent");
		return;
	}

	AttackEvent = new CAttackEvent;
	
	if(AttackEvent == 0){
		MsgBox("Failed to Create CAttackEvent");
		return;
	}

	ReadCommonServerInfo();

	g_MapServerManager.LoadData( gDirPath.GetNewPath("MapServerInfo.dat"));


	if(g_CastleSiege.Ready(g_MapServerManager.GetMapSvrGroup()) == TRUE){
		if(g_CastleSiege.LoadData(gDirPath.GetNewPath("MuCastleData.dat"))){
			g_CastleSiege.LoadPreFixData(gDirPath.GetNewPath("Commonserver.cfg"));
			g_CastleSiege.SetDataLoadState(CASTLESIEGE_DATALOAD_2);
		}
	}

	if(this->EnableServerDivision != 0){
		MessageBox(NULL, "Server Division was not Enable", "Warning", MB_OK);
	}

	if(CSSocket.CreateSocket() == 0){
		MsgBox("UDP Socket Create Error");
		return;
	}

	gObjInit();
	InitBattleSoccer();

	gGameServerAuth.RequestData(7);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();

	gMAttr.LoadAttr(DataBuffer, DataBufferSize);

	gGameServerAuth.RequestData(9);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();

	gMSetBase.LoadSetBase(DataBuffer, DataBufferSize);

	g_MonsterItemMng.Init();

	this->LevelExperience[0]=0;
	LevelOver_N=1;

	for(int n=1;n<MAX_CHAR_LEVEL+1;n++){
		this->LevelExperience[n] = (((n+9)*n)*n)*10;

		if(n > 255){
			this->LevelExperience[n] += ((((LevelOver_N+9)*LevelOver_N)*LevelOver_N)*1000);
			LevelOver_N++;
		}
	}

	g_MasterLevelSystem.SetExperienceTable();

	char MapAttrName[MAX_NUMBER_MAP][25] =
	{
		"..\\Data\\Maps\\Map1.att",
		"..\\Data\\Maps\\Map2.att",
		"..\\Data\\Maps\\Map3.att",
		"..\\Data\\Maps\\Map4.att",
		"..\\Data\\Maps\\Map5.att",
		"..\\Data\\Maps\\Map6.att",
		"..\\Data\\Maps\\Map7.att",
		"..\\Data\\Maps\\Map8.att",
		"..\\Data\\Maps\\Map9.att",
		"..\\Data\\Maps\\Map10.att",
		"..\\Data\\Maps\\Map11.att",
		"..\\Data\\Maps\\Map12.att",
		"..\\Data\\Maps\\Map12.att",
		"..\\Data\\Maps\\Map12.att",
		"..\\Data\\Maps\\Map12.att",
		"..\\Data\\Maps\\Map12.att",
		"..\\Data\\Maps\\Map12.att",
		"..\\Data\\Maps\\Map12.att",
		"..\\Data\\Maps\\Map19.att",
		"..\\Data\\Maps\\Map19.att",
		"..\\Data\\Maps\\Map19.att",
		"..\\Data\\Maps\\Map19.att",
		"..\\Data\\Maps\\Map19.att",
		"..\\Data\\Maps\\Map19.att",
		"..\\Data\\Maps\\Map25.att",
		"..\\Data\\Maps\\Map25.att",
		"..\\Data\\Maps\\Map25.att",
		"..\\Data\\Maps\\Map25.att",
		"..\\Data\\Maps\\Map25.att",
		"..\\Data\\Maps\\Map25.att",
		"..\\Data\\Maps\\Map31.att",
		"..\\Data\\Maps\\Map32.att",
		"..\\Data\\Maps\\Map33.att",
		"..\\Data\\Maps\\Map34.att",
		"..\\Data\\Maps\\Map35.att",
		"..\\Data\\Maps\\Map36.att",
		"..\\Data\\Maps\\Map37.att",
		"..\\Data\\Maps\\Map38.att",
		"..\\Data\\Maps\\Map39.att",
		"..\\Data\\Maps\\Map40.att",
		"..\\Data\\Maps\\Map41.att",
		"..\\Data\\Maps\\Map42.att",
		"..\\Data\\Maps\\Map43.att",
		"..\\Data\\Maps\\Map43.att",
		"..\\Data\\Maps\\Map43.att",
		"..\\Data\\Maps\\Map46.att",
		"..\\Data\\Maps\\Map46.att",
		"..\\Data\\Maps\\Map46.att",
		"..\\Data\\Maps\\Map46.att",
		"..\\Data\\Maps\\Map46.att",
		"..\\Data\\Maps\\Map46.att",
		"..\\Data\\Maps\\Map52.att",
		"..\\Data\\Maps\\Map12.att",
		"..\\Data\\Maps\\Map19.att",
		"..\\Data\\Maps\\Null.att",
		"..\\Data\\Maps\\Null.att",
		"..\\Data\\Maps\\Map57.att",
		"..\\Data\\Maps\\Map58.att",
		"..\\Data\\Maps\\Map59.att",
		"..\\Data\\Maps\\Map59.att",
		"..\\Data\\Maps\\Map59.att",
		"..\\Data\\Maps\\Map59.att",
		"..\\Data\\Maps\\Map63.att",
		"..\\Data\\Maps\\Map64.att",
		"..\\Data\\Maps\\Map65.att",
		"..\\Data\\Maps\\Map66.att",
		"..\\Data\\Maps\\Map67.att",
		"..\\Data\\Maps\\Map68.att",
		"..\\Data\\Maps\\Map69.att",
		"..\\Data\\Maps\\Map70.att",
		"..\\Data\\Maps\\Map71.att",
		"..\\Data\\Maps\\Map72.att",
		"..\\Data\\Maps\\Map73.att",
		"..\\Data\\Maps\\Map74.att",
		"..\\Data\\Maps\\Map75.att",
		"..\\Data\\Maps\\Null.att",
		"..\\Data\\Maps\\Null.att",
		"..\\Data\\Maps\\Null.att",
		"..\\Data\\Maps\\Null.att",
		"..\\Data\\Maps\\Map80.att",
	};

	for(int n=0;n<MAX_NUMBER_MAP;n++){
		MapC[n].ItemInit();
		MapC[n].LoadMapAttr(gDirPath.GetNewPath(MapAttrName[n]), n);
	}


	g_Crywolf.LoadCrywolfMapAttr(gDirPath.GetNewPath("terrain35_PEACE.att"),0);
	g_Crywolf.LoadCrywolfMapAttr(gDirPath.GetNewPath("terrain35_OCCUPIED.att"),1);
	g_Crywolf.LoadCrywolfMapAttr(gDirPath.GetNewPath("terrain35_WAR.att"),2);
	g_Kanturu.LoadKanturuMapAttr(gDirPath.GetNewPath("terrain40_CLOSE.att"), 0);
	g_Kanturu.LoadKanturuMapAttr(gDirPath.GetNewPath("terrain40_OPEN.att"), 1);

	DCInfo.Init();
	ShopDataLoad();
	cGameServer.CreateSocket(hWnd);
	cJoinServer.CreateSocket(hWnd);
	cDataServer.CreateSocket(hWnd);

	cRankingServer.CreateSocket(hWnd);
	cEventServer.CreateSocket(hWnd);
	GameServerInfoSend();
	GameMonsterAllAdd();

	acceptIP.Load(gDirPath.GetNewPath("Iplist.dat"));
	ConMember.Load(gDirPath.GetNewPath("ConnectMember.txt"));
	GCTeleportSend(gObj, 1, 1, 1, 2, 3);
	g_Raklion.SetState(RAKLION_STATE_END);
}


int GetWarehouseUsedHowMuch(int UserLevel, int MasterLevel, BOOL IsLock)
{
	int rZen=0;

	int iBaseLevel = UserLevel+MasterLevel;

	rZen = (iBaseLevel * iBaseLevel)* 0.1 * 0.4;

	if (Configs.CanWarehouseLock == TRUE)
	{
		if ( IsLock != 0)
		{
			rZen = rZen + ( UserLevel * 2 );
		}
	}
	if ( rZen < 1 )
	{
		rZen=1;
	}

	if ( rZen >= 1000 )
	{
		rZen = (rZen/100)*100;
	}
	
	else if ( rZen >= 100 )
	{
		rZen = (rZen/10)*10;
	}
	
	if ( rZen == 0 )
	{
		rZen=1;
	}
	
	return rZen;
}


void GameMonsterAllAdd()
{
	gCurPaintPlayer = 0;

	int result;

	for ( int n=0;n< gMSetBase.m_Count ;n++)
	{
		if ( DS_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE )
		{
			continue;
		}

		if ( BC_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE )
		{
			if ( gMSetBase.m_Mp[n].m_Type != 232 )
			{
				continue;
			}
		}

		if ( CC_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE )
		{
			continue;
		}

		if ( IT_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE && gMSetBase.m_Mp[n].m_Type != 381 && gMSetBase.m_Mp[n].m_Type != 382)
		{
			if( gMSetBase.m_Mp[n].m_Type == 380 || gMSetBase.m_Mp[n].m_Type == 383 || gMSetBase.m_Mp[n].m_Type == 384 )
			{
				g_IllusionTempleEvent.CreateNPCPosition(gMSetBase.m_Mp[n].m_MapNumber,gMSetBase.m_Mp[n].m_Type, n);
			}
			else if(gMSetBase.m_Mp[n].m_Type >= 386 || gMSetBase.m_Mp[n].m_Type <= 403)
			{
				g_IllusionTempleEvent.CreateMonsterPosition(gMSetBase.m_Mp[n].m_MapNumber,gMSetBase.m_Mp[n].m_Type, n);
			}

			continue;
		}
			
		result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);

		if ( result >= 0 )
		{
			gObjSetPosMonster(result, n);
			gObjSetMonster(result, gMSetBase.m_Mp[n].m_Type);
			gCurPaintPlayer++;

			if ( BC_MAP_RANGE(gObj[result].MapNumber) )
			{
				if ( gObj[result].Class == 232 )
				{
					gObj[result].m_cBloodCastleIndex = g_BloodCastle.GetBridgeIndex(gObj[result].MapNumber);
				}
			}


				if ( gObj[result].Class == 216 )
				{
					g_CastleSiege.SetCrownIndex(result);
				}
				if( CRYWOLF_MAP_RANGE(gObj[result].MapNumber) )
				{
					if(gObj[result].Type == OBJ_NPC)
					{
						if(gObj[result].Class == 406 || gObj[result].Class == 407)
						{
							
						}
						else if ( CRYWOLF_ALTAR_CLASS_RANGE(gObj[result].Class) ) //Altar?
						{
							g_Crywolf.m_ObjSpecialNPC.AddObj(result);
						}
						else
						{
							g_Crywolf.m_ObjCommonNPC.AddObj(result);
						}
					}
				}
				if( CRYWOLF_MAP_RANGE(gObj[result].MapNumber) )
				{
					if(gObj[result].Type == OBJ_MONSTER)
					{
						g_Crywolf.m_ObjCommonMonster.AddObj(result);
					}
				}

		}
	}


	g_DevilSquare.Init();
	g_BloodCastle.LoadItemDropRate();

	g_BloodCastle.Init(Configs.BloodCastle);
	g_ChaosCastle.Init(Configs.ChaosCastle);
	
	if(g_iIllusionTempleEvent != FALSE){
		g_IllusionTempleEvent.Init();
	}

	//Season5 New Events
	Doppelganger.StartProcess(0);

	ImperialGuardian.StartProcess(0);
}

void GameMonsterAllCloseAndReLoad()
{
	for ( int n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Type == OBJ_MONSTER || gObj[n].Type == OBJ_NPC )
		{
			
			if(gObj[n].m_btCsNpcType || gObj[n].MapNumber == MAP_INDEX_KANTURU_BOSS){
				continue;
			}

			if ( gObj[n].m_iCurrentAI )
			{
				gObj[n].Live = FALSE;
				gObjViewportListProtocolDestroy(&gObj[n]);
				gObjViewportClose(&gObj[n]);
			}
			
			gObjDel(n);
		}
	}


	g_Crywolf.m_ObjCommonNPC.Reset();
	g_Crywolf.m_ObjSpecialNPC.Reset();
	g_Crywolf.m_ObjCommonMonster.Reset();
	g_Crywolf.m_ObjSpecialMonster.Reset();

	gGameServerAuth.RequestData(7);
	int DataBufferSize = gGameServerAuth.GetDataBufferSize();
	char * DataBuffer = gGameServerAuth.GetDataBuffer();
	gMAttr.LoadAttr(DataBuffer, DataBufferSize);

	gGameServerAuth.RequestData(9);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	gMSetBase.LoadSetBase(DataBuffer, DataBufferSize);

	g_IllusionTempleEvent.AllObjReset();

	g_MonsterItemMng.Init();
	gObjMonCount = 0;
	GameMonsterAllAdd();

	g_Raklion.SetState(RAKLION_STATE_END);
}

void GameMainFree()
{
	gDbSave.End();
	GDGameServerInfoSave();
	gObjEnd();
	ClearBattleSoccer();
	LogClose();
	CleanupGameguardAuth();
}

BOOL GMJoinServerConnect(LPSTR ConnectServer, DWORD wMsg)
{
	cJoinServer.SetProtocolCore(SProtocolCore);
	int result = cJoinServer.Connect(ConnectServer, Configs.JoinServer_Port, wMsg);

	if ( result == 0 )
	{
		return FALSE;
	}

	GJServerLogin();

	LogAdd( lMsg.Get( MSGGET(1, 145)), ConnectServer);
	return TRUE;

}

BOOL GMRankingServerConnect(char* RankingServer, DWORD wMsg)
{
	cRankingServer.SetProtocolCore(DevilSquareProtocolCore);
	int result = cRankingServer.Connect(RankingServer, RANKING_SERVER_PORT, wMsg);
	if ( result == 0 )
	{
		return FALSE;
	}
	LogAdd("RankingServer Connect IP [ %s ]", RankingServer);
	return TRUE;
}

BOOL GMEventChipServerConnect(char* ServerIP, DWORD wMsg)
{
	cEventServer.SetProtocolCore(EventChipEventProtocolCore);
	int result = cEventServer.Connect(ServerIP, EVENT_CHIP_SERVER_PORT, wMsg);
	if ( result == 0 )
	{
		return FALSE;
	}

	LogAdd("EventChip Connect IP [ %s ]", ServerIP);
	return TRUE;
}

BOOL GMDataServerConnect(char* ConnectServer, WPARAM wMsg)
{
	cDataServer.SetProtocolCore(DataServerProtocolCore);
	int result = cDataServer.Connect(ConnectServer, Configs.DataServer1_Port, wMsg);
	if ( result == 0 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 147)), ConnectServer, Configs.DataServer1_Port);
		return FALSE;
	}
	DataServerLogin(0);
	LogAdd(lMsg.Get(MSGGET(1, 146)), ConnectServer, Configs.DataServer1_Port);
	return TRUE;
}

BOOL ExDataServerConnect(char* ConnectServer, DWORD wMsg)
{
	cExDB.SetProtocolCore(ExDataServerProtocolCore);
	int result = cExDB.Connect(ConnectServer, Configs.ExDB_Port, wMsg);
	if ( result == 0 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 147)), ConnectServer, Configs.ExDB_Port);
		return FALSE;
	}

	ExDataServerLogin();
	LogAddC(2, "ExDB Server Online");
	return TRUE;
}

BOOL GameMainServerCreate(DWORD sMsg, DWORD cMsg)
{
	gDbSave.Begin();
	LogAdd( lMsg.Get( MSGGET(1, 148)), Configs.GameServer_Port);
	return TRUE;
}


void GMServerMsgProc(WPARAM wParam, LPARAM lParam)
{
	
	switch ( lParam & 0xFFFF & 0xFFFF)
	{
	case FD_ACCEPT :
		/*SOCKET cSocket;
		IN_ADDR cInAddr;
		int ClientIndex;
		//LogAdd("SFD_ACCEPT MESSAGE");
		if( cGameServer.AcceptSocket(cSocket, cInAddr) == TRUE )
		{
			ClientIndex = gObjAddSearch(cSocket, inet_ntoa(cInAddr) );

			int addnumber = gObjAdd(cSocket, inet_ntoa(cInAddr), ClientIndex);
			if( addnumber >= 0 ) 
			{
				SCPJoinResultSend( addnumber, 0x01);
			}
			else 
			{// »??????¦ ?©¶??? µ?..
				cGameServer.Close(cSocket);
			}
		}
		else {
			return;
		}*/
		break;
	}
}

void GMClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	int wp = wParam;
	int lp = lParam;

	switch ( lParam & 0xFFFF & 0xFFFF)
	{
		SOCKET socket;

		case 1:
			cGameServer.DataRecv(wParam);
			break;
		case 2:
			cGameServer.FDWRITE_MsgDataSend(wParam);
			break;
		case 32:
			socket = gObjGetSocket(wParam);
			break;

	}
}

void GMJoinClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			cJoinServer.DataRecv();
			break;
		case 2:
			cJoinServer.FDWRITE_MsgDataSend();
			break;
		case 32:
			LogAddC(2, lMsg.Get(MSGGET(1, 149)));
			SendMessage(ghWnd, WM_CLOSE, 0, 0);
	}
}

void GMRankingClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			cRankingServer.DataRecv();
			break;
		case 2:
			cRankingServer.FDWRITE_MsgDataSend();
			break;
		case 32:
			IsDevilSquareEventConnected = FALSE;
	}
}

void GMEventChipClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			cEventServer.DataRecv();
			break;
		case 2:
			cEventServer.FDWRITE_MsgDataSend();
			break;
		case 32:
			IsEventChipServerConnected = FALSE;
	}
}

void ExDataClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			cExDB.DataRecv();
			break;
		case 2:
			cExDB.FDWRITE_MsgDataSend();
			break;
		case 32:
			LogAddC(2, lMsg.Get(MSGGET(1, 150)));
			cExDB.Close();

			for ( int i =0;i<OBJMAX;i++)
			{
				if ( gObjIsConnectedGP(i) == TRUE )
				{
					PMSG_FRIEND_STATE pMsg;
					
					pMsg.h.set((LPBYTE)&pMsg, 0xC4 , sizeof(pMsg) );
					pMsg.State = -4;

					DataSend(i, (LPBYTE)&pMsg, sizeof(pMsg) );

					gObj[i].m_FriendServerOnline = FRIEND_SERVER_STATE_OFFLINE;
				}
			}

			LogAddTD("Error-L1 : Friend Server Down (State Send Ok)");
			LogAddC(2, "Error-L1 : Friend Server Down (State Send Ok)");
			break;
	}
}

void GMDataClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			cDataServer.DataRecv();
			break;
		case 2:
			cDataServer.FDWRITE_MsgDataSend();
			break;
		case 32:
			LogAddC(2, lMsg.Get(MSGGET(1, 150)));
	}
}

void ReadServerInfo()
{
	if (!IsFile(SERVER_INFO_PATH))
	{
		MsgBox("ServerInfo.dat file not found");
	}
	GetPrivateProfileString("GameServerInfo","ServerName","",Configs.szServerName,50,SERVER_INFO_PATH);
	Configs.GameServerCode=GetPrivateProfileInt("GameServerInfo","ServerCode",0,SERVER_INFO_PATH);
	Configs.CanConnectMember=GetPrivateProfileInt("GameServerInfo","ConnectMemberLoad",0,SERVER_INFO_PATH);
	Configs.CanChangeCharacterName=GetPrivateProfileInt("GameServerInfo","ChangeCharacterName",0,SERVER_INFO_PATH);
}

void ReadCommonServerInfo()
{
	char szTemp[256];
	char szCheckSum[256];
	char* cvstr;
	int DataBufferSize;
	char* DataBuffer;
	char szlMsgName[256];

	////////////////////////////////////////////////////////
	
	wsprintf(Configs.szGameServerVersion,"%s",GAMESERVER_VERSION);
	wsprintf(Configs.szClientVersion,"%s","000000");
	wsprintf(Configs.CountryName,"%s","Kor");
	
	ReadServerInfo();

	gGameServerAuth.RequestData(8);

	DataBufferSize = gGameServerAuth.GetDataBufferSize();

	DataBuffer = gGameServerAuth.GetDataBuffer();

	gGateC.Load(DataBuffer, DataBufferSize);

	if(!IsFile(gDirPath.GetNewPath("commonserver.cfg")))
	{
		MsgBox("[commonserver.cfg] file not found");
	}

	GetPrivateProfileString("GameServerInfo", "Language", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.Language = atoi(szTemp);
	Configs.StalkProtocol	= GetPrivateProfileInt("GameServerInfo","StalkProtocol",0,gDirPath.GetNewPath("commonserver.cfg"));
	GetPrivateProfileString("GameServerInfo", "StalkProtocolId", "0", Configs.StalkProtocolId, 10, gDirPath.GetNewPath("commonserver.cfg"));

	if(Configs.StalkProtocol != 0)
	{
		LogAdd("Stalk Protocol ID = '%s' ", Configs.StalkProtocolId);
	}

	if(g_SimpleModulusCS.LoadDecryptionKey(gDirPath.GetNewPath("Dec1.dat")) == FALSE)
	{
		MsgBox("Dec1.dat file not found");
	}

	if(g_SimpleModulusSC.LoadEncryptionKey(gDirPath.GetNewPath("Enc2.dat")) == FALSE)
	{
		MsgBox("Enc2.dat file not found");
	}

	strcpy(Configs.szKorItemTextFileName, gDirPath.GetNewPath("lang\\kor\\item(kor).txt"));
	strcpy(Configs.szKorSkillTextFileName, gDirPath.GetNewPath("lang\\kor\\skill(kor).txt"));
	strcpy(szlMsgName, gDirPath.GetNewPath("Message_Kor.wtf"));
	strcpy(Configs.szItemTextFileName, gDirPath.GetNewPath("lang\\kor\\item(kor).txt"));
	strcpy(Configs.szSkillTextFileName, gDirPath.GetNewPath("lang\\kor\\skill(kor).txt"));
	strcpy(Configs.szQuestTextFileName, gDirPath.GetNewPath("lang\\kor\\Quest(Kor).txt"));
	strcpy(szCheckSum, gDirPath.GetNewPath("lang\\kor\\CheckSum.dat"));
	strcpy(Configs.szMoveReqFileName, gDirPath.GetNewPath("lang\\kor\\movereq(Kor).txt"));
	strcpy(Configs.szCommonlocIniFileName, gDirPath.GetNewPath("lang\\kor\\commonloc.cfg"));
	
	Configs.NonPK = GetPrivateProfileInt("GameServerInfo","NonPK",0, SERVER_INFO_PATH);
	if(Configs.NonPK != 0){
		LogAdd("NON-PK Server");
	}

	Configs.PkLimitFree = GetPrivateProfileInt("GameServerInfo","PkLimitFree",0, SERVER_INFO_PATH);
	if(Configs.PkLimitFree != 0){
		LogAdd("PK Limit Free Server");
	}

	Configs.NewServer = GetPrivateProfileInt("GameServerInfo","NewServer",0, SERVER_INFO_PATH);
	if(Configs.NewServer != 0){
		LogAdd("New Server");
	}

	Configs.EventOff = GetPrivateProfileInt("GameServerInfo","EventOff",0, SERVER_INFO_PATH);
	if(Configs.EventOff != 0){
		LogAdd("EventOff");
	}

	CheckSumFileLoad(szCheckSum);

	lMsg.LoadWTF(szlMsgName);

	SetMapName();

	gGameServerAuth.RequestData(0);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	OpenItemScript(DataBuffer, DataBufferSize);

	if(Configs.Language != 0)
	{
		gGameServerAuth.RequestData(1);
		DataBufferSize = gGameServerAuth.GetDataBufferSize();
		DataBuffer = gGameServerAuth.GetDataBuffer();
		OpenItemNameScript( DataBuffer, DataBufferSize);
	}

	gGameServerAuth.RequestData(2);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	MagicDamageC.LogSkillList(DataBuffer, DataBufferSize);

	if(Configs.Language != 0)
	{
		gGameServerAuth.RequestData(3);
		DataBufferSize = gGameServerAuth.GetDataBufferSize();
		DataBuffer = gGameServerAuth.GetDataBuffer();
		MagicDamageC.LogSkillNameList(DataBuffer, DataBufferSize);
	}

	SkillSpearHitBox.Load(gDirPath.GetNewPath("skillSpear.hit"));
	SkillElectricSparkHitBox.Load(gDirPath.GetNewPath("skillElect.hit"));
	//SkillDrainLifeHitBox.Load(gDirPath.GetNewPath("SkillDrainLife.hit"));
	//SkillLightingOrbHitBox.Load(gDirPath.GetNewPath("SkillLightingOrb.hit"));

	gGameServerAuth.RequestData(26);

	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();

	gSetItemOption.LoadOptionInfo(DataBuffer, DataBufferSize);

	gGameServerAuth.RequestData(28);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	gSetItemOption.LoadTypeInfo(DataBuffer, DataBufferSize);

	g_kJewelOfHarmonySystem.LoadScript(gDirPath.GetNewPath("JewelOfHarmonyOption.txt"));
	g_kJewelOfHarmonySystem.LoadScriptOfSmelt(gDirPath.GetNewPath("JewelOfHarmonySmelt.txt"));
	g_kItemSystemFor380.Load380ItemOptionInfo(gDirPath.GetNewPath("ItemAddOption.txt"));
	g_ItemAddOption.Load(gDirPath.GetNewPath("ItemAddOption.txt"));
	g_MasterSkillSystem.Load(gDirPath.GetNewPath("MasterSkillTree.txt"));
	g_SocketOption.LoadScript(gDirPath.GetNewPath("socketitem.txt"));

	gGameServerAuth.RequestData(24);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	gMoveCommand.Load(DataBuffer, DataBufferSize);
	gMoveCommand.LoadMoveLevel(gDirPath.GetNewPath("MoveLevel.txt"));

	ConMember.Load(gDirPath.GetNewPath("ConnectMember.txt"));

	gGameServerAuth.RequestData(4);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	g_QuestInfo.LoadQuestInfo(DataBuffer, DataBufferSize);	
	gGameServerAuth.GetClientVersion(szTemp, Configs.szGameServerExeSerial);

	cvstr = strtok(szTemp, ".");
	Configs.szClientVersion[0] = cvstr[0];
	cvstr = strtok(NULL, ".");
	Configs.szClientVersion[1] = cvstr[0];
	Configs.szClientVersion[2] = cvstr[1];
	cvstr = strtok(NULL, ".");
	Configs.szClientVersion[3] = cvstr[0];
	Configs.szClientVersion[4] = cvstr[1];	


	TMonsterSkillElement::LoadData(gDirPath.GetNewPath("MonsterSkillElement.txt"));
	TMonsterSkillUnit::LoadData(gDirPath.GetNewPath("MonsterSkillUnit.txt"));
	TMonsterSkillManager::LoadData(gDirPath.GetNewPath("MonsterSkill.txt"));
	TMonsterAIElement::LoadData(gDirPath.GetNewPath("MonsterAIElement.txt"));
	TMonsterAIAutomata::LoadData(gDirPath.GetNewPath("MonsterAutomata.txt"));
	TMonsterAIUnit::LoadData(gDirPath.GetNewPath("MonsterAIUnit.txt"));
	TMonsterAIRule::LoadData(gDirPath.GetNewPath("MonsterAIRule.txt"));
	TMonsterAIGroup::LoadData(gDirPath.GetNewPath("MonsterAIGroup.txt"));

	g_Crywolf.LoadData(gDirPath.GetNewPath("Crywolf.dat"));
	g_SkillAdditionInfo.Load("SkillAdditionInfo.dat");

	g_Kanturu.LoadData(gDirPath.GetNewPath("Kanturu.dat"));
	g_KanturuMonsterMng.LoadData(gDirPath.GetNewPath("KanturuMonsterSetBase.txt"));

	g_PCBangPointSystem.Load(gDirPath.GetNewPath("PCBangPointItemOpt.txt"));

	g_Raklion.LoadData(gDirPath.GetNewPath("Raklion.dat"));

	GetPrivateProfileString("GameServerInfo", "CreateCharacter", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.CreateCharacter = atoi(szTemp);
	GetPrivateProfileString("GameServerInfo", "CharacterDeleteMinLevel", "40", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.CharacterDeleteMinLevel = atoi(szTemp);


	int iMaxUser = GetPrivateProfileInt("GameServerInfo","NumberOfMaxUser",500, gDirPath.GetNewPath("commonserver.cfg"));

	if(iMaxUser >= 0 && iMaxUser <= OBJMAXUSER)
	{
		gServerMaxUser = iMaxUser;
		LogAddTD("[Option Reload] ¡Ú¡Ú¡Ú MaxUser ¡Ù¡Ù¡Ù  : %d", gServerMaxUser);
	}


	Configs.ServerGroupGuildChatting = GetPrivateProfileInt("GameServerInfo","ServerGroupGuildChatting",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ServerGroupUnionChatting = GetPrivateProfileInt("GameServerInfo","ServerGroupUnionChatting",0, gDirPath.GetNewPath("commonserver.cfg"));
	GetPrivateProfileString("GameServerInfo", "GuildCreate", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.GuildCreate = atoi(szTemp);

	LogAdd(lMsg.Get(MSGGET(2, 57)), Configs.GuildCreate);
	GetPrivateProfileString("GameServerInfo", "GuildDestroy", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.GuildDestroy = atoi(szTemp);
	LogAdd(lMsg.Get(MSGGET(2, 58)), Configs.GuildDestroy);
	GetPrivateProfileString("GameServerInfo", "GuildCreateLevel", "100", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.GuildCreateLevel = atoi(szTemp);

	LogAdd(lMsg.Get(MSGGET(2, 59)), Configs.GuildCreateLevel);

	Configs.CastleGuildDestoyLimit = GetPrivateProfileInt("GameServerInfo", "CastleOwnerGuildDestroyLimit", 1, gDirPath.GetNewPath("commonserver.cfg"));

	GetPrivateProfileString("GameServerInfo", "SpeedHackPlayerBlock", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.SpeedHackPlayerBlock = atoi(szTemp);
	GetPrivateProfileString("GameServerInfo", "ItemSerialCheck", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ItemSerialCheck = atoi(szTemp);
	GetPrivateProfileString("GameServerInfo", "AddExperience", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.AddExperience = atof(szTemp);	
	GetPrivateProfileString("GameServerInfo", "AddZen", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.AddZen = atof(szTemp);	
	GetPrivateProfileString("GameServerInfo", "Trade", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.CanTrade = atoi(szTemp);

	if(Configs.CanTrade != 0){
		LogAddTD(lMsg.Get(407));
	} else {
		LogAddTD(lMsg.Get(408));
	}

	Configs.CanChaosBox = GetPrivateProfileInt("GameServerInfo","ChaosBox",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ChaosEvent = GetPrivateProfileInt("GameServerInfo","ChaosEvent",0, gDirPath.GetNewPath("commonserver.cfg"));
	GetPrivateProfileString("GameServerInfo", "ChaosEventServer", "210.181.89.241", Configs.ChaosEventServerIP, 20, gDirPath.GetNewPath("commonserver.cfg"));
	LogAdd("[Option] ChaosBox = %d", Configs.CanChaosBox);
	LogAdd("[Option] ChaosEvent = %d", Configs.ChaosEvent);
	LogAdd("[Option] ChaosEventServer = %s", Configs.ChaosEventServerIP);
	LogAdd("[Option] AttackEventRegenTime = %d", Configs.AttackEventRegenTime);
	
	GetPrivateProfileString("GameServerInfo", "PKTIME", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.PkTime = atoi(szTemp);

	if(Configs.PkTime != 0){
		LogAddTD(lMsg.Get(409), Configs.PkTime);
	}

	GetPrivateProfileString("GameServerInfo", "MonsterHp", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.MonsterHp = atoi(szTemp);
	if(Configs.MonsterHp != 0){
		LogAddTD(lMsg.Get(410), Configs.MonsterHp);
	}

	GetPrivateProfileString("GameServerInfo", "LootingTime", "3", szTemp, 5, Configs.szCommonlocIniFileName);
	Configs.LootingTime = atoi(szTemp);

	if(Configs.LootingTime != 0){
		LogAddTD("[Option] Auto Looting Time Set : %d", Configs.LootingTime);
	}

	GetPrivateProfileString("GameServerInfo", "PKItemDrop", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.PkItemDrop = atoi(szTemp);

	if(Configs.PkItemDrop != 0){
		LogAddTD(lMsg.Get(MSGGET(1, 155)));
	} else {
		LogAddTD(lMsg.Get(MSGGET(1, 156)));
	}

	GetPrivateProfileString("GameServerInfo", "ItemDropPer", "10", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ItemDropPer = atoi(szTemp);
	LogAddTD(lMsg.Get(413), Configs.ItemDropPer);
	Configs.ZenDurationTime = GetPrivateProfileInt("GameServerInfo","ZenDurationTime",30, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.EventChipEvent = GetPrivateProfileInt("GameServerInfo","EventChipEvent",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.BoxOfGoldDropRate = GetPrivateProfileInt("GameServerInfo","BoxOfGoldDropRate",2, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ItemDropRateForBoxOfGold = GetPrivateProfileInt("GameServerInfo","ItemDropRateForBoxOfGold",2, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.EventChipDropRateForBoxOfGold = GetPrivateProfileInt("GameServerInfo","EventChipDropRateForBoxOfGold",2, gDirPath.GetNewPath("commonserver.cfg"));

	if(Configs.DevilSquareEvent == FALSE){
		g_DevilSquare.SetClose();
	}

	Configs.AttackSpeedTimeLimit = GetPrivateProfileInt("GameServerInfo","AttackSpeedTimeLimit",800, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.IsIgnorePacketSpeedHackDetect = GetPrivateProfileInt("GameServerInfo","IsIgnorePacketHackDetect",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.HackCheckCount = GetPrivateProfileInt("GameServerInfo","HackCheckCount",5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.MinimumAttackSpeedTime = GetPrivateProfileInt("GameServerInfo","MinimumAttackSpeedTime",200, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.DetectedHackKickCount = GetPrivateProfileInt("GameServerInfo","DetectedHackKickCount",10, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.IsKickDetecHackCountLimit = GetPrivateProfileInt("GameServerInfo","IsKickDetecHackCountLimit",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.TamaJJangEvent = GetPrivateProfileInt("GameServerInfo", "TamaJJangEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.TamaJJangKeepTime = GetPrivateProfileInt("GameServerInfo", "TamaJJangKeepTime", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.TamaJJangDisappearTime = GetPrivateProfileInt("GameServerInfo", "TamaJJangDisappearTime", 1800, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.TamaJJangDisappearTimeRandomRange = GetPrivateProfileInt("GameServerInfo", "TamaJJangDisappearTimeRandomRange", 1800, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.TamaJJangTime = 10;

	Configs.IsItemDropRingOfTransform = GetPrivateProfileInt("GameServerInfo","IsItemDropRingOfTransform",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ItemDropRingOfTransform = GetPrivateProfileInt("GameServerInfo","ItemDropRingOfTransform",1, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.WriteChatLog = GetPrivateProfileInt("GameServerInfo","WriteChatLog",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.DisconnectHackUser = GetPrivateProfileInt("GameServerInfo","DisconnectHackUser",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.UseNPGGChecksum = GetPrivateProfileInt("GameServerInfo","UseNPGGChecksum",0, gDirPath.GetNewPath("commonserver.cfg"));
	GetPrivateProfileString("GameServerInfo", "DecTimePerAttackSpeed", "5.33", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.DecTimePerAttackSpeed = atof(szTemp);
	Configs.SpeedHackPenalty = GetPrivateProfileInt("GameServerInfo","SpeedHackPenalty",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.DQChaosSuccessRateLevel1 = GetPrivateProfileInt("GameServerInfo","DQChaosSuccessRateLevel1",75, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.DQChaosSuccessRateLevel2 = GetPrivateProfileInt("GameServerInfo","DQChaosSuccessRateLevel2",70, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.DQChaosSuccessRateLevel3 = GetPrivateProfileInt("GameServerInfo","DQChaosSuccessRateLevel3",65, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.DQChaosSuccessRateLevel4 = GetPrivateProfileInt("GameServerInfo","DQChaosSuccessRateLevel4",60, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.DQChaosSuccessRateLevel5 = GetPrivateProfileInt("GameServerInfo","DQChaosSuccessRateLevel5",55, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.DQChaosSuccessRateLevel6 = GetPrivateProfileInt("GameServerInfo","DQChaosSuccessRateLevel6",50, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.DQChaosSuccessRateLevel7 = GetPrivateProfileInt("GameServerInfo","DQChaosSuccessRateLevel7",45, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.WriteSkillLog = GetPrivateProfileInt("GameServerInfo","WriteSkillLog",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.QuestNPCTeleportTime = GetPrivateProfileInt("GameServerInfo","QuestNPCTeleportTime",900, gDirPath.GetNewPath("commonserver.cfg"));


	Configs.MarkOfTheLord = GetPrivateProfileInt("GameServerInfo","MarkOfTheLord",0, gDirPath.GetNewPath("commonserver.cfg"));


	Configs.RibbonBoxEvent = GetPrivateProfileInt("GameServerInfo","RibbonBoxEvent",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.RedRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.RedRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.RedRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.RedRibbonBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.RedRibbonBoxDropZen = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.GreenRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.GreenRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.GreenRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.GreenRibbonBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.GreenRibbonBoxDropZen = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.BlueRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.BlueRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.BlueRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.BlueRibbonBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.BlueRibbonBoxDropZen = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.ChocolateBoxEvent = GetPrivateProfileInt("GameServerInfo","ChocolateEvent",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.PinkChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.PinkChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.PinkChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.PinkChocolateBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.PinkChocolateBoxDropZen = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.RedChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.RedChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.RedChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.RedChocolateBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.RedChocolateBoxDropZen = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.BlueChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.BlueChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.BlueChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.BlueChocolateBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.BlueChocolateBoxDropZen = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.CandyBoxEvent = GetPrivateProfileInt("GameServerInfo","CandyBoxEvent",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.LightPurpleCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.LightPurpleCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.LightPurpleCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.LightPurpleCandyBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.LightPurpleCandyBoxDropZen = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.VermilionCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.VermilionCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.VermilionCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.VermilionCandyBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.VermilionCandyBoxDropZen = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.DeepBlueCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.DeepBlueCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.DeepBlueCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.DeepBlueCandyBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.DeepBlueCandyBoxDropZen = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));
	
	Configs.KundunRefillHPSec = GetPrivateProfileInt("GameServerInfo","KundunRefillHPSec",500, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.KundunRefillHP = GetPrivateProfileInt("GameServerInfo","KundunRefillHP",10000, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.KundunRefillHPTime = GetPrivateProfileInt("GameServerInfo","KundunRefillHPTime",600, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.KundunHPLogSaveTime = GetPrivateProfileInt("GameServerInfo","KundunHPLogSaveTime",120, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.FenrirStuffItemDrop = GetPrivateProfileInt("GameServerInfo","FenrirStuffItemDrop", 0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.FenrirStuff_01_DropLv_Min = GetPrivateProfileInt("GameServerInfo","FenrirStuff_01_DropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.FenrirStuff_01_DropLv_Max = GetPrivateProfileInt("GameServerInfo","FenrirStuff_01_DropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.FenrirStuff_01_DropMap = GetPrivateProfileInt("GameServerInfo","FenrirStuff_01_DropMap", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.FenrirStuff_01_DropRate = GetPrivateProfileInt("GameServerInfo","FenrirStuff_01_DropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.FenrirStuff_02_DropLv_Min = GetPrivateProfileInt("GameServerInfo","FenrirStuff_02_DropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.FenrirStuff_02_DropLv_Max = GetPrivateProfileInt("GameServerInfo","FenrirStuff_02_DropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.FenrirStuff_02_DropMap = GetPrivateProfileInt("GameServerInfo","FenrirStuff_02_DropMap", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.FenrirStuff_02_DropRate = GetPrivateProfileInt("GameServerInfo","FenrirStuff_02_DropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.FenrirStuff_03_DropLv_Min = GetPrivateProfileInt("GameServerInfo","FenrirStuff_03_DropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.FenrirStuff_03_DropLv_Max = GetPrivateProfileInt("GameServerInfo","FenrirStuff_03_DropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.FenrirStuff_03_DropMap = GetPrivateProfileInt("GameServerInfo","FenrirStuff_03_DropMap", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.FenrirStuff_03_DropRate = GetPrivateProfileInt("GameServerInfo","FenrirStuff_03_DropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.FenrirRepairRate = GetPrivateProfileInt("GameServerInfo","FenrirRepairRate", 4000, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.FenrirDefaultMaxDurSmall = GetPrivateProfileInt("GameServerInfo","FenrirDefaultMaxDurSmall", 200, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.FenrirElfMaxDurSmall = GetPrivateProfileInt("GameServerInfo","FenrirElfMaxDurSmall", 160, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.Fenrir_01Level_MixRate = GetPrivateProfileInt("GameServerInfo","Fenrir_01Level_MixRate", 70, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.Fenrir_02Level_MixRate = GetPrivateProfileInt("GameServerInfo","Fenrir_02Level_MixRate", 50, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.Fenrir_03Level_MixRate = GetPrivateProfileInt("GameServerInfo","Fenrir_03Level_MixRate", 30, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.CrywolfMonsterDarkElfItemDrop = GetPrivateProfileInt("GameServerInfo","CrywolfMonsterDarkElfItemDrop",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.CrywolfMonsterDarkElfItemDropRate = GetPrivateProfileInt("GameServerInfo","CrywolfMonsterDarkElfItemDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.CrywolfMonsterDarkElfDropZenRate = GetPrivateProfileInt("GameServerInfo","CrywolfMonsterDarkElfDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.CrywolfMonsterDarkElfDropZen = GetPrivateProfileInt("GameServerInfo","CrywolfMonsterDarkElfDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.CrywolfBossMonsterItemDrop = GetPrivateProfileInt("GameServerInfo","CrywolfBossMonsterItemDrop",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.CrywolfBossMonsterItemDropRate = GetPrivateProfileInt("GameServerInfo","CrywolfBossMonsterItemDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.CrywolfBossMonsterDropZenRate = GetPrivateProfileInt("GameServerInfo","CrywolfBossMonsterDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.CrywolfBossMonsterDropZen = GetPrivateProfileInt("GameServerInfo","CrywolfBossMonsterDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));
		
	Configs.CrywolfApplyMvpBenefit = GetPrivateProfileInt("GameServerInfo","CrywolfApplyMvpBenefit",0, gDirPath.GetNewPath("commonserver.cfg"));

	int iCrywolfPlusChaosRateBenefit = GetPrivateProfileInt("GameServerInfo","CrywolfPlusChaosRateBenefit",0, gDirPath.GetNewPath("commonserver.cfg"));
	int iCrywolfMonHPRateBenefit = GetPrivateProfileInt("GameServerInfo","CrywolfMonHPRateBenefit", 100, gDirPath.GetNewPath("commonserver.cfg"));
	int iCrywolfKundunHPRefillState = GetPrivateProfileInt("GameServerInfo","CrywolfKundunHPRefillBenefit", 1, gDirPath.GetNewPath("commonserver.cfg"));

	g_CrywolfSync.SetKundunHPRefillState(iCrywolfKundunHPRefillState);
	g_CrywolfSync.SetPlusChaosRate(iCrywolfPlusChaosRateBenefit);
	g_CrywolfSync.SetMonHPBenefitRate(iCrywolfPlusChaosRateBenefit);

	Configs.CrywolfApplyMvpPenalty = GetPrivateProfileInt("GameServerInfo","CrywolfApplyMvpPenalty",0, gDirPath.GetNewPath("commonserver.cfg"));

	int iCrywolfGemDropPenaltyRate = GetPrivateProfileInt("GameServerInfo","CrwyolfGemDropPenaltyRate",100, gDirPath.GetNewPath("commonserver.cfg"));
	int iCrywolfGettingExpPenaltyRate = GetPrivateProfileInt("GameServerInfo","CrwyolfGettingExpPenaltyRate",100, gDirPath.GetNewPath("commonserver.cfg"));

	g_CrywolfSync.SetGemDropPenaltiyRate(iCrywolfGemDropPenaltyRate);
	g_CrywolfSync.SetGettingExpPenaltyRate(iCrywolfGettingExpPenaltyRate);


	int iKanturuEnableValue = GetPrivateProfileInt("GameServerInfo","KanturuEvent",0, gDirPath.GetNewPath("commonserver.cfg"));
	g_Kanturu.SetKanturuEnable(iKanturuEnableValue);

	
	Configs.BlockKanturuMapEnter = GetPrivateProfileInt("GameServerInfo","BlockKanturuMapEnter",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.BlockCastleSiegeMapEnter = GetPrivateProfileInt("GameServerInfo","BlockCastleSiegeMapEnter",0, gDirPath.GetNewPath("commonserver.cfg"));


	Configs.KanturuMayaHandItemDrop = GetPrivateProfileInt("GameServerInfo","KanturuMayaHandItemDrop",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.KanturuMayaHandItemDropRate = GetPrivateProfileInt("GameServerInfo","KanturuMayaHandItemDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.KanturuMayaHandDropZenRate = GetPrivateProfileInt("GameServerInfo","KanturuMayaHandDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.KanturuMayaHandDropZen = GetPrivateProfileInt("GameServerInfo","KanturuMayaHandDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.KanturuNightmareItemDrop = GetPrivateProfileInt("GameServerInfo","KanturuNightmareItemDrop",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.KanturuNightmareItemDropRate = GetPrivateProfileInt("GameServerInfo","KanturuNightmareItemDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.KanturuNightmareDropZenRate = GetPrivateProfileInt("GameServerInfo","KanturuNightmareDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.KanturuNightmareDropZen = GetPrivateProfileInt("GameServerInfo","KanturuNightmareDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.KanturuSpecialItemDropOn = GetPrivateProfileInt("GameServerInfo","KanturuSpecialItemDropOn",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.KanturuMoonStoneDropRate = GetPrivateProfileInt("GameServerInfo","KanturuMoonStoneDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.KanturuJewelOfHarmonyDropRate = GetPrivateProfileInt("GameServerInfo","KanturuJewelOfHarmonyDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));


	//CHECK GS-N POSITION
	int iRaklionEnableValue = GetPrivateProfileInt("GameServerInfo","RaklionEvent",0, gDirPath.GetNewPath("commonserver.cfg"));
	g_Raklion.SetRaklionEnable(iRaklionEnableValue);

	Configs.RaklionSelupanItemDrop = GetPrivateProfileInt("GameServerInfo","RaklionSelupanItemDrop",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.RaklionSelupanItemDropRate = GetPrivateProfileInt("GameServerInfo","RaklionSelupanItemDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.RaklionSelupanDropZenRate = GetPrivateProfileInt("GameServerInfo","RaklionSelupanDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.RaklionSelupanDropZen = GetPrivateProfileInt("GameServerInfo","RaklionSelupanDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.BlockRaklionMapEnter = GetPrivateProfileInt("GameServerInfo","BlockRaklionMapEnter",0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.HallowinDayEventOn = GetPrivateProfileInt("GameServerInfo","HallowinEventOn",0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.HallowinDayEventItemDropRate = GetPrivateProfileInt("GameServerInfo","HallowinEventPumpkinOfLuckDropRate",100, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.HallowinDayEventJOLBlessDropRate = GetPrivateProfileInt("GameServerInfo","HallowinEventJOLBlessDropRate",10, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.HallowinDayEventJOLAngerDropRaTe = GetPrivateProfileInt("GameServerInfo","HallowinEventJOLAngerDropRate",15, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.HallowinDayEventJOLScreamDropRate = GetPrivateProfileInt("GameServerInfo","HallowinEventJOLScreamDropRate",15, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.HallowinDayEventJOLFoodDropRate = GetPrivateProfileInt("GameServerInfo","HallowinEventJOLFoodDropRate",30, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.HallowinDayEventJOLDrinkDropRate = GetPrivateProfileInt("GameServerInfo","HallowinEventJOLDrinkDropRate",25, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.HallowinDayEventJOLPolymorphRingDropRate = GetPrivateProfileInt("GameServerInfo","HallowinEventJOLPolymorphRingDropRate", 5, gDirPath.GetNewPath("commonserver.cfg"));

	//Season 2.5 addon
	Configs.SantaPolymorphRingDropOn = GetPrivateProfileInt("GameServerInfo","SantaPolymorphRingDropOn", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.SantaPolymorphRingDropRate = GetPrivateProfileInt("GameServerInfo","SantaPolymorphRingDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.NewYearLuckyBagMonsterEventOn = GetPrivateProfileInt("GameServerInfo","NewYearLuckyBagMonsterEventOn", 0, gDirPath.GetNewPath("commonserver.cfg"));

	g_IllusionTempleEvent.ReadCommonServerInfo();

	Configs.CondorFlameDropRate = GetPrivateProfileInt("GameServerInfo","CondorFlameDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));

	//Season 3 add-on
	Configs.CherryBlossomEventOn = GetPrivateProfileInt("GameServerInfo","CherryBlossomEventOn", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.CherryBlossomEventItemDropRate = GetPrivateProfileInt("GameServerInfo","CherryBlossomEventItemDropRate", 8000, gDirPath.GetNewPath("commonserver.cfg"));

	//Season4.5 add-on
	Configs.LuckyCoinEventOn = GetPrivateProfileInt("GameServerInfo","LuckyCoinEventOn", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.LuckyCoinDropRate = GetPrivateProfileInt("GameServerInfo","LuckyCoinDropRate", 1000, gDirPath.GetNewPath("commonserver.cfg"));

	g_MasterLevelSystem.ReadCommonServerInfo();

	Configs.ML_OldScrollDropRate = GetPrivateProfileInt("GameServerInfo","ML_OldScrollDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ML_CovenantOfIllusionDropRate = GetPrivateProfileInt("GameServerInfo","ML_CovenantOfIllusionDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.ML_AngelKingsPaperDropRate = GetPrivateProfileInt("GameServerInfo","ML_AngelKingsPaperDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ML_BloodBoneDropRate = GetPrivateProfileInt("GameServerInfo","ML_BloodBoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.ML_EyesOfDevilSquareDropRate = GetPrivateProfileInt("GameServerInfo","ML_EyesOfDevilSquareDropRate ", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ML_KeysOfDevilSquareDropRate = GetPrivateProfileInt("GameServerInfo","ML_KeyOfDevilSquareDropRate ", 0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.ShadowPhantomMaxLevel = GetPrivateProfileInt("GameServerInfo","ShadowPhantomMaxLevel", 220, gDirPath.GetNewPath("commonserver.cfg"));

	GetPrivateProfileString("ConnectServerInfo", "IP", "", Configs.ConnectServer_IP, 20, Configs.szCommonlocIniFileName);
	GetPrivateProfileString("ConnectServerInfo", "PORT", "", szTemp, 10, Configs.szCommonlocIniFileName);
	Configs.ConnectServer_Port = atoi(szTemp);
	LogAddTD(lMsg.Get(MSGGET(1, 158)), Configs.ConnectServer_IP, Configs.ConnectServer_Port);

	CSSocket.SendSet(Configs.ConnectServer_IP, Configs.ConnectServer_Port);

	DevilSquareEventConnect = GetPrivateProfileInt("GameServerInfo","DevilSquareEventConnect", 1, gDirPath.GetNewPath("commonserver.cfg"));
	GetPrivateProfileString("GameServerInfo", "DevilSquareEventServer", "210.181.89.241", Configs.RankingServerIP, 20, gDirPath.GetNewPath("commonserver.cfg"));
	EventChipServerConnect = GetPrivateProfileInt("GameServerInfo","EventChipServerConnect", 0, gDirPath.GetNewPath("commonserver.cfg"));
	GetPrivateProfileString("GameServerInfo", "EventChipServerIp", "192.168.10.150", Configs.EventServerIP, 20, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.ApplyHeroSystem = GetPrivateProfileInt("GameServerInfo","ApplyHeroSystem", 0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.EnableBattleSoccer = GetPrivateProfileInt("GameServerInfo","EnableBattleSoccer", 1, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.EnableEventNPCTalk = GetPrivateProfileInt("GameServerInfo","EnableEventNPCTalk", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.EnableServerDivision = GetPrivateProfileInt("GameServerInfo","EnableServerDivision", 0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.MonsterHPAdjust = GetPrivateProfileInt("GameServerInfo","MonsterHPAdjust", 100, gDirPath.GetNewPath("commonserver.cfg"));

	cManager.Init();

	GetPrivateProfileString("GameServerInfo", "HackLogServer", "10.1.2.69", Configs.HackLogServerIP, 20, gDirPath.GetNewPath("commonserver.cfg"));
	gSendHackLog.SendSet(Configs.HackLogServerIP, 60005);

	Configs.EnableCheckPenetrationSkill = GetPrivateProfileInt("GameServerInfo","EnableCheckPenetrationSkill", 1, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.ShieldSystemOn = GetPrivateProfileInt("GameServerInfo","ShieldSystemOn", 0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.DamageDevideToSDRate = GetPrivateProfileInt("GameServerInfo","DamageDevideToSD", 90, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.DamageDevideToHPRate = GetPrivateProfileInt("GameServerInfo","DamageDevideToHP", 10, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.SuccessAttackRateOption = (double)(GetPrivateProfileInt("GameServerInfo","SuccessAttackRateOption", 20000, gDirPath.GetNewPath("commonserver.cfg")) / 10000 );
	Configs.SDChargingOption = GetPrivateProfileInt("GameServerInfo","SDChargingOption", 1, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ConstNumberOfShieldPoint = GetPrivateProfileInt("GameServerInfo","ConstNumberOfShieldPoint", 20, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ShieldAutoRefillOn = GetPrivateProfileInt("GameServerInfo","ShieldAutoRefillOn", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ShieldAutoRefillOnSafeZone = GetPrivateProfileInt("GameServerInfo","ShieldAutoRefilOnSafeZone", 1, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.PKLevelIncreaseOff = GetPrivateProfileInt("GameServerInfo","PKLevelIncreaseOff", 0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.CompoundPotionDropOn = GetPrivateProfileInt("GameServerInfo","CompoundPotionDropOn", 0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.CompoundPotionLv1DropRate = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv1DropRate", 100, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.CompoundPotionLv2DropRate = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv2DropRate", 80, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.CompoundPotionLv3DropRate = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv3DropRate", 50, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.CompoundPotionLv1DropLevel = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv1DropLevel", 68, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.CompoundPotionLv2DropLevel = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv2DropLevel", 96, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.CompoundPotionLv3DropLevel = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv3DropLevel", 118, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.ShieldComboMissOptionOn = GetPrivateProfileInt("GameServerInfo","ShieldComboMissOptionOn", 0, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.ShieldPotionLv1MixSuccessRate = GetPrivateProfileInt("GameServerInfo","ShieldPotionLv1MixSuccessRate", 50, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ShieldPotionLv1MixMoney = GetPrivateProfileInt("GameServerInfo","ShieldPotionLv1MixMoney", 100000, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ShieldPotionLv2MixSuccessRate = GetPrivateProfileInt("GameServerInfo","ShieldPotionLv2MixSuccessRate", 30, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ShieldPotionLv2MixMoney = GetPrivateProfileInt("GameServerInfo","ShieldPotionLv2MixMoney", 500000, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ShieldPotionLv3MixSuccessRate = GetPrivateProfileInt("GameServerInfo","ShieldPotionLv3MixSuccessRate", 30, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ShieldPotionLv3MixMoney = GetPrivateProfileInt("GameServerInfo","ShieldPotionLv3MixMoney", 1000000, gDirPath.GetNewPath("commonserver.cfg"));
	
	Configs.ShieldGageConstA = GetPrivateProfileInt("GameServerInfo","ShieldGageConstA", 12, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.ShieldGageConstB = GetPrivateProfileInt("GameServerInfo","ShieldGageConstB", 30, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.CheckSpeedHack = GetPrivateProfileInt("GameServerInfo","CheckSpeedHack", TRUE, gDirPath.GetNewPath("commonserver.cfg"));

	char cGsVersion[16];
	int iServerType = 0;
	iServerType = Configs.ServerType;
	wsprintf(cGsVersion, "%s_%d",Configs.szGameServerVersion, iServerType);

	wsprintf(szTemp, lMsg.Get(MSGGET(1, 160)), cGsVersion, Configs.szServerName, Configs.szClientVersion[0], Configs.szClientVersion[1], Configs.szClientVersion[2], Configs.szClientVersion[3], Configs.szClientVersion[4], Configs.szGameServerExeSerial, Configs.CountryName);
	LogAdd("User Object Size = %d", sizeof(OBJECTSTRUCT));

	char szDiplayGSInfo[512] = {0};
	wsprintfA(szDiplayGSInfo, "%s [%d][%d][%d]", szTemp, Configs.dwgCheckSum[1]%1000, Configs.dwgCheckSum[17]%1000, Configs.dwgCheckSum[1004]%1000);
	SetWindowText(ghWnd, szDiplayGSInfo);

	LoadItemBag();

	gPacketCheckSum.Init();

	Configs.DoPShopOpen = GetPrivateProfileInt("GameServerInfo","PersonalShopOpen", 0, gDirPath.GetNewPath("commonserver.cfg"));

	ReadEventInfo(MU_EVENT_ALL );

	Configs.UseCharacterAutoRecuperationSystem = GetPrivateProfileInt("GameServerInfo","UseCharacterAutoRecuperationSystem", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.CharacterRecuperationMaxLevel = GetPrivateProfileInt("GameServerInfo","CharacterRecuperationMaxLevel", 100, gDirPath.GetNewPath("commonserver.cfg"));

	Configs.SkillDistanceCheck = GetPrivateProfileInt("GameServerInfo","SkillDistanceCheck", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.SkillDistanceCheckTemp = GetPrivateProfileInt("GameServerInfo","SkillDistanceCheckTemp", 2, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.SkillDistanceKick = GetPrivateProfileInt("GameServerInfo","SkillDistanceKick", 0, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.SkillDistanceKickCount = GetPrivateProfileInt("GameServerInfo","SkillDistanceKickCount", 5, gDirPath.GetNewPath("commonserver.cfg"));
	Configs.SkillDiatanceKickCheckTime = GetPrivateProfileInt("GameServerInfo","SkillDistanceKickCheckTime", 10, gDirPath.GetNewPath("commonserver.cfg"));

	g_CashShop.CashShopOptioNReload();
	g_CashItemPeriodSystem.Initialize();

	//Season 3.0 add-on
	g_ChaosCard.Load("ChaosCard", gDirPath.GetNewPath("ChaosCardProbability.txt"));
	g_ChaosCard.Load("ChaosCardMini", gDirPath.GetNewPath("ChaosCardProbability1.txt"));
	g_ChaosCard.Load("ChaosCardGold", gDirPath.GetNewPath("ChaosCardProbability2.txt"));
	g_ChaosCard.Load("ChaosCardRare", gDirPath.GetNewPath("ChaosCardProbability3.txt"));

	g_BuffEffect.Load(gDirPath.GetNewPath("BuffEffect.txt"));
	
	g_EventItemList.Load(gDirPath.GetNewPath("EventItemList.txt"));

	SetEventOff();

	//Load all Configs!
	Configs.LoadConfigs();

	//Season4
	DuelManager.StartProcess();

	//Season5 Quests
	QuestManager.StartProcess();

	//New Move System
	WarpSystem.LoadMoves();

	//Extreme Points
	ExtremePoints.LoadStepsList();

	//Safe Map
	SafeMap.StartProcess();

	//Party System
	PartySystem.StartProcess();

	//GameMaster Manager
	GMManager.LoadConfigs();

	//Marry System
	MarrySystem.StartProcess(0);

	//Inferno Event
	InfernoEvent.LoadItemList();

	//Blood Castle Bag
	BloodCastleBag.LoadItemList();
}

void GameServerInfoSendStop()
{
	GSInfoSendFlag = 0;
}

void GameServerInfoSendStart()
{
	GSInfoSendFlag = 1;
}

struct PMSG_SERVERINFO
{
	PBMSG_HEAD h;	// C1:01
	short ServerCode;	// 4
	BYTE Percent;	// 6
	short UserCount;	// 8
	short AccountCount;	// A
	short PCbangCount;	// C
	short MaxUserCount;	// E
};

void GameServerInfoSend()
{
	PMSG_SERVERINFO pMsg;

	if ( gDisconnect == TRUE )
	{
		return;
	}

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x01;
	pMsg.h.size = sizeof( pMsg );

	if ( gObjTotalUser == 0 )
	{
		pMsg.Percent = 0;
	}
	else
	{
		pMsg.Percent = (gObjTotalUser * 100)/gServerMaxUser;
	}

	pMsg.ServerCode = Configs.GameServerCode;
	pMsg.UserCount = gObjTotalUser;
	pMsg.AccountCount = gLCount[0].Get();
	pMsg.PCbangCount = gLCount[1].Get();
	pMsg.MaxUserCount = gServerMaxUser;

	CSSocket.SendData((LPBYTE)&pMsg, pMsg.h.size);
}

void CheckSumFileLoad(char * szCheckSum)
{
	int DataBufferSize;
	char* DataBuffer;

	gGameServerAuth.RequestData(6);
	DataBufferSize=gGameServerAuth.GetDataBufferSize();
	DataBuffer=gGameServerAuth.GetDataBuffer();
	memcpy(Configs.dwgCheckSum, DataBuffer, DataBufferSize);
}

void LoadItemBag() //0053E2F0
{
	if ( LuckboxItemBag != FALSE )
	{
		delete LuckboxItemBag;
	}

	LuckboxItemBag = new CItemBag;

	if ( LuckboxItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	LuckboxItemBag->Init("eventitembag.txt");

	if ( Mon55 != FALSE )
	{
		delete Mon55;
	}

	Mon55 = new CItemBag;

	if ( Mon55 == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	Mon55->Init("eventitembag2.txt");

	if ( Mon53 != FALSE )
	{
		delete Mon53;
	}

	Mon53 = new CItemBag;

	if ( Mon53 == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	Mon53->Init("eventitembag3.txt");

	if ( StarOfXMasItemBag != FALSE )	
	{
		delete StarOfXMasItemBag;
	}

	StarOfXMasItemBag = new CItemBagEx;

	if ( StarOfXMasItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	StarOfXMasItemBag->Init("eventitembag4.txt");

	if ( FireCrackerItemBag != FALSE )	
	{
		delete FireCrackerItemBag;
	}

	FireCrackerItemBag = new CItemBag;

	if ( FireCrackerItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	FireCrackerItemBag->Init("eventitembag5.txt");

	if ( HeartOfLoveItemBag != FALSE )	
	{
		delete HeartOfLoveItemBag;
	}

	HeartOfLoveItemBag = new CItemBag;

	if ( HeartOfLoveItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	HeartOfLoveItemBag->Init("eventitembag5.txt");

	if ( GoldMedalItemBag != FALSE )	
	{
		delete GoldMedalItemBag;
	}

	GoldMedalItemBag = new CItemBag;

	if ( GoldMedalItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldMedalItemBag->Init("eventitembag6.txt");

	if ( SilverMedalItemBag != FALSE )	
	{
		delete SilverMedalItemBag;
	}

	SilverMedalItemBag = new CItemBag;

	if ( SilverMedalItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	SilverMedalItemBag->Init("eventitembag7.txt");

	if ( GoldGoblenItemBag != FALSE )	
	{
		delete GoldGoblenItemBag;
	}

	GoldGoblenItemBag = new CProbabilityItemBag;

	if ( GoldGoblenItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldGoblenItemBag->Init("eventitembag8.txt");

	if ( TitanItemBag != FALSE )	
	{
		delete TitanItemBag;
	}

	TitanItemBag = new CProbabilityItemBag;

	if ( TitanItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	TitanItemBag->Init("eventitembag9.txt");

	if ( GoldDerconItemBag != FALSE )	
	{
		delete GoldDerconItemBag;
	}

	GoldDerconItemBag = new CProbabilityItemBag;

	if ( GoldDerconItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldDerconItemBag->Init("eventitembag10.txt");

	if ( DevilLizardKingItemBag != FALSE )	
	{
		delete DevilLizardKingItemBag;
	}

	DevilLizardKingItemBag = new CProbabilityItemBag;

	if ( DevilLizardKingItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	DevilLizardKingItemBag->Init("eventitembag11.txt");

	if ( KanturItemBag != FALSE )	
	{
		delete KanturItemBag;
	}

	KanturItemBag = new CProbabilityItemBag;

	if ( KanturItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	KanturItemBag->Init("eventitembag12.txt");

	if ( RingEventItemBag != FALSE )	
	{
		delete RingEventItemBag;
	}

	RingEventItemBag = new CItemBag;

	if ( RingEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	RingEventItemBag->Init("eventitembag13.txt");

	if ( FriendShipItemBag != FALSE )	
	{
		delete FriendShipItemBag;
	}

	FriendShipItemBag = new CItemBag;

	if ( FriendShipItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	FriendShipItemBag->Init("eventitembag14.txt");

	if ( DarkLordHeartItemBag != FALSE )	
	{
		delete DarkLordHeartItemBag;
	}

	DarkLordHeartItemBag = new CItemBag;

	if ( DarkLordHeartItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	DarkLordHeartItemBag->Init("eventitembag15.txt");

	if ( KundunEventItemBag != FALSE )	
	{
		delete KundunEventItemBag;
	}

	KundunEventItemBag = new CItemBagEx;

	if ( KundunEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	KundunEventItemBag->Init("eventitembag17.txt");


	if ( CastleHuntZoneBossItemBag != FALSE )
	{
		delete CastleHuntZoneBossItemBag;
	}

	CastleHuntZoneBossItemBag = new CItemBagEx; 

	if ( CastleHuntZoneBossItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	CastleHuntZoneBossItemBag->Init("eventitembag18.txt");

	//GS_CASTLE EventItemBag for Special Mix
	if ( CastleItemMixItemBag != FALSE )
	{
		delete CastleItemMixItemBag;
	}

	CastleItemMixItemBag = new CItemBagEx;

	if ( CastleItemMixItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	CastleItemMixItemBag->Init("eventitembag19.txt");


	if ( HiddenTreasureBoxItemBag != NULL )
		delete HiddenTreasureBoxItemBag;

	HiddenTreasureBoxItemBag = new CItemBagEx; 
	if ( HiddenTreasureBoxItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	HiddenTreasureBoxItemBag->Init("eventitembag20.txt");

	if ( RedRibbonBoxEventItemBag != NULL )
		delete RedRibbonBoxEventItemBag;

	RedRibbonBoxEventItemBag = new CItemBagEx; 
	if ( RedRibbonBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	RedRibbonBoxEventItemBag->Init("eventitembag21.txt");

	if ( GreenRibbonBoxEventItemBag != NULL )
		delete GreenRibbonBoxEventItemBag;

	GreenRibbonBoxEventItemBag = new CItemBagEx; 
	if ( GreenRibbonBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GreenRibbonBoxEventItemBag->Init("eventitembag22.txt");

	if ( BlueRibbonBoxEventItemBag != NULL )
		delete BlueRibbonBoxEventItemBag;

	BlueRibbonBoxEventItemBag = new CItemBagEx; 
	if ( BlueRibbonBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	BlueRibbonBoxEventItemBag->Init("eventitembag23.txt");

	if ( PinkChocolateBoxEventItemBag != NULL )
		delete PinkChocolateBoxEventItemBag;

	PinkChocolateBoxEventItemBag = new CItemBagEx; 
	if ( PinkChocolateBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	PinkChocolateBoxEventItemBag->Init("eventitembag24.txt");

	if ( RedChocolateBoxEventItemBag != NULL )
		delete RedChocolateBoxEventItemBag;

	RedChocolateBoxEventItemBag = new CItemBagEx; 
	if ( RedChocolateBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	RedChocolateBoxEventItemBag->Init("eventitembag25.txt");

	if ( BlueChocolateBoxEventItemBag != NULL )
		delete BlueChocolateBoxEventItemBag;

	BlueChocolateBoxEventItemBag = new CItemBagEx; 
	if ( BlueChocolateBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	BlueChocolateBoxEventItemBag->Init("eventitembag26.txt");

	if ( LightPurpleCandyBoxEventItemBag != NULL )
		delete LightPurpleCandyBoxEventItemBag;

	LightPurpleCandyBoxEventItemBag = new CItemBagEx; 
	if ( LightPurpleCandyBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	LightPurpleCandyBoxEventItemBag->Init("eventitembag27.txt");

	if ( VermilionCandyBoxEventItemBag != NULL )
		delete VermilionCandyBoxEventItemBag;

	VermilionCandyBoxEventItemBag = new CItemBagEx; 
	if ( VermilionCandyBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	VermilionCandyBoxEventItemBag->Init("eventitembag28.txt");

	if ( DeepBlueCandyBoxEventItemBag != NULL )
		delete DeepBlueCandyBoxEventItemBag;

	DeepBlueCandyBoxEventItemBag = new CItemBagEx; 
	if ( DeepBlueCandyBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	DeepBlueCandyBoxEventItemBag->Init("eventitembag29.txt");

	if ( CrywolfDarkElfItemBag != NULL )
		delete CrywolfDarkElfItemBag;

	CrywolfDarkElfItemBag = new CItemBagEx; 
	if ( CrywolfDarkElfItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	CrywolfDarkElfItemBag->Init("eventitembag30.txt");

	if ( CrywolfBossMonsterItemBag != NULL )
		delete CrywolfBossMonsterItemBag;

	CrywolfBossMonsterItemBag = new CItemBagEx; 
	if ( CrywolfBossMonsterItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	CrywolfBossMonsterItemBag->Init("eventitembag31.txt");


	if ( KanturuMayaHandItemBag != NULL )
		delete KanturuMayaHandItemBag;

	KanturuMayaHandItemBag = new CItemBagEx; 
	if ( KanturuMayaHandItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	KanturuMayaHandItemBag->Init("eventitembag32.txt");

	if ( KanturuNightmareItemBag != NULL )
		delete KanturuNightmareItemBag;

	KanturuNightmareItemBag = new CItemBagEx; 
	if ( KanturuNightmareItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	KanturuNightmareItemBag->Init("eventitembag33.txt");


	if ( HallowinDayEventItemBag != NULL )
		delete HallowinDayEventItemBag;

	HallowinDayEventItemBag = new CItemBagEx; 
	if ( HallowinDayEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	HallowinDayEventItemBag->Init("eventitembag34.txt");

	//////////////////////////////////////////////

	if ( RingOfHeroBoxItemBag != NULL )
		delete RingOfHeroBoxItemBag;

	RingOfHeroBoxItemBag = new CItemBag; 
	if ( RingOfHeroBoxItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	RingOfHeroBoxItemBag->Init("eventitembag35.txt");

	if ( NewYearLuckyPouchItemBag != NULL )
		delete NewYearLuckyPouchItemBag;

	NewYearLuckyPouchItemBag = new CProbabilityItemBag; 
	if ( NewYearLuckyPouchItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	NewYearLuckyPouchItemBag->Init("eventitembag37.txt");

	if ( GMPresentBoxItemBag != NULL )
		delete GMPresentBoxItemBag;

	GMPresentBoxItemBag = new CProbabilityItemBag; 
	if ( GMPresentBoxItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GMPresentBoxItemBag->Init("eventitembag41.txt");

	//PCBangGageGreenBox
	if ( PCBangGageGreenBox != NULL )
		delete PCBangGageGreenBox;

	PCBangGageGreenBox = new CProbabilityItemBag; 
	if ( PCBangGageGreenBox == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	PCBangGageGreenBox->Init("eventitembag38.txt");

	//PCBangGageRedBox
	if ( PCBangGageRedBox != NULL )
		delete PCBangGageRedBox;

	PCBangGageRedBox = new CProbabilityItemBag; 
	if ( PCBangGageRedBox == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	PCBangGageRedBox->Init("eventitembag39.txt");

	//PCBangGagePurpleBox
	if ( PCBangGagePurpleBox != NULL )
		delete PCBangGagePurpleBox;

	PCBangGagePurpleBox = new CProbabilityItemBag; 
	if ( PCBangGagePurpleBox == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	PCBangGagePurpleBox->Init("eventitembag40.txt");

	//ReservedBox
	if ( ReservedBox != NULL )
		delete ReservedBox;

	ReservedBox = new CProbabilityItemBag; 
	if ( ReservedBox == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	ReservedBox->Init("eventitembag45.txt");

	//RaklionSelupanItemBag
	if ( RaklionSelupanItemBag != NULL )
		delete RaklionSelupanItemBag;

	RaklionSelupanItemBag = new CProbabilityItemBag; 
	if ( RaklionSelupanItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CProbabilityItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	RaklionSelupanItemBag->Init("eventitembag52.txt");

	//LuckyCoin10
	if ( LuckyCoin10 != NULL )
		delete LuckyCoin10;

	LuckyCoin10 = new CProbabilityItemBag; 
	if ( LuckyCoin10 == NULL )
	{
		// Memory allocation error
		MsgBox("CProbabilityItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	LuckyCoin10->Init("eventitembag56.txt");

	//LuckyCoin20
	if ( LuckyCoin20 != NULL )
		delete LuckyCoin20;

	LuckyCoin20 = new CProbabilityItemBag; 
	if ( LuckyCoin20 == NULL )
	{
		// Memory allocation error
		MsgBox("CProbabilityItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	LuckyCoin20->Init("eventitembag57.txt");

	//LuckyCoin30
	if ( LuckyCoin30 != NULL )
		delete LuckyCoin30;

	LuckyCoin30 = new CProbabilityItemBag; 
	if ( LuckyCoin30 == NULL )
	{
		// Memory allocation error
		MsgBox("CProbabilityItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	LuckyCoin30->Init("eventitembag58.txt");

	if ( IllusionTemple1ItemBag != NULL )
		delete IllusionTemple1ItemBag;

	IllusionTemple1ItemBag = new CProbabilityItemBag; 
	if ( IllusionTemple1ItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	IllusionTemple1ItemBag->Init("eventitembag42.txt");

	if ( IllusionTemple2ItemBag != NULL )
		delete IllusionTemple2ItemBag;

	IllusionTemple2ItemBag = new CProbabilityItemBag; 
	if ( IllusionTemple2ItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	IllusionTemple2ItemBag->Init("eventitembag43.txt");

	if ( IllusionTemple3ItemBag != NULL )
		delete IllusionTemple3ItemBag;

	IllusionTemple3ItemBag = new CProbabilityItemBag; 
	if ( IllusionTemple3ItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	IllusionTemple3ItemBag->Init("eventitembag44.txt");

	if ( MoonHarvestItemBag != NULL )
		delete MoonHarvestItemBag;

	MoonHarvestItemBag = new CProbabilityItemBag; 
	if ( MoonHarvestItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CProbabilityItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	MoonHarvestItemBag->Init("eventitembag46.txt");

	if ( CherryBlossom1 != NULL )
		delete CherryBlossom1;

	CherryBlossom1 = new CProbabilityItemBag; 
	if ( CherryBlossom1 == NULL )
	{
		// Memory allocation error
		MsgBox("CProbabilityItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	CherryBlossom1->Init("eventitembag48.txt");

	if ( CherryBlossom2 != NULL )
		delete CherryBlossom2;

	CherryBlossom2 = new CProbabilityItemBag; 
	if ( CherryBlossom2 == NULL )
	{
		// Memory allocation error
		MsgBox("CProbabilityItemBag-A %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	CherryBlossom2->Init("eventitembag49.txt");

	if ( CherryBlossom3 != NULL )
		delete CherryBlossom3;

	CherryBlossom3 = new CProbabilityItemBag; 
	if ( CherryBlossom3 == NULL )
	{
		// Memory allocation error
		MsgBox("CProbabilityItemBag-B %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	CherryBlossom3->Init("eventitembag50.txt");

	if ( CherryBlossom4 != NULL )
		delete CherryBlossom4;

	CherryBlossom4 = new CProbabilityItemBag; 
	if ( CherryBlossom4 == NULL )
	{
		// Memory allocation error
		MsgBox("CProbabilityItemBag-C %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	CherryBlossom4->Init("eventitembag51.txt");

	//XMasEventA
	if ( XMasEventA != NULL )
		delete XMasEventA;

	XMasEventA = new CProbabilityItemBag; 
	if ( XMasEventA == NULL )
	{
		// Memory allocation error
		MsgBox("CProbabilityItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	XMasEventA->Init("eventitembag53.txt");

	//XMasEventB
	if ( XMasEventB != NULL )
		delete XMasEventB;

	XMasEventB = new CProbabilityItemBag; 
	if ( XMasEventB == NULL )
	{
		// Memory allocation error
		MsgBox("CProbabilityItemBag100 %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	XMasEventB->Init("eventitembag54.txt");

	//XMasEventC
	if ( XMasEventC != NULL )
		delete XMasEventC;

	XMasEventC = new CProbabilityItemBag; 
	if ( XMasEventC == NULL )
	{
		// Memory allocation error
		MsgBox("CProbabilityItemBag1000 %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	XMasEventC->Init("eventitembag55.txt");

	//Medussa ItemBag
	if(MedussaItemBag != NULL) delete MedussaItemBag;
	MedussaItemBag = new CProbabilityItemBag; 
	if(MedussaItemBag == NULL){
		// Memory allocation error
		MsgBox("CProbabilityItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	MedussaItemBag->Init("eventitembag59.txt");

	//Silver Box
	if(SilverBoxItemBag != NULL) delete SilverBoxItemBag;
	SilverBoxItemBag = new CProbabilityItemBag; 
	if(SilverBoxItemBag == NULL){
		// Memory allocation error
		MsgBox("CProbabilityItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	SilverBoxItemBag->Init("eventitembag60.txt");

	//Golden Box
	if(GoldenBoxItemBag != NULL) delete GoldenBoxItemBag;
	GoldenBoxItemBag = new CProbabilityItemBag; 
	if(GoldenBoxItemBag == NULL){
		// Memory allocation error
		MsgBox("CProbabilityItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldenBoxItemBag->Init("eventitembag61.txt");

	//Gambling System
	if(pGamblingSystemItemBag != NULL) delete pGamblingSystemItemBag;
	pGamblingSystemItemBag	= new CGamblingItemBag; 

	if(pGamblingSystemItemBag == NULL){
		MsgBox("CProbabilityItemBag %s", lMsg.Get(110));
		return;
	}

	pGamblingSystemItemBag->GamblingItemBagInit(gDirPath.GetNewPath("GamblingSystem.txt"));


}

void SetMapName()
{
	strcpy(Configs.MapName[0], lMsg.Get(MSGGET(7,208)));
	strcpy(Configs.MapName[1], lMsg.Get(MSGGET(7,209)));
	strcpy(Configs.MapName[2], lMsg.Get(MSGGET(7,210)));
	strcpy(Configs.MapName[3], lMsg.Get(MSGGET(7,211)));
	strcpy(Configs.MapName[4], lMsg.Get(MSGGET(7,212)));
	strcpy(Configs.MapName[5], "Reserved");
	strcpy(Configs.MapName[6], lMsg.Get(MSGGET(7,214)));
	strcpy(Configs.MapName[7], lMsg.Get(MSGGET(7,213)));
	strcpy(Configs.MapName[8], lMsg.Get(MSGGET(7,216)));
	strcpy(Configs.MapName[9], lMsg.Get(MSGGET(7,217)));
	strcpy(Configs.MapName[10], lMsg.Get(MSGGET(7,221)));
	strcpy(Configs.MapName[11], lMsg.Get(MSGGET(7,222)));
	strcat(Configs.MapName[11], "1");
	strcpy(Configs.MapName[12], lMsg.Get(MSGGET(7,222)));
	strcat(Configs.MapName[12], "2");
	strcpy(Configs.MapName[13], lMsg.Get(MSGGET(7,222)));
	strcat(Configs.MapName[13], "3");
	strcpy(Configs.MapName[14], lMsg.Get(MSGGET(7,222)));
	strcat(Configs.MapName[14], "4");
	strcpy(Configs.MapName[15], lMsg.Get(MSGGET(7,222)));
	strcat(Configs.MapName[15], "5");
	strcpy(Configs.MapName[16], lMsg.Get(MSGGET(7,222)));
	strcat(Configs.MapName[16], "6");
}

struct ST_EVENT_FLAG
{
	BOOL bDevilSquare : 1;
	BOOL bBloodCastle : 1;
	BOOL			  : 6;
	BOOL bXMaxEvent   : 1;
	BOOL bFireCracker : 1;
	BOOL bHeartOfLove : 1;
	BOOL bMedalEvent  : 1;
	BOOL bRingEvent   : 1;
	BOOL bEventChip   : 1;
	BOOL bEledorado   : 1;
	BOOL			  : 1;
	BOOL bNPGGChecksum: 1;
};

union STU_EVENT_FLAG
{
	ST_EVENT_FLAG EventData;
	DWORD AllData;
};

int GetEventFlag()
{
	STU_EVENT_FLAG flag;

	flag.AllData = 0;

	if (Configs.DevilSquareEvent!=0)
	{
		flag.EventData.bDevilSquare=1;
	}
	if (Configs.BloodCastle!=0)
	{
		flag.EventData.bBloodCastle=1;
	}
	if (Configs.XMasEvent!=0)
	{
		flag.EventData.bXMaxEvent=1;
	}
	if (Configs.FireCrackerEvent!=0)
	{
		flag.EventData.bFireCracker=1;
	}
	if (Configs.HeartOfLoveEvent!=0)
	{
		flag.EventData.bHeartOfLove=1;
	}
	if (Configs.MedalEvent!=0)
	{
		flag.EventData.bMedalEvent=1;
	}
	if (Configs.DoRingEvent!=0)
	{
		flag.EventData.bRingEvent=1;
	}
	if (Configs.EventChipEvent!=0)
	{
		flag.EventData.bEventChip=1;
	}
	if (Configs.IsEledoradoEvent!=0)
	{
		flag.EventData.bEledorado=1;
	}
	if (Configs.UseNPGGChecksum!=0)
	{
		flag.EventData.bNPGGChecksum=1;
	}	

	return flag.AllData;	
}

void ReadEventInfo(MU_EVENT_TYPE eEventType)
{
	char szTemp[256];
	switch(eEventType)
	{
		case 0: //All Events
			g_DevilSquare.Load(gDirPath.GetNewPath("DevilSquare.dat"));
			Configs.DevilSquareEvent = GetPrivateProfileInt("GameServerInfo","DevilSquareEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EyesOfDevilSquareDropRate = GetPrivateProfileInt("GameServerInfo","EyesOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.KeyOfDevilSquareDropRate = GetPrivateProfileInt("GameServerInfo","KeyOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
			g_BloodCastle.Load(gDirPath.GetNewPath("BloodCastle.dat"));
			Configs.BloodCastle = GetPrivateProfileInt("GameServerInfo","BloodCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.BloodCastle_StartHour = GetPrivateProfileInt("GameServerInfo","BloodCastleStartHour", 1, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.AngelKingsPaperDropRate = GetPrivateProfileInt("GameServerInfo","AngelKingsPaperDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.BloodBoneDropRate = GetPrivateProfileInt("GameServerInfo","BloodBoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.StoneDropRate = GetPrivateProfileInt("GameServerInfo","StoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.BloodCastle_OddEvenHour = GetPrivateProfileInt("GameServerInfo","BloodCastleOddEvenHour", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.IsDropDarkLordItem = GetPrivateProfileInt("GameServerInfo","IsDropDarkLordItem", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.SleeveOfLordDropRate = GetPrivateProfileInt("GameServerInfo","SleeveOfLordDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.SleeveOfLordDropLevel = GetPrivateProfileInt("GameServerInfo","SleeveOfLordDropLevel", 86, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.SoulOfDarkHorseDropRate = GetPrivateProfileInt("GameServerInfo","SoulOfDarkHorseDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.SoulOfDarkHorseropLevel = GetPrivateProfileInt("GameServerInfo","SoulOfDarkHorseropLevel", 125, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.AngelKingsPaperDropRate = GetPrivateProfileInt("GameServerInfo","AngelKingsPaperDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.SoulOfDarkSpiritDropRate = GetPrivateProfileInt("GameServerInfo","SoulOfDarkSpiritDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.SoulOfDarkSpiritDropLevel = GetPrivateProfileInt("GameServerInfo","SoulOfDarkSpiritDropLevel", 96, gDirPath.GetNewPath("commonserver.cfg"));
			GetPrivateProfileString("GameServerInfo", "DarkSpiritAddExperience", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.DarkSpiritAddExperience = (float)atof(szTemp);

			Configs.IsDropGemOfDefend = GetPrivateProfileInt("GameServerInfo","IsDropGemOfDefend", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.GemOfDefendDropRate = GetPrivateProfileInt("GameServerInfo","GemOfDefendDropRate", 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.GemOfDefendDropLevel = GetPrivateProfileInt("GameServerInfo","GemOfDefendDropLevel", 75, gDirPath.GetNewPath("commonserver.cfg"));

			Configs.IsDropSetItemInCastleHuntZone = GetPrivateProfileInt("GameServerInfo","IsDropSetItemInCastleHuntZone", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.SetItemInCastleHuntZoneDropRate = GetPrivateProfileInt("GameServerInfo","SetItemInCastleHuntZoneDropRate", 1, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.SetItemInCastleHuntZoneDropLevel = GetPrivateProfileInt("GameServerInfo","SetItemInCastleHuntZoneDropLevel", 75, gDirPath.GetNewPath("commonserver.cfg"));

			Configs.AttackEventRegenTime = GetPrivateProfileInt("GameServerInfo","AttackEventRegenTime", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.IsEledoradoEvent = GetPrivateProfileInt("GameServerInfo","IsEledoradoEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldGoblenRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGoblenRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoTitanRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoTitanRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldDerconRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDerconRegenTime", 720, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoDevilLizardKingRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoDevilLizardKingRegenTime", 360, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoDevilTantarosRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoDevilTantarosRegenTime", 360, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldGoblenItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGoblenItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoTitanItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoTitanItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldDerconItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDerconItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoDevilLizardKingItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilLizardKingItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoDevilTantarosItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilTantarosItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldGoblenExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGoblenExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoTitanExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoTitanExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldDerconExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDerconExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoDevilLizardKingExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilLizardKingExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoDevilTantarosExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilTantarosExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			
			//Season 4.5 addon
			Configs.EledoradoGoldenRabbitRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenRabbitRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldenDarkKnightRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenDarkKnightRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldenDevilRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenDevilRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldenMonsterRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenMonsterRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldenCrustRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenCrustRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldenSatirosRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenSatirosRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldenTwintailRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenTwintailRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldenIronKnightRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenIronKnightRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldenNeipinRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenNeipinRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldenGreatDragonRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenGreatDragonRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			//Season 3.0 moved
			gEledoradoEvent.SetEventState(Configs.IsEledoradoEvent);
			gEledoradoEvent.Init();

			GetPrivateProfileString("GameServerInfo", "RingAttackEvent", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.DoRingEvent = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "RingOrcKillGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.RingOrcKillGiftRate = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "RingDropGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.RingDropGiftRate = atoi(szTemp);
			g_RingAttackEvent.Load(gDirPath.GetNewPath("RingAttackEvent.dat"));

			Configs.DoRingEvent = FALSE; //HermeX Fix

			g_RingAttackEvent.EnableEvent(Configs.DoRingEvent);

			//Season4 add-on
			Configs.DoXMasAttackEvent = GetPrivateProfileInt("GameServerInfo","XMasAttackEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.XMasAttackEventDropRate = GetPrivateProfileInt("GameServerInfo","XMasAttackEvent_DropRate", 50, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.XMasAttackEventDropZen = GetPrivateProfileInt("GameServerInfo","XMasAttackEvent_DropZen", 10000, gDirPath.GetNewPath("commonserver.cfg"));

			g_XMasAttackEvent.Load(gDirPath.GetNewPath("XMasAttackEvent.dat"));
			g_XMasAttackEvent.EnableEvent(Configs.DoXMasAttackEvent);

			Configs.XMasEvent_LuckNumber1st = GetPrivateProfileInt("GameServerInfo","XMasEvent_LuckNumber1st", 100, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.XMasEvent_LuckNumber2nd = GetPrivateProfileInt("GameServerInfo","XMasEvent_LuckNumber2nd", 200, gDirPath.GetNewPath("commonserver.cfg"));



			GetPrivateProfileString("GameServerInfo", "CastleDeepEvent", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.DoCastleDeepEvent = atoi(szTemp);
			g_CastleDeepEvent.Load(gDirPath.GetNewPath("CastleDeepEvent.dat"));
			g_CastleDeepEvent.EnableEvent(Configs.DoCastleDeepEvent);


			GetPrivateProfileString("GameServerInfo", "EVENT1", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.Event1 = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayMax", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.Event1ItemDropTodayCount = 0;
			Configs.Event1ItemDropTodayMax = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayPercent", "10000000", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.Event1ItemDropTodayPercent = atoi(szTemp);
			Configs.FireCrackerEvent = GetPrivateProfileInt("GameServerInfo","FireCrackerEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.FireCrackerDropRate = GetPrivateProfileInt("GameServerInfo","FireCrackerDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.ItemDropRateForgFireCracker = GetPrivateProfileInt("GameServerInfo","ItemDropRateForFireCracker", 2, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.OnlyFireCrackerEffectUse = GetPrivateProfileInt("GameServerInfo","OnlyFireCrackerEffectUse", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.MedalEvent = GetPrivateProfileInt("GameServerInfo","MedalEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.GoldMedalDropRate = GetPrivateProfileInt("GameServerInfo","GoldMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.SilverMedalDropRate = GetPrivateProfileInt("GameServerInfo","SilverMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.ItemDropRateForGoldMedal = GetPrivateProfileInt("GameServerInfo","ItemDropRateForGoldMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.ItemDropRateForSilverMedal = GetPrivateProfileInt("GameServerInfo","ItemDropRateForSilverMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.XMasEvent = GetPrivateProfileInt("GameServerInfo","XMasEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.XMasEvent_StarOfXMasDropRate = GetPrivateProfileInt("GameServerInfo","XMasEvent_StarOfXMasDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.XMasEvent_ItemDropRateForStarOfXMas = GetPrivateProfileInt("GameServerInfo","XMasEvent_ItemDropRateForStarOfXMas", 2, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.HeartOfLoveEvent = GetPrivateProfileInt("GameServerInfo","HeartOfLoveEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.HeartOfLoveDropRate = GetPrivateProfileInt("GameServerInfo","HeartOfLoveDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.ItemDropRateForgHeartOfLove = GetPrivateProfileInt("GameServerInfo","ItemDropRateForHeartOfLove", 2, gDirPath.GetNewPath("commonserver.cfg"));
			GetPrivateProfileString("GameServerInfo", "HappyNewYearTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.HappyNewYearNpcEvent = atoi(szTemp);

			if(Configs.HappyNewYearNpcEvent != 0)
			{
				// (Option) Happy-new-year NPC speaks
				LogAddTD(lMsg.Get(MSGGET(2, 61)));
			}

			GetPrivateProfileString("GameServerInfo", "MerryXMasTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.MerryXMasNpcEvent = atoi(szTemp);

			if(Configs.MerryXMasNpcEvent != 0)
			{
				// (Option) Christmas NPC speaks
				LogAddTD(lMsg.Get(MSGGET(2, 60)));
			}

			g_ChaosCastle.Load(gDirPath.GetNewPath("ChaosCastle.dat"));
			Configs.ChaosCastle = GetPrivateProfileInt("GameServerInfo","ChaosCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));

			Configs.KundunMarkDropRate = GetPrivateProfileInt("GameServerInfo","KundunMarkDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

			Configs.MysteriousBeadDropRate1 = GetPrivateProfileInt("GameServerInfo","MysteriouseBeadDropRate1", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.MysteriousBeadDropRate2 = GetPrivateProfileInt("GameServerInfo","MysteriouseBeadDropRate2", 0, gDirPath.GetNewPath("commonserver.cfg"));

			Configs.HiddenTreasureBoxOfflineRate = GetPrivateProfileInt("GameServerInfo","HiddenTreasureBoxOfflineRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
     
			GetPrivateProfileString("GameServerInfo", "EventManagerOn", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EventManagerOn = atoi(szTemp);
			g_EventManager.Load(gDirPath.GetNewPath("EventManagement.dat"));
			g_EventManager.RegEvent(0, DragonEvent);
			g_EventManager.RegEvent(1, AttackEvent);
			g_EventManager.RegEvent(2, &gEledoradoEvent);
			g_EventManager.RegEvent(3, &g_RingAttackEvent);
			g_EventManager.RegEvent(4, &g_XMasAttackEvent);//Season 4.5 addon
			g_EventManager.Init(Configs.EventManagerOn);

			Configs.DarkLordHeartDropRate = GetPrivateProfileInt("GameServerInfo","DarkLordHeartDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.DarkLordHeartOffEventRate = GetPrivateProfileInt("GameServerInfo","DarkLoadHeartOffEventRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

			// Ribbon Box Event
			Configs.RibbonBoxEvent = GetPrivateProfileInt("GameServerInfo","RibbonBoxEvent",0, gDirPath.GetNewPath("commonserver.cfg"));

			// Red Ribbon Box
			Configs.RedRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.RedRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.RedRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.RedRibbonBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.RedRibbonBoxDropZen = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

			// Green Ribbon Box
			Configs.GreenRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.GreenRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.GreenRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.GreenRibbonBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.GreenRibbonBoxDropZen = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

			// Blue Ribbon Box
			Configs.BlueRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.BlueRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.BlueRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.BlueRibbonBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.BlueRibbonBoxDropZen = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));


			// Chocolate Event
			Configs.ChocolateBoxEvent = GetPrivateProfileInt("GameServerInfo","ChocolateEvent",0, gDirPath.GetNewPath("commonserver.cfg"));

			// Pink Chocolate Box
			Configs.PinkChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.PinkChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.PinkChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.PinkChocolateBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.PinkChocolateBoxDropZen = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

			// Red Chocolate Box
			Configs.RedChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.RedChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.RedChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.RedChocolateBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.RedChocolateBoxDropZen = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

			// Blue Chocolate Box
			Configs.BlueChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.BlueChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.BlueChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.BlueChocolateBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.BlueChocolateBoxDropZen = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

			// Candy Event
			Configs.CandyBoxEvent = GetPrivateProfileInt("GameServerInfo","CandyBoxEvent",0, gDirPath.GetNewPath("commonserver.cfg"));

			// LightPurple Candy Box
			Configs.LightPurpleCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.LightPurpleCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.LightPurpleCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.LightPurpleCandyBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.LightPurpleCandyBoxDropZen = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

			// Vermilion Candy Box
			Configs.VermilionCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.VermilionCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.VermilionCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.VermilionCandyBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.VermilionCandyBoxDropZen = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

			// DeepBlue Candy Box
			Configs.DeepBlueCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.DeepBlueCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.DeepBlueCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.DeepBlueCandyBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.DeepBlueCandyBoxDropZen = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));
			break;
		case 1: //Devil Square
			g_DevilSquare.Load(gDirPath.GetNewPath("DevilSquare.dat"));
			Configs.DevilSquareEvent = GetPrivateProfileInt("GameServerInfo","DevilSquareEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EyesOfDevilSquareDropRate = GetPrivateProfileInt("GameServerInfo","EyesOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.KeyOfDevilSquareDropRate = GetPrivateProfileInt("GameServerInfo","KeyOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
			break;
		case 2: //Blood Castle
			g_BloodCastle.Load(gDirPath.GetNewPath("BloodCastle.dat"));
			Configs.BloodCastle = GetPrivateProfileInt("GameServerInfo","BloodCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.BloodCastle_StartHour = GetPrivateProfileInt("GameServerInfo","BloodCastleStartHour", 1, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.AngelKingsPaperDropRate = GetPrivateProfileInt("GameServerInfo","AngelKingsPaperDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.BloodBoneDropRate = GetPrivateProfileInt("GameServerInfo","BloodBoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.StoneDropRate = GetPrivateProfileInt("GameServerInfo","StoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.BloodCastle_OddEvenHour = GetPrivateProfileInt("GameServerInfo","BloodCastleOddEvenHour", 0, gDirPath.GetNewPath("commonserver.cfg"));
			break;
		case 3: //Attack Event
			Configs.AttackEventRegenTime = GetPrivateProfileInt("GameServerInfo","AttackEventRegenTime", 10, gDirPath.GetNewPath("commonserver.cfg"));
			break;
		case 4: //Eledorado Event
			Configs.IsEledoradoEvent = GetPrivateProfileInt("GameServerInfo","IsEledoradoEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldGoblenRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGoblenRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoTitanRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoTitanRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldDerconRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDerconRegenTime", 720, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoDevilLizardKingRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoDevilLizardKingRegenTime", 360, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoDevilTantarosRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoDevilTantarosRegenTime", 360, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldGoblenItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGoblenItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoTitanItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoTitanItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldDerconItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDerconItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoDevilLizardKingItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilLizardKingItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoDevilTantarosItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilTantarosItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldGoblenExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGoblenExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoTitanExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoTitanExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoGoldDerconExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDerconExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoDevilLizardKingExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilLizardKingExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.EledoradoDevilTantarosExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilTantarosExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			
			/* WZ MISSED ?
			gEledoradoGoldenRabbitRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenRabbitRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			gEledoradoGoldenDarkKnightRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenDarkKnightRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			gEledoradoGoldenDevilRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenDevilRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			gEledoradoGoldenMonsterRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenMonsterRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			gEledoradoGoldenCrustRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenCrustRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			gEledoradoGoldenSatirosRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenSatirosRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			gEledoradoGoldenTwintailRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenTwintailRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			gEledoradoGoldenIronKnightRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenIronKnightRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			gEledoradoGoldenNeipinRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenNeipinRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			gEledoradoGoldenGreatDragonRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldenGreatDragonRegenTime", 180, gDirPath.GetNewPath("commonserver.cfg"));
			*/
			break;
		case 5: //Ring Event
			{
				char szTemp[256];

				GetPrivateProfileString("GameServerInfo", "RingAttackEvent", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.DoRingEvent = atoi(szTemp);
				GetPrivateProfileString("GameServerInfo", "RingOrcKillGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.RingOrcKillGiftRate = atoi(szTemp);
				GetPrivateProfileString("GameServerInfo", "RingDropGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.RingDropGiftRate = atoi(szTemp);
				g_RingAttackEvent.Load(gDirPath.GetNewPath("RingAttackEvent.dat"));
			
				//Crazzy-fix
				Configs.DoRingEvent = FALSE;
				
				g_RingAttackEvent.EnableEvent(Configs.DoRingEvent);
			}
			break;

		case 6: //Event 1
			GetPrivateProfileString("GameServerInfo", "EVENT1", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.Event1 = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayMax", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.Event1ItemDropTodayCount = 0;
			Configs.Event1ItemDropTodayMax = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayPercent", "10000000", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.Event1ItemDropTodayPercent = atoi(szTemp);
			break;
		case 7: //Fire Cracker
			Configs.FireCrackerEvent = GetPrivateProfileInt("GameServerInfo","FireCrackerEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.FireCrackerDropRate = GetPrivateProfileInt("GameServerInfo","FireCrackerDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.ItemDropRateForgFireCracker = GetPrivateProfileInt("GameServerInfo","ItemDropRateForFireCracker", 2, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.OnlyFireCrackerEffectUse = GetPrivateProfileInt("GameServerInfo","OnlyFireCrackerEffectUse", 0, gDirPath.GetNewPath("commonserver.cfg"));
			break;
		case 8: //Medal Event
			Configs.MedalEvent = GetPrivateProfileInt("GameServerInfo","MedalEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.GoldMedalDropRate = GetPrivateProfileInt("GameServerInfo","GoldMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.SilverMedalDropRate = GetPrivateProfileInt("GameServerInfo","SilverMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.ItemDropRateForGoldMedal = GetPrivateProfileInt("GameServerInfo","ItemDropRateForGoldMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.ItemDropRateForSilverMedal = GetPrivateProfileInt("GameServerInfo","ItemDropRateForSilverMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));
			break;
		case 9: //XMax Event
			Configs.XMasEvent = GetPrivateProfileInt("GameServerInfo","XMasEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.XMasEvent_StarOfXMasDropRate = GetPrivateProfileInt("GameServerInfo","XMasEvent_StarOfXMasDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.XMasEvent_ItemDropRateForStarOfXMas = GetPrivateProfileInt("GameServerInfo","XMasEvent_ItemDropRateForStarOfXMas", 2, gDirPath.GetNewPath("commonserver.cfg"));
			break;
		case 10: //Heart Of Love Event
			Configs.HeartOfLoveEvent = GetPrivateProfileInt("GameServerInfo","HeartOfLoveEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.HeartOfLoveDropRate = GetPrivateProfileInt("GameServerInfo","HeartOfLoveDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.ItemDropRateForgHeartOfLove = GetPrivateProfileInt("GameServerInfo","ItemDropRateForHeartOfLove", 2, gDirPath.GetNewPath("commonserver.cfg"));
			break;
		case 11: //Happy New Year Event
			GetPrivateProfileString("GameServerInfo", "HappyNewYearTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.HappyNewYearNpcEvent = atoi(szTemp);

			if(Configs.HappyNewYearNpcEvent != 0){
				LogAddTD(lMsg.Get(MSGGET(2, 61)));
			}
			break;
		case 12: //Merry  XMax Event NPC talk
			GetPrivateProfileString("GameServerInfo", "MerryXMasTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.MerryXMasNpcEvent = atoi(szTemp);

			if(Configs.MerryXMasNpcEvent != 0){
				LogAddTD(lMsg.Get(MSGGET(2, 60)));
			}
			break;
		case 13: //Chaos Castle
			g_ChaosCastle.Load(gDirPath.GetNewPath("ChaosCastle.dat"));
			Configs.ChaosCastle = GetPrivateProfileInt("GameServerInfo","ChaosCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			break;
		case 14:
				// Ribbon Box Event
				Configs.RibbonBoxEvent = GetPrivateProfileInt("GameServerInfo","RibbonBoxEvent",0, gDirPath.GetNewPath("commonserver.cfg"));

				// Red Ribbon Box
				Configs.RedRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.RedRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.RedRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.RedRibbonBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.RedRibbonBoxDropZen = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

				// Green Ribbon Box
				Configs.GreenRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.GreenRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.GreenRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.GreenRibbonBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.GreenRibbonBoxDropZen = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

				// Blue Ribbon Box
				Configs.BlueRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.BlueRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.BlueRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.BlueRibbonBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.BlueRibbonBoxDropZen = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));
				break;

		case 15:
				// Chocolate Event
				Configs.ChocolateBoxEvent = GetPrivateProfileInt("GameServerInfo","ChocolateEvent",0, gDirPath.GetNewPath("commonserver.cfg"));

				// Pink Chocolate Box
				Configs.PinkChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.PinkChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.PinkChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.PinkChocolateBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.PinkChocolateBoxDropZen = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

				// Red Chocolate Box
				Configs.RedChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.RedChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.RedChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.RedChocolateBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.RedChocolateBoxDropZen = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

				// Blue Chocolate Box
				Configs.BlueChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.BlueChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.BlueChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.BlueChocolateBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.BlueChocolateBoxDropZen = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));
				break;

		case 16:
				// Candy Event
				Configs.CandyBoxEvent = GetPrivateProfileInt("GameServerInfo","CandyBoxEvent",0, gDirPath.GetNewPath("commonserver.cfg"));

				// LightPurple Candy Box
				Configs.LightPurpleCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.LightPurpleCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.LightPurpleCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.LightPurpleCandyBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.LightPurpleCandyBoxDropZen = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

				// Vermilion Candy Box
				Configs.VermilionCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.VermilionCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.VermilionCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.VermilionCandyBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.VermilionCandyBoxDropZen = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));

				// DeepBlue Candy Box
				Configs.DeepBlueCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.DeepBlueCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.DeepBlueCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.DeepBlueCandyBoxDropZenRate = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropZenRate",0, gDirPath.GetNewPath("commonserver.cfg"));
				Configs.DeepBlueCandyBoxDropZen = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropZen",0, gDirPath.GetNewPath("commonserver.cfg"));
				break;
	}
}

void ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType)
{
	char szTemp[256];

	switch ( eGameEtcType )
	{
		case 0:
			GetPrivateProfileString("GameServerInfo", "CreateCharacter", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.CreateCharacter = atoi(szTemp);

			GetPrivateProfileString("GameServerInfo", "GuildCreate", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.GuildCreate = atoi(szTemp);
			LogAdd(lMsg.Get(MSGGET(2, 57)), Configs.GuildCreate);
			GetPrivateProfileString("GameServerInfo", "GuildDestroy", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.GuildDestroy = atoi(szTemp);
			LogAdd(lMsg.Get(MSGGET(2, 58)), Configs.GuildDestroy);
			GetPrivateProfileString("GameServerInfo", "GuildCreateLevel", "100", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.GuildCreateLevel = atoi(szTemp);
			LogAdd(lMsg.Get(MSGGET(2, 59)), Configs.GuildCreateLevel);

			Configs.CastleGuildDestoyLimit = GetPrivateProfileInt("GameServerInfo", "CastleOwnerGuildDestroyLimit", 1, gDirPath.GetNewPath("commonserver.cfg"));

			GetPrivateProfileString("GameServerInfo", "Trade", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.CanTrade = atoi(szTemp);

			Configs.CanChaosBox = GetPrivateProfileInt("GameServerInfo", "ChaosBox", 0, gDirPath.GetNewPath("commonserver.cfg"));

			GetPrivateProfileString("GameServerInfo", "PKItemDrop", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.PkItemDrop = atoi(szTemp);

			GetPrivateProfileString("GameServerInfo", "ItemDropPer", "10", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.ItemDropPer = atoi(szTemp);

			Configs.DoPShopOpen = GetPrivateProfileInt("GameServerInfo", "PersonalShopOpen", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.AttackSpeedTimeLimit = GetPrivateProfileInt("GameServerInfo", "AttackSpeedTimeLimit", 800, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.IsIgnorePacketSpeedHackDetect = GetPrivateProfileInt("GameServerInfo", "IsIgnorePacketHackDetect", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.HackCheckCount = GetPrivateProfileInt("GameServerInfo", "HackCheckCount", 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.MinimumAttackSpeedTime = GetPrivateProfileInt("GameServerInfo", "MinimumAttackSpeedTime", 200 , gDirPath.GetNewPath("commonserver.cfg"));
			Configs.DetectedHackKickCount = GetPrivateProfileInt("GameServerInfo", "DetectedHackKickCount", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.IsKickDetecHackCountLimit = GetPrivateProfileInt("GameServerInfo", "IsKickDetecHackCountLimit", 0, gDirPath.GetNewPath("commonserver.cfg"));
			break;

		case 1:
			GetPrivateProfileString("GameServerInfo", "CreateCharacter", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.CreateCharacter = atoi(szTemp);
			break;

		case 2:
			GetPrivateProfileString("GameServerInfo", "GuildCreate", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.GuildCreate = atoi(szTemp);
			LogAdd(lMsg.Get(MSGGET(2, 57)), Configs.GuildCreate);
			GetPrivateProfileString("GameServerInfo", "GuildDestroy", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.GuildDestroy = atoi(szTemp);
			LogAdd(lMsg.Get(MSGGET(2, 58)), Configs.GuildDestroy);
			GetPrivateProfileString("GameServerInfo", "GuildCreateLevel", "100", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.GuildCreateLevel = atoi(szTemp);
			LogAdd(lMsg.Get(MSGGET(2, 59)), Configs.GuildCreateLevel);
			Configs.CastleGuildDestoyLimit = GetPrivateProfileInt("GameServerInfo", "CastleOwnerGuildDestroyLimit", 1, gDirPath.GetNewPath("commonserver.cfg"));
			break;
		case 3:
			GetPrivateProfileString("GameServerInfo", "Trade", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.CanTrade = atoi(szTemp);
			break;
		case 4:
			Configs.CanChaosBox = GetPrivateProfileInt("GameServerInfo", "ChaosBox", 0, gDirPath.GetNewPath("commonserver.cfg"));
			break;
		case 5:
			Configs.DoPShopOpen = GetPrivateProfileInt("GameServerInfo", "PersonalShopOpen", 0, gDirPath.GetNewPath("commonserver.cfg"));
			break;
		case 6:
			GetPrivateProfileString("GameServerInfo", "PKItemDrop", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.PkItemDrop = atoi(szTemp);
			break;
		case 7:
			GetPrivateProfileString("GameServerInfo", "ItemDropPer", "10", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.ItemDropPer = atoi(szTemp);
			break;
		case 8:
			Configs.AttackSpeedTimeLimit = GetPrivateProfileInt("GameServerInfo", "AttackSpeedTimeLimit", 800, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.IsIgnorePacketSpeedHackDetect = GetPrivateProfileInt("GameServerInfo", "IsIgnorePacketHackDetect", 0, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.HackCheckCount = GetPrivateProfileInt("GameServerInfo", "HackCheckCount", 5, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.MinimumAttackSpeedTime = GetPrivateProfileInt("GameServerInfo", "MinimumAttackSpeedTime", 200 , gDirPath.GetNewPath("commonserver.cfg"));
			Configs.DetectedHackKickCount = GetPrivateProfileInt("GameServerInfo", "DetectedHackKickCount", 10, gDirPath.GetNewPath("commonserver.cfg"));
			Configs.IsKickDetecHackCountLimit = GetPrivateProfileInt("GameServerInfo", "IsKickDetecHackCountLimit", 0, gDirPath.GetNewPath("commonserver.cfg"));
			break;
		case 9:
			Configs.UseNPGGChecksum = GetPrivateProfileInt("GameServerInfo", "UseNPGGChecksum", 0, gDirPath.GetNewPath("commonserver.cfg"));
			break;
	}
}

void SetEventOff() //0058C4F0
{
	if(Configs.EventOff == TRUE)
	{
		//Lucky Coin Event
		Configs.LuckyCoinEventOn = FALSE;
		Configs.LuckyCoinDropRate = 0;

		//Ribbon Box Event
		Configs.RibbonBoxEvent = FALSE;
		Configs.RedRibbonBoxDropLevelMin = 0;
		Configs.RedRibbonBoxDropLevelMax = 0;
		Configs.RedRibbonBoxDropRate = 0;
		Configs.RedRibbonBoxDropZenRate = 0;
		Configs.RedRibbonBoxDropZen = 0;
		Configs.GreenRibbonBoxDropLevelMin = 0;
		Configs.GreenRibbonBoxDropLevelMax = 0;
		Configs.GreenRibbonBoxDropRate = 0;
		Configs.GreenRibbonBoxDropZenRate = 0;
		Configs.GreenRibbonBoxDropZen = 0;
		Configs.BlueRibbonBoxDropLevelMin = 0;
		Configs.BlueRibbonBoxDropLevelMax = 0;
		Configs.BlueRibbonBoxDropRate = 0;
		Configs.BlueRibbonBoxDropZenRate = 0;
		Configs.BlueRibbonBoxDropZen = 0;

		//Pouch of Blessing
		Configs.NewYearLuckyBagMonsterEventOn = FALSE;

		//Chocolate Event
		Configs.ChocolateBoxEvent = FALSE;
		Configs.PinkChocolateBoxDropLevelMin = 0;
		Configs.PinkChocolateBoxDropLevelMax = 0;
		Configs.PinkChocolateBoxDropRate = 0;
		Configs.PinkChocolateBoxDropZenRate = 0;
		Configs.PinkChocolateBoxDropZen = 0;
		Configs.RedChocolateBoxDropLevelMin = 0;
		Configs.RedChocolateBoxDropLevelMax = 0;
		Configs.RedChocolateBoxDropRate = 0;
		Configs.RedChocolateBoxDropZenRate = 0;
		Configs.RedChocolateBoxDropZen = 0;
		Configs.BlueChocolateBoxDropLevelMin = 0;
		Configs.BlueChocolateBoxDropLevelMax = 0;
		Configs.BlueChocolateBoxDropRate = 0;
		Configs.BlueChocolateBoxDropZenRate = 0;
		Configs.BlueChocolateBoxDropZen = 0;

		//Candy Box Event
		Configs.CandyBoxEvent = FALSE;
		Configs.LightPurpleCandyBoxDropLevelMin = 0;
		Configs.LightPurpleCandyBoxDropLevelMax = 0;
		Configs.LightPurpleCandyBoxDropRate = 0;
		Configs.LightPurpleCandyBoxDropZenRate = 0;
		Configs.LightPurpleCandyBoxDropZen = 0;
		Configs.VermilionCandyBoxDropLevelMin = 0;
		Configs.VermilionCandyBoxDropLevelMax = 0;
		Configs.VermilionCandyBoxDropRate = 0;
		Configs.VermilionCandyBoxDropZenRate = 0;
		Configs.VermilionCandyBoxDropZen = 0;
		Configs.DeepBlueCandyBoxDropLevelMin = 0;
		Configs.DeepBlueCandyBoxDropLevelMax = 0;
		Configs.DeepBlueCandyBoxDropRate = 0;
		Configs.DeepBlueCandyBoxDropZenRate = 0;
		Configs.DeepBlueCandyBoxDropZen = 0;
	}
}
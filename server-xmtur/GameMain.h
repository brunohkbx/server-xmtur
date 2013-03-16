#ifndef GAMEMAIN_H
#define	GAMEMAIN_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wsJoinServerCli.h"
#include "wsGameServer.h"
#include "DirPath.h"
#include "MapClass.h"
#include "NSerialCheck.h"
#include "..\common\classdef.h"
#include "CLoginCount.h"
#include "DragonEvent.h"
#include "AttackEvent.h"
#include "WzUdp.h"
#include "WhisperCash.h"
#include "DbSave.h"
#include "SimpleModulus.h"
#include "MonsterAttr.h"
#include "MonsterSetBase.h"
#include "PartyClass.h"
#include "ItemBagEx.h"
#include "ProbabilityItemBag.h"
#include "CastleSiege.h"
#include "CastleDeepEvent.h"
#include "Crywolf.h"
#include "GamblingSystem.h"

#define FIRST_PATH "c:\\Muproject\\data\\" //Original Path
#define COMMONSERVER_MAINSECTION "GameServerInfo"
#define COMMONSERVER_FILE "commonserver.cfg"
#define CONFIG_FILE ".\\Plugin.xtr"
#define NEWS_FILE ".\\NewsSystem.xtr"

struct PMSG_FRIEND_STATE
{
	PBMSG_HEAD h;	// C1:C4
	char Name[10];	// 3
	BYTE State;	// D
};

enum MU_EVENT_TYPE {
	MU_EVENT_ALL = 0x0,
	MU_EVENT_DEVILSQUARE = 0x1,
	MU_EVENT_BLOODCASTLE = 0x2,
	MU_EVENT_ATTACKEVENTTROOP = 0x3,
	MU_EVENT_GOLDENTROOP = 0x4,
	MU_EVENT_WHITEMAGETROOP = 0x5,
	MU_EVENT_LOVEPANGPANG = 0x6,
	MU_EVENT_FIRECRACKER = 0x7,
	MU_EVENT_MEDALION = 0x8,
	MU_EVENT_XMASSTAR = 0x9,
	MU_EVENT_HEARTOFLOVE = 0xa,
	MU_EVENT_SAY_HAPPYNEWYEAR = 0xb,
	MU_EVENT_SAY_MERRYXMAS = 0xc,
	MU_EVENT_CHAOSCASTLE = 0xd,
	MU_EVENT_CHRISTMAS_RIBBONBOX = 0xe,
	MU_EVENT_VALENTINESDAY_CHOCOLATEBOX = 0xf,
	MU_EVENT_WHITEDAY_CANDYBOX = 0x10,
};

enum MU_ETC_TYPE {
	MU_ETC_ALL = 0x0,
	MU_ETC_CREATECHARACTER = 0x1,
	MU_ETC_GUILD = 0x2,
	MU_ETC_TRADE = 0x3,
	MU_ETC_USECHAOSBOX = 0x4,
	MU_ETC_PERSONALSHOP = 0x5,
	MU_ETC_PKITEMDROP = 0x6,
	MU_ETC_ITEMDROPRATE = 0x7,
	MU_ETC_SPEEDHACK = 0x8,
	MU_ETC_GAMEGUARD = 0x9,
};

extern BOOL JoinServerConnected;
extern BOOL DataServerConnected;
extern BOOL GameServerCreated;
extern BOOL DevilSquareEventConnect;
extern BOOL IsDevilSquareEventConnected;
extern BOOL EventChipServerConnect;
extern BOOL IsEventChipServerConnected;
extern CDragonEvent * DragonEvent;
extern CAttackEvent * AttackEvent;
extern CItemBag * LuckboxItemBag;
extern CItemBag * Mon55;
extern CItemBag * Mon53;
extern CItemBagEx * StarOfXMasItemBag;
extern CItemBag * FireCrackerItemBag;
extern CItemBag * HeartOfLoveItemBag;
extern CItemBag * GoldMedalItemBag;
extern CItemBag * SilverMedalItemBag;
extern CItemBag * EventChipItemBag;
extern CProbabilityItemBag * GoldGoblenItemBag;
extern CProbabilityItemBag * TitanItemBag;
extern CProbabilityItemBag * GoldDerconItemBag;
extern CProbabilityItemBag * DevilLizardKingItemBag;
extern CProbabilityItemBag * KanturItemBag;
extern CItemBag * RingEventItemBag;
extern CItemBag * FriendShipItemBag;
extern CItemBag * DarkLordHeartItemBag;
extern CItemBagEx * KundunEventItemBag;

extern CItemBagEx * CastleHuntZoneBossItemBag;
extern CItemBagEx * CastleItemMixItemBag;


extern CItemBagEx * HiddenTreasureBoxItemBag;
extern CItemBagEx * RedRibbonBoxEventItemBag;
extern CItemBagEx * GreenRibbonBoxEventItemBag;
extern CItemBagEx * BlueRibbonBoxEventItemBag;
extern CItemBagEx * PinkChocolateBoxEventItemBag;
extern CItemBagEx * RedChocolateBoxEventItemBag;
extern CItemBagEx * BlueChocolateBoxEventItemBag;
extern CItemBagEx * LightPurpleCandyBoxEventItemBag;
extern CItemBagEx * VermilionCandyBoxEventItemBag;
extern CItemBagEx * DeepBlueCandyBoxEventItemBag;
extern CItemBagEx * CrywolfDarkElfItemBag;
extern CItemBagEx * CrywolfBossMonsterItemBag;
extern CItemBagEx * KanturuMayaHandItemBag;
extern CItemBagEx * KanturuNightmareItemBag;
extern CItemBagEx * HallowinDayEventItemBag;

extern CItemBag * RingOfHeroBoxItemBag;

extern CProbabilityItemBag * NewYearLuckyPouchItemBag; //test
extern CProbabilityItemBag * GMPresentBoxItemBag; //test
extern CProbabilityItemBag * IllusionTemple1ItemBag; //test
extern CProbabilityItemBag * IllusionTemple2ItemBag; //test
extern CProbabilityItemBag * IllusionTemple3ItemBag; //test
extern CProbabilityItemBag * MoonHarvestItemBag; //test
extern CProbabilityItemBag * MoonHarvestItemBag2; //test

extern CProbabilityItemBag * CherryBlossom1; //test
extern CProbabilityItemBag * CherryBlossom2; //test
extern CProbabilityItemBag * CherryBlossom3; //test
extern CProbabilityItemBag * CherryBlossom4; //test

extern CProbabilityItemBag * PCBangGageGreenBox; //test
extern CProbabilityItemBag * PCBangGageRedBox; //test
extern CProbabilityItemBag * PCBangGagePurpleBox; //test
extern CProbabilityItemBag * ReservedBox; //test

extern CProbabilityItemBag * RaklionSelupanItemBag; //test

extern CProbabilityItemBag * XMasEventA; //test
extern CProbabilityItemBag * XMasEventB; //test
extern CProbabilityItemBag * XMasEventC; //test

extern CProbabilityItemBag * LuckyCoin10; //test
extern CProbabilityItemBag * LuckyCoin20; //test
extern CProbabilityItemBag * LuckyCoin30; //test


extern CGamblingItemBag	*pGamblingSystemItemBag;


#ifdef INTERNATIONAL_JAPAN
extern int g_iJapan1StAnivItemDropRate;
#endif

extern CwsGameServer cGameServer;
extern wsJoinServerCli cJoinServer;
extern wsJoinServerCli cDataServer;
extern wsJoinServerCli cExDB;
extern wsJoinServerCli cRankingServer;
extern wsJoinServerCli cEventServer;
extern WzUdp CSSocket;

extern CDirPath gDirPath;

extern MapClass MapC[MAX_NUMBER_MAP];

extern CMonsterAttr gMAttr;
extern CMonsterSetBase gMSetBase;
extern classdef DCInfo;
extern CWhisperCash WhisperCash;
extern PartyClass gParty;
extern CDbSave gDbSave;

extern CGuildClass Guild;
extern CMsg lMsg;
extern NSerialCheck gNSerialCheck[OBJMAX];
extern CLoginCount gLCount[3];

extern CSimpleModulus g_SimpleModulusCS;
extern CSimpleModulus g_SimpleModulusSC;

extern int MapMinUserLevel[MAX_NUMBER_MAP];

struct CConfigs {

public:
	void LoadConfigs();
	void GameMainInit(HWND hWnd);
	
	//GameServer Common
	
	char ExDB_IP[256];
	char DataServer2_IP[256];
	char DataServer1_IP[256];
	char JoinServer_IP[256];
	char ConnectServer_IP[20];
	char ChaosEventServerIP[20];
	char RankingServerIP[20];
	char HackLogServerIP[20];
	char EventServerIP[20];
	int ConnectServer_Port;
	int GameServer_Port;
	int JoinServer_Port;
	int DataServer1_Port;
	int DataServer2_Port;
	int ExDB_Port;
	short GameServerCode;

	BOOL SpeedHackPlayerBlock;
	BOOL CanConnectMember;
	BOOL CanChangeCharacterName;
	int ServerType;
	int Partition;
	BOOL ApplyHeroSystem;
	int SpeedHackPenalty;
	BOOL EnableEventNPCTalk;
	BOOL EnableServerDivision;
	BOOL Event1;
	int MonsterHp;
	BOOL MerryXMasNpcEvent;
	BOOL HappyNewYearNpcEvent;
	int Event1ItemDropTodayCount;
	int Language;
	BOOL ChaosEvent;


	char StalkProtocolId[11];
	BOOL NonPK;
	BOOL PkLimitFree;
	BOOL XMasEvent;
	BOOL FireCrackerEvent;
	BOOL HeartOfLoveEvent;
	BOOL MedalEvent;
	BOOL EventChipEvent;
	BOOL DevilSquareEvent;
	BOOL WriteSkillLog;
	BOOL StoneItemDrop;
	BOOL DoRingEvent;
	BOOL DoXMasAttackEvent;
	BOOL EventManagerOn;
	int KundunMarkDropRate;
	int MarkOfTheLord;

	int DarkLordHeartDropRate;
	int DarkLordHeartOffEventRate;
	int MysteriousBeadDropRate1;
	int MysteriousBeadDropRate2;
	int HiddenTreasureBoxOfflineRate;
	BOOL IsIgnorePacketSpeedHackDetect;
	BOOL IsKickDetecHackCountLimit;
	BOOL TamaJJangEvent;
	int AppearTamaJJang;
	int TamaJJangTime;
	BOOL IsItemDropRingOfTransform;
	BOOL IsEledoradoEvent;
	BOOL DoPShopOpen;
	BOOL WriteChatLog;
	BOOL DisconnectHackUser;
	int BlockKanturuMapEnter;
	int BlockCastleSiegeMapEnter;

	BOOL SInfoSendFlag;


	BOOL NewServer;
	BOOL EventOff;
	DWORD LevelExperience[MAX_CHAR_LEVEL+1];
	char szGameServerExeSerial[24];
	char szServerName[50];
	char szKorItemTextFileName[256];
	char szKorSkillTextFileName[256];
	char szItemTextFileName[256];
	char szSkillTextFileName[256];
	char szQuestTextFileName[256];
	char szMoveReqFileName[256];
	char szCommonlocIniFileName[256];

	BOOL IsDropDarkLordItem;
	int SleeveOfLordDropRate;
	int SleeveOfLordDropLevel;
	int SoulOfDarkHorseDropRate;
	int SoulOfDarkHorseropLevel;
	int SoulOfDarkSpiritDropRate;
	int SoulOfDarkSpiritDropLevel;
	float DarkSpiritAddExperience;
	BOOL IsDropGemOfDefend;
	int GemOfDefendDropRate;
	int GemOfDefendDropLevel;
	int UseCharacterAutoRecuperationSystem;
	int CharacterRecuperationMaxLevel;
	int ServerGroupGuildChatting;
	int ServerGroupUnionChatting;
	BOOL RibbonBoxEvent;
	int RedRibbonBoxDropLevelMin;
	int RedRibbonBoxDropLevelMax;
	int RedRibbonBoxDropRate;
	int RedRibbonBoxDropZenRate;
	int RedRibbonBoxDropZen;
	int GreenRibbonBoxDropLevelMin;
	int GreenRibbonBoxDropLevelMax;
	int GreenRibbonBoxDropRate;
	int GreenRibbonBoxDropZenRate;
	int GreenRibbonBoxDropZen;
	int BlueRibbonBoxDropLevelMin;
	int BlueRibbonBoxDropLevelMax;
	int BlueRibbonBoxDropRate;
	int BlueRibbonBoxDropZenRate;
	int BlueRibbonBoxDropZen;

	BOOL NewYearLuckyBagMonsterEventOn;

	BOOL ChocolateBoxEvent;
	int PinkChocolateBoxDropLevelMin;
	int PinkChocolateBoxDropLevelMax;
	int PinkChocolateBoxDropRate;
	int PinkChocolateBoxDropZenRate;
	int PinkChocolateBoxDropZen;
	int RedChocolateBoxDropLevelMin;
	int RedChocolateBoxDropLevelMax;
	int RedChocolateBoxDropRate;
	int RedChocolateBoxDropZenRate;
	int RedChocolateBoxDropZen;
	int BlueChocolateBoxDropLevelMin;
	int BlueChocolateBoxDropLevelMax;
	int BlueChocolateBoxDropRate;
	int BlueChocolateBoxDropZenRate;
	int BlueChocolateBoxDropZen;
	BOOL CandyBoxEvent;
	int LightPurpleCandyBoxDropLevelMin;
	int LightPurpleCandyBoxDropLevelMax;
	int LightPurpleCandyBoxDropRate;
	int LightPurpleCandyBoxDropZenRate;
	int LightPurpleCandyBoxDropZen;
	int VermilionCandyBoxDropLevelMin;
	int VermilionCandyBoxDropLevelMax;
	int VermilionCandyBoxDropRate;
	int VermilionCandyBoxDropZenRate;
	int VermilionCandyBoxDropZen;
	int DeepBlueCandyBoxDropLevelMin;
	int DeepBlueCandyBoxDropLevelMax;
	int DeepBlueCandyBoxDropRate;
	int DeepBlueCandyBoxDropZenRate;
	int DeepBlueCandyBoxDropZen;
	BOOL FenrirStuffItemDrop;
	int FenrirStuff_01_DropLv_Min;
	int FenrirStuff_01_DropLv_Max;
	int FenrirStuff_01_DropMap;
	int FenrirStuff_01_DropRate;
	int FenrirStuff_02_DropLv_Min;
	int FenrirStuff_02_DropLv_Max;
	int FenrirStuff_02_DropMap;
	int FenrirStuff_02_DropRate;
	int FenrirStuff_03_DropLv_Min;
	int FenrirStuff_03_DropLv_Max;
	int FenrirStuff_03_DropMap;
	int FenrirStuff_03_DropRate;
	int FenrirRepairRate;
	int FenrirDefaultMaxDurSmall;
	int FenrirElfMaxDurSmall;
	int Fenrir_01Level_MixRate;
	int Fenrir_02Level_MixRate;
	int Fenrir_03Level_MixRate;
	BOOL CrywolfMonsterDarkElfItemDrop;
	int CrywolfMonsterDarkElfItemDropRate;
	int CrywolfMonsterDarkElfDropZenRate;
	int CrywolfMonsterDarkElfDropZen;
	BOOL CrywolfBossMonsterItemDrop;
	int CrywolfBossMonsterItemDropRate;
	int CrywolfBossMonsterDropZenRate;
	int CrywolfBossMonsterDropZen;
	int CrywolfApplyMvpBenefit;
	int CrywolfApplyMvpPenalty;
	int SkillDistanceCheck;
	int SkillDistanceCheckTemp;
	int SkillDistanceKick;
	int SkillDistanceKickCount;
	int SkillDiatanceKickCheckTime;

	BOOL KanturuMayaHandItemDrop;
	int KanturuMayaHandItemDropRate;
	int KanturuMayaHandDropZenRate;
	int KanturuMayaHandDropZen;
	BOOL KanturuNightmareItemDrop;
	int KanturuNightmareItemDropRate;
	int KanturuNightmareDropZenRate;
	int KanturuNightmareDropZen;
	BOOL KanturuSpecialItemDropOn;
	int KanturuMoonStoneDropRate;
	int KanturuJewelOfHarmonyDropRate;

	BOOL RaklionSelupanItemDrop;
	int RaklionSelupanItemDropRate;
	int RaklionSelupanDropZenRate;
	BOOL RaklionSelupanDropZen;
	int BlockRaklionMapEnter;

	BOOL LuckyCoinEventOn;
	int LuckyCoinDropRate;

	BOOL HallowinDayEventOn;
	int HallowinDayEventItemDropRate;
	int HallowinDayEventJOLBlessDropRate;
	int HallowinDayEventJOLAngerDropRaTe;
	int HallowinDayEventJOLScreamDropRate;
	int HallowinDayEventJOLFoodDropRate;
	int HallowinDayEventJOLDrinkDropRate;
	int HallowinDayEventJOLPolymorphRingDropRate;

	//Season2.5 add-on
	int SantaPolymorphRingDropOn;
	int SantaPolymorphRingDropRate;
	int CondorFlameDropRate;

	//Season3 add-on
	BOOL CherryBlossomEventOn;
	int CherryBlossomEventItemDropRate;
	int ML_OldScrollDropRate;
	int ML_CovenantOfIllusionDropRate;
	int ML_AngelKingsPaperDropRate;
	int ML_BloodBoneDropRate;
	int ML_EyesOfDevilSquareDropRate;
	int ML_KeysOfDevilSquareDropRate;
	int ShadowPhantomMaxLevel;

	int AbilityDebug; //For GMMNG

	//Season 4.5 addon
	int XMasVisitCount;
	int XMasEvent_LuckNumber1st;
	int XMasEvent_LuckNumber2nd;

	DWORD dwgCheckSum[MAX_CHECKSUM_KEY];

	int PkTime;
	BOOL CastleGuildDestoyLimit;
	DWORD ItemNumberCount;
	BOOL StalkProtocol;
	DWORD AttackEventRegenTime;
	int Year;
	BOOL OnlyFireCrackerEffectUse;
	int RingOrcKillGiftRate; //F8
	int RingDropGiftRate; //F4
	int XMasAttackEventDropRate; //F0
	int XMasAttackEventDropZen; //EC
	int EledoradoGoldGoblenItemDropRate;
	int EledoradoTitanItemDropRate;
	int EledoradoGoldDerconItemDropRate;
	int EledoradoDevilLizardKingItemDropRate;
	int EledoradoDevilTantarosItemDropRate;
	int EledoradoGoldGoblenExItemDropRate;
	int EledoradoTitanExItemDropRate;
	int EledoradoGoldDerconExItemDropRate;
	int EledoradoDevilLizardKingExItemDropRate;
	int EledoradoDevilTantarosExItemDropRate;
	int EledoradoGoldenRabbitRegenTime;
	int EledoradoGoldenDarkKnightRegenTime;
	int EledoradoGoldenDevilRegenTime;
	int EledoradoGoldenMonsterRegenTime;
	int EledoradoGoldenCrustRegenTime;
	int EledoradoGoldenSatirosRegenTime;
	int EledoradoGoldenTwintailRegenTime;
	int EledoradoGoldenIronKnightRegenTime;
	int EledoradoGoldenNeipinRegenTime;
	int EledoradoGoldenGreatDragonRegenTime;
	int KundunRefillHPSec;
	int KundunRefillHP;
	int KundunRefillHPTime;
	int KundunHPLogSaveTime;
	BOOL UseNPGGChecksum;
	int ShieldSystemOn;
	int DamageDevideToSDRate;
	int DamageDevideToHPRate;
	float SuccessAttackRateOption;
	int SDChargingOption;
	int ConstNumberOfShieldPoint;
	int ShieldAutoRefillOn;
	int ShieldAutoRefillOnSafeZone;
	int PKLevelIncreaseOff;
	int CompoundPotionDropOn;
	int CompoundPotionLv1DropRate;
	int CompoundPotionLv2DropRate;
	int CompoundPotionLv3DropRate;
	int CompoundPotionLv1DropLevel;
	int CompoundPotionLv2DropLevel;
	int CompoundPotionLv3DropLevel;
	BOOL ShieldComboMissOptionOn;
	int ShieldPotionLv1MixSuccessRate;
	int ShieldPotionLv1MixMoney;
	int ShieldPotionLv2MixSuccessRate;
	int ShieldPotionLv2MixMoney;
	int ShieldPotionLv3MixSuccessRate;
	int ShieldPotionLv3MixMoney;
	int ShieldGageConstA;
	int ShieldGageConstB;
	BOOL CheckSpeedHack;
	char MapName[MAX_NUMBER_MAP][255];
	char szGameServerVersion[12];
	char szClientVersion[8];
	BOOL CanTrade;
	BOOL CanChaosBox;
	BOOL CanWarehouseLock;
	BOOL EnableBattleSoccer;
	int LootingTime;
	int PkItemDrop;
	int ItemDropPer;
	int Event1ItemDropTodayMax;
	int Event1ItemDropTodayPercent;
	char CountryName[20];
	int CharacterDeleteMinLevel;
	BOOL CreateCharacter;
	BOOL GuildCreate;
	BOOL GuildDestroy;
	int GuildCreateLevel;
	BOOL ItemSerialCheck;
	float AddExperience;
	float AddZen;
	int XMasEvent_StarOfXMasDropRate;
	int XMasEvent_ItemDropRateForStarOfXMas;
	int FireCrackerDropRate;
	int ItemDropRateForgFireCracker;
	int HeartOfLoveDropRate;
	int ItemDropRateForgHeartOfLove;
	int GoldMedalDropRate;
	int SilverMedalDropRate;
	int ItemDropRateForGoldMedal;
	int ItemDropRateForSilverMedal;
	int BoxOfGoldDropRate;
	int ItemDropRateForBoxOfGold;
	int EventChipDropRateForBoxOfGold;
	int EyesOfDevilSquareDropRate;
	int KeyOfDevilSquareDropRate;
	int DQChaosSuccessRateLevel1;
	int DQChaosSuccessRateLevel2;
	int DQChaosSuccessRateLevel3;
	int DQChaosSuccessRateLevel4;
	int DQChaosSuccessRateLevel5;
	int DQChaosSuccessRateLevel6;
	int DQChaosSuccessRateLevel7;
	BOOL BloodCastle;
	int BloodCastle_StartHour;
	int StoneDropRate;
	int BloodCastle_OddEvenHour;
	int AngelKingsPaperDropRate;
	int BloodBoneDropRate;
	BOOL ChaosCastle;
	DWORD AttackSpeedTimeLimit;
	DWORD HackCheckCount;
	float DecTimePerAttackSpeed;
	int MinimumAttackSpeedTime;
	int DetectedHackKickCount;
	int TamaJJangKeepTime;
	int TamaJJangDisappearTime;
	int TamaJJangDisappearTimeRandomRange;
	BOOL ItemDropRingOfTransform;
	int QuestNPCTeleportTime;
	DWORD EledoradoGoldGoblenRegenTime;
	DWORD EledoradoTitanRegenTime;
	DWORD EledoradoGoldDerconRegenTime;
	DWORD EledoradoDevilLizardKingRegenTime;
	DWORD EledoradoDevilTantarosRegenTime;
	int ZenDurationTime;
	int MonsterHPAdjust;
	BOOL EnableCheckPenetrationSkill;

	CCastleSiege CastleSiege;
	CCastleDeepEvent CastleDeepEvent;
	int CastleItemMixLimit;
	int IsDropSetItemInCastleHuntZone;
	int SetItemInCastleHuntZoneDropRate;
	int SetItemInCastleHuntZoneDropLevel;
	BOOL DoCastleDeepEvent;

	char ConnectMessage[60];

	//Commands
	int PostEnable;
	int PostLevel;
	int PostCost;
	int PostColor;

	int AddStatsEnable;
	int AddStatsMaxPoints;
	int AddStatsLevel;
	int AddStatsCost;

	int ItemDropEnable;

	int PKClearEnable;
	int PKClearCost;
	int PKClearLevel;
	int PKClearMap;

	int MultiVaultEnable;
	int MultiVaultMax;
	int MultiVaultFloodTime;

	BYTE SummonerCreation_WOCard;

	int ML_AddZen;

	//Payment System
	BYTE Payment_IsEnable;
	int Payment_ExtraExperience;

	//Max Stats for All Class
	int MaxStr[6];
	int MaxEne[6];
	int MaxVit[6];
	int MaxAgi[6];
	int MaxCom;


	/////////////////////////
	//Fixes
	int DisableCheckSum;

	//Items
	int EnableExcellentWithAncient;
	int EnableExcellentWithSocket;
	int JewelOfLifeMaxOption;
	int RemovePotionLimit;
	int FixMarlonQuestRemove;
	int FixPStoreLevel5Remove;
	int FixSerial0Items;
	int FixRemovePKPartySystem;
	int FixMasterLevelEvents;
	int EnablePKPlayersInEvents;
	int EnableSiegeSkills;
	int RemoveAntiHack;
	int RemoveCastleMixLimit; //GetPrivateProfileInt("CastleMix","RemoveMixLimit",0,CONFIG_FILE)
	int RemovePartyZenPercent; //GetPrivateProfileInt("Fixes","RemovePartyZenPercent",0,CONFIG_FILE)
	int RemovePKItemDrop;

	//Customs Configs
	//Events
	int BC_Group;
	int BC_ID;
	int BC_Level;
	int BC_Skill;
	int BC_Luck;
	int BC_Dur;
	int BC_Exc;
	int BC_Opt;
	int BC_Anc;
	
	int IT_Group;
	int IT_ID;
	int IT_Level;
	int IT_Skill;
	int IT_Luck;
	int IT_Dur;
	int IT_Opt;
	int IT_Exc;
	int IT_Anc;
	int IT_MinPlayers;

	int WW_Group;
	int WW_ID;
	int WW_Level;
	int WW_Skill;
	int WW_Luck;
	int WW_Dur;
	int WW_Exc;
	int WW_Anc;
	int WW_Opt;

	int GDragonBoxCount;

	int CCDropGroup1;
	int CCDropID1;
	int CCDropGroup2;
	int CCDropID2;
	int CCDropGroup3;
	int CCDropID3;
	int CCDropGroup4;
	int CCDropID4;
	int CCItem1;
	int CCItem2;
	int CCItem3;
	int CCItem4;
	int CCMinPlayers;

	//Pets
	int DarkHorseMixRate;
	int DarkHorseMixCost;
	int DarkRavenMixRate;
	int DarkRavenMixCost;
	int DinorantMixRate;
	int FruitMixRate;

	//Chaos Machine Mix's
	int SoulPotionMixRate;
	int SoulPotionMixCost;
	int BlessPotionMixRate;
	int BlessPotionMixCost;
	int WingsLvl2MixRate;
	int WingsLvl3MixRate;
	int CondorMixRate;

	//Plus Items
	int DefaultMixRate;
	int DefaultMixRateWLuck;
	int DefaultMixMaxRate;
	int Items10MixRate;
	int Items10SpecialRate;
	int Items10WingsRate;
	int Items11MixRate;
	int Items11SpecialRate;
	int Items11WingsRate;
	int Items12MixRate;
	int Items12SpecialRate;
	int Items12WingsRate;
	int Items13MixRate;
	int Items13SpecialRate;
	int Items13WingsRate;
	int Items14MixRate;
	int Items15MixRate;

	//Kundun Configs
	int KundunAncientDrop;
	int KundunItemDrop;

	//Great Golden Dercon
	int GreatGoldenDragonCountDrop;

	//Raklion Item Drop
	int SelupanItemDrop;

	//Castle siege mix
	int CastleMixRate;

	//Kalima Gate
	int KalimaGateDissapearTime;

	//LevelPoints
	int DLPoints;
	int MGPoints;
	int NormalPoints;
	int MarlonPoints;
	int BCDLMGPoints;
	int BCNormalPoints;
	int BCMarlonPoints;
	int CCDLMGPoints;
	int CCNormalPoints;
	int CCMarlonPoints;
	int ITDLMGPoints;
	int ITNormalPoints;
	int ITMarlonPoints;
	int CWDLMGPoints;
	int CWNormalPoints;
	int CWMarlonPoints;

	//Summon Monsters
	int Summon1;
	int Summon2;
	int Summon3;
	int Summon4;
	int Summon5;
	int Summon6;
	int Summon7;

	//Transform Rings
	int Ring1;
	int Ring2;
	int Ring3;
	int Ring4;
	int Ring5;
	int Ring6;

	//Jewel Prices
	int SoulPrice;
	int BlessPrice;
	int ChaosPrice;
	int CreationPrice;
	int LifePrice;
	int FruitPrice;
	int MonarchPrice;
	int FeatherPrice;

	//Jewel Rate
	int SoulRate;
	int SoulSpecialRate;
	int SoulSocketRate;
	int SoulLuckRate;
	int LifeRate;

	//Skill Level
	int Skill1;
	int Skill2;
	int Skill3;
	int Skill4;
	int Skill5;
	int Skill6;

	//ManaShield
	int MS_Agility;
	int MS_Energy;
	int MS_Divisor;
	int ML_MS_Agility;
	int ML_MS_Energy;
	int ML_MS_Divisor;

	//Reflect Summoner
	int ReflectRate;
	int ReflectDivisor;

	//Berserker
	int Berserker_EnergyDivisor;
	int Berserker_EnergyAdd;
	int Berserker_ManaRate;
	int Berserker_Timer;

	//Inner Shield
	int Inner_Base;
	int Inner_Energy;
	int Inner_Vitality;
	int Inner_Timer;
	int Inner_Timer_Divisor;
	int ML_Inner_Base;
	int ML_Inner_Energy;
	int ML_Inner_Vitality;
	int ML_Inner_Timer;
	int ML_Inner_Timer_Divisor;

	//ItemDrop
	int ExcDropRate;
	int ExcLuckRate;
	int ExcSkilRate;
	int NormalLuckRate;
	int NormalSkillRate;

	//Party Exp
	int PartyExp_03;
	int PartyExp_04;
	int PartyExp_05;
	int PartyExp_Default;
	int SetPartyExp_02;
	int SetPartyExp_03;
	int SetPartyExp_04;
	int SetPartyExp_05;
	int SetPartyExp_Default;

	int AttackDamageMin_Bow_StrDexDiv_Elf;
	int AttackDamageMax_Bow_StrDexDiv_Elf;
	int AttackDamageMin_NoBow_DexDiv_Elf;
	int AttackDamageMax_NoBow_DexDiv_Elf;
	int AttackDamageMin_NoBow_StrDiv_Elf;
	int AttackDamageMax_NoBow_StrDiv_Elf;
	int AttackDamageMin_DexStrDiv_Elf;
	int AttackDamageMax_DexStrDiv_Elf;

	int AttackDamageMin_StrDiv_DK;
	int AttackDamageMax_StrDiv_DK;

	int AttackDamageMin_StrDiv_MG;
	int AttackDamageMax_StrDiv_MG;
	int AttackDamageMin_EneDiv_MG;
	int AttackDamageMax_EneDiv_MG;

	int AttackDamageMin_StrDiv_DL;
	int AttackDamageMin_EneDiv_DL;
	int AttackDamageMax_StrDiv_DL;
	int AttackDamageMax_EneDiv_DL;

	int AttackDamageMin_StrDiv_SUM;
	int AttackDamageMax_StrDiv_SUM;

	int AttackDamageMin_StrDiv_DW;
	int AttackDamageMax_StrDiv_DW;

	int AttackSpeed_Elf1;
	int AttackSpeed_DK_MG1;
	int AttackSpeed_DL1;
	int AttackSpeed_Sum1;
	int AttackSpeed_DW1;

	int AttackSpeed_Elf2;
	int AttackSpeed_DK_MG2;
	int AttackSpeed_DL2;
	int AttackSpeed_Sum2;
	int AttackSpeed_DW2;

	int SuccessfulBlocking_Elf;
	int SuccessfulBlocking_DW_DK_MG;
	int SuccessfulBlocking_DL;
	int SuccessfulBlocking_Sum;

	int Defense_Elf;
	int Defense_DK;
	int Defense_DL;
	int Defense_Sum;
	int Defense_DW_MG;
};


extern CConfigs Configs;

void gSetDate();
BOOL gJoomin15Check(char* szJN);
BOOL gJoominCheck(char* szJN, int iLimitAge);
void GameMainInit(HWND hWnd);
int GetWarehouseUsedHowMuch(int UserLevel, int MasterLevel, BOOL IsLock);
void GameMonsterAllAdd();
void GameMonsterAllCloseAndReLoad();
void GameMainFree();
BOOL GMJoinServerConnect(char* ConnectServer, DWORD wMsg);
BOOL GMRankingServerConnect(char* RankingServer, DWORD wMsg);
BOOL GMEventChipServerConnect(char* ServerIP, DWORD wMsg);
BOOL GMDataServerConnect(char* ConnectServer, WPARAM wMsg);
BOOL ExDataServerConnect(char* ConnectServer, DWORD wMsg);
BOOL GameMainServerCreate(DWORD sMsg, DWORD cMsg);
void GMServerMsgProc( WPARAM wParam, LPARAM lParam);
void GMClientMsgProc( WPARAM wParam, LPARAM lParam);
void GMJoinClientMsgProc(WPARAM wParam, LPARAM lParam);
void GMRankingClientMsgProc(WPARAM wParam, LPARAM lParam);
void GMEventChipClientMsgProc(WPARAM wParam, LPARAM lParam);
void ExDataClientMsgProc(WPARAM wParam, LPARAM lParam);
void GMDataClientMsgProc(WPARAM wParam, LPARAM lParam);
void ReadServerInfo();
void ReadCommonServerInfo();
void GameServerInfoSendStop();
void GameServerInfoSendStart();
void GameServerInfoSend();
void CheckSumFileLoad(char * szCheckSum);
void LoadItemBag();
void SetMapName();
int GetEventFlag();
void ReadEventInfo(MU_EVENT_TYPE eEventType);
void ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType);
void SetEventOff();
#endif
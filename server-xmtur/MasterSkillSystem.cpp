// MasterSkillSystem.cpp: implementation of the CMasterSkillSystem class.
//	GS	1.00.90	JPN	0xXXXXXXXX	-	Completed
//////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "MasterSkillSystem.h"
#include "Readscript.h"
#include "Gamemain.h"
#include "LogProc.h"
#include "MagicDamage.h"
#include "ObjCalCharacter.h"
#include "User.h"
#include "Protocol.h"
#include "MasterLevelSystem.h"
#include "Winutil.h"
#include "SkillAdditionInfo.h"
#include "BuffManager.h"
#include "ChaosCastle.h"
#include "GameServerAuth.h"
#include "MathLib.h"
#include "IllusionTempleEvent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMasterSkillSystem g_MasterSkillSystem;

CMasterSkillSystem::CMasterSkillSystem()
{
	this->Clear();
}

CMasterSkillSystem::~CMasterSkillSystem()
{

}

void CMasterSkillSystem::Clear()
{
	for(int i = 0; i < MAX_TYPES; i++)
	{
		memset(&this->m_SkillData[i], 0, 132); //sizeof SkillRowsData
	}

	for(int n = 0; n < MAX_SKILL_TREE_INFO ; n++)
	{
		memset(this->m_iClassSkillData[n], 0, 16); //sizeof ?
	}
}

void CMasterSkillSystem::Load(LPSTR lpszFileName)
{
	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		MsgBox("[MasterLevelSkillTree] - File load error : File Name Error");
		return;
	}

	try
	{
		SMDFile = fopen(lpszFileName, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("[MasterLevelSkillTree] - Can't Open %s ", lpszFileName);
			return;
		}

		this->Clear();

		SMDToken Token;
		int iType = -1;
		int iSkillIndex = 0;
		int iClassIndex = 0;

		while ( true )
		{
			Token = GetToken();
			
			if ( Token ==  END ) //2
			{
				break;
			}

			iType = TokenNumber;

			while ( true )
			{
				if ( iType == 0 ) //ebp-1c
				{
					Token = GetToken();

					if (strcmp("end", TokenString) == 0)
					{
						break;
					}

					this->m_iClassSkillData[iSkillIndex][iClassIndex] = TokenNumber;
					this->m_iClassSkillCount[iSkillIndex] += (int)TokenNumber;

					iClassIndex++;

					if ( iClassIndex >= 5 )
					{
						iClassIndex = 0;
						iSkillIndex++;
					}
				}
				else if( iType > 0 )
                {
                    this->m_SkillData[iType].Type = iType/4;
                    this->m_SkillData[iType].Index = iType%4;

                    Token = GetToken();

                    if(strcmp("end",TokenString) == 0)
					{
                        break;
					}

                    this->m_SkillData[iType].Unknown = TokenNumber;

                    for(int i = 0;i < MAX_SKILL_TREE_INFO-1;i++)
					{
                        for(int n = 0;n < MAX_CHAR_SKILL_TREE-1;n++)
                        {
                            Token = GetToken();
                            this->m_SkillData[iType].Unk[i][n] = TokenNumber;
                        }
                    }
                }
				else
				{
					Token = GetToken();

					if (strcmp("end", TokenString) == 0)
					{
						break;
					}
				}
			}
		}
		fclose(SMDFile);
		LogAddTD("[MasterLevelSkillTree] - %s file is Loaded", lpszFileName);
	}
	catch(...)
	{
		DWORD dwError = GetLastError();
		MsgBox("[MasterLevelSkillTree] - Loading Exception Error (%s) File. ", lpszFileName);
	}
}

void CMasterSkillSystem::ClearPassiveSkill(LPOBJ lpObj)
{
	memset(&lpObj->m_MLPassiveSkill, 0, sizeof(lpObj->m_MLPassiveSkill));
}

void CMasterSkillSystem::AddPassiveSkill(LPOBJ lpObj, int Skill, int Point) //0050AB10
{
	int loc2 = MagicDamageC.GetSkillReqStrength(Skill); //Check Strength??

	if(lpObj->Strength < loc2)
	{
		return;
	}

	int loc3 = MagicDamageC.GetSkillReqDexterity(Skill); //Check Dexterity??

	if(lpObj->Dexterity < loc3)
	{
		return;
	}

	int loc4 = MagicDamageC.SkillGet(Skill); //Check Damage or Value
	int loc5 = this->GetSkillID(Skill); //Check SkillID

	switch(loc5)
	{
	case 300:{	lpObj->m_MLPassiveSkill.m_iML_IncreaseAttackSuccessRate		= loc4;	}	break;
	case 305:{	lpObj->m_MLPassiveSkill.m_iML_AttackRate					= loc4;	}	break;
	case 310:{	lpObj->m_MLPassiveSkill.m_iML_IncreasePvPDefenceRate		= loc4;	}	break;
	case 315:{	lpObj->m_MLPassiveSkill.m_iML_DurationReduction1			= loc4;	
		lpObj->m_MLPassiveSkill.m_btML_DurationReduction1 = Point;	}	break; //Useless BYTE
	case 320:{	lpObj->m_MLPassiveSkill.m_iML_DurationReduction2			= loc4;	}	break; //Not Used
	case 325:{	lpObj->m_MLPassiveSkill.m_iML_ResistanceIncrease_Poison		= loc4;	}	break;
	case 330:{	lpObj->m_MLPassiveSkill.m_iML_ResistanceIncrease_Lightning	= loc4;	}	break;
	case 335:{	lpObj->m_MLPassiveSkill.m_iML_ResistanceIncrease_Ice		= loc4;	}	break;
	case 340:{	lpObj->m_MLPassiveSkill.m_iML_HPAutoIncrement				= loc4;	}	break;
	case 345:{	lpObj->m_MLPassiveSkill.m_iML_ZenIncrease					= loc4;	}	break;
	case 350:{	lpObj->m_MLPassiveSkill.m_iML_DefenseIncrease				= loc4;	}	break;
	case 355:{	lpObj->m_MLPassiveSkill.m_iML_MaximumLifeIncrease			= loc4;	}	break;
	case 360:{	lpObj->m_MLPassiveSkill.m_iML_MaximumAGIncrease				= loc4;	}	break;
	case 365:{	lpObj->m_MLPassiveSkill.m_iML_MonsterAttackManaIncrement	= loc4;	}	break;
	case 370:{	lpObj->m_MLPassiveSkill.m_iML_MonsterAttackLifeIncrement	= loc4;	}	break;
	case 375:{	lpObj->m_MLPassiveSkill.m_iML_MonsterAttackSDIncrement		= loc4;	}	break;
	case 380:{	lpObj->m_MLPassiveSkill.m_iML_IncreaseExpRate				= loc4;	}	break;

	//Season 4 add-on
	case 385:{	lpObj->m_MLPassiveSkill.m_iML_IncreaseMaxSD					= loc4;	}	break;
	case 390:{	lpObj->m_MLPassiveSkill.m_iML_IncreaseSDRecovery			= loc4;	}	break;
	case 395:{	lpObj->m_MLPassiveSkill.m_iML_IncreaseMaxDamage				= loc4;	}	break;
	case 400:{	lpObj->m_MLPassiveSkill.m_iML_IncreaseMinDamage				= loc4;	}	break;
	case 405:{	lpObj->m_MLPassiveSkill.m_iML_IncreaseManaReduction			= loc4;	}	break;
	case 410:{	lpObj->m_MLPassiveSkill.m_iML_IncreaseMaxWizardry			= loc4;	}	break;
	case 415:{	lpObj->m_MLPassiveSkill.m_iML_IncreaseMinWizardry			= loc4;	}	break;
	case 420:{	lpObj->m_MLPassiveSkill.m_iML_DecreasePetDurReduction		= loc4;	}	break;
	case 425:{	lpObj->m_MLPassiveSkill.m_iML_IncreaseMaxDmgWzdry			= loc4;	}	break;
	case 430:{	lpObj->m_MLPassiveSkill.m_iML_IncreaseMinDmgWzdry			= loc4;	}	break;
	}

	gObjCalCharacter(lpObj->m_Index);
}

int CMasterSkillSystem::CheckMagicAdd(LPOBJ lpObj, int skill, int level)
{
	if(gObjIsConnected(lpObj) == FALSE)
	{
		GCResultSend(lpObj->m_Index, 0x51, 3);
		return 2;
	}

	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return 3;
	}

	if(g_MasterLevelSystem.CheckIsMasterLevelCharacter(lpObj) == 0)
	{
		return 2;
	}

	if(g_MasterSkillSystem.CheckRequireStatus(skill) == 0) //should be "this->" lol
	{
		return 4;
	}

	int loc2 = MagicDamageC.GetSkillReqMLPoint(skill); //Check Requirement

	if(lpObj->MLPoint < loc2)
	{
		return 4;
	}

	if(this->FindPrimarySkill(lpObj, skill) == 0) //Former Skill
	{
		return 4;
	}

	int loc3 = MagicDamageC.GetSkillGroup(skill); //Check Skill
	int loc4 = 0;

	if(level > 1)
	{
		for(int i = 0; i < MAX_MAGIC; i++)
		{
			if(lpObj->Magic[i].IsMagic() == TRUE)
			{
				loc4 = MagicDamageC.GetSkillGroup(lpObj->Magic[i].m_Skill); //ebp10

				if(loc4 > 0)
				{
					if(loc4 == loc3 && lpObj->Magic[i].m_Skill == skill - 1 && lpObj->Magic[i].m_Level == level - 1)
					{
						return 1;
					}
				}
			}
		}
		return 4;
	}
	return 1;
}

int CMasterSkillSystem::FindPrimarySkill(LPOBJ lpObj, int skill)
{
	int loc2 = MagicDamageC.GetSkillReqBrandOfSkill(skill);

	if(loc2 <= 0)
	{
		return 1;
	}

	for(int i = 0; i < MAX_MAGIC; i++)
	{
		if(lpObj->Magic[i].IsMagic() == TRUE && lpObj->Magic[i].m_Skill == loc2)
		{
			return 1;
		}
	}

	return 0;
}

int CMasterSkillSystem::CheckRequireStatus(int skill)
{
	if(MagicDamageC.GetSkillUseType(skill) == 3 || MagicDamageC.GetSkillUseType(skill) == 4)
	{
		return 1;
	}

	return 0;
}

int CMasterSkillSystem::CheckPrimarySkill(LPOBJ lpObj, int skill)
{
	int loc2 = 0;

	for(int i = 0; i < MAX_MAGIC; i++)
	{
		if(lpObj->Magic[i].IsMagic() == TRUE && this->CheckRequireStatus(lpObj->Magic[i].m_Skill) != FALSE)
		{
			loc2 = this->GetSkillID(lpObj->Magic[i].m_Skill);

			if(loc2 > 0 && skill == MagicDamageC.GetSkillReqBrandOfSkill(loc2))
			{
				return 1;
			}
		}
	}

	return 0;
}

int CMasterSkillSystem::GetSkillID(int skill)
{
	if(this->CheckRequireStatus(skill) == FALSE)
	{
		return -1;
	}

	int loc2 = MagicDamageC.GetSkillReqLevel(skill);

	if(loc2 <= 0)
	{
		return -1;
	}

	int loc3 = skill - loc2 + 1;

	return loc3;
}

int CMasterSkillSystem::MagicAdd(LPOBJ lpObj, int aSkill, int level) //(Add Skill) 0050B420  (changed alot)
{
	int skill = -1,n,reqeng;

	skill = -1;

	if(g_EnergyCheckOff == 1)
	{
		reqeng = MagicDamageC.SkillGetRequireEnergy(aSkill);

		if(reqeng < 0)
		{
			LogAdd(lMsg.Get(549));
			return -1;
		}

		//Season 4 Update (Fix)
		if(lpObj->Class == CLASS_KNIGHT)
		{
			reqeng -= 10;
		}

		if(lpObj->Class == CLASS_ELF)
		{
			reqeng -= 5;
		}

		if((lpObj->Energy + lpObj->AddEnergy) < reqeng)
		{
			LogAdd(lMsg.Get(549));
			return -1;
		}
	}

	if(MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->ChangeUP,lpObj->ChangeUP3rd,aSkill) < 1)
	{
		LogAdd(lMsg.Get(550));
		return -1;
	}

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == aSkill)
			{
				LogAdd(lMsg.Get(555),lpObj->Magic[n].m_Skill,aSkill);
				return -1;
			}
		}
	}

	if(MagicDamageC.GetSkillUseType(aSkill) == 3) //Check Use Type
	{
		if(level > 1)
		{
			for(n = 0; n < MAGIC_SIZE; n++)
			{
				if(lpObj->Magic[n].IsMagic() == 1 && 
					MagicDamageC.GetSkillGroup(lpObj->Magic[n].m_Skill) == MagicDamageC.GetSkillGroup(aSkill) && 
					lpObj->Magic[n].m_Skill == aSkill - 1 && 
					lpObj->Magic[n].m_Level == level - 1)
				{
					skill = lpObj->Magic[n].UpdateML(aSkill, level);

					if(skill < 0)
					{
						return -1;
					}
					else
					{
						return n;
					}
				}
			}
		}
	}
	else if(MagicDamageC.GetSkillUseType(aSkill) == 4) //Check Use Type
	{
		for(n = 0; n < MAGIC_SIZE; n++)
		{
			if(lpObj->Magic[n].IsMagic() == 1 && 
				lpObj->Magic[n].m_Skill == MagicDamageC.GetSkillReqBrandOfSkill(aSkill))
			{
				skill = lpObj->Magic[n].UpdateML(aSkill, level);

				if(skill < 0)
				{
					return -1;
				}
				else
				{
					return n;
				}		
			}
		}
	}
	else
	{
		return -1;
	}

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(aSkill, level);

			if(skill < 0)
			{
				LogAdd(lMsg.Get(553),__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				return n;
			}
		}
	}

	LogAdd(lMsg.Get(554),__FILE__,__LINE__);
	return -1;
}

struct PMSG_ML_SKILL_SEND
{
    PBMSG_HEAD2 h;
	BYTE Result;
	WORD MLPoint;
	int  MLSkill;
	int  MLSkillLevel;
};

void CMasterSkillSystem::CGMasterSkillSend(PMSG_ML_SKILL_RECV_INFO * lpMsg, int aIndex)
{
	if(gObjIsConnected(aIndex) == FALSE)
	{
		CloseClient(aIndex);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_ML_SKILL_SEND pMsg;
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x52, sizeof(pMsg));

	pMsg.Result = 0;
	pMsg.MLPoint = lpObj->MLPoint;
	pMsg.MLSkill = -1;
	pMsg.MLSkillLevel = 0;

	int loc7 = MagicDamageC.GetSkillReqLevel(lpMsg->MLSkill);
	int loc8 = this->CheckMagicAdd(lpObj, lpMsg->MLSkill, loc7);

	if(loc8 == 1)
	{
		if(this->MagicAdd(lpObj, lpMsg->MLSkill, loc7) < 0)
		{
			loc8 = 5;
			LogAddTD("[MasterSkill] [%s][%s] Fail - Add Magic List, Skill:%d Level:%d", lpObj->AccountID, lpObj->Name, lpMsg->MLSkill, loc7);
		}
		else
		{
			lpObj->MLPoint -= MagicDamageC.GetSkillReqMLPoint(lpMsg->MLSkill);
			pMsg.Result = loc8;
			pMsg.MLPoint = lpObj->MLPoint;
			pMsg.MLSkill = lpMsg->MLSkill;
			pMsg.MLSkillLevel = loc7;

			this->AddPassiveSkill(lpObj, lpMsg->MLSkill, loc7);

			LogAddTD("[MasterSkill] [%s][%s] Success - Add Magic List, Skill:%d Level:%d MLPoint:%d", lpObj->AccountID, lpObj->Name, lpMsg->MLSkill, loc7, lpObj->MLPoint);
		}
	}
	else
	{
		pMsg.Result = loc8;
	}

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

BOOL CMasterSkillSystem::RunningSkill(LPOBJ lpObj, int aTargetIndex, CMagicInf *lpMagic, BOOL bCombo, BYTE x, BYTE y, BYTE TargetPos) //0050BBC0 
{
	if(lpObj == NULL || lpMagic == NULL)
	{
		return FALSE;
	}

	int MagicNumber = g_MasterSkillSystem.GetSkillID(lpMagic->m_Skill);

	switch(MagicNumber)
	{
	case 435:{this->WizardMagicDefense(lpObj, aTargetIndex, lpMagic);}								break;
	case 475:{this->SkillHealing(lpObj, aTargetIndex, lpMagic);}									break;
	case 455:
	case 495:{this->SkillWheel(lpObj, lpMagic, aTargetIndex, bCombo);}								break;
	case 470:{this->KnightSkillAddLife(lpObj, lpMagic);}											break;
	case 515:{this->SkillDarkHorseAttack(lpObj, aTargetIndex, lpMagic);}							break;
	case 520:{this->SkillFireBurst(lpObj, aTargetIndex, lpMagic);}									break;

	//Season 4 Add-on
	case 450:{this->SkillSuddenIce(lpObj->m_Index, lpMagic, x, y, aTargetIndex);}					break;
	case 460:{this->SkillKnightBlow(lpObj->m_Index, aTargetIndex, lpMagic, bCombo);}				break;
	case 465:{this->SkillBlowOfFury(lpObj->m_Index, lpMagic, x, y, aTargetIndex, bCombo);}			break;
	case 480:{this->SkillDefense(lpObj->m_Index, aTargetIndex, lpMagic);}							break;
	case 485:{this->SkillAttack(lpObj->m_Index, aTargetIndex, lpMagic);}							break;
	case 505:{this->SkillPowerSlash(lpObj->m_Index, lpMagic, x, y, TargetPos, aTargetIndex);}		break;

	//New Summoenr Skills
	case 535:{this->SkillSleep(lpObj->m_Index,aTargetIndex,lpMagic);}								break;
	case 550:{this->SkillDrainLife(lpObj->m_Index,aTargetIndex,lpMagic);} 							break;
	case 540:{this->SkillChainLightning(lpObj->m_Index,aTargetIndex,lpMagic);} 						break;
	case 545:{this->SkillLightningShock(lpObj->m_Index,aTargetIndex,lpMagic);} 						break;
	}

	return TRUE;
}

void CMasterSkillSystem::WizardMagicDefense(LPOBJ lpObj, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	int loc3 = 1;

	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_WIZARD && lpObj->Class != CLASS_MAGUMSA) //Prevent other class to cast Soul Barrier
	{
		return;
	}

	if(lpObj->PartyNumber != lpTargetObj->PartyNumber) //Party Antihack
	{
		return;
	}

	int loc4 = 0;
	int loc5 = 0;

	loc4 = 10 + ((lpObj->Dexterity + lpObj->AddDexterity) / Configs.ML_MS_Agility + (lpObj->Energy + lpObj->AddEnergy) / Configs.ML_MS_Energy);

	int loc6 = MagicDamageC.SkillGet(lpMagic->m_Skill);

	loc4 += loc6;

	int loc7 = g_SkillAdditionInfo.GetSoulBarrierManaRateLevel(lpMagic->m_Level);

	loc5 = 60 + (lpObj->Energy + lpObj->AddEnergy) / Configs.ML_MS_Divisor;

	gObjApplyBuffEffectDuration(lpTargetObj, AT_WIZARD_DEFENSE, 21, loc4, 28, loc7, loc5);
	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, lpTargetObj->m_Index, loc3);
}

void CMasterSkillSystem::SkillHealing(LPOBJ lpObj, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	int loc3 = 1;

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_ELF) //Prevent other class to cast Healing
	{
		return;
	}

	if(CC_MAP_RANGE(lpObj->MapNumber) && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	float Formula1 = ((lpObj->Energy + lpObj->AddEnergy) / 5) + 5;
	int SkillGet = MagicDamageC.SkillGet(lpMagic->m_Skill);

	if(SkillGet > 0)
	{
		Formula1 += Formula1 * SkillGet / 100.0f;
	}

	lpTargetObj->Life += Formula1;


    if(lpTargetObj->Life > lpTargetObj->AddLife+lpTargetObj->MaxLife)
    {
        lpTargetObj->Life = lpTargetObj->AddLife+lpTargetObj->MaxLife;
    }

	if(lpTargetObj->Type == OBJ_USER)
	{
		GCReFillSend(lpTargetObj->m_Index,lpTargetObj->Life,0xFF,0,lpTargetObj->iShield);
		GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpTargetObj->m_Index,loc3);

		for(int n = 0; n < MAX_VIEWPORT; n++)
		{
			if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
			{
				LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
				if(lpMonster->m_iCurrentAI != 0)
				{
					lpMonster->m_Agro.IncAgro(lpObj->m_Index,(lpObj->Energy / 5)/40);
				}
			}
		}
	}
	else if(lpTargetObj->m_RecallMon >= 0)
	{
		GCRecallMonLife(lpTargetObj->m_RecallMon,lpTargetObj->MaxLife,lpTargetObj->Life);
	}
}

void CMasterSkillSystem::SkillWheel(LPOBJ lpObj, CMagicInf * lpMagic, int aTargetIndex , BOOL isCombo)
{
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}

				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
				{
					EnableAttack = 1;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					int GetSkillDistance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);

					if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= GetSkillDistance)
					{
						bAttack = 0;

						if(HitCount > 15)
						{
							bAttack = 0;
						}

						if(HitCount >= 10)
						{
							if(rand()%2)
							{
								bAttack = 1;
							}
						}
						else
						{
							bAttack = 1;
						}

						if(bAttack != 0)
						{

							gObjAttack(lpObj,&gObj[tObjNum],lpMagic,0,1,0,isCombo);

							if(isCombo != 0)
							{
								GCMagicAttackNumberSend(lpObj,AT_SKILL_COMBO,gObj[tObjNum].m_Index,1);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
}

void CMasterSkillSystem::KnightSkillAddLife(LPOBJ lpObj, CMagicInf * lpMagic)
{
	int skillSuccess = true;

	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_KNIGHT && lpObj->Class != CLASS_MAGUMSA)
	{
		return;
	}

	int number;
	int partynum = 0;
	int totallevel = 0;
	int partycount;
	int dis;

	int ApplyPartyIndex[MAX_USER_IN_PARTY];

	int viewplayer = 0;
	int viewpercent = 0;

	LPOBJ lpPartyObj;

	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));

	partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyS[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					dis = gObjCalDistance(lpObj,&gObj[number]);

					if(dis < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
						viewplayer++;
					}
				}
			}
		}
	}

	if(viewplayer > 1)
	{
		if(viewplayer == 2)
		{
			viewpercent = 1;
		}
		else if(viewplayer == 3)
		{
			viewpercent = 2;
		}
		else if(viewplayer == 4)
		{
			viewpercent = 3;
		}
		else
		{
			viewpercent = 4;
		}
	}

	int SkillGet = MagicDamageC.SkillGet(lpMagic->m_Skill);

	int iaddLifepower = (Configs.ML_Inner_Base + (lpObj->Vitality + lpObj->AddVitality) / Configs.ML_Inner_Vitality + (lpObj->Energy + lpObj->AddEnergy) / Configs.ML_Inner_Energy + viewpercent) + SkillGet;

	int iLifeTime = Configs.ML_Inner_Timer + (lpObj->Energy + lpObj->AddEnergy) / Configs.ML_Inner_Timer_Divisor;
	int iaddlife;

	if(partynum == -1)
	{
		iaddlife = int(lpObj->MaxLife) * iaddLifepower / 100;
		gObjApplyBuffEffectDuration(lpObj, AT_SWELL_LIFE, 4, iaddlife, 0, 0, iLifeTime);
		GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpObj->m_Index,skillSuccess);
	}
	else
	{
		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			if(ApplyPartyIndex[n]!= -1)
			{
				lpPartyObj = &gObj[ApplyPartyIndex[n]];

				iaddlife = int(lpPartyObj->MaxLife) * iaddLifepower / 100;
				gObjApplyBuffEffectDuration(lpPartyObj, AT_SWELL_LIFE, 4, iaddlife, 0, 0, iLifeTime);
				GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpPartyObj->m_Index,skillSuccess);
			}
		}
	}
}

BOOL CMasterSkillSystem::SkillDarkHorseAttack(LPOBJ lpObj, int aTargetIndex, CMagicInf * lpMagic)
{ //Check for Siege Update
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpObj->m_Index,1);

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				if(gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110)
				{
	
					if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
					{
						EnableAttack = 1;
					}
					else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
					{
						EnableAttack = 1;
					}

					else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
					{
						EnableAttack = 1;
					}

					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;
	
						if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
	
						if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
						{
							EnableAttack = 1;
						}
					}
				}
	
				if(EnableAttack != 0)
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 5)
					{
						bAttack = 0;
	
						if(HitCount > 15)
						{
							bAttack = 0;
						}
	
						if(HitCount >= 10)
						{
							if(rand()%2)
							{
								bAttack = 1;
							}
						}
						else
						{
							bAttack = 1;
						}
	
						if(bAttack != 0)
						{
							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,500,lpMagic->m_Skill,0);
						}
					}
				}
			}
		}
	
		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return TRUE;
}

BOOL CMasterSkillSystem::SkillFireBurst(LPOBJ lpObj, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	int StartDis = 1;
	int tObjNum;
	int count = 0;
	int loopcount = 0;

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpTargetObj->m_Index,1);
	gObjAttack(lpObj,lpTargetObj,lpMagic,0,1,0,0);

	int attackcheck;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;
	int delaytime;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0 && lpTargetObj->m_Index != tObjNum)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == lpTargetObj->m_Index || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					attackcheck = 0;

					if(loopcount == 0)
					{
						attackcheck = 1;
					}
					else if(rand()%3==0)
					{
						attackcheck = 1;
					}

					if(attackcheck != 0)
					{
						if(gObjCalDistance(lpTargetObj,&gObj[tObjNum]) < 3)
						{
							delaytime = (rand()*17)%300 + 500;

							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,delaytime,lpMagic->m_Skill,0);
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return TRUE;
}

//Season 4 Add-on
BOOL CMasterSkillSystem::TestSkill(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, int aTargetIndex, int dis, BOOL bVpAttack, int random, int delay) //0050D1E0 
{
	LPOBJ lpObj = &gObj[aIndex]; //loc2

	BOOL result = FALSE; //loc3
	int count = 0; //loc4
	int tObjNum; //loc5
	int FirstHit; //loc6
	int DuelIndex = lpObj->m_iDuelUser; //loc7
	int HitCount = 0; //loc8
	int bAttack = 0; //loc9

	if(x == 0 && y == 0)
	{
		x = lpObj->X;
		y = lpObj->Y;
	}

	int skilldis = MagicDamageC.GetSkillDistance(lpMagic->m_Skill); //loc10

	int EnableAttack; //loc11

	if(this->CalDistance(lpObj->X, lpObj->Y, x, y) <= skilldis)
	{
		while ( true )
		{
			if(lpObj->VpPlayer2[count].state != 0)
			{
				tObjNum = lpObj->VpPlayer2[count].number;

				if(tObjNum >= 0)
				{
					EnableAttack = 0;

					if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
					{
						EnableAttack = 1;
					}
					else if(tObjNum == aTargetIndex && bVpAttack == TRUE || DuelIndex == tObjNum)
					{
						EnableAttack = 1;
					}

					else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
					{
						EnableAttack = 1;
					}

					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;

						if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}

						if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
						{
							EnableAttack = 1;
						}
					}

					if(EnableAttack != 0)
					{
						FirstHit = 0;
						
						if(random == 0)
						{
							FirstHit = 1;
						}
						else if(rand()%random == 0)
						{
							FirstHit = 1;
						}

						if(FirstHit != 0)
						{
							if( gObj[tObjNum].X >= x-dis &&
								gObj[tObjNum].X <= x+dis &&
								gObj[tObjNum].Y >= y-dis &&
								gObj[tObjNum].Y <= y+dis)
							{
								gObj[tObjNum].lpAttackObj = lpObj;

								HitCount++;

								if(HitCount > 5)
								{
									if(rand()%2)
									{
										bAttack = 1;
									}
								}
								else
								{
									bAttack = 1;
								}

								if(HitCount > 8)
								{
									bAttack = 0;
								}

								if(bAttack == 1)
								{
									if(delay == 0)
									{
										gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 0, 0, 0);
									}
									else
									{
										gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,delay,lpMagic->m_Skill,0);
									}
								}

								result = TRUE;
							}
						}
					}
				}
			}

			count++;
			if(count > MAX_VIEWPORT -1)
			{
				break;
			}
		}
	}
	return result;
}

BOOL CMasterSkillSystem::SkillSuddenIce(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, int aTargetIndex) //0050D700
{
	LPOBJ lpObj = &gObj[aIndex]; //loc2
	int tObjNum; //loc3
	int count = 0; //loc4
	int FirstHit = 0; //loc5
	int HitCount = 0; //loc6
	int bAttack; //loc7
	LPOBJ lpTargetObj = &gObj[aTargetIndex]; //loc8
	int DuelIndex = lpObj->m_iDuelUser; //loc9
	int EnableAttack; //loc10

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}

				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
				{
					EnableAttack = 1;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					if(this->CalDistance(x,y,gObj[tObjNum].X,gObj[tObjNum].Y) < 4)
					{
						bAttack = 0;
						
						if(HitCount >= 5)
						{
							if(rand()%2)
							{
								bAttack = 1;
							}
						}
						else
						{
							bAttack = 1;
						}

						if(HitCount > 10)
						{
							bAttack = 0;
						}

						if(bAttack != 0)
						{
							int delay = rand()%500;
							gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return TRUE;
}

int CMasterSkillSystem::CalDistance(int x, int y, int x1, int y1) //0050DA90 
{
	if(x == x1 && y == y1)
	{
		return 0;
	}

	float tx = x - x1;
	float ty = y - y1;

	return sqrt(tx*tx + ty*ty);
}

int CMasterSkillSystem::GetAngle(int x, int y, int tx, int ty) //0050DB20 
{
	double diffX = x - tx;
	double diffY = y - ty;

	double rad = atan2(diffY,diffX);

	int angle = rad * 180 / 3.141592741012573 + 90;

	if(angle < 0)
	{
		angle += 360;
	}
	return angle;
}

void CMasterSkillSystem::SkillFrustrum(int aIndex, float fangle, float tx, float ty) //0050DBC0 
{
	LPOBJ lpObj = &gObj[aIndex];

	vec3_t p[4];

	Vector(-tx ,ty , 0.f, p[0]);
	Vector( tx ,ty , 0.f, p[1]);
	Vector( 1.0f, 0.f, 0.f, p[2]);
	Vector(-1.0f, 0.f, 0.f, p[3]);

	vec3_t Angle;

	Vector(0.f, 0.f, fangle, Angle);

	float Matrix[3][4];

	vec3_t vFrustrum[4];

	AngleMatrix(Angle,Matrix);

	for(int i = 0; i < 4; i++)
	{
		VectorRotate(p[i],Matrix,vFrustrum[i]);

		lpObj->fSkillFrustrumX[i] = (int)vFrustrum[i][0] + lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)vFrustrum[i][1] + lpObj->Y;
	}
}

BOOL CMasterSkillSystem::SkillKnightBlow(int aIndex, int aTargetIndex, CMagicInf * lpMagic, BOOL isCombo) //0050DD80 
{
	LPOBJ lpObj = &gObj[aIndex]; //loc2
	int StartDis = 1; //loc3
	int tObjNum; //loc4

	float fangle = this->GetAngle(lpObj->X,lpObj->Y,gObj[aTargetIndex].X,gObj[aTargetIndex].Y); //loc5
	
	float GetSkillDistance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill) + 1.0f; //loc6

	this->SkillFrustrum(aIndex,fangle,1.5f,GetSkillDistance);

	int count = 0; //loc7
	int loopcount = 0; //loc8

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	int attackcheck; //loc9
	int DuelIndex = lpObj->m_iDuelUser; //loc10
	int EnableAttack; //loc11

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					attackcheck = 0;

					if(loopcount == 0)
					{
						attackcheck = 1;
					}
					else if(rand()%3==0)
					{
						attackcheck = 1;
					}

					if(attackcheck != 0)
					{
						if(::SkillTestFrustrum(gObj[tObjNum].X,gObj[tObjNum].Y,aIndex))
						{
							gObjAttack(lpObj,&gObj[tObjNum],lpMagic,0,1,0,isCombo);
							if(isCombo != 0)
							{
								GCMagicAttackNumberSend(lpObj,AT_SKILL_COMBO,gObj[tObjNum].m_Index,1);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			loopcount++;
			count = 0;
			if(loopcount >= 3)
			{
				break;
			}
		}
	}
	return TRUE;
}

void CMasterSkillSystem::SkillBlowOfFury(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, int aTargetIndex, BOOL isCombo) //0050E190 
{
	LPOBJ lpObj = &gObj[aIndex]; //loc2
	int tObjNum; //loc3
	int count = 0; //loc4
	int DuelIndex = lpObj->m_iDuelUser; //loc5
	int HitCount = 0; //loc6
	int bAttack = 0; //loc7
	int EnableAttack; //loc8

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}

				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
				{
					EnableAttack = 1;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					int GetSkillDistance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);

					if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= GetSkillDistance)
					{
						bAttack = 0;

						if(HitCount > 15)
						{
							bAttack = 0;
						}

						if(HitCount >= 10)
						{
							if(rand()%2)
							{
								bAttack = 1;
							}
						}
						else
						{
							bAttack = 1;
						}

						if(bAttack != 0)
						{
							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,500,lpMagic->m_Skill,isCombo);

							if(isCombo != 0)
							{
								GCMagicAttackNumberSend(lpObj,AT_SKILL_COMBO,aIndex,1);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
}

void CMasterSkillSystem::SkillDefense(int aIndex, int aTargetIndex, CMagicInf *lpMagic) //0050E4F0 
{
	int skillSuccess = true; //loc2
	LPOBJ lpObj = &gObj[aIndex]; //loc3
	LPOBJ lpTargetObj = &gObj[aTargetIndex]; //loc4
	int skill_level = lpMagic->m_Level; //loc5

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_ELF)
	{
		return;
	}

	if(CC_MAP_RANGE(lpObj->MapNumber) && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	if(skill_level < 0 || skill_level > 7)
	{
		skill_level = 0;
	}

	float skill_defense; //loc6

	float SkillGet = MagicDamageC.SkillGet(lpMagic->m_Skill) / 10.0f; //loc7

	skill_defense = float(skill_level + 1)*2 + (lpObj->Energy + lpObj->AddEnergy) / SkillGet;

	if(lpTargetObj->Class == CLASS_KNIGHT)
	{
		skill_defense += skill_defense/10.0f;
	}

	if(gObjCheckBuffEffectValue(lpTargetObj, AT_INCREASE_DEFENSE, skill_defense, 0) == 1) //season3 add-on
	{
		GCMagicAttackNumberSend(lpObj,480,lpTargetObj->m_Index,0);
		return;
	}

	gObjApplyBuffEffectDuration(lpTargetObj, AT_INCREASE_DEFENSE, 3, skill_defense, 0, 0, 60); //season3 add-on

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
			if(lpMonster->m_iCurrentAI != 0)
			{
				lpMonster->m_Agro.IncAgro(lpObj->m_Index,int(skill_defense)/10);
			}
		}
	}

	GCMagicAttackNumberSend(lpObj,AT_SKILL_DEFENSE,lpTargetObj->m_Index,skillSuccess); //Wz Bug
}

void CMasterSkillSystem::SkillAttack(int aIndex, int aTargetIndex, CMagicInf *lpMagic) //0050E820 
{
	int skillSuccess = true; //loc2
	LPOBJ lpObj = &gObj[aIndex]; //loc3
	LPOBJ lpTargetObj = &gObj[aTargetIndex]; //loc4
	int skill_level = lpMagic->m_Level; //loc5

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_ELF)
	{
		return;
	}

	if(CC_MAP_RANGE(lpObj->MapNumber) && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	if(skill_level < 0 || skill_level > 7)
	{
		skill_level = 0;
	}

	float skill_attack; //loc6
	
	float SkillGet = MagicDamageC.SkillGet(lpMagic->m_Skill) / 10.0f; //loc7

	skill_attack = float((skill_level + 1) * 3.0f) + (lpObj->Energy + lpObj->AddEnergy) / SkillGet;

	if(lpTargetObj->Class == CLASS_KNIGHT)
	{
		skill_attack += skill_attack/10.0f;
	}

	if(gObjCheckBuffEffectValue(lpTargetObj, AT_INCREASE_ATTACK, skill_attack, 0) == 1) //season3 add-on
	{
		GCMagicAttackNumberSend(lpObj,485,lpTargetObj->m_Index,0);
		return;
	}

	gObjApplyBuffEffectDuration(lpTargetObj, AT_INCREASE_ATTACK, 2, skill_attack, 0, 0, 60); //season3 add-on

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
			if(lpMonster->m_iCurrentAI != 0)
			{
				lpMonster->m_Agro.IncAgro(lpObj->m_Index,int(skill_attack)/10);
			}
		}
	}

	GCMagicAttackNumberSend(lpObj,AT_SKILL_ATTACK,lpTargetObj->m_Index,skillSuccess); //Wz bug
}

void CMasterSkillSystem::MaGumSkillDefenseDown(int aIndex, int aTargetIndex) //0050EB50 
{
	int skillSuccess = 1; //loc2
	LPOBJ lpObj = &gObj[aIndex]; //loc3
	LPOBJ lpTargetObj = &gObj[aTargetIndex]; //loc4

	if(lpObj->Type == OBJ_USER && lpObj->Class != CLASS_MAGUMSA)
	{
		return;
	}

	LogAdd("마검사 몬스터방어력감소 : %d",10);

	if(retCalcSkillResistance(lpTargetObj,1) != FALSE)
	{
		gObjApplyBuffEffectDuration(lpTargetObj, AT_REDUCE_DEFENSE, 23, 10, 0, 0, 10); //Season3 add-on
		GCMagicAttackNumberSend(lpObj,500,lpTargetObj->m_Index,skillSuccess);
	}
}

void CMasterSkillSystem::SkillPowerSlash(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, int aTargetIndex) //0050EC40 
{
	LPOBJ lpObj = &gObj[aIndex]; //loc2
	int StartDis = 1; //loc3
	int GetSkillDistance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill); //loc4
	float fangle = (360 * dir) / 255; //loc5
	this->SkillFrustrum(aIndex,fangle,GetSkillDistance+1.0f,GetSkillDistance+1.0f);
	int tObjNum; //loc6
	int count = 0; //loc7
	int HitCount = 0; //loc8
	int bAttack; //loc9
	int DuelIndex = lpObj->m_iDuelUser; //loc10
	int EnableAttack; //loc11

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}

				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
				{
					EnableAttack = 1;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					if(SkillTestFrustrum(gObj[tObjNum].X,gObj[tObjNum].Y,aIndex) != FALSE)
					{
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= GetSkillDistance)
						{
							bAttack = 0;

							if(HitCount > 10)
							{
								bAttack = 0;
							}
							
							if(HitCount >= 5)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}

							if(bAttack != 0)
							{
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
							}

							HitCount++;
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
}

BOOL CMasterSkillSystem::SkillFireScream(int aIndex, int aTargetIndex, CMagicInf * lpMagic) //0050F040 
{
	if(g_SkillAdditionInfo.GetFireScreamSkill() == FALSE)
	{
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(gObjIsConnected(lpObj->m_Index) == FALSE && gObjIsConnected(lpTargetObj->m_Index) == FALSE)
	{
		return FALSE;
	}

	if(lpObj->Type != OBJ_USER)
	{
		return FALSE;
	}

	if(lpObj->Class != CLASS_DARKLORD)
	{
		return FALSE;
	}

	gObjAttack(lpObj,lpTargetObj,lpMagic,0,0,0,0);
	return TRUE;
}

BOOL CMasterSkillSystem::WeaponMagicDel(LPOBJ lpObj, BYTE aSkill, BYTE Level) //0050F140 
{
	int skill = 0;

	for(int i = 0; i < MAX_MAGIC; i++)
	{
		if(lpObj->Magic[i].IsMagic() == TRUE && this->CheckRequireStatus(lpObj->Magic[i].m_Skill) != FALSE)
		{
			skill = this->GetSkillID(lpObj->Magic[i].m_Skill);

			if(skill > 0 )
			{
				int loc4 = MagicDamageC.GetSkillReqBrandOfSkill(skill);

				if(aSkill == loc4)
				{
					switch(loc4)
					{
					case 24:
					case 56:
						GCMagicListOneDelSend(lpObj->m_Index, i , lpObj->Magic[i].m_Skill, lpObj->Magic[i].m_Level, 0, 0);
						return TRUE;
						break;
					}
				}
			}
		}
	}
	return FALSE;
}

BOOL CMasterSkillSystem::WeaponMagicAdd(LPOBJ lpObj, BYTE aSkill, BYTE Level) //0050F2B0  
{
	int skill = 0;

	for(int i = 0; i < MAX_MAGIC; i++)
	{
		if(lpObj->Magic[i].IsMagic() == TRUE && this->CheckRequireStatus(lpObj->Magic[i].m_Skill) != FALSE)
		{
			skill = this->GetSkillID(lpObj->Magic[i].m_Skill);

			if(skill > 0 && aSkill == MagicDamageC.GetSkillReqBrandOfSkill(skill))
			{
				GCMagicListOneSend(lpObj->m_Index, i , lpObj->Magic[i].m_Skill, lpObj->Magic[i].m_Level, 0, 0);
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CMasterSkillSystem::GetWeaponMagicSearch(LPOBJ lpObj, int pos) //0050F400   
{
	int skill = 0;

	if(this->CheckRequireStatus(lpObj->Magic[pos].m_Skill) != FALSE)
	{
		skill = this->GetSkillID(lpObj->Magic[pos].m_Skill);

		if(skill > 0 && MagicDamageC.GetSkillUseType(skill) == 4)
		{
			int loc3 = MagicDamageC.GetSkillReqBrandOfSkill(skill);

			switch(loc3)
			{
			case 24:
			case 56:
				{
					BOOL HasSkill = FALSE;

					if(lpObj->pInventory[1].IsItem() == TRUE)
					{
						if(lpObj->pInventory[1].m_Special[0] == loc3)
						{
							HasSkill = TRUE;
						}
					}

					if(lpObj->pInventory[0].IsItem() == TRUE)
					{
						if(lpObj->pInventory[0].m_Special[0] == loc3)
						{
							HasSkill = TRUE;
						}
					}

					if(HasSkill == FALSE)
					{
						return FALSE;
					}
				}
				break;
			}
		}
	}

	return TRUE;
}

////////////////////////// Summoner Skill Tree

int CMasterSkillSystem::SkillDrainLife(int aIndex, int aTargetIndex, CMagicInf* lpMagic)
{
	if(OBJMAX_RANGE(aTargetIndex) == false) //season4 add-on
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex]; //loc2 ebp-8
	int Result = 0; //loc3 ebp-c
	int tObjNum; //loc4 ebp-10

	int fangle = this->GetAngle(lpObj->X,lpObj->Y,gObj[aTargetIndex].X,gObj[aTargetIndex].Y); //loc5 -> ebp-14

	int count = 0; //loc6 ebp-18

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	int DuelIndex = lpObj->m_iDuelUser; //loc8 -> ebp-1c
	int EnableAttack; //loc9 -> ebp-20

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					if(tObjNum == aTargetIndex)
					{
						if(gCheckSkillDistance(aIndex, aTargetIndex, lpMagic->m_Skill) != FALSE)
						{
							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,700,lpMagic->m_Skill,0);
							Result = 1;
							break;
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return Result;
}

struct PMSG_CHAIN_LIGHTNING_MAGIC_SEND
{
	struct PBMSG_HEAD2 h;
	BYTE MagicNumberH;
	BYTE MagicNumberL;
	short aIndex;
	BYTE MaxHitUser;
};

int CMasterSkillSystem::SkillChainLightning(int aIndex, int aTargetIndex, CMagicInf* lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false) //season4 add-on
	{
		return 0;
	}

	if(OBJMAX_RANGE(aTargetIndex) == false) //season4 add-on
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex]; //loc2 ebp-8
	LPOBJ lpTargetObj = &gObj[aTargetIndex]; //loc3 ebp-c

	int local4, tObjNum, count, local7, local8, local9[3], CallMonIndex, local13;

    local4 = 1, count = 0, local9[2] = -1, local8 = -1, local7 = -1;

    BYTE buffer[256];

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	local9[1] = lpObj->m_iDuelUser;

	if(gCheckSkillDistance(aIndex, aTargetIndex, lpMagic->m_Skill) == FALSE)
	{
		return FALSE;
	}

	local9[2] = aTargetIndex;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				local9[0] = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0 && 
					(gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110)) //Season 4 add-on
				{
					local9[0] = 1;
				}
				else if(tObjNum == aTargetIndex || local9[1] == tObjNum)
				{
					local9[0] = 1;
				}
				else
				{
					CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						local9[0] = 1;
					}
				}

				if(local9[0] != 0) //More than 1 monster
				{
					if (gObj[tObjNum].X >= lpTargetObj->X-1 && gObj[tObjNum].X <= lpTargetObj->X+1 && 
						gObj[tObjNum].Y >= lpTargetObj->Y-1 && gObj[tObjNum].Y <= lpTargetObj->Y+1 && 
						gObj[tObjNum].Type == OBJ_MONSTER)
                    {
                        local8 = tObjNum;

                        if(local7 != -1)
						{
                            break;
						}
                    }
					else if(gObj[tObjNum].X >= lpTargetObj->X-2 && gObj[tObjNum].X <= lpTargetObj->X+2 && 
							gObj[tObjNum].Y >= lpTargetObj->Y-2 && gObj[tObjNum].Y <= lpTargetObj->Y+2 && 
							gObj[tObjNum].Type == OBJ_MONSTER)
                    {
                        local7 = tObjNum;

                        if(local8 != -1)
						{
                            break;
						}
                    }
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}

	if(local7 == -1)
	{
		local7 = local8;
	}

	if(local8 == -1)
	{
		local8 = local9[2];
		local7 = local8;
	}

	local13 = 0;

	PMSG_CHAIN_LIGHTNING_MAGIC_SEND pMsg;

	pMsg.MagicNumberH = SET_NUMBERH(lpMagic->m_Skill);
	pMsg.MagicNumberL = SET_NUMBERL(lpMagic->m_Skill);

	pMsg.aIndex = aIndex;

	pMsg.MaxHitUser = 3;

	local13 += sizeof(pMsg);

	int i;

	for(i = 0; i< 3; i++)
	{
		WORD wUnk = local9[i+2];
        memcpy(&buffer[local13], &wUnk, sizeof(wUnk));
		local13 += 2;
	}

	PHeadSubSetB((LPBYTE)&pMsg,0xBF,0x0A, local13);

	memcpy(buffer, &pMsg, sizeof(pMsg));

    if(lpObj->Type == OBJ_USER)
    {
        ::DataSend(lpObj->m_Index, buffer, pMsg.h.size);
    }

    MsgSendV2(lpObj, buffer, pMsg.h.size);

    for(i = 0;i < 3;i++)
    {
        gObjAddAttackProcMsgSendDelay(lpObj, 50, local9[i+2], i*200+200, lpMagic->m_Skill, i+1);
    }

	return local4;
}


int CMasterSkillSystem::SkillSleep(int aIndex,int aTargetIndex,CMagicInf * lpMagic) //004C9100
{
	LPOBJ lpObj = &gObj[aIndex]; //loc2

	int result = 0; //loc3 ebpC

	int tObjNum; //loc4 -> ebp-10

	int angle = this->GetAngle(lpObj->X,lpObj->Y,gObj[aTargetIndex].X,gObj[aTargetIndex].Y); //loc5 ebp14

	int count = 0; //loc6 ebp18

	int DuelIndex; //loc7 ebp1c

	BOOL bEnableAttack; //loc8 ebp20

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	DuelIndex = lpObj->m_iDuelUser;

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				bEnableAttack = FALSE; //ebp20

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					bEnableAttack = TRUE;
				}
				else if(tObjNum == aTargetIndex && DuelIndex == tObjNum)
				{
					bEnableAttack = TRUE;
				}
				else if (CC_MAP_RANGE(lpObj->MapNumber) != FALSE || IT_MAP_RANGE(lpObj->MapNumber) != FALSE)
				{
					bEnableAttack = TRUE;
				}

				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && 
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && 
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
				{
					bEnableAttack = TRUE;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==TRUE)
					{
						bEnableAttack = TRUE;
					}
				}

				if(bEnableAttack != FALSE)
				{
					if(tObjNum == aTargetIndex)
					{
						if(gCheckSkillDistance(aIndex, aTargetIndex, lpMagic->m_Skill) != FALSE)
						{
							/*int Time = 0;
							if(gObj[aTargetIndex].Type == OBJ_MONSTER){
								Time = ((lpObj->Energy / 100) + 5) - (gObj[aTargetIndex].Level / 20);
							} else if(gObj[aTargetIndex].Type == OBJ_USER){
								Time = ((lpObj->Energy / 250) + ((lpObj->Level - gObj[aTargetIndex].Level) / 100) + 4);
							}

							gObjApplyBuffEffectDuration(&gObj[aTargetIndex], 72, 25, Time, 0, 0, Time);*/

							gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,1,0,0,0);
							result = 1;
							break;
						}
					}
				}
			}
		}

		count++;
			
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}

	return result;
}

int CMasterSkillSystem::SkillLightningShock(int aIndex, int aTargetIndex, CMagicInf* lpMagic) //004F7090
{
	LPOBJ lpObj = &gObj[aIndex]; //loc2
	int loc3 = 0; //loc3
	BOOL bAttack = FALSE; //loc4
	int tObjNum; //loc5
	int count = 0; //loc6
	int loc7 = 0; //loc7
	int loc8 = 7; //loc8
	int maxhit = 0; //loc9

	BOOL bEnableAttack; //loc10

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	int loc11 = 1;

	int DuelIndex = lpObj->m_iDuelUser; //loc12

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				bEnableAttack = FALSE;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					bEnableAttack = TRUE;
				}
				else if (CC_MAP_RANGE(lpObj->MapNumber) != FALSE || IT_MAP_RANGE(lpObj->MapNumber) != FALSE)
				{
					bEnableAttack = TRUE;
				}
				else if(tObjNum == aTargetIndex && loc11 == 1 || DuelIndex == tObjNum )
				{
					bEnableAttack = TRUE;
				}

				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && 
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && 
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
				{
					bEnableAttack = 1;
				}

				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==TRUE)
					{
						bEnableAttack = TRUE;
					}
				}

				if(bEnableAttack != FALSE)
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 7)
					{
						maxhit++;

						if(maxhit > 5)
						{
							if(rand()%2 != 0)
							{
								bAttack = TRUE;
							}
						}
						else
						{
							bAttack = TRUE;
						}

						if(maxhit > 12)
						{
							bAttack = FALSE;
						}

						if(bAttack == TRUE)
						{
							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,250,lpMagic->m_Skill,0);
						}						
					}
				}
			}
		}

		count++;
			
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}

	return true;
}
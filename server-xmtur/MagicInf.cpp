//	GS-N	1.00.77	JPN		-	Completed
//	GS-N	1.00.90	JPN		-	Completed
#include "Stdafx.h"
#include "MagicInf.h"
#include "LogProc.h"
#include "GameMain.h"

#define MAX_MAGICINF		255

CMagicInf	DefMagicInf[MAX_MAGICINF];

CMagicInf::CMagicInf()
{
	this->Clear();
}

CMagicInf::~CMagicInf()
{
	this->m_DamageMin  = 0;
	this->m_DamageMax  = 0;
}

void CMagicInf::Clear()
{
	this->m_Skill  = -1;
	this->m_Level  = 0;
}

BOOL CMagicInf::IsMagic(){
	if(this->m_Skill == -1){
		return FALSE;
	}
	return TRUE;
}


int GetSkillNumberInex(int type, int Index, int level) //004BABE0
{
	int skillnumber = -1;

	if ( type == 15 )
	{
		if ( Index >= 0 && Index < 16 )
		{
			skillnumber = Index + 1;
		}
		else if ( Index == 16 )
		{
			skillnumber = 38;
		}
		else if ( Index == 17 )
		{
			skillnumber = 39;
		}
		else if ( Index == 18 )
		{
			skillnumber = 40;
		}
		else if ( Index == 19 )
		{
			skillnumber = 215;
		}
		else if ( Index == 20 )
		{
			skillnumber = 214;
		}
		else if ( Index == 21 )
		{
			skillnumber = 230; //lightning shock
		}
		else if ( Index == 22 )
		{
			skillnumber = 217;
		}
		else if ( Index == 23 )
		{
			skillnumber = 218; //berserker
		}
		else if ( Index == 24 )
		{
			skillnumber = 219;
		}
		else if ( Index == 25 )
		{
			skillnumber = 220;
		}
		else if ( Index == 26 )
		{
			skillnumber = 221;
		}
		else if ( Index == 27 )
		{
			skillnumber = 222;
		}
		//s4
		else if ( Index == 28 )
		{
			skillnumber = 233;
		}
		else if ( Index == 29 ){
			skillnumber = 237;
		} else if ( Index == 30 ){ //Rage Fighter Skills
			skillnumber = AT_SKILL_CHAIN_DRIVE;
		} else if ( Index == 31 ){
			skillnumber = AT_SKILL_DARKSIDE;
		} else if ( Index == 32 ){
			skillnumber = AT_SKILL_DRAGON_LORE;
		} else if ( Index == 33 ){
			skillnumber = AT_SKILL_DRAGON_SLAYER;
		} else if ( Index == 34 ){
			skillnumber = AT_SKILL_REDARMOR_IGNORE;
		} else if ( Index == 35 ){
			skillnumber = AT_SKILL_FITNESS;
		} else if ( Index == 36 ){
			skillnumber = AT_SKILL_DEF_SUCC_RATE;
		}
	}
	else if ( type == 12 )
	{
		switch ( Index )
		{
			case 7:		skillnumber = 41;	break;
			case 8:		skillnumber = 26;	break;
			case 9:		skillnumber = 27;	break;
			case 10:	skillnumber = 28;	break;
			case 12:	skillnumber = 42;	break;
			case 13:	skillnumber = 47;	break;
			case 14:	skillnumber = 48;	break;
			case 19:	skillnumber = 43;	break;
			case 17:	skillnumber = 52;	break;
			case 18:	skillnumber = 51;	break;
			case 16:	skillnumber = 55;	break;
			case 21:	skillnumber = 61;	break;
			case 22:	skillnumber = 63;	break;
			case 23:	skillnumber = 64;	break;
			case 24:	skillnumber = 65;	break;
			case 11:	skillnumber = level + 30;	break;
			case 35:	skillnumber = 78;	break;
			//
			case 44: skillnumber = 232; break;
			case 45: skillnumber = 235; break;
			case 46: skillnumber = 234; break;
			case 47: skillnumber = 236; break;
			case 48: skillnumber = 238; break;
		}
	}
		
	return skillnumber;
}

int CMagicInf::Set(WORD aType, BYTE aIndex, BYTE aLevel)
{
	if ( this->m_Skill != -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 203)), __FILE__, __LINE__);
		return -1;
	}

	this->m_Level = aLevel;
	this->m_Skill = GetSkillNumberInex(aType, aIndex, aLevel);
	
	if ( this->m_Skill < 0 )
	{
		this->m_Skill = 1;
	}

	int damage = MagicDamageC.SkillGet(this->m_Skill);
	this->m_DamageMin = damage;
	this->m_DamageMax = damage + damage/2;

	return this->m_Skill;
}

int CMagicInf::GetDamage()
{
	int damage = 0;

	if ( this->m_Skill == -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), __FILE__, __LINE__);
		return 0;
	}

	int subd = this->m_DamageMax - this->m_DamageMin;

	__try
	{
		damage = this->m_DamageMin + (rand() % 1);
	}
	__except(subd=1,1)
	{
		LogAdd(lMsg.Get(MSGGET(0, 111)), __FILE__, __LINE__);
	}

	return damage;
}

//Identical
int CMagicInf::Set(int aSkill, BYTE aLevel)
{
	if ( this->m_Skill != -1 )
	{
		return -1;
	}

	this->m_Skill = aSkill;
	this->m_Level = aLevel;
	int damage = MagicDamageC.SkillGet(this->m_Skill);
	this->m_DamageMin = damage;
	this->m_DamageMax = damage + damage/2;

	return this->m_Skill;
}

//Identical
int CMagicInf::UpdateML(DWORD aSkill, BYTE aLevel) //00490E60
{
	if ( this->m_Skill != -1 )
	{
		this->m_Skill = aSkill;
		this->m_Level = aLevel;

		int damage = MagicDamageC.SkillGet(this->m_Skill);

		this->m_DamageMin = damage;
		this->m_DamageMax = damage + damage/2;

		return this->m_Skill;
	}
	else
	{
		LogAdd("[MasterSkill] [ERROR!!] Fail - Update Master Skill:%d, Level:%d", aSkill, aLevel);
	}

	return -1;
}



//Identical
void MagicByteConvert(LPBYTE buf, CMagicInf* const Magici, int maxmagic) //00490F10
{
	int X=0; //loc1

	int SkillLevel;
	int Skill1;
	int Skill2;

	for (int index = 0;index < maxmagic;index++)
	{
		Skill1 = Magici[index].m_Skill;
		SkillLevel = 0;
		Skill2 = 0;

		if((int)Magici[index].m_Skill > 765){
			Skill1 = 0xFF;
			SkillLevel = 3;
			Skill2 = Magici[index].m_Skill - (0xFF * 3);
		} else if((int)Magici[index].m_Skill > 510){
			Skill1 = 0xFF;
			SkillLevel = 2;
			Skill2 = Magici[index].m_Skill - (0xFF << 1);
		} else if((int)Magici[index].m_Skill > 255){
			Skill1 = 0xFF;
			SkillLevel = 1;
			Skill2 = Magici[index].m_Skill - 0xFF;
		}

		buf[X] = Skill1; //Skill
		X++;
		buf[X] = Magici[index].m_Level << 3;
		buf[X] |= SkillLevel & 0x07; //Season3 add-on
		X++;
		buf[X] = Skill2;
		X++;
	}
}
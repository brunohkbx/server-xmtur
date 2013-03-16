//GameServer 1.00.77 JPN - Completed
#include "Stdafx.h"
#include "PartyClass.h"
#include "LogProc.h"
#include "Protocol.h"
#include "Winutil.h"

PartyClass::PartyClass()
{

}

PartyClass::~PartyClass()
{

}

BOOL PartyClass::IsParty(int party_number)
{
	if ( party_number < 0 || party_number > OBJMAX )
	{
		return FALSE;
	}

	if ( this->m_PartyS[party_number].Count == 0 )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL PartyClass::Isleader(int party_number, int usernumber, int dbnumber)
{
	if ( this->IsParty(party_number) == FALSE )
	{
		return FALSE;
	}

	if ( this->m_PartyS[party_number].Number[0] == usernumber && this->m_PartyS[party_number].DbNumber[0] == dbnumber)
	{
		return TRUE;
	}

	return FALSE;
}


int PartyClass::GetCount(int party_number)
{
	if ( this->IsParty(party_number) == FALSE )
	{
		return 0;
	}

	return this->m_PartyS[party_number].Count;
}


BOOL PartyClass::GetIndexUser(int party_number, int index, int& usernumber,  int& dbnumber)
{
	if ( this->IsParty(party_number) == FALSE )
	{
		return FALSE;
	}

	if ( index <0 || index > MAX_USER_IN_PARTY-1 )
	{
		return FALSE;
	}

	usernumber = this->m_PartyS[party_number].Number[index];
	dbnumber = this->m_PartyS[party_number].DbNumber[index];

	return TRUE;
}

int PartyClass::GetIndex(int party_number, int usernumber, int dbnumber)
{
	if ( this->IsParty(party_number) == FALSE )
	{
		return 0;
	}

	for ( int n=0; n<MAX_USER_IN_PARTY;n++)
	{
		if ( this->m_PartyS[party_number].Number[n] == usernumber || this->m_PartyS[party_number].DbNumber[n] == dbnumber )
		{
			return n;
		}
	}

	return -1;
}

int PartyClass::Create(int usernumber, int dbnumber, int level)
{
	int n = this->m_PartyCount;
	int loopcount = 0;

	while ( true )
	{
		if ( this->m_PartyS[n].Count == 0 )
		{
			this->m_PartyS[n].Count = 1;
			this->m_PartyS[n].Number[0] = usernumber;
			this->m_PartyS[n].DbNumber[0] = dbnumber;

			for ( int i=1;i<MAX_USER_IN_PARTY;i++)
			{
				this->m_PartyS[n].DbNumber[i] = -1;
				this->m_PartyS[n].Number[i] = -1;
			}

			this->m_PartyCount++;
			this->m_PartyS[n].m_MaxLevel = 0;
			this->m_PartyS[n].m_MinLevel = 0;
			this->SetLevel(n, level);

			if ( this->m_PartyCount > OBJMAX-1 )
			{
				this->m_PartyCount=0;
			}

			this->m_PartyS[n].m_PkLevel[0] = gObj[usernumber].m_PK_Level; //Season 2.5 add-on (PK Party)

			if( this->m_PartyS[n].m_PkLevel[0] >= 5 ) //Season 2.5 add-on
			{
				this->m_PartyS[n].m_PartyPkLevel = this->m_PartyS[n].m_PkLevel[0];
				this->m_PartyS[n].m_PkCount = 1;
			}
			else
			{
				this->m_PartyS[n].m_PartyPkLevel = 3;
				this->m_PartyS[n].m_PkCount = 0;
			}

			return n;
		}

		n++;

		if ( n> OBJMAX-1 )
		{
			n=0;
		}

		loopcount++;

		if ( loopcount > OBJMAX-1 )
		{
			break;
		}
	}

	return -1;
}

BOOL PartyClass::SetLevel(int party_number, int level)
{
	if ( this->m_PartyS[party_number].m_MaxLevel  < level )
	{
		this->m_PartyS[party_number].m_MaxLevel  = level;
	}

	if ( this->m_PartyS[party_number].m_MinLevel  == 0 )
	{
		this->m_PartyS[party_number].m_MinLevel = level;
	}

	if ( this->m_PartyS[party_number].m_MinLevel > level )
	{
		this->m_PartyS[party_number].m_MinLevel = level;
	}

	return TRUE;
}


BOOL PartyClass::GetLevel(int party_number, int& maxlevel, int& minlevel)
{
	if ( this->IsParty(party_number) == FALSE )
	{
		return FALSE;
	}

	maxlevel = this->m_PartyS[party_number].m_MaxLevel;
	minlevel = this->m_PartyS[party_number].m_MinLevel;

	return TRUE;
}


BOOL PartyClass::Destroy(int party_number)
{
	if ( this->IsParty(party_number) == FALSE )
	{
		return FALSE;
	}

	this->m_PartyS[party_number].Count = 0;
	this->m_PartyS[party_number].Number[0] = -1;
	this->m_PartyS[party_number].DbNumber[0] = -1;
	this->m_PartyCount--;

	return FALSE;
}


int PartyClass::Add(int party_number, int usernumber, int dbnumber, int level)
{
	if ( this->IsParty(party_number ) == FALSE )
	{
		return -1;
	}

	if ( this->m_PartyS[party_number].Count >= OBJMAX )
	{
		return -1;
	}

	for ( int i = 0; i<MAX_USER_IN_PARTY ; i++)
	{
		if ( (this->m_PartyS[party_number].Number[i] >= 0) && (this->m_PartyS[party_number].Number[i] == usernumber) && (this->m_PartyS[party_number].DbNumber[i] == dbnumber) )
		{
			return -2;
		}
	}

	for ( int i = 0; i<MAX_USER_IN_PARTY ; i++ )
	{
		if ( this->m_PartyS[party_number].Number[i] == -1 )
		{
			this->m_PartyS[party_number].Number[i] = usernumber;
			this->m_PartyS[party_number].DbNumber[i] = dbnumber;
			this->m_PartyS[party_number].Count++;
			this->SetLevel(party_number, level);
			this->m_PartyS[party_number].m_PkLevel[i] = gObj[usernumber].m_PK_Level; //Season 2.5 add-on
			
			if( this->m_PartyS[party_number].m_PkLevel[i] >= 5 ) //Season 2.5 add-on
			{
				if( this->m_PartyS[party_number].m_PartyPkLevel < this->m_PartyS[party_number].m_PkLevel[i] )
				{
					this->m_PartyS[party_number].m_PartyPkLevel = this->m_PartyS[party_number].m_PkLevel[i];
				}
					
				this->m_PartyS[party_number].m_PkCount += 1;
			}

			return i;
		}
	}

	return -1;
}

int PartyClass::Delete(int party_number, int usernumber, int dbnumber)
{
	if ( this->IsParty(party_number) == FALSE )
	{
		return -1;
	}

	for ( int i = 0 ; i<MAX_USER_IN_PARTY ; i++ )
	{
		if ( (this->m_PartyS[party_number].Number[i] == usernumber) && (this->m_PartyS[party_number].DbNumber[i] == dbnumber) )
		{
			this->m_PartyS[party_number].Number[i] = -1;
			this->m_PartyS[party_number].DbNumber[i] = -1;
			this->m_PartyS[party_number].Count--;
			this->m_PartyS[party_number].m_PkLevel[i] = 3; //Season 2.5 add-on
			return i;
		}
	}

	return -1;
}

void PartyClass::Delete(int party_number, int index)
{
	if ( this->IsParty(party_number) == FALSE )
	{
		return;
	}

	if ( index < 0 || index > MAX_USER_IN_PARTY -1 )
	{
		return;
	}

	if ( this->m_PartyS[party_number].Number[index] >= 0 )
	{
		this->m_PartyS[party_number].Number[index] = -1;
		this->m_PartyS[party_number].DbNumber[index] = -1;
		this->m_PartyS[party_number].Count--;
		this->m_PartyS[party_number].m_PkLevel[index] = 3; //Season 2.5 add-on
	}
}


int PartyClass::GetPartyCount(int party_number)
{
	if ( this->IsParty( party_number ) == FALSE )
	{
		return -1;
	}

	return this->m_PartyS[party_number].Count;
}


void PartyClass::Paint(int party_number)
{
	if (this->IsParty(party_number) == FALSE )
	{
		return;
	}

	int usern = 0;
	wsprintf( this->m_szTempPaint, "PartyMember(%d) :", this->m_PartyS[party_number].Count);

	for ( int i = 0; i< MAX_USER_IN_PARTY ; i++ )
	{
		if ( (this->m_PartyS[party_number].Number[i] != 0 ) && (this->m_PartyS[party_number].DbNumber[i] != 0 ) )
		{
			usern = this->m_PartyS[party_number].Number[i];

			if ( !gObjIsConnected(usern) )
				continue;

			strcat(this->m_szTempPaint, "[");
			strcat(this->m_szTempPaint, gObj[usern].AccountID);
			strcat(this->m_szTempPaint, "][");
			strcat(this->m_szTempPaint, gObj[usern].Name);
			strcat(this->m_szTempPaint, "], ");
		}
	}

	LogAddTD(this->m_szTempPaint);
}

struct PMSG_DEFAULT_COUNT
{
	PBMSG_HEAD h;
	BYTE Count;	// 3
};

struct PMSG_PARTYLIFEALL
{
	BYTE Number;	// 0
};

void PartyClass::PartyMemberLifeSend(int party_number)
{
	if ( this->IsParty(party_number) == FALSE )
	{
		return;
	}

	PMSG_DEFAULT_COUNT pCount;
	PMSG_PARTYLIFEALL pList;
	int usern;
	char sendbuf[512];
	int lOfs = 0;
	int tlife = 0;

	PHeadSetB((LPBYTE)&pCount, 0x44, sizeof(pCount));
	pCount.Count = this->m_PartyS[party_number].Count;
	lOfs += sizeof(pCount);
	
	for ( int i = 0; i<MAX_USER_IN_PARTY ; i++ )
	{
		if ( (this->m_PartyS[party_number].Number[i] >= 0 ) && (this->m_PartyS[party_number].DbNumber[i] >= 0) )
		{
			usern = this->m_PartyS[party_number].Number[i];

			if ( gObjIsConnected(usern) != PLAYER_EMPTY )
			{
				pList.Number = (i << 4) & 0xF0 ;
				tlife = ( (BYTE)(  gObj[usern].Life / ((gObj[usern].MaxLife + gObj[usern].AddLife )/10.0f)     )  ) & 0x0F;
				pList.Number |= tlife;
				memcpy(sendbuf+lOfs, (char*)&pList, 1);
				lOfs++;
			}
			else
			{
				this->m_PartyS[party_number].Number[i] = -1;
				this->m_PartyS[party_number].DbNumber[i] = -1;
				this->m_PartyS[party_number].Count--;
			}
		}
	}

	pCount.h.size = lOfs;
	memcpy(sendbuf, (char*)&pCount, sizeof(pCount));

	for ( int i=0 ; i<MAX_USER_IN_PARTY ; i++ )
	{
		usern = this->m_PartyS[party_number].Number[i];

		if ( usern >= 0 )
		{
			DataSend(usern, (LPBYTE)sendbuf, lOfs);
		}
	}
}

char PartyClass::GetPkLevel(int party_number) //Season 2.5 add-on Identical
{
	return this->m_PartyS[party_number].m_PartyPkLevel;
}

void PartyClass::SetPkLevel(int party_number, int usernumber, int dbnumber, char pklevel) //Season 2.5 add-on Identical
{
	if ( this->IsParty(party_number) == FALSE )
	{
		return;
	}
	
	for ( int i = 0; i<MAX_USER_IN_PARTY ; i++ )
	{
		if ( (this->m_PartyS[party_number].Number[i] == usernumber ) && (this->m_PartyS[party_number].DbNumber[i] == dbnumber) )
		{
			this->m_PartyS[party_number].m_PkLevel[i] = pklevel;
			break;
		}
	}
}

void PartyClass::SetPkCount(int party_number) //Season 2.5 add-on Identical
{
	int PKUserCount = 0;//lc2
	int usern = 0;//lc3

	for ( int i = 0; i<MAX_USER_IN_PARTY ; i++ )
	{
		usern = this->m_PartyS[party_number].Number[i];

		if( usern >= 0 && this->m_PartyS[party_number].m_PkLevel[i] >= 5)
		{
			if( this->m_PartyS[party_number].m_PartyPkLevel  < this->m_PartyS[party_number].m_PkLevel[i] )
			{
				this->m_PartyS[party_number].m_PartyPkLevel = this->m_PartyS[party_number].m_PkLevel[i];
			}
			PKUserCount++;
		}
	}
	
	this->m_PartyS[party_number].m_PkCount = PKUserCount;

	if( PKUserCount == 0 )
	{
		this->m_PartyS[party_number].m_PartyPkLevel = 3;
	}
}

void PartyClass::ResetPkLevel(int party_number) //Season 2.5 add-on Identical
{
	int usern = 0;

	for ( int i = 0; i<MAX_USER_IN_PARTY ; i++ )
	{
		usern = this->m_PartyS[party_number].Number[i];

		if( usern >= 0 )
		{
			this->m_PartyS[party_number].Number[0] = this->m_PartyS[party_number].Number[i];
			this->m_PartyS[party_number].Number[i] = -1;
			this->m_PartyS[party_number].DbNumber[0] = this->m_PartyS[party_number].DbNumber[i];
			this->m_PartyS[party_number].DbNumber[i] = -1;
			this->m_PartyS[party_number].m_PkLevel[0] = this->m_PartyS[party_number].m_PkLevel[i];
			this->m_PartyS[party_number].m_PkLevel[i] = 3;
			break;
		}
	}
}
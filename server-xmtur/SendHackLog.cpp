//GameServer 1.00.77 JPN - Completed
#include "Stdafx.h"
#include "SendHackLog.h"
#include "Winutil.h"
#include "GameMain.h"
#include "User.h"

CSendHackLog gSendHackLog;


CSendHackLog::CSendHackLog()
{
	return;
}


CSendHackLog::~CSendHackLog()
{
	return;
}


void CSendHackLog::SendSet(char * ip, int port)
{
	this->m_UdpSocCHackLog.CreateSocket();
	this->m_UdpSocCHackLog.SendSet(ip, port);
}

struct PMSG_HACK_LOG
{
	PBMSG_HEAD h;	// C1:01
	WORD wServer;	// 4
	char AccountID[11];	// 6
	char Name[11];	// 11
	BYTE btType;	// 1C
	char szPacketContext[50];	// 1D
};

void CSendHackLog::Send(int aIndex, BYTE type, LPSTR Context)
{
	PMSG_HACK_LOG Msg;
	
	PHeadSetB((LPBYTE)&Msg, 0x01, sizeof(Msg) );
	Msg.wServer = Configs.GameServerCode;
	memcpy(Msg.AccountID, gObj[aIndex].AccountID , MAX_ACCOUNT_LEN );
	memcpy(Msg.Name, gObj[aIndex].Name, MAX_ACCOUNT_LEN );
	Msg.AccountID[MAX_ACCOUNT_LEN] = 0;
	Msg.Name[MAX_ACCOUNT_LEN] = 0;
	Msg.btType = type;
	memcpy(Msg.szPacketContext, Context, sizeof(Msg.szPacketContext) );

	this->m_UdpSocCHackLog.SendData((LPBYTE)&Msg, sizeof(Msg) );
}


BOOL CSendHackLog::SendData(LPBYTE SendData, DWORD nSendDataLen)
{
	return this->m_UdpSocCHackLog.SendData(SendData, nSendDataLen);
}
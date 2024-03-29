//GameServer 1.00.77 JPN - Completed
#include "Stdafx.h"
#include "MyWinsockBase.h"
#include "wsJoinServerCli.h"
#include "GameServer.h"
#include "GameMain.h"
#include "DBSockMng.h"
#include "DSProtocol.h"
#include "LogProc.h"

DBSockMng cDBSMng;

DBSockMng::DBSockMng()
{
	this->m_ActiveServer=DS_UNINITIALIZED;
}

DBSockMng::~DBSockMng()
{

}

void DBSockMng::Connect()
{
	if (this->wsCDBS[0].GetSocket()  == INVALID_SOCKET )
	{
		int State;
		this->wsCDBS[0].CreateSocket(ghWnd);
		this->wsCDBS[0].SetProtocolCore(DataServerProtocolCore);
		State=this->wsCDBS[0].Connect(Configs.DataServer1_IP, Configs.DataServer1_Port, WM_GM_DATA1_CLIENT_MSG_PROC);
		if (State == 0)
		{
			LogAdd(lMsg.Get(MSGGET(1, 147)), Configs.DataServer1_IP, Configs.DataServer1_Port);
			this->wsCDBS[0].Close();
		}
		else
		{
			LogAdd(lMsg.Get(MSGGET(1, 146)), Configs.DataServer1_IP, Configs.DataServer1_Port);	
			this->m_ActiveServer=DS_0;
			DataServerLogin(DS_0);
		}
	}
	
	if (this->wsCDBS[1].GetSocket() == INVALID_SOCKET)	
	{
		int State;
		this->wsCDBS[1].CreateSocket(ghWnd);
		this->wsCDBS[1].SetProtocolCore(DataServerProtocolCore);
		State=this->wsCDBS[1].Connect(Configs.DataServer2_IP, Configs.DataServer2_Port, WM_GM_DATA2_CLIENT_MSG_PROC);
		if (State==0)
		{
			LogAdd(lMsg.Get(MSGGET(1, 147)), Configs.DataServer2_IP, Configs.DataServer2_Port);
			this->wsCDBS[1].Close();
		}
		else
		{
			LogAdd(lMsg.Get(MSGGET(1, 146)), Configs.DataServer2_IP, Configs.DataServer2_Port);
			if (this->m_ActiveServer==-1)
			{
				this->m_ActiveServer=DS_1;
			}
			DataServerLogin(DS_1);
		}
	}
	
	if ( cExDB.GetSocket()  == INVALID_SOCKET)
	{
		LogAddC(2, "ExDB Server Check..");
		cExDB.CreateSocket(ghWnd);
		ExDataServerConnect(Configs.ExDB_IP, WM_GM_EXDATA_CLIENT_MSG_PROC);
	}
	else
	{
		if ( cExDB.GetConnect() == 0 )
		{
			LogAddC(2, "ExDB Server Check..");
			ExDataServerConnect(Configs.ExDB_IP, WM_GM_EXDATA_CLIENT_MSG_PROC);
		}
	}
}

void DBSockMng::ActiveChange()
{
	{
		int Status;
		if (this->m_ActiveServer==DS_0)
		{
			this->m_ActiveServer=DS_1;
			Status=1;
		}
		else
		{
			this->m_ActiveServer=DS_0;
			Status=0;
		}
		LogAdd("Active DB Change");
	}
}

int DBSockMng::Send(char* buf, int len)
{
	if(this->m_ActiveServer >= 2){
		return 0;
	}
	if(this->m_ActiveServer < 0){
		return 0;
	}

	if(this->wsCDBS[this->m_ActiveServer].DataSend((char*)buf, len) == 0){
		this->ActiveChange();
		this->wsCDBS[this->m_ActiveServer].DataSend((char*)buf, len);
	}
	return 1;
}

int DBSockMng::Send(int server, char* buf, int len)
{
	if (server >= 2)
	{
		return 0;
	}
	if (server < 0)
	{
		return 0;
	}
	if (this->wsCDBS[server].DataSend(buf, len) == 0)
	{
		this->ActiveChange();
	}
	return 1;
}

int DBSockMng::MsgProc(int line, WPARAM wParam, LPARAM lParam)
{
	switch (lParam & 0xFFFF & 0xFFFF)
	{
	case 1: // Recv Data
		this->wsCDBS[line].DataRecv();
		break;
	case 2: // Send Data
		this->wsCDBS[line].FDWRITE_MsgDataSend();
		break;
	case 32: // Close
		this->wsCDBS[line].Close();
	}
	return 1;
}

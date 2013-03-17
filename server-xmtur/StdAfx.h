#pragma warning (disable:4786)

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include <afxwin.h>
#include <afxext.h>
#include <afxdisp.h>
#include <windows.h>
#include <winbase.h>
#include <winsock2.h>
#include "COMMCTRL.h"
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <math.h>
#include <process.h>
#include <map>
#include <string>
#include <ios>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <afx.h>
#include "Msg.h"

#define WIN32_LEAN_AND_MEAN	
#define _WIN32_WINNT 0x500

#define LOG_INMEDIATLY 0
#define REMOVE_CHECKSUM 1
#define DEBUG_IT 0
#define DEBUG_BUFF_EFFECT 0
#define DEBUG_SUMMONER_SKILL 0
#define DEBUG_EVENT_COUNTER	0
#define USE_M_DRIVE 0
#define TESTSERVER 0

#define FINAL_PATH "..\\Data\\"
#define COMMONSERVER_PATH "..\\Data\\CommonServer.cfg"
#define SERVER_INFO_PATH "Data\\Serverinfo.dat"

//#define INTERNATIONAL_JAPAN "1"
//#define INTERNATIONAL_ENGLISH "0"

#ifdef INTERNATIONAL_KOREA	
#define PROTOCOL_MOVE 0xD3
#define PROTOCOL_POSITION 0xDF
#define PROTOCOL_ATTACK 0xD7
#define PROTOCOL_BEATTACK 0x10
#endif

#ifdef INTERNATIONAL_ENGLISH
#define PROTOCOL_MOVE 0xD4
#define PROTOCOL_POSITION 0x15
#define PROTOCOL_ATTACK 0x11
#define PROTOCOL_BEATTACK 0xDB
#endif

#ifdef INTERNATIONAL_JAPAN
#define PROTOCOL_MOVE 0x1D
#define PROTOCOL_POSITION 0xD6
#define PROTOCOL_ATTACK 0xDC
#define PROTOCOL_BEATTACK 0xD7
#endif

#ifdef INTERNATIONAL_CHINA
#define PROTOCOL_MOVE 0xD7
#define PROTOCOL_POSITION 0xDD
#define PROTOCOL_ATTACK 0xD9
#define PROTOCOL_BEATTACK 0x1D
#endif

#ifdef INTERNATIONAL_TAIWAN
#define PROTOCOL_MOVE 0xD6
#define PROTOCOL_POSITION 0xDF
#define PROTOCOL_ATTACK 0xDD
#define PROTOCOL_BEATTACK 0xD2
#endif

#ifdef INTERNATIONAL_VIETNAM
#define PROTOCOL_MOVE 0xD9
#define PROTOCOL_POSITION 0xDC
#define PROTOCOL_ATTACK 0x15
#define PROTOCOL_BEATTACK 0x1D
#endif

#ifdef INTERNATIONAL_PHILIPPINES
#define PROTOCOL_MOVE 0xDD
#define PROTOCOL_POSITION 0xDF
#define PROTOCOL_ATTACK 0xD6
#define PROTOCOL_BEATTACK 0x11
#endif

extern CMsg lMsg;

#define MSGGET(x,y)	(x*256)+y

#endif
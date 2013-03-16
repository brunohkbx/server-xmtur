#include "StdAfx.h"
#include "NewsSystem.h"
#include "User.h"
#include "LogProc.h"
#include "GameMain.h"
#include "Readscript.h"

CNewsSystem NewsSystem;

void NewsSystem_StartCore(void * lpParam);

CNewsSystem::CNewsSystem(){
	
	WIN32_FIND_DATAA wFD;
	if(FindFirstFileA(NEWS_FILE,&wFD)==INVALID_HANDLE_VALUE){
		MessageBoxA(0,"Failed to open XTR News File","Error",MB_OK);
		ExitProcess(0);
	}

	//Load Configs
	this->NewsTime = GetPrivateProfileInt("NewsSystem","NewsTime",15,NEWS_FILE);
	this->NewsMultiplier = GetPrivateProfileInt("NewsSystem","NewsMultiplier",5,NEWS_FILE);
	this->IsEnable = GetPrivateProfileInt("NewsSystem","IsEnable",1,NEWS_FILE);

	char News_Line[10];

	this->News_Step = 0;

	if(this->IsEnable == 1){
		this->NewsCount = this->NewsMultiplier * 3;
		if(this->NewsCount > 30) this->NewsCount = 30;

		for(int i=0; i < this->NewsCount;++i){
			sprintf(News_Line,"News_%d",i);
			GetPrivateProfileString("NewsSystem",News_Line,"",this->NewsName[i],256,NEWS_FILE);
		}

		//_beginthread(NewsSystem_StartCore,0,NULL);
	}
}

CNewsSystem::~CNewsSystem(void){
}

void CNewsSystem::ReloadConfigs(){

	//Load Configs
	this->NewsTime = GetPrivateProfileInt("NewsSystem","NewsTime",15,NEWS_FILE);
	this->NewsMultiplier = GetPrivateProfileInt("NewsSystem","NewsMultiplier",5,NEWS_FILE);
	this->IsEnable = GetPrivateProfileInt("NewsSystem","IsEnable",1,NEWS_FILE);

	char News_Line[10];

	this->News_Step = 0;
	memset(this->NewsName,NULL,sizeof(this->NewsName));

	if(this->IsEnable == 1){
		this->NewsCount = this->NewsMultiplier * 3;
		if(this->NewsCount > 30) this->NewsCount = 30;

		for(int i=0; i < this->NewsCount;++i){
			sprintf(News_Line,"News_%d",i);
			GetPrivateProfileString("NewsSystem",News_Line,"",this->NewsName[i],256,NEWS_FILE);
		}

	}
}

void NewsSystem_StartCore(void * lpParam){
	while(TRUE){
		NewsSystem.NewsCore();
	}
	_endthread();
}

void CNewsSystem::NewsCore(){

	int Number = 0;
	this->News_Seconds = this->NewsTime * 60000;
	Sleep(this->News_Seconds);

	for(int i=0; i < 3;++i)
	{
			Number = ((this->News_Step*3)+i);
			GCServerMsgStringSendAll(this->NewsName[Number],0);

			LogAddTD("[NewsSystem][%d] News_%d, Step: %d",this->News_Seconds,Number,this->News_Step);
			LogAddTD("[NewsSystem][%d] %s",Number,this->NewsName[Number]);

			if(i == 2){
				if(this->News_Step < this->NewsMultiplier){
					this->News_Step++;
					LogAddTD("[NewsSystem] Range Up!");
				} else {
					this->News_Step = 0;
					LogAddTD("[NewsSystem] Range Default!");
				}
			} 
	}
}
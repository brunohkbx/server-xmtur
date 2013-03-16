#include "Stdafx.h"
#include "User.h"
#include "QuestManager.h"
#include "LogProc.h"
#include "..\include\readscript.h"
#include "DSProtocol.h"
#include "BuffManager.h"
#include "GameMain.h"
#include "GensSystem.h"
#include "..\include\Prodef.h"

CQuestManager QuestManager;

void CQuestManager::StartProcess(){
	this->LoadQuestList();
}

void CQuestManager::QuestProtocolCore(int aIndex,unsigned char* aRecv){
	
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];
	
	switch(aRecv[3]){
		case 0x30: //Send QuestList to NPC
			this->SendQuestList(aIndex, 275);
		break;

		case 0x0A: //Send Accept or Deny Dialog
			this->SendAcceptDenyDialog(aIndex,(PMSG_QUEST_GETINFO*)aRecv);
		break;
		
		case 0x0B: //Accept or Deny Quest
			this->AcceptDenyQuest(aIndex,(PMSG_QUEST_GETINFO*)aRecv);
		break;

		case 0x0D: //Finish Quest
			this->FinishQuest(aIndex,(PMSG_QUEST_GETINFO*)aRecv);
		break;
		
		case 0x0F: //Delete Quest from List
			this->DeleteQuest(aIndex,(QUEST_DETELE_REQ*)aRecv);
		break;

		case 0x10: //C,I & V Buttons
		break;

		case 0x1B: //T Key Information
			this->SendQuestInformation(aIndex,(PMSG_QUEST_GETINFO*)aRecv);
		break;

		case 0x31:{ //Send Buff to Players ^^!
				if(gObjIsConnected(lpObj->m_Index) == FALSE){
					return;
				}

				if(lpObj->Level > Configs.ShadowPhantomMaxLevel || lpObj->ChangeUP3rd != false){
					GCServerCmd(lpObj->m_Index, 0x0D, 0, 0);
					return;
				}

				int iAttack = 0;
				int iDefense = 0;
				int iDuration = 0;

				if(lpObj->Level <= 180){
					iAttack = lpObj->Level / 3 + 45;
					iDefense = lpObj->Level / 5 + 50;
				} else {
					iAttack = 105;
					iDefense = 86;
				}

				iDuration = (lpObj->Level / 6 + 30) * 60;

				gObjApplyBuffEffectDuration(lpObj, AT_NPC_HELP, 2, iAttack, 3, iDefense, iDuration);
		}
		break;
		

		default:
			LogAddTD("[QuestManager] Unk Packet Debug : %02X %02X Size: %02X",aRecv[2],aRecv[3],aRecv[1]);
		break;
	}
}


struct PMSG_KILLPLAYER
{
	PBMSG_HEAD h;	// C1:16
	unsigned char NumberH;	// 3
	unsigned char NumberL;	// 4
	unsigned char ExpH;	// 5
	unsigned char ExpL;	// 6
	unsigned char DamageH;	// 7
	unsigned char DamageL;	// 8
};

void Finish_ExpSend(OBJECTSTRUCT* lpObj, int Experience)
{
	PMSG_KILLPLAYER pMsg = {0xC3,sizeof(pMsg),0x16,0,0};
	pMsg.ExpH = SET_NUMBERH(Experience);
	pMsg.ExpL = SET_NUMBERL(Experience);
	pMsg.DamageH = 0;
	pMsg.DamageL = 0;

	lpObj->Experience += Experience;

	int Exp[2];
	if(lpObj->ChangeUP3rd){
		Exp[1] = Experience;
	} else {
		Exp[0] = Experience;
	}

	__int64 NewExp = Experience;
	gObjLevelUp(lpObj,NewExp,0,0);

	DataSend(lpObj->m_Index,(LPBYTE)&pMsg,sizeof(pMsg));
}

void CQuestManager::FinishQuest(int aIndex, PMSG_QUEST_GETINFO* lpMsg){

	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];
	
	int QuestID = GetQuestID(lpMsg->QuestType,lpMsg->QuestNumber);
	QuestInfo * lpQuest = &QuestList[QuestID];

	PMSG_QUEST_FINISH pMsg = {0xC1, sizeof(pMsg), 0xF6, 0x0D};

	int Durability = 0;
			
	pMsg.bAnswer = 1;

	if(this->QuestAlreadyAcepted(aIndex,lpQuest->QuestType,lpQuest->QuestNumber) == FALSE){
		this->DeleteQuest(aIndex,(QUEST_DETELE_REQ*)lpMsg);
		return;
	}

	for(int i=0; i < lpQuest->RequirementsCount;++i){

		Durability = -1;

		if(lpQuest->Requirements[i].ItemType >= 13 && lpQuest->Requirements[i].ItemType <= 15){
			Durability = lpQuest->Requirements[i].Durability;
		}

		this->RemoveRequirements(aIndex,
		lpQuest->Requirements[i].ItemType,
		lpQuest->Requirements[i].ItemIndex,
		Durability,
		lpQuest->Requirements[i].Level,
		lpQuest->Requirements[i].Luck,
		lpQuest->Requirements[i].Skill,
		lpQuest->Requirements[i].Option,
		lpQuest->Requirements[i].Excellent,
		lpQuest->Requirements[i].Ancient);	
	}

	__int64 NewExp = 0;

	int HasItems = 0;
	int Money = 0;
	for(int i=0; i < lpQuest->RewardsCount;++i){
		if(lpQuest->Rewards[i].Type == 4){
			HasItems = 1;
		} else if(lpQuest->Rewards[i].Type == 2){
			Money += lpQuest->Rewards[i].Count;
		}
	}

	//Full Money
	if((lpObj->Money+Money) > 2000000000){
		pMsg.bAnswer = 2;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.Size);
		return;
	}

	//Check for Empty Space!
	if(!CheckInventoryEmptySpace(lpObj,4,2) && HasItems == 1){
		pMsg.bAnswer = 3;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.Size);
		return;
	}

	for(int i=0; i < lpQuest->RewardsCount;++i){

		switch(lpQuest->Rewards[i].Type){
			case 1: //Experience
				Finish_ExpSend(lpObj,lpQuest->Rewards[i].Count);
				LogAddTD("[QuestManager][%s][%s] %d, Exp: %d",lpObj->AccountID,lpObj->Name,i,lpQuest->Rewards[i].Count);
			break;

			case 2: //Zen
				lpObj->Money += lpQuest->Rewards[i].Count;
				GCMoneySend(lpObj->m_Index,lpObj->Money);
				LogAddTD("[QuestManager][%s][%s] %d, Money: %d",lpObj->AccountID,lpObj->Name,i,lpQuest->Rewards[i].Count);
			break;

			case 4: //Items
				for(int u=0; u < lpQuest->Rewards[i].Count;++u){

					ItemSerialCreateSend(aIndex,235,lpObj->X,lpObj->Y,
					ITEMGET(lpQuest->Rewards[i].ItemType,lpQuest->Rewards[i].ItemIndex),
					lpQuest->Rewards[i].Level,
					lpQuest->Rewards[i].Durability,
					lpQuest->Rewards[i].Skill,
					lpQuest->Rewards[i].Luck,
					lpQuest->Rewards[i].Option,
					aIndex,
					lpQuest->Rewards[i].Excellent,
					lpQuest->Rewards[i].Ancient);
				}
			break;

			case 16: //Gens Contribution
				lpObj->GensContribution += lpQuest->Rewards[i].Count;
				GensSystem.SendGensInfo(aIndex);
				LogAddTD("[QuestManager][%s][%s] %d, Contribution: %d",lpObj->AccountID,lpObj->Name,i,lpQuest->Rewards[i].Count);
			break;
		}

	}

	this->DeleteQuest(aIndex,(QUEST_DETELE_REQ*)lpMsg);
	this->SendQuestList(aIndex,257);

	LogAddTD("[QuestManager][%s][%s](%d,%d) Quest Finished - %s",lpObj->AccountID,lpObj->Name,lpQuest->QuestType,lpQuest->QuestNumber,lpQuest->Name);
	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}


void CQuestManager::SendAcceptDenyDialog(int aIndex, PMSG_QUEST_GETINFO* lpMsg){
	
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];
	
	int QuestID = GetQuestID(lpMsg->QuestType,lpMsg->QuestNumber);
	QuestInfo * lpQuest = &QuestList[QuestID];

	if(this->QuestAlreadyAcepted(aIndex,lpQuest->QuestType,lpQuest->QuestNumber) == TRUE){
		this->SendQuestInformation(aIndex,lpMsg);
		return;
	}

	PMSG_QUEST_DIALOG_1 pMsg;

	pMsg.H = 0xC1;
	pMsg.Size = sizeof(pMsg);
	pMsg.Headcode = 0xF6;
	pMsg.Subcode = 0x0B;
	pMsg.Quest.Dialog = this->QuestList[QuestID].QuestNumber;
	pMsg.Quest.Quest = this->QuestList[QuestID].QuestType;
	pMsg.Unk = 0x00;
	
	DataSend(aIndex,(LPBYTE)&pMsg,pMsg.Size);
}

void CQuestManager::AcceptDenyQuest(int aIndex, PMSG_QUEST_GETINFO* lpMsg){

	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	int QuestID = GetQuestID(lpMsg->QuestType,lpMsg->QuestNumber);
	QuestInfo * lpQuest = &QuestList[QuestID];

	if(lpMsg->Result == 1){

		if(this->QuestAlreadyAcepted(aIndex,lpQuest->QuestType,lpQuest->QuestNumber) == FALSE){
			//Check for Empty Space
			int Place = this->GetEmptyPlace(aIndex);
			if(Place == -1) return;

			//Add to Quest List
			lpObj->QuestInfo[Place].Index = lpQuest->QuestNumber; 
			lpObj->QuestInfo[Place].Type = lpQuest->QuestType;
			memset(lpObj->QuestInfo[Place].Requirement,0,sizeof(lpObj->QuestInfo[Place].Requirement));
		}

		this->SendQuestInformation(aIndex,lpMsg);
		this->SendQuestToPlayer(aIndex);

		DGInsertQuestList(lpObj->m_Index,lpQuest->QuestType,lpQuest->QuestNumber);
	
		LogAddTD("[QuestManager][%s][%s] Quest Accepted - QuestID: %d",lpObj->AccountID,lpObj->Name,QuestID);

	} else if(lpMsg->Result == 2){
	
		PMSG_QUEST_ACCEPTQUEST pMsg = { 0xC1,sizeof(pMsg),0xF6,0x0C };
	
		pMsg.Quest.Dialog = lpQuest->RejectDialog;
		pMsg.Quest.Quest = lpQuest->QuestType;
		pMsg.Position = 0;
		pMsg.RequirementsCount = 0;
		pMsg.RewardsCount = 0;

		DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
		LogAddTD("[QuestManager][%s][%s] Quest Rejected - QuestID: %d",lpObj->AccountID,lpObj->Name,QuestID);

		this->SendQuestToPlayer(aIndex);
	}
}

void CQuestManager::DeleteQuest(int aIndex, QUEST_DETELE_REQ * lpMsg){


	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	int QuestID = GetQuestID(lpMsg->QuestType,lpMsg->QuestNumber);
	QuestInfo * lpQuest = &QuestList[QuestID];

	for(int X=0; X < MAX_ACCEPT_QUEST;X++){

		if(lpObj->QuestInfo[X].Type == lpQuest->QuestType
			&& lpObj->QuestInfo[X].Index == lpQuest->QuestNumber){

			lpObj->QuestInfo[X].Index = (BYTE)-1;
			lpObj->QuestInfo[X].Type = (BYTE)-1;
			memset(lpObj->QuestInfo[X].Requirement,0,sizeof(lpObj->QuestInfo[X].Requirement));

			DGDeleteQuestList(lpObj->m_Index,lpQuest->QuestType,lpQuest->QuestNumber);
	
			LogAddTD("[QuestManager][%s][%s] Quest Deleted - QuestID: %d",lpObj->AccountID,lpObj->Name,X);
			this->SendQuestToPlayer(aIndex);
			return;
		}
	}
}

void CQuestManager::GetQuestList(int aIndex){
	
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	for(int X=0; X < MAX_ACCEPT_QUEST; X++){
		lpObj->QuestInfo[X].Index = (BYTE)-1;
		lpObj->QuestInfo[X].Type = (BYTE)-1;
		memset(lpObj->QuestInfo[X].Requirement,0,sizeof(lpObj->QuestInfo[X].Requirement));
	}

	JGPQuestManagerList(aIndex);
}


void CQuestManager::SendQuestToPlayer(int aIndex){

	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_QUESTLIST_TKEY pMsg;

	pMsg.H = 0xC1;
	pMsg.Headcode = 0xF6;
	pMsg.Subcode = 0x1A;
	pMsg.Amount = 0;
	memset(pMsg.Quest,NULL,sizeof(pMsg.Quest));

	int Count = 0;
	int QuestID;
	for(int X=0; X < MAX_ACCEPT_QUEST;X++){

		if(lpObj->QuestInfo[X].Type != (BYTE)-1 && lpObj->QuestInfo[X].Index != (BYTE)-1){

			QuestID = this->GetQuestID(lpObj->QuestInfo[X].Type,lpObj->QuestInfo[X].Index);
			QuestInfo * lpQuest = &QuestList[QuestID];

			pMsg.Quest[X].DialogL = LOBYTE(lpQuest->AcceptDialog);
			pMsg.Quest[X].DialogH = HIBYTE(lpQuest->AcceptDialog);
			pMsg.Quest[X].QuestL = LOBYTE(lpQuest->QuestType);
			pMsg.Quest[X].QuestH = HIBYTE(lpQuest->QuestType);
			Count++;
		}
	}

	pMsg.Amount = Count;
	pMsg.Size = sizeof(pMsg)+(Count * 4);

	DataSend(aIndex,(PBYTE)&pMsg,pMsg.Size);
	LogAddTD("[QuestManager][%s][%s] %d Quest Send to Player",lpObj->AccountID,lpObj->Name,Count);

}

void CQuestManager::SendQuestInformation(int aIndex, PMSG_QUEST_GETINFO* lpMsg){
	
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];
	
	int QuestID = this->GetQuestID(lpMsg->QuestType,lpMsg->QuestNumber);
	QuestInfo * lpQuest = &QuestList[QuestID];

	PMSG_QUEST_ACCEPTQUEST pMsg = { 0xC1,sizeof(pMsg),0xF6,0x1B };

	if(lpMsg->Subcode != 0x1B){
		pMsg.Subcode = 0x0C;
	}

	if(this->QuestAlreadyAcepted(aIndex,lpQuest->QuestType,lpQuest->QuestNumber) == FALSE){
		this->DeleteQuest(aIndex,(QUEST_DETELE_REQ*)lpMsg);
		return;
	}

	pMsg.Quest.Dialog= lpQuest->AcceptDialog;
	pMsg.Quest.Quest = lpQuest->QuestType;
	pMsg.Position = 0;
	pMsg.RequirementsCount = (BYTE)lpQuest->RequirementsCount;
	pMsg.RewardsCount = (BYTE)lpQuest->RewardsCount;

	for(int i=0; i < lpQuest->RequirementsCount;++i){
			
		//Requirement Type - 1: Monster, 2: Skill, 3: ?, 4: Item
		pMsg.Requirements[i].TypeL = LOBYTE(lpQuest->Requirements[i].Type);
		pMsg.Requirements[i].TypeH = HIBYTE(lpQuest->Requirements[i].Type);

		//Empty, not Necesary
		pMsg.Requirements[i].Unk[0] = 0;
		pMsg.Requirements[i].Unk[1] = 0;
		pMsg.Requirements[i].Unk1[0] = 0;
		pMsg.Requirements[i].Unk1[1] = 0;

		int wQID = this->GetQuestPlace(aIndex,lpQuest->QuestType,lpQuest->QuestNumber);
		switch(lpQuest->Requirements[i].Type){

			case 1: //Monster
				pMsg.Requirements[i].CountL = LOBYTE(lpQuest->Requirements[i].Count);
				pMsg.Requirements[i].CountH = HIBYTE(lpQuest->Requirements[i].Count);
				pMsg.Requirements[i].ClassL = LOBYTE(lpQuest->Requirements[i].ItemType);
				pMsg.Requirements[i].ClassH = HIBYTE(lpQuest->Requirements[i].ItemType);
				pMsg.Requirements[i].TotalL = LOBYTE(lpObj->QuestInfo[wQID].Requirement[i]); //Here need to get the Kill Count
				pMsg.Requirements[i].TotalH = HIBYTE(lpObj->QuestInfo[wQID].Requirement[i]);
			break;

			case 2: //Skill
				pMsg.Requirements[i].ClassL = LOBYTE(lpQuest->Requirements[i].ItemType);
				pMsg.Requirements[i].ClassH = HIBYTE(lpQuest->Requirements[i].ItemType);

				if(gObjGetMagicSearch(lpObj,(BYTE)lpQuest->Requirements[i].ItemType) > 0){
					pMsg.Requirements[i].TotalL = LOBYTE(1); //Here need to get the Skill Count
					pMsg.Requirements[i].TotalH = HIBYTE(1);
				} 
			break;

			case 4: //Items
				int ItemNumber = ITEMGET(lpQuest->Requirements[i].ItemType,
					lpQuest->Requirements[i].ItemIndex);

				int Durability = -1;
				if(lpQuest->Requirements[i].ItemType >= 13 && lpQuest->Requirements[i].ItemType <= 15){
					Durability = lpQuest->Requirements[i].Durability;
				}

				int ItemCount = GetItemsCount(aIndex,
				lpQuest->Requirements[i].ItemType,
				lpQuest->Requirements[i].ItemIndex,
				Durability,
				lpQuest->Requirements[i].Level,
				lpQuest->Requirements[i].Luck,
				lpQuest->Requirements[i].Skill,
				lpQuest->Requirements[i].Option,
				lpQuest->Requirements[i].Excellent,
				lpQuest->Requirements[i].Ancient);

				pMsg.Requirements[i].CountL = LOBYTE(lpQuest->Requirements[i].Count);
				pMsg.Requirements[i].CountH = HIBYTE(lpQuest->Requirements[i].Count);
				pMsg.Requirements[i].ClassL = LOBYTE(ItemNumber);
				pMsg.Requirements[i].ClassH = HIBYTE(ItemNumber);
				pMsg.Requirements[i].TotalL = 0; //Here need to get the Kill Count
				pMsg.Requirements[i].TotalH = 0;

				CItem Item;
				Item.m_Type = ItemNumber;
				Item.m_Level = lpQuest->Requirements[i].Level;
				Item.m_Option1 = lpQuest->Requirements[i].Luck;
				Item.m_Option2 = lpQuest->Requirements[i].Skill;
				Item.m_Option3 = lpQuest->Requirements[i].Option;
				Item.m_NewOption = lpQuest->Requirements[i].Excellent;
				Item.m_ItemOptionEx = lpQuest->Requirements[i].Ancient;Item.m_SpecialNum = 0;
				Item.m_SetOption = 0;
				Item.m_bItemExist = 1;
				Item.m_JewelOfHarmonyOption = 0;
				Item.m_ItemOptionEx = 0;
				ItemByteConvert(pMsg.Requirements[i].ItemInfo,Item);
			break;
		}
	}

	for(int i=0; i < lpQuest->RewardsCount;++i){
			
		//Rewards Info
		pMsg.Rewards[i].TypeL = LOBYTE(lpQuest->Rewards[i].Type);
		pMsg.Rewards[i].TypeH = HIBYTE(lpQuest->Rewards[i].Type);

		pMsg.Rewards[i].ClassL = LOBYTE(lpQuest->Rewards[i].ItemType);
		pMsg.Rewards[i].ClassH = HIBYTE(lpQuest->Rewards[i].ItemType);

		pMsg.Rewards[i].Count[0] = (lpQuest->Rewards[i].Count & 0xFF);
		pMsg.Rewards[i].Count[1] = (lpQuest->Rewards[i].Count >> 8);
		pMsg.Rewards[i].Count[2] = (lpQuest->Rewards[i].Count >> 16);
		pMsg.Rewards[i].Count[3] = (lpQuest->Rewards[i].Count >> 24);

		//Empty, not Necesary
		pMsg.Rewards[i].Unk[0] = 0;
		pMsg.Rewards[i].Unk[1] = 0;

		switch(lpQuest->Rewards[i].Type){

			case 4: //Items
				int ItemNumber = ITEMGET(lpQuest->Rewards[i].ItemType,
					lpQuest->Rewards[i].ItemIndex);

				pMsg.Rewards[i].Count[0] = LOBYTE(lpQuest->Rewards[i].Count);
				pMsg.Rewards[i].Count[1] = HIBYTE(lpQuest->Rewards[i].Count);
				pMsg.Rewards[i].Count[2] = 0;
				pMsg.Rewards[i].Count[3] = 0;
				pMsg.Rewards[i].ClassL = LOBYTE(ItemNumber);
				pMsg.Rewards[i].ClassH = HIBYTE(ItemNumber);

				CItem Item;
				Item.m_Type = ItemNumber;
				Item.m_Level = lpQuest->Rewards[i].Level;
				Item.m_Option1 = lpQuest->Rewards[i].Luck;
				Item.m_Option2 = lpQuest->Rewards[i].Skill;
				Item.m_Option3 = lpQuest->Rewards[i].Option;
				Item.m_NewOption = lpQuest->Rewards[i].Excellent;
				Item.m_ItemOptionEx = lpQuest->Rewards[i].Ancient;
				Item.m_SpecialNum = 0;
				Item.m_SetOption = 0;
				Item.m_bItemExist = 1;
				Item.m_JewelOfHarmonyOption = 0;
				Item.m_ItemOptionEx = 0;
				ItemByteConvert(pMsg.Rewards[i].ItemInfo,Item);
			break;
		}
	}

	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}



void CQuestManager::SendQuestList(int aIndex, int NPC){ //Send Quest List to NPC
	
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_QUESTLIST_PHANTOM pMsg;

	pMsg.H = 0xC1;
	pMsg.Headcode = 0xF6;
	pMsg.Subcode = 0x0A;
	pMsg.NPCType = NPC;
	
	int Total=0;
	BOOL AddQuest;

	if((lpObj->GensFamily == 1 && lpObj->TargetShopNumber == 544)
		|| (lpObj->GensFamily == 2 && lpObj->TargetShopNumber == 543)){
		BYTE GensNPC[5] = { 0xC1,0x05,0xF8,0x04,0x03 };
		DataSend(aIndex,(LPBYTE)&GensNPC,GensNPC[1]);
		return;
	}

	for(int i=0; i < this->Count;++i){
		QuestInfo * lpQuest = &QuestList[i];

		AddQuest = FALSE;

		if(this->AllowedClass(aIndex,i) == TRUE){
			if(this->QuestAlreadyAcepted(aIndex,lpQuest->QuestType,lpQuest->QuestNumber)){
				AddQuest = TRUE;
			} else if(lpObj->Level >= lpQuest->MinLevel && lpObj->Level <= lpQuest->MaxLevel){
				AddQuest = TRUE;
			}
		}

		if(lpQuest->AllowedNPC == lpObj->TargetShopNumber && AddQuest == TRUE){
			pMsg.Quest[Total].Dialog = lpQuest->QuestNumber;
			pMsg.Quest[Total].Quest = lpQuest->QuestType;
			Total++;
		}
		
	}

	pMsg.CountH = HIBYTE(Total);
	pMsg.CountL = LOBYTE(Total);
	pMsg.Size = sizeof(pMsg);
	DataSend(aIndex,(PBYTE)&pMsg,pMsg.Size);

	LogAddTD("[QuestManager][%s][%s] %d Quest Send (%d)",lpObj->AccountID,lpObj->Name,Total,lpObj->TargetShopNumber);
}

void CQuestManager::LoadQuestList(){

	int Token, Step;
	SMDFile = fopen("..\\Data\\QuestManager.dat","r");

	if(!SMDFile){
		MsgBox("[QuestManager] Failed Loading Quests File");
		return;
	}

	memset(this->QuestList,NULL,sizeof(this->QuestList));
	this->Count = 0;

	while(true){
	    Token = GetToken();

        if(Token == 2){ break; }
		if(!strcmp("end",TokenString)){ break; }
	
		Step = (int)TokenNumber;

		if(Step == 0){ //Load First Step

			while(true){

				Token = GetToken();

				if(Token == 2){ break; }
				if(!strcmp("end",TokenString)){ break; }

				this->QuestList[this->Count].QuestType = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].QuestNumber = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].AcceptDialog = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].RejectDialog = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].AllowedNPC = (int)TokenNumber;

				Token = GetToken();
				memcpy(this->QuestList[this->Count].Name,TokenString,sizeof(this->QuestList[this->Count].Name));

				Token = GetToken();
				this->QuestList[this->Count].MinLevel = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].MaxLevel = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Class[0] = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Class[1] = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Class[2] = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Class[3] = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Class[4] = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Class[5] = (int)TokenNumber;

			}
		}

		if(Step == 1){ //Load Requirements

			while(true){
				Token = GetToken();

				if(Token == 2){ break; }
				if(!strcmp("end",TokenString)){ 
					break; 
				}

				this->QuestList[this->Count].Requirements[this->QuestList[this->Count].RequirementsCount].Type = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Requirements[this->QuestList[this->Count].RequirementsCount].ItemType = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Requirements[this->QuestList[this->Count].RequirementsCount].ItemIndex = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Requirements[this->QuestList[this->Count].RequirementsCount].Level = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Requirements[this->QuestList[this->Count].RequirementsCount].Option = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Requirements[this->QuestList[this->Count].RequirementsCount].Luck = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Requirements[this->QuestList[this->Count].RequirementsCount].Skill = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Requirements[this->QuestList[this->Count].RequirementsCount].Durability = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Requirements[this->QuestList[this->Count].RequirementsCount].Excellent = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Requirements[this->QuestList[this->Count].RequirementsCount].Ancient = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Requirements[this->QuestList[this->Count].RequirementsCount].Count = (int)TokenNumber;

				this->QuestList[this->Count].RequirementsCount++;
			}
		}

		if(Step == 2){ //Load Rewards

			while(true){
				Token = GetToken();

				if(Token == 2){ break; }
				if(!strcmp("end",TokenString)){
					LogAddTD("[QuestManager](%d) %s Loaded",this->Count,this->QuestList[this->Count].Name);
					this->Count += 1;
					break; 
				}

				this->QuestList[this->Count].Rewards[this->QuestList[this->Count].RewardsCount].Type = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Rewards[this->QuestList[this->Count].RewardsCount].ItemType = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Rewards[this->QuestList[this->Count].RewardsCount].ItemIndex = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Rewards[this->QuestList[this->Count].RewardsCount].Level = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Rewards[this->QuestList[this->Count].RewardsCount].Option = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Rewards[this->QuestList[this->Count].RewardsCount].Luck = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Rewards[this->QuestList[this->Count].RewardsCount].Skill = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Rewards[this->QuestList[this->Count].RewardsCount].Durability = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Rewards[this->QuestList[this->Count].RewardsCount].Excellent = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Rewards[this->QuestList[this->Count].RewardsCount].Ancient = (int)TokenNumber;

				Token = GetToken();
				this->QuestList[this->Count].Rewards[this->QuestList[this->Count].RewardsCount].Count = (int)TokenNumber;

				this->QuestList[this->Count].RewardsCount++;
			}
		}
	}
	
	fclose(SMDFile);
}

/////////////////////////////////////////////////////////////////////
/// Check Functions

int CQuestManager::GetQuestID(int QuestType, int QuestNumber){

	for(int X=0; X < this->Count;X++){
		if(this->QuestList[X].QuestType == QuestType && 
		(this->QuestList[X].AcceptDialog == QuestNumber 
		|| this->QuestList[X].QuestNumber == QuestNumber)){
			return X;
		}
	}

	return -1;
}

BOOL CQuestManager::QuestAlreadyAcepted(int aIndex, int QuestType, int QuestNumber){

	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return FALSE;
	if(!gObjIsConnected(aIndex)) return FALSE;

	LPOBJ lpObj = &gObj[aIndex];
	
	for(int X=0; X < MAX_ACCEPT_QUEST; X++){
		if(lpObj->QuestInfo[X].Index == QuestNumber && lpObj->QuestInfo[X].Type == QuestType){
			return TRUE;
		}
	}

	return FALSE;
}

int CQuestManager::GetQuestPlace(int aIndex, int QuestType, int QuestNumber){

	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return -1;
	if(!gObjIsConnected(aIndex)) return -1;

	LPOBJ lpObj = &gObj[aIndex];
	
	for(int X=0; X < MAX_ACCEPT_QUEST; X++){
		if(lpObj->QuestInfo[X].Index == QuestNumber && 
			lpObj->QuestInfo[X].Type == QuestType){

			return X;
		}
	}

	return -1;
}

int CQuestManager::GetEmptyPlace(int aIndex){
	
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return -1;
	if(!gObjIsConnected(aIndex)) return -1;

	LPOBJ lpObj = &gObj[aIndex];
	
	for(int X=0; X < MAX_ACCEPT_QUEST; X++){
		if(lpObj->QuestInfo[X].Index == (BYTE)-1 && 
			lpObj->QuestInfo[X].Type == (BYTE)-1){
			return X;
		}
	}

	return -1;
}

BOOL CQuestManager::AllowedClass(int aIndex, int QuestID){

	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return FALSE;
	if(!gObjIsConnected(aIndex)) return FALSE;

	LPOBJ lpObj = &gObj[aIndex];

	int DbClass = 0;

	switch(lpObj->DbClass){
		case 0: case 1: case 3: DbClass = 0; break;
		case 16: case 17: case 19: DbClass = 1; break;
		case 32: case 33: case 35: DbClass = 2; break;
		case 80: case 81: case 83: DbClass = 5; break;
		case 48: case 50: DbClass = 3; break;
		case 64: case 68: DbClass = 4; break;
	}

	QuestInfo * lpQuest = &QuestList[QuestID];

	if(lpQuest->Class[DbClass] == 1) return TRUE;
	return FALSE;
}

int CQuestManager::RemoveRequirements(int aIndex,int ItemType,int ItemIndex,
int Durability,int Level,int Luck,int Skill,int Option,int Excellent,int Ancient)
{
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return 0;
	if(!gObjIsConnected(aIndex)) return 0;

	LPOBJ lpObj = &gObj[aIndex];

	int Item = ITEMGET(ItemType,ItemIndex);

	int Count = 0;
	for(int i=12; i < 76;i++){	
		
		if(Durability != -1 && lpObj->pInventory[i].m_Durability == Durability) continue;

		if(lpObj->pInventory[i].m_Type == Item
			&& lpObj->pInventory[i].m_Level == Level
			&& lpObj->pInventory[i].m_Option1 == Skill
			&& lpObj->pInventory[i].m_Option2 == Luck
			&& lpObj->pInventory[i].m_Option3 == Option
			&& lpObj->pInventory[i].m_NewOption == Excellent
			&& lpObj->pInventory[i].m_SetOption == Ancient){
			//Remove Items
			gObjInventoryDeleteItem(aIndex,i);
			GCInventoryItemDeleteSend(aIndex,i,1);
			lpObj->pInventory[i].Clear();
		}
	}

	return Count;
}

int CQuestManager::GetItemsCount(int aIndex,int ItemType,int ItemIndex,
int Durability,int Level,int Luck,int Skill,int Option,int Excellent,int Ancient)
{
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return 0;
	if(!gObjIsConnected(aIndex)) return 0;

	LPOBJ lpObj = &gObj[aIndex];

	int Item = ITEMGET(ItemType,ItemIndex);

	int Count = 0;
	for(int i=12; i < 76;i++){	
		
		if(Durability != -1 && lpObj->pInventory[i].m_Durability == Durability) continue;

		if(lpObj->pInventory[i].m_Type == Item
			&& lpObj->pInventory[i].m_Level == Level
			&& lpObj->pInventory[i].m_Option1 == Skill
			&& lpObj->pInventory[i].m_Option2 == Luck
			&& lpObj->pInventory[i].m_Option3 == Option
			&& lpObj->pInventory[i].m_NewOption == Excellent
			&& lpObj->pInventory[i].m_SetOption == Ancient){
				Count++;
		}
	}

	return Count;
}
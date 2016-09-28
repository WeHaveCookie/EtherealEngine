#include "stdafx.h"
#include "MinerKidOwnedStates.h"
#include "fsm/State.h"
#include "MinerKid.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"
#include "Utils/Random.h"
#include "EtherealDreamManagers.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


	//this is a singleton
PlayWithMom* PlayWithMom::Instance()
{
	static PlayWithMom instance;

	return &instance;
}

void PlayWithMom::Enter(MinerKid* pMinerKid)
{
	Dispatch->DispatchMessage(1.5,                  //time delay
		pMinerKid->ID(),           //sender ID
		ent_Elsa,           //receiver ID
		Msg_WantToPlay,        //msg
		NO_ADDITIONAL_INFO);

	if (pMinerKid->Location() != shack)
	{
		pMinerKid->displayEnterText("I want to play with Mom !");

		pMinerKid->ChangeLocation(shack);
	}
}

void PlayWithMom::Execute(MinerKid* pMinerKid)
{
	if (pMinerKid->isPlaying())
	{
		pMinerKid->IncreaseFatigue();
		pMinerKid->addHappyness(2);
		pMinerKid->displayText("Ahaha So funny mom ! I love play with u");

		if (pMinerKid->Happy())
		{
			Dispatch->DispatchMessage(1.5,                  //time delay
				pMinerKid->ID(),           //sender ID
				ent_Elsa,           //receiver ID
				Msg_StopPlaying,        //msg
				NO_ADDITIONAL_INFO);
			
			pMinerKid->GetFSM()->ChangeState(EnterForestAndFindStone::Instance());
		}
	}
	else
	{
		pMinerKid->IncreaseBored();

		if (pMinerKid->BoredToWait())
		{
			Dispatch->DispatchMessage(1.5,                  //time delay
				pMinerKid->ID(),           //sender ID
				ent_Elsa,           //receiver ID
				Msg_StopPlaying,        //msg
				NO_ADDITIONAL_INFO);
			pMinerKid->displayText("I'm bored to wait you Mom, I'm leave !");
			pMinerKid->GetFSM()->ChangeState(EnterForestAndFindStone::Instance());
		}
		else
		{
			pMinerKid->displayText("Come on Mom...");
		}
	}

}

 void PlayWithMom::Exit(MinerKid* pMinerKid)
{
	 pMinerKid->displayExitText("I'm bored. See a later !");
}

bool PlayWithMom::OnMessage(MinerKid* pMinerKid, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_MomArrive:

		cout << "\nMessage handled by " << GetNameOfEntity(pMinerKid->ID())
			<< " at time: " << g_CurrentTime;


		pMinerKid->displayOnMessageText("Finally !");

		pMinerKid->setPlaying(true);

		return true;

	}//end switch

	return false; //send message to global message handler
}



//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
EnterForestAndFindStone* EnterForestAndFindStone::Instance()
{
	static EnterForestAndFindStone instance;

	return &instance;
}


void EnterForestAndFindStone::Enter(MinerKid* pMinerKid)
{
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	if (pMinerKid->Location() != forest)
	{
		pMinerKid->displayEnterText("I want to be a miner like Pa' !");

		pMinerKid->ChangeLocation(forest);
	}
}


void EnterForestAndFindStone::Execute(MinerKid* pMinerKid)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	pMinerKid->AddToStoneCarried(1);

	pMinerKid->IncreaseFatigue();

	pMinerKid->displayText("I find a stone. It's like a gold, but grey !!! Yeaaah");

	//if enough gold mined, go and put it in the bank
	if (pMinerKid->PocketsFull())
	{
		pMinerKid->GetFSM()->ChangeState(VisitVillage::Instance());
	}

	if (pMinerKid->Thirsty())
	{
		int res = randIntBorned(0, 100);
		if (res < 50)
		{
			pMinerKid->GetFSM()->ChangeState(VisitArcaneRoom::Instance());
		}
		else
		{
			pMinerKid->GetFSM()->ChangeState(PlayWithMom::Instance());
		}
	}
}


void EnterForestAndFindStone::Exit(MinerKid* pMinerKid)
{
	pMinerKid->displayExitText("I'm carry out. I just leave this forest.");
}


bool EnterForestAndFindStone::OnMessage(MinerKid* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

VisitVillage* VisitVillage::Instance()
{
	static VisitVillage instance;

	return &instance;
}

void VisitVillage::Enter(MinerKid* pMinerKid)
{
	//on entry the miner makes sure he is located at the bank
	if (pMinerKid->Location() != village)
	{
		pMinerKid->displayEnterText("Time to play on village !!");

		pMinerKid->ChangeLocation(village);
	}
}


void VisitVillage::Execute(MinerKid* pMinerKid)
{
	//Play with stone

	pMinerKid->AddToStoneCarried(-1);
	pMinerKid->displayText("I'm play with my nugget of stone !");

	pMinerKid->addHappyness(1);

	if (pMinerKid->Happy())
	{
		pMinerKid->displayText("I'm bored now. I just go home.");

		pMinerKid->GetFSM()->ChangeState(GoHomeAndSleep::Instance());
	}

	//otherwise get more stone
	else if(pMinerKid->StoneCarried() == 0)
	{
		pMinerKid->GetFSM()->ChangeState(EnterForestAndFindStone::Instance());
	}
}


void VisitVillage::Exit(MinerKid* pMinerKid)
{
	pMinerKid->displayExitText("I'm leaving the village");
}


bool VisitVillage::OnMessage(MinerKid* pMinerKid, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}
//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

GoHomeAndSleep* GoHomeAndSleep::Instance()
{
	static GoHomeAndSleep instance;

	return &instance;
}

void GoHomeAndSleep::Enter(MinerKid* pMinerKid)
{
	if (pMinerKid->Location() != shack)
	{
		pMinerKid->displayEnterText("Sweet Home !");

		pMinerKid->ChangeLocation(shack);

		//let the wife know I'm home
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMinerKid->ID(),        //ID of sender
			ent_Elsa,            //ID of recipient
			Msg_HiMomImBack,   //the message
			NO_ADDITIONAL_INFO);
	}
}

void GoHomeAndSleep::Execute(MinerKid* pMinerKid)
{
	//if miner is not fatigued start to dig for nuggets again.
	if (!pMinerKid->Fatigued())
	{
		pMinerKid->displayText("I'm felling so good ! Time to work like Pa' !");

		if (pMinerKid->Thirsty())
		{
			pMinerKid->GetFSM()->ChangeState(PlayWithMom::Instance());
		}
		else
		{
			pMinerKid->GetFSM()->ChangeState(EnterForestAndFindStone::Instance());
		}
	}

	else
	{
		//sleep
		pMinerKid->DecreaseFatigue();

		pMinerKid->displayText("ZZZZUnicornZZZ....");
	}
}

void GoHomeAndSleep::Exit(MinerKid* pMiner)
{
}


bool GoHomeAndSleep::OnMessage(MinerKid* pMinerKid, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_StewReady:

		cout << "\nMessage handled by " << GetNameOfEntity(pMinerKid->ID())
			<< " at time: " << g_CurrentTime;


		pMinerKid->displayOnMessageText("I'm so hungry !!!! I'm comming !!!!!");

		pMinerKid->GetFSM()->ChangeState(EatStewLikeBeast::Instance());

		return true;

	}//end switch

	return false; //send message to global message handler
}

//------------------------------------------------------------------------QuenchThirst

VisitArcaneRoom* VisitArcaneRoom::Instance()
{
	static VisitArcaneRoom instance;

	return &instance;
}

void VisitArcaneRoom::Enter(MinerKid* pMinerKid)
{
	if (pMinerKid->Location() != arcane)
	{
		pMinerKid->ChangeLocation(arcane);

		pMinerKid->displayEnterText("Time to play with potion !");
	}
}

void VisitArcaneRoom::Execute(MinerKid* pMinerKid)
{
	pMinerKid->PlayVideoGame();

	pMinerKid->displayText("A piece of root and BOOOM");

	pMinerKid->GetFSM()->ChangeState(EnterForestAndFindStone::Instance());
}


void VisitArcaneRoom::Exit(MinerKid* pMinerKid)
{
	pMinerKid->displayExitText("Break finish ! Time to find some stone");
}


bool VisitArcaneRoom::OnMessage(MinerKid* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------EatStew

EatStewLikeBeast* EatStewLikeBeast::Instance()
{
	static EatStewLikeBeast instance;

	return &instance;
}


void EatStewLikeBeast::Enter(MinerKid* pMinerKid)
{
	pMinerKid->displayEnterText("Eat, eat, eat !");
}

void EatStewLikeBeast::Execute(MinerKid* pMinerKid)
{
	pMinerKid->displayText("Grblblbllblb");

	pMinerKid->GetFSM()->RevertToPreviousState();
}

void EatStewLikeBeast::Exit(MinerKid* pMinerKid)
{
	pMinerKid->displayExitText("See later");
}


bool EatStewLikeBeast::OnMessage(MinerKid* pMinerKid, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}



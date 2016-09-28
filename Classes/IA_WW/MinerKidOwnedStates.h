#ifndef MINERKID_OWNED_STATES_H
#define MINERKID_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinerOwnedStates.h
//
//  Desc:   All the states that can be assigned to the Miner class.
//          Note that a global state has not been implemented.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "fsm/State.h"


class MinerKid;
struct Telegram;



class PlayWithMom : public State<MinerKid>
{

public:
	PlayWithMom() {}

	//copy ctor and assignment should be private
	PlayWithMom(const PlayWithMom&);
	PlayWithMom& operator=(const PlayWithMom&);


	//this is a singleton
	static PlayWithMom* Instance();

	virtual void Enter(MinerKid* pMinerKid);

	virtual void Execute(MinerKid* pMinerKid);

	virtual void Exit(MinerKid* pMinerKid);

	virtual bool OnMessage(MinerKid* pMinerKid, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//  In this state the kid will walk to a forest and pick up a nugget
//  of stone. If the kid already has a nugget of stone he'll change state
//  to VisitVillage. If he gets thirsty he'll change state
//  to QuenchThirst
//------------------------------------------------------------------------
class EnterForestAndFindStone : public State<MinerKid>
{
private:

	EnterForestAndFindStone() {}

	//copy ctor and assignment should be private
	EnterForestAndFindStone(const EnterForestAndFindStone&);
	EnterForestAndFindStone& operator=(const EnterForestAndFindStone&);

public:

	//this is a singleton
	static EnterForestAndFindStone* Instance();

	virtual void Enter(MinerKid* miner);

	virtual void Execute(MinerKid* miner);

	virtual void Exit(MinerKid* miner);

	virtual bool OnMessage(MinerKid* agent, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//  Entity will go to a village and play with stone he is carrying. If the 
//  child is subsequently wealthy enough he'll walk home, otherwise he'll
//  keep going to get more stone
//------------------------------------------------------------------------
class VisitVillage : public State<MinerKid>
{
private:

	VisitVillage() {}

	//copy ctor and assignment should be private
	VisitVillage(const VisitVillage&);
	VisitVillage& operator=(const VisitVillage&);

public:

	//this is a singleton
	static VisitVillage* Instance();

	virtual void Enter(MinerKid* miner);

	virtual void Execute(MinerKid* miner);

	virtual void Exit(MinerKid* miner);

	virtual bool OnMessage(MinerKid* agent, const Telegram& msg);
};


//------------------------------------------------------------------------
//
//  Kid will go home and sleep until his fatigue is decreased
//  sufficiently
//------------------------------------------------------------------------
class GoHomeAndSleep : public State<MinerKid>
{
private:

	GoHomeAndSleep() {}

	//copy ctor and assignment should be private
	GoHomeAndSleep(const GoHomeAndSleep&);
	GoHomeAndSleep& operator=(const GoHomeAndSleep&);

public:

	//this is a singleton
	static GoHomeAndSleep* Instance();

	virtual void Enter(MinerKid* miner);

	virtual void Execute(MinerKid* miner);

	virtual void Exit(MinerKid* miner);

	virtual bool OnMessage(MinerKid* agent, const Telegram& msg);
};


//------------------------------------------------------------------------
//
//  Kid changes location to the ArcaneRoom and keeps play until
//  his thirst is quenched. When satisfied he returns to the forest
//  and resumes his quest for nuggets.
//------------------------------------------------------------------------
class VisitArcaneRoom : public State<MinerKid>
{
private:

	VisitArcaneRoom() {}

	//copy ctor and assignment should be private
	VisitArcaneRoom(const VisitArcaneRoom&);
	VisitArcaneRoom& operator=(const VisitArcaneRoom&);

public:

	//this is a singleton
	static VisitArcaneRoom* Instance();

	virtual void Enter(MinerKid* miner);

	virtual void Execute(MinerKid* miner);

	virtual void Exit(MinerKid* miner);

	virtual bool OnMessage(MinerKid* agent, const Telegram& msg);
};


//------------------------------------------------------------------------
//
//  this is implemented as a state blip. The kid eats the stew, gives
//  Mom' some compliments and then returns to his previous state
//------------------------------------------------------------------------
class EatStewLikeBeast : public State<MinerKid>
{
private:

	EatStewLikeBeast() {}

	//copy ctor and assignment should be private
	EatStewLikeBeast(const EatStewLikeBeast&);
	EatStewLikeBeast& operator=(const EatStewLikeBeast&);

public:

	//this is a singleton
	static EatStewLikeBeast* Instance();

	virtual void Enter(MinerKid* miner);

	virtual void Execute(MinerKid* miner);

	virtual void Exit(MinerKid* miner);

	virtual bool OnMessage(MinerKid* agent, const Telegram& msg);
};




#endif
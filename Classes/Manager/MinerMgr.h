#pragma once

#include "Manager.h"

class Miner;
class MinersWife;
class MinerKid;

struct UpdateTask
{
	int nbrStep;

	UpdateTask()
	{
		nbrStep = 0;
	}
};

class MinerMgr : public Manager
{
public:
	MinerMgr();
	~MinerMgr();

	void init();
	void process(const float dt);
	void end();

private:
	Miner*		m_bob;
	MinersWife* m_elsa;
	MinerKid*	m_bobJr;

	float		m_timeElapsed;
};
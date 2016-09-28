#include "stdafx.h"
#include "MinerMgr.h"
#include "IA_WW/Locations.h"
#include "IA_WW/Miner.h"
#include "IA_WW/MinersWife.h"
#include "IA_WW/MinerKid.h"
#include "IA_WW/EntityManager.h"
#include "IA_WW/MessageDispatcher.h"
#include "IA_WW/EntityNames.h"
#include "Entity/Entity.h"

MinerMgr::MinerMgr()
	:Manager(ManagerType::Miner)
{

}

MinerMgr::~MinerMgr()
{

}

void MinerMgr::init()
{
	//create a miner
	m_bob = new Miner(ent_Miner_Bob);

	//create his wife
	m_elsa = new MinersWife(ent_Elsa);

	//Create his kids
	m_bobJr = new MinerKid(ent_BobJr);

	//register them with the entity manager
	EntityMinerMgr->RegisterEntity(m_bob);
	EntityMinerMgr->RegisterEntity(m_elsa);
	EntityMinerMgr->RegisterEntity(m_bobJr);

	m_timeElapsed = 0.0;
}

void MinerMgr::process(const float dt)
{
	m_timeElapsed += dt;
	if (m_timeElapsed > 2.0f)
	{
		m_bob->pushTask(1);
		m_elsa->pushTask(1);
		m_bobJr->pushTask(1);
		Dispatch->DispatchDelayedMessages();
		m_timeElapsed = 0.0f;
	}
}

void MinerMgr::end()
{
	//tidy up
	m_bob->stop();
	m_elsa->stop();
	m_bobJr->stop();
	delete m_bob;
	delete m_elsa;
	delete m_bobJr;
}
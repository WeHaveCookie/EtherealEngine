#include "stdafx.h"
#include "GestureRecognition.h"
#include "Manager/Input/InputMgr.h"
#include "Manager/Action/CommandMgr.h"
#include "Actions/Command.h"

GestureRecognition::GestureRecognition()
{
}


GestureRecognition::~GestureRecognition()
{
}

void GestureRecognition::init()
{
	m_gesture.m_finish = true;
	m_gesture.m_valid = false;
	m_displayWindow = false;
	m_updateTimer = 0.05f;
	m_timer = 0.0f;
}

void GestureRecognition::process(const float dt)
{
	displayWindow();
	m_timer += dt;
	if (m_timer > m_updateTimer)
	{
		auto inputMgr = InputMgr::getSingleton();
		auto pads = inputMgr->getActivePads();
		if (pads.size() > 0)
		{
			auto padR = inputMgr->getPadKeyValue(KeyType::padR, inputMgr->getActivePads()[0]);
			auto padU = inputMgr->getPadKeyValue(KeyType::padU, inputMgr->getActivePads()[0]);
			if ((padR < -20.0f || padR > 20.0f
				|| padU < -20.0f || padU > 20.0f))
			{
				if (m_gesture.m_finish && !m_gesture.m_start)
				{
					m_gesture.m_gPoints.clear();
					m_gesture.m_timer = 0.0f;
					m_gesture.m_start = true;
				}
				m_gesture.addGPoint(sf::Vector2f(padR, padU));
				m_gesture.m_timer += dt;
			}
			else
			{
				
				m_gesture.m_finish = true;
				if (m_gesture.m_gPoints.size() > 2 && m_gesture.m_start)
				{
					m_gesture.m_valid = true;
				}
				m_gesture.m_start = false;
			}
		}
		m_timer = 0.0f;
	}
}

std::vector<GesturePoint> GestureRecognition::getCardinalPoint()
{
	std::vector<GesturePoint> cardinalPoints;
	GesturePoint gp;
	gp.m_index = 1000;
	gp.m_pos = sf::Vector2f(-60.0f, 0.0f);
	cardinalPoints.push_back(gp);
	gp.m_pos = sf::Vector2f(0.0f, 60.0f);
	cardinalPoints.push_back(gp);
	gp.m_pos = sf::Vector2f(60.0f, 0.0f);
	cardinalPoints.push_back(gp);
	gp.m_pos = sf::Vector2f(0.0f, -60.0f);
	cardinalPoints.push_back(gp);
	
	for (auto& gPoint : m_gesture.m_gPoints)
	{
		if (gPoint.m_pos.x < cardinalPoints[0].m_pos.x && gPoint.m_index <= cardinalPoints[0].m_index)
		{
			cardinalPoints[0] = gPoint;
		} else if (gPoint.m_pos.x > cardinalPoints[2].m_pos.x && gPoint.m_index <= cardinalPoints[2].m_index)
		{
			cardinalPoints[2] = gPoint;
		} else if (gPoint.m_pos.y < cardinalPoints[3].m_pos.y && gPoint.m_index <= cardinalPoints[3].m_index)
		{
			cardinalPoints[3] = gPoint;
		} else if (gPoint.m_pos.y > cardinalPoints[1].m_pos.y && gPoint.m_index <= cardinalPoints[1].m_index)
		{
			cardinalPoints[1] = gPoint;
		}
	}
	return cardinalPoints;
}

bool GestureRecognition::tryClokwiseConvolution()
{
	auto cardinalPoint = getCardinalPoint();
	return (cardinalPoint[0].m_index < cardinalPoint[1].m_index &&
		cardinalPoint[1].m_index < cardinalPoint[2].m_index &&
		cardinalPoint[2].m_index < cardinalPoint[3].m_index);
}

bool GestureRecognition::tryCounterClokwiseConvolution()
{
	auto cardinalPoint = getCardinalPoint();
	return (cardinalPoint[0].m_index < cardinalPoint[3].m_index &&
		cardinalPoint[3].m_index < cardinalPoint[2].m_index &&
		cardinalPoint[2].m_index < cardinalPoint[1].m_index);
}

bool GestureRecognition::tryUpDownLeftRight()
{
	auto cardinalPoint = getCardinalPoint();
	return (cardinalPoint[1].m_index < cardinalPoint[2].m_index &&
		cardinalPoint[2].m_index < cardinalPoint[3].m_index &&
		(cardinalPoint[0].m_pos.x > -20.0f || cardinalPoint[0].m_index == 1000));
}

ShootType::Enum GestureRecognition::getGesture()
{
	if (m_gesture.m_finish && m_gesture.m_valid)
	{
		if (m_gesture.m_gPoints[0].m_pos.x < -30.0f)
		{ // Try to recognize 
			if (tryClokwiseConvolution())
			{
				return ShootType::Sinus;
			}
			else if (tryCounterClokwiseConvolution())
			{
				return ShootType::Triangle;
			}
		} 
		if (m_gesture.m_gPoints[0].m_pos.y > 30.0f)
		{
			if (tryUpDownLeftRight())
			{
				return ShootType::Spike;
			}
		}
	}
	return ShootType::None;
}

void GestureRecognition::displayWindow()
{
	if (m_displayWindow)
	{
		if (ImGui::Begin("Gesture Recognition", &m_displayWindow, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Start : %s", (m_gesture.m_start) ? "True" : "False");
			ImGui::Text("Finish : %s", (m_gesture.m_finish) ? "True" : "False");
			ImGui::Text("Valid : %s", (m_gesture.m_valid) ? "True" : "False");
			ImGui::Text("Gesture : %i", getGesture());
			ImGui::NewLine();
			ImGui::Indent();
			for (auto& gpoint : m_gesture.m_gPoints)
			{
				ImGui::Text("%i => x : %f | y : %f", gpoint.m_index, gpoint.m_pos.x, gpoint.m_pos.y);
			}
		}
		ImGui::End();
	}
}

Command* GestureRecognition::getGestureCommand()
{
	auto shootType = getGesture();
	int id;
	auto cmd = CommandMgr::getSingleton()->getCommand("CommandShoot", &id);
	cmd->init(nullptr, (void*)&shootType);
	m_gesture.invalid();
	return cmd;
}
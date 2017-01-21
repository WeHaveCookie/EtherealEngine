#pragma once
#include "Manager/Entity/EntityMgr.h"

class Command;

struct GesturePoint
{
	sf::Vector2f	m_pos;
	uint32_t		m_index;
};

struct GestureHandle
{
	std::vector<GesturePoint>	m_gPoints;
	float						m_timer;
	bool						m_finish;
	bool						m_valid;
	bool						m_start;

	void addGPoint(sf::Vector2f pos)
	{
		GesturePoint gPoint;
		gPoint.m_pos = pos;
		gPoint.m_index = m_gPoints.size();
		m_gPoints.push_back(gPoint);
	}

	void invalid()
	{
		m_gPoints.clear();
		m_timer = 0.0f;
		m_finish = false;
		m_valid = false;
		m_start = false;
	}
};

class GestureRecognition
{
	public:
		GestureRecognition();
		~GestureRecognition();

		void init();
		void process(const float dt);
		ShootType::Enum getGesture();
		bool tryClokwiseConvolution();
		bool tryCounterClokwiseConvolution();
		bool tryUpDownLeftRight();
		void showInfo() { m_displayWindow = !m_displayWindow; }
		Command* getGestureCommand();

	private:
		std::vector<GesturePoint> getCardinalPoint();
		void displayWindow();

		GestureHandle	m_gesture;
		bool			m_displayWindow;
		float			m_updateTimer;
		float			m_timer;
};


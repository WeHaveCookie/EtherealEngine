#pragma once
class Camera
{
	public:
		Camera();
		~Camera();

		void update(const float dt);
		void setTarget(uint32_t entityUID);
		
		inline void reset(sf::FloatRect fRect) { m_view.reset(fRect); };
		inline void setCamera(sf::FloatRect fRect) { m_view.setViewport(fRect); }
		

	private:
		sf::View	m_view;
		uint32_t	m_target;
};


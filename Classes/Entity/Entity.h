#pragma once

struct EntityAnimation {
	std::vector<sf::Sprite> m_animation;
	float					m_timePerFrame;
	uint32_t				m_currentFrame;
	float					m_timeElapsedSinceUdpate = 0.0f;

	sf::Sprite* getCurrentAnimation() 
	{
		if (m_timeElapsedSinceUdpate >= m_timePerFrame)
		{
			nextFrame();
			m_timeElapsedSinceUdpate = 0.0f;
		}
		return &m_animation[m_currentFrame];
	}

	void nextFrame() {
		m_currentFrame++;
		if (m_currentFrame >= m_animation.size())
		{
			m_currentFrame = 0;
		}
	}

	void update(const float dt)
	{
		m_timeElapsedSinceUdpate += dt;
	}

	void reset()
	{
		m_timeElapsedSinceUdpate = 0.0f;
		m_currentFrame = 0;
	}

	~EntityAnimation()
	{
		m_animation.clear();
	}

};

namespace EntityAnimationState {
	enum Enum {
		Right,
		startEnum = Right,
		Left,
		Idle,
		IdleRight,
		IdleLeft,
		endEnum = IdleLeft
	};
}

class Entity
{
	public:
		Entity();
		~Entity();

		virtual void paint();
		virtual void update(const float dt);
		const bool process(const float dt);

		void setName(const char* name) { m_state.m_live.m_name = name; }
		void addAnimation(EntityAnimationState::Enum entAnimState, EntityAnimation entAnim);
		void setSpeed(float speed);
		const char* getName() const { return m_state.m_live.m_name.c_str();}
		const uint32_t getUID() const { return m_uid; }
		void setPosition(sf::Vector2f pos);
		const sf::Vector2f getPosition() const { return m_state.m_live.m_currentPosition; }
		void setAngle(float rad) { m_state.m_live.m_angle = rad * RADTODEG; }
		const float getAngle() const { return m_state.m_live.m_angle * DEGTORAD; }
		const float getMass() const { return m_state.m_live.m_mass; }
		void setState(EntityAnimationState::Enum state);
		const EntityAnimation* getAnimation(EntityAnimationState::Enum state);
		void setNext(Entity* ent) { m_state.m_next = ent; }
		Entity* getNext() const { return m_state.m_next; }
		const sf::FloatRect getGlobalBounds() const;
		const bool isCollidable() const { return m_state.m_live.m_collidable; }

		void setLive(bool b) { m_live = b; }

		void addMotion(sf::Vector2f motion) { m_state.m_live.m_motion += motion; }
		const sf::Vector2f getMotion() const { return m_state.m_live.m_motion; }
		void rollbackXAxis();
		void rollbackYAxis();
		void rollBackAllAxis();

		void retry();
		void editable() { m_editable = !m_editable; };

		const sf::FloatRect getLastPosition() const { return sf::FloatRect(m_state.m_live.m_lastPosition, sf::Vector2f(m_state.m_live.m_width, m_state.m_live.m_height)); }
		const bool isAlive() const { return m_live; }
		const bool asMoved() const { return m_state.m_live.m_lastPosition != m_state.m_live.m_currentPosition; }
		const bool isEditable() const { return m_editable; }

		void release();
		void build(const char* path);
		void load() { m_onLoading = true; };

		void showInfo() { m_displayInfo = !m_displayInfo; }
		void closeInfo() { m_displayInfo = false; }

	protected:
		static uint32_t		newUID;
		const uint32_t		m_uid;

	private:
		void updatePosition();
		void displayInfo();

		bool m_live;
		bool m_loaded;
		bool m_onLoading;
		bool m_displayInfo;
		bool m_editable;

		union State
		{
			struct
			{
				std::string												m_name;
				std::string												m_texturePath;
				std::map<EntityAnimationState::Enum, EntityAnimation>	m_animations;
				float													m_speed;
				EntityAnimationState::Enum								m_currentState;
				sf::Vector2f											m_currentPosition;
				sf::Vector2f											m_lastPosition;
				sf::Vector2f											m_lastMotion;
				sf::Vector2f											m_motion;
				sf::Texture												m_texture;
				sf::Texture												m_errorTexture;
				uint32_t												m_height;
				uint32_t												m_width;
				bool													m_collidable;
				bool													m_animate;
				float													m_angle;
				float													m_mass;

				void clear()
				{
					m_animations.clear();
					for (int i = EntityAnimationState::startEnum; i <= EntityAnimationState::endEnum; i++)
					{
						auto ste = static_cast<EntityAnimationState::Enum>(i);
						sf::Sprite spr;
						spr.setTexture(m_errorTexture);
						spr.setTextureRect(sf::IntRect(0 , 0, m_width, m_height));
						m_animations[ste].m_animation.push_back(spr);
					}
					m_animate = true;
				}
			} m_live;

			Entity*		m_next;

			State()
				:m_live()
			{
				m_next = NULL;
			}

			~State()
			{
			}

		} m_state;
};
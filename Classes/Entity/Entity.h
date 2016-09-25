#pragma once

namespace EntityType
{
	enum Enum
	{
		Anchor = 0,
		Movable,
		All
	};
}

namespace CollisionState
{
	enum Enum
	{
		None = 1 << 0,
		Right = 1 << 1,
		Left = 1 << 2,
		Bottom = 1 << 3,
		Top = 1 << 4,
		All = Right | Left | Bottom | Top
	};
}

namespace EntityOrientation
{
	enum Enum
	{
		Left,
		Right
	};
}
	
struct EntityAnimation {
	std::vector<sf::Sprite> m_animation;
	float					m_timePerFrame;
	uint32_t				m_currentFrame;
	float					m_timeElapsedSinceUdpate = 0.0f;

	const bool activate() const
	{
		return m_animation.size() > 1;
	}

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
		Jump,
		Fall,
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
		void setPosition(Vector2 pos);
		const Vector2 getPosition() const { return m_state.m_live.m_currentPosition; }
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
		void sleep();
		void wakeUp();
		const bool isSleeping() const { return m_state.m_live.m_sleep; }
		const Vector2 getLastImpulse() const { return m_state.m_live.m_lastImpulse; }

		void addMotion(Vector2 motion);
		const Vector2 getMotion() const { return m_state.m_live.m_motion; }
		void rollback(Vector2  impulse);

		void retry();
		void edition() { m_edition = !m_edition; };

		const sf::FloatRect getLastGlobalBounds() { return sf::FloatRect(m_state.m_live.m_lastPosition.sf(), Vector2(m_state.m_live.m_width, m_state.m_live.m_height).sf()); }
		const bool isAlive() const { return m_live; }
		const bool asMoved() { return m_state.m_live.m_lastPosition != m_state.m_live.m_currentPosition; }
		const bool isEdition() const { return m_edition; }
		const bool isFall() const { return m_state.m_live.m_fall; }
		void setFall(bool b) { m_state.m_live.m_fall = b; }
		const EntityType::Enum getType() const { return m_state.m_live.m_type; }

		const CollisionState::Enum getCollisionState() {  return m_state.m_live.m_collisionState; }
		void setCollisionState(CollisionState::Enum state);
		void resetCollisionState();
		const Vector2 getLastMotion() const { return m_state.m_live.m_lastMotion; }
		const bool collisionResolved() const { return m_state.m_live.m_collisionResolved; }
		const bool collisionProcessOngoing() const { return m_state.m_live.m_collisionProceed; }
		void proceedCollision();
		void setCollisionProcess(bool b) { m_state.m_live.m_collisionProceed = b; }
		void setCollisionResolved(bool b) { m_state.m_live.m_collisionResolved = b; }
		void release();
		void build(const char* path);
		void load() { m_onLoading = true; };
		const int getCounterSleeping() const { return m_state.m_live.m_sleepCounter; }


		void showInfo() { m_displayInfo = !m_displayInfo; }
		void closeInfo() { m_displayInfo = false; }

		const float getDistance(Entity* ent);
		const EntityOrientation::Enum getOrientation() const { return m_state.m_live.m_orientation; }

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
		bool m_edition;




		union State
		{
			struct
			{
				std::string												m_name;
				std::string												m_texturePath;
				std::map<EntityAnimationState::Enum, EntityAnimation>	m_animations;
				float													m_speed;
				EntityAnimationState::Enum								m_currentState;
				Vector2													m_currentPosition;
				Vector2													m_lastPosition;
				Vector2													m_lastMotion;
				Vector2													m_motion;
				Vector2													m_lastImpulse;
				Vector2													m_impulse;
				sf::Texture												m_texture;
				sf::Texture												m_errorTexture;
				uint32_t												m_height;
				uint32_t												m_width;
				bool													m_collidable;
				bool													m_animate;
				bool													m_collisionResolved;
				bool													m_collisionProceed;
				bool													m_fall;
				bool													m_sleep;
				int														m_sleepCounter;
				float													m_angle;
				float													m_mass;
				double													m_hSpeed;
				double													m_vSpeed;
				CollisionState::Enum									m_collisionState;
				EntityType::Enum										m_type;
				EntityOrientation::Enum									m_orientation;

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
					m_collisionState = CollisionState::None;
				}
			} m_live;

			Entity*		m_next;

			State()
				:m_live()
			{
				m_live.clear();
				m_next = NULL;
			}

			~State()
			{
			}

		} m_state;
};


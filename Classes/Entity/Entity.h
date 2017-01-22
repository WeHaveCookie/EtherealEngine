#pragma once
#include "Manager/Entity/EntityMgr.h"

namespace EntityAction
{
	enum Enum
	{
		None = 1 << 0,
		Attack = 1 << 1,
		Jump = 1 << 2,
		Fall = 1 << 3,
		Hit = 1 << 4
	};

	static void operator|=(Enum& a1, const Enum& a2)
	{
		a1 = static_cast<Enum>(a1 | a2);
	}

	static void operator&=(Enum& a1, const Enum& a2)
	{
		a1 = static_cast<Enum>(a1 & a2);
	}

	static void operator&=(Enum& a1, const int a2)
	{
		a1 = static_cast<Enum>(a1 & a2);
	}

	static void operator^=(Enum& a1, const Enum& a2)
	{
		a1 = static_cast<Enum>(a1 ^ a2);
	}
}

namespace EntityType
{
	enum Enum
	{
		Anchor = 0,
		Movable,
		Projectile,
		Button,
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
	
class Command;

struct AnimationHandler {
	sf::Sprite				m_sprite;
	std::vector<Command*>	m_commands;

	void execute();
};

struct EntityAnimation {
	std::vector<AnimationHandler>	m_animation;
	float							m_timePerFrame;
	uint32_t						m_currentFrame;
	float							m_timeElapsedSinceUdpate = 0.0f;
	bool							m_finish;
	bool							m_unstopable;
	bool							m_loop;
	uint32_t						m_loopStart;
	uint32_t						m_loopEnd;

	const bool isFinished() const
	{
		if (!m_unstopable || (m_unstopable && m_finish))
		{
			return true;
		}
		else if (m_unstopable && !m_finish)
		{
			return false;
		}
		return false;
	}

	const bool activate() const
	{
		return m_animation.size() > 1;
	}

	sf::Sprite* getCurrentAnimation() 
	{
		return &m_animation[m_currentFrame].m_sprite;
	}

	void nextFrame() {
		if (m_currentFrame == 0)
		{
			m_animation[m_currentFrame].execute();
		}
		m_currentFrame++;
		if (m_loop)
		{
			if (m_currentFrame > m_loopEnd)
			{
				m_currentFrame = m_loopStart;
				m_finish = true;
				return;
			}
		}
		if (m_currentFrame >= m_animation.size())
		{
			m_finish = true;
			m_currentFrame = 0;
		}
		else
		{
			m_animation[m_currentFrame].execute();
		}
	}

	void update(const float dt)
	{
		m_timeElapsedSinceUdpate += dt;
		if (m_timePerFrame > 0.0f)
		{
			if (m_timeElapsedSinceUdpate >= m_timePerFrame)
			{
				nextFrame();
				m_timeElapsedSinceUdpate -= m_timePerFrame;
			}
		}
	}

	void reset()
	{
		m_finish = false;
		m_timeElapsedSinceUdpate = 0.0f;
		m_currentFrame = 0;
	}

	const uint32_t getIndexOfAnim() const
	{
		return m_currentFrame;
	}

	const uint32_t getSizeOfAnim() const 
	{
		return m_animation.size();
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
		Jump,
		startAction = Jump,
		JumpRight,
		JumpLeft,
		Fall,
		FallRight,
		FallLeft,
		Attack,
		AttackRight,
		AttackLeft,
		Hit,
		HitRight,
		HitLeft,
		endAction = HitLeft,
		Dead,
		endEnum = endAction
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

		//void setName(const char* name) { m_state.m_live.m_name = name; }
		void addAnimation(EntityAnimationState::Enum entAnimState, EntityAnimation entAnim);
		void setSpeed(float speed);
		const char* getName() const { return m_state.m_live.m_name.c_str();}
		const uint32_t getUID() const { return m_uid; }
		void setPosition(Vector2 pos);
		const Vector2 getPosition() const { return m_state.m_live.m_currentPosition; }
		void setAngle(float rad) { m_state.m_live.m_angle = rad * RADTODEG; }
		const float getAngle() const { return m_state.m_live.m_angle * DEGTORAD; }
		void setState(EntityAnimationState::Enum state);
		EntityAnimation* getAnimation(EntityAnimationState::Enum state);
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

		const sf::FloatRect getLastGlobalBounds();
		const bool isAlive() const { return m_live; }
		const bool asMoved() { return m_state.m_live.m_lastPosition != m_state.m_live.m_currentPosition; }
		const bool isEdition() const { return m_edition; }
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
		void move(Vector2 motion);
		void jump();
		void setAction(EntityAction::Enum action, bool b);
		const bool isInAction(EntityAction::Enum action) const;
		void Entity::setJump(bool b) { setAction(EntityAction::Jump, b); }
		void Entity::setFall(bool b) { setAction(EntityAction::Fall, b); }
		const bool isJump() const { return isInAction(EntityAction::Jump); }
		const bool isFall() const { return isInAction(EntityAction::Fall); }
		const bool isAttack() const { return isInAction(EntityAction::Attack); }
		const bool isHit() const { return isInAction(EntityAction::Hit); }
		const bool isDead() const { return m_state.m_live.m_currentState == EntityAnimationState::Dead; }
		void attack() { m_state.m_live.m_action = EntityAction::Attack; }
		void hit() { setAction(EntityAction::Hit, true); }


		void showInfo() { m_displayInfo = !m_displayInfo; }
		void closeInfo() { m_displayInfo = false; }

		const float getDistance(Entity* ent);
		const EntityOrientation::Enum getOrientation() const { return m_state.m_live.m_orientation; }

		const float getVX() const;
		const float getVGrav() const { return m_state.m_live.m_vGrav; }
		const float getVJump() const { return m_state.m_live.m_vJump; }
		const float getVY() const { return m_state.m_live.m_vy; }
		const float getVMax() const { return m_state.m_live.m_vMax; }
		const int getBackgroundLevel() const { return m_state.m_live.m_backgroundLevel; }
		void setVY(float v) { m_state.m_live.m_vy = std::min(v, m_state.m_live.m_vMax); }
		void setBackgroundLevel(uint32_t level) { m_state.m_live.m_backgroundLevel = level; }
		void showImGuiWindow();
		void setTarget(Vector2 pos) { m_state.m_live.m_targetPos = pos; }
		const bool isProjectile() const { return m_state.m_live.m_type == EntityType::Projectile; }
		void onClick();
		void setCommandOnClick(Command* cmd) { m_state.m_live.m_commandOnClick = cmd; }
		void setType(EntityType::Enum type) { m_state.m_live.m_type = type; }
		const EntityType::Enum getTypeByName(const char* name) const;
		const float getWidth() const { return m_state.m_live.m_width * m_state.m_live.m_scale.x; }
		const float getHeight() const { return m_state.m_live.m_height * m_state.m_live.m_scale.y; }
		const EntityAnimationState::Enum getState() const { return m_state.m_live.m_currentState; }
		void setAnimate(bool b) { m_state.m_live.m_animate = b; }
		const ShootType::Enum getElement() const { return m_state.m_live.m_element; }

	protected:
		static uint32_t		newUID;
		const uint32_t		m_uid;

	private:
		void updatePosition();
		
		void moveToTarget(const float dt);

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
				bool													m_sleep;
				float													m_sleepTime;
				float													m_angle;
				CollisionState::Enum									m_collisionState;
				EntityType::Enum										m_type;
				EntityOrientation::Enum									m_orientation;
				float													m_vx;
				float													m_vy;
				float													m_vGrav;
				float													m_vJump;
				float													m_vMax;
				Vector2													m_scale;
				EntityAction::Enum										m_action;
				uint32_t												m_backgroundLevel; //  0 => foreground | Higher => background
				std::string												m_targetName;
				Vector2													m_targetPos;
				float													m_maxSpeed;
				ShootType::Enum											m_element;
				Command*												m_commandOnClick;

				void clear()
				{
					m_animations.clear();
					m_action = EntityAction::Fall;
					for (int i = EntityAnimationState::startEnum; i <= EntityAnimationState::endEnum; i++)
					{
						AnimationHandler animHandler;
						auto ste = static_cast<EntityAnimationState::Enum>(i);
						sf::Sprite spr;
						spr.setTexture(m_errorTexture);
						spr.setTextureRect(sf::IntRect(0 , 0, m_width, m_height));
						spr.setScale(m_scale.sf());
						animHandler.m_sprite = spr;
						m_animations[ste].m_animation.push_back(animHandler);
					}
					m_animate = true;
					m_collisionState = CollisionState::None;
					m_currentState = EntityAnimationState::Idle;
					m_element = ShootType::None;
					m_commandOnClick = NULL;
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


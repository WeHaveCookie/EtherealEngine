#include "stdafx.h"
#include "Entity.h"

#include "EtherealEngineManagers.h"
#include "Manager/Render/RenderMgr.h"
#include "Manager/Physic/PhysicMgr.h"
#include "Manager/Input/InputMgr.h"
#include "Manager/File/FileMgr.h"
#include "../../External/rapidjson/document.h"
#include "Utils/Random.h"
#include "Utils/VectorUtils.h"
#include "Actions/Command.h"
#include "Manager/Action/CommandMgr.h"
#include "Utils/jsonUtils.h"
#include "Manager/Game/GameMgr.h"
#include "Manager/Sound/SoundMgr.h"

#define ERROR_TEXTURE "Data/Texture/error.png"

float clamp(float value, float a, float b)
{
	float res = value;
	if (value > a && value < b)
	{
		res = 0.0f;
	}
	return res;
}

void AnimationHandler::execute()
{
	for (auto& command : m_commands)
	{
		command->execute();
	}
}

std::map<std::string, EntityType::Enum> stringToEntityType =
{
	{"Anchor", EntityType::Anchor },
	{"Movable", EntityType::Movable },
	{"Projectile", EntityType::Projectile},
	{ "Button", EntityType::Button }
};

std::vector<const char*> entityTypeToString =
{
	"Anchor",
	"Movable",
	"Projectile",
	"Button"
};

std::map<std::string, EntityAnimationState::Enum> stringToEntityAnimationState =
{
	{ "RIGHT", EntityAnimationState::Right },
	{ "LEFT", EntityAnimationState::Left },
	{ "IDLE", EntityAnimationState::Idle },
	{ "IDLE_RIGHT", EntityAnimationState::IdleRight },
	{ "IDLE_LEFT", EntityAnimationState::IdleLeft },
	{ "JUMP", EntityAnimationState::Jump },
	{ "JUMP_RIGHT", EntityAnimationState::JumpRight },
	{ "JUMP_LEFT", EntityAnimationState::JumpLeft },
	{ "FALL", EntityAnimationState::Fall },
	{ "FALL_RIGHT", EntityAnimationState::FallRight },
	{ "FALL_LEFT", EntityAnimationState::FallLeft },
	{ "ATTACK", EntityAnimationState::Attack },
	{ "ATTACK_RIGHT", EntityAnimationState::AttackRight },
	{ "ATTACK_LEFT", EntityAnimationState::AttackLeft },
	{ "HIT", EntityAnimationState::Hit },
	{ "HIT_RIGHT", EntityAnimationState::HitRight },
	{ "HIT_LEFT", EntityAnimationState::HitLeft },
	{ "DEAD", EntityAnimationState::Dead},
};

std::vector<const char*> entityAnimationStateToString = 
{
	"RIGHT",
	"LEFT",
	"IDLE",
	"IDLE_RIGHT",
	"IDLE_LEFT",
	"JUMP",
	"JUMP_RIGHT",
	"JUMP_LEFT",
	"FALL",
	"FALL_RIGHT",
	"FALL_LEFT",
	"ATTACK",
	"ATTACK_RIGHT",
	"ATTACK_LEFT",
	"HIT",
	"HIT_RIGHT",
	"HIT_LEFT",
	"DEAD"
};

Entity::Entity()
	:m_uid(newUID++)
{
	setLive(false);
	m_loaded = false;
	m_onLoading = false;
	m_displayInfo = false;
	m_edition = false;
	m_state.m_live.m_errorTexture.loadFromFile(ERROR_TEXTURE);
	m_state.m_live.m_errorTexture.setRepeated(true);
}

Entity::~Entity()
{

}

void Entity::paint()
{
	if (m_live)
	{
		auto renderMgr = RENDER_MGR;
		auto rdrWin = renderMgr->getMainRenderWindow();
		sf::Sprite* currentAnim = m_state.m_live.m_animations[m_state.m_live.m_currentState].getCurrentAnimation();
		rdrWin->draw(*currentAnim);
	}
}

void Entity::moveToTarget(const float dt)
{
	if (getState() != EntityAnimationState::Dead && getType() != EntityType::Button)
	{
		if (m_state.m_live.m_targetName != "")
		{
			auto target = EntityMgr::getSingleton()->getEntity(m_state.m_live.m_targetName);
			if (target != nullptr)
			{
				auto pos = Vector2(getPosition().x + getGlobalBounds().width / 2.0, getPosition().y + getGlobalBounds().height / 2.0);
				auto targetPos = Vector2(target->getPosition().x + target->getGlobalBounds().width / 2.0, target->getPosition().y + target->getGlobalBounds().height / 2.0);
				Vector2 vect = targetPos - pos;
				Vector2 DesiredVelocity = vect.norm() * m_state.m_live.m_maxSpeed;

				Vector2 lastVelocity = DesiredVelocity * dt * 10 * m_state.m_live.m_speed;
				if (EntityMgr::getSingleton()->playerIsDead())
				{
					lastVelocity = lastVelocity % 180 * RADTODEG;
					lastVelocity /= 9;
				}
				move(lastVelocity);
				return;
			}
		}

		if (m_state.m_live.m_targetPos != getPosition())
		{
			auto pos = Vector2(getPosition().x + getGlobalBounds().width / 2.0, getPosition().y + getGlobalBounds().height / 2.0);
			Vector2 vect = m_state.m_live.m_targetPos - pos;
			Vector2 DesiredVelocity = vect.norm() * m_state.m_live.m_maxSpeed;

			Vector2 lastVelocity = DesiredVelocity * dt * 10 * m_state.m_live.m_speed;
			if (EntityMgr::getSingleton()->playerIsDead())
			{
				lastVelocity = lastVelocity % 180 * RADTODEG;
				lastVelocity /= 9;
			}
			move(lastVelocity);
		}
	}
}

void Entity::update(const float dt)
{
	moveToTarget(dt);
	if (!m_state.m_live.m_collidable && isEdition())
	{
		move(getMotion());
	}
	if (!m_state.m_live.m_sleep && m_state.m_live.m_collidable)
	{
		m_state.m_live.m_sleepTime += dt;
	}
	Vector2 motion = m_state.m_live.m_currentPosition - m_state.m_live.m_lastPosition;
// 	if (m_state.m_live.m_type != EntityType::Anchor && !m_edition && m_state.m_live.m_animate)
// 	{
// 		if (dt > 0.0f)
// 		{
// 			if (isInAction(EntityAction::Attack))
// 			{
// 				if ((m_state.m_live.m_currentState == EntityAnimationState::AttackLeft || m_state.m_live.m_currentState == EntityAnimationState::AttackRight) && m_state.m_live.m_animations[m_state.m_live.m_currentState].m_finish)
// 				{
// 					setAction(EntityAction::Attack, false);
// 					setState(EntityAnimationState::Idle);
// 				}
// 				else
// 				{
// 					setState(EntityAnimationState::Attack);
// 				}
// 			}
// 			else if (isInAction(EntityAction::Jump))
// 			{
// 				if ((m_state.m_live.m_currentState == EntityAnimationState::JumpLeft || m_state.m_live.m_currentState == EntityAnimationState::JumpRight) && m_state.m_live.m_animations[m_state.m_live.m_currentState].m_finish)
// 				{
// 					if (m_state.m_live.m_vy > 0.0f)
// 					{
// 						setAction(EntityAction::Jump, false);
// 						setState(EntityAnimationState::Fall);
// 					}
// 					else
// 					{
// 
// 						m_state.m_live.m_animations[m_state.m_live.m_currentState].m_currentFrame = 2;
// 					}
// 					
// 				}
// 				else
// 				{
// 					setState(EntityAnimationState::Jump);
// 				}
// 			}
// 			else if (isInAction(EntityAction::Hit))
// 			{
// 				setState(EntityAnimationState::Hit);
// 			} 
// 			else if (motion.x < 0.0f)
// 			{
// 				if (!isFall())
// 				{
// 					m_state.m_live.m_orientation = EntityOrientation::Left;
// 					setState(EntityAnimationState::Left);
// 				}
// 				else
// 				{
// 					setState(EntityAnimationState::FallLeft);
// 				}
// 			}
// 			else if (motion.x > 0.0f)
// 			{
// 				if(!isFall())
// 				{
// 					m_state.m_live.m_orientation = EntityOrientation::Right;
// 					setState(EntityAnimationState::Right);
// 				}
// 				else
// 				{
// 					setState(EntityAnimationState::FallRight);
// 				}
// 			}
// 			else if (m_state.m_live.m_vy < 0.0f)
// 			{
// 				setState(EntityAnimationState::Jump);
// 			}
// 			else if (m_state.m_live.m_vy > 0.0f)
// 			{
// 				if (isFall())
// 				{
// 					setState(EntityAnimationState::Fall);
// 				} else
// 				{
// 					setState(EntityAnimationState::Idle);
// 				}
// 			}
// 			else
// 			{
// 				setState(EntityAnimationState::Idle);
// 			}
// 		}
// 	}

	if (m_state.m_live.m_animate)
	{
		m_state.m_live.m_animations[m_state.m_live.m_currentState].update(dt);
	}
	EntityAnimation* anim = &m_state.m_live.m_animations[m_state.m_live.m_currentState];
	sf::Sprite* currentAnim = anim->getCurrentAnimation();
	currentAnim->setPosition(m_state.m_live.m_currentPosition.sf().x + getGlobalBounds().width / 2.0f, m_state.m_live.m_currentPosition.sf().y + getGlobalBounds().height / 2.0f);
	currentAnim->setOrigin(sf::Vector2f(getGlobalBounds().width / 2.0f, getGlobalBounds().height / 2.0f));
	auto mumu = currentAnim->getOrigin();
	currentAnim->setRotation(m_state.m_live.m_angle);
	if (getState() == EntityAnimationState::Dead && anim->getIndexOfAnim() >= anim->getSizeOfAnim() - 1)
	{
		m_live = false;
	}
}

const bool Entity::process(const float dt)
{
	if (m_live)
	{
		update(dt);
	}
	return m_live;
}

void Entity::addAnimation(EntityAnimationState::Enum entAnimState, EntityAnimation entAnim)
{
	m_state.m_live.m_animations[entAnimState] = entAnim;
}

void Entity::setSpeed(float speed)
{
	m_state.m_live.m_speed = std::max(speed, 0.0f);
}

void Entity::setState(EntityAnimationState::Enum state)
{
	if (m_state.m_live.m_currentState == EntityAnimationState::Dead)
	{
		return;
	}
	if (m_state.m_live.m_currentState != state)
	{
		switch (state)
		{
		case EntityAnimationState::Right:
		case EntityAnimationState::IdleRight:
		case EntityAnimationState::IdleLeft:
		case EntityAnimationState::Left:
		case EntityAnimationState::JumpRight:
		case EntityAnimationState::JumpLeft:
		case EntityAnimationState::FallRight:
		case EntityAnimationState::FallLeft:
		case EntityAnimationState::AttackRight:
		case EntityAnimationState::AttackLeft:
		case EntityAnimationState::HitRight:
		case EntityAnimationState::HitLeft:
		case EntityAnimationState::Dead:
			// Intentional fall through
			if (m_state.m_live.m_animations[m_state.m_live.m_currentState].isFinished())
			{
				m_state.m_live.m_animations[m_state.m_live.m_currentState].reset();
				m_state.m_live.m_currentState = state;
			}
			break;
		case EntityAnimationState::Idle:
			if (m_state.m_live.m_orientation == EntityOrientation::Left)
			{
				if (m_state.m_live.m_animations[EntityAnimationState::IdleLeft].activate())
				{
					setState(EntityAnimationState::IdleLeft);
				}
				else
				{
					m_state.m_live.m_currentState = state;
				}
			}
			else
			{
				if (m_state.m_live.m_animations[EntityAnimationState::IdleRight].activate())
				{
					setState(EntityAnimationState::IdleRight);
				}
				else
				{
					m_state.m_live.m_currentState = state;
				}
			}
			break;
		case EntityAnimationState::Jump:
			if (m_state.m_live.m_orientation == EntityOrientation::Left)
			{
				if (m_state.m_live.m_animations[EntityAnimationState::JumpLeft].activate())
				{
					setState(EntityAnimationState::JumpLeft);
				}
				else
				{
					m_state.m_live.m_currentState = state;
				}
			}
			else
			{
				if (m_state.m_live.m_animations[EntityAnimationState::JumpRight].activate())
				{
					setState(EntityAnimationState::JumpRight);
				}
				else
				{
					m_state.m_live.m_currentState = state;
				}
			}
			break;
		case EntityAnimationState::Fall:
			if (m_state.m_live.m_orientation == EntityOrientation::Left)
			{
				if (m_state.m_live.m_animations[EntityAnimationState::FallLeft].activate())
				{
					setState(EntityAnimationState::FallLeft);
				}
				else
				{
					m_state.m_live.m_currentState = state;
				}
			}
			else
			{
				if (m_state.m_live.m_animations[EntityAnimationState::FallRight].activate())
				{
					setState(EntityAnimationState::FallRight);
				}
				else
				{
					m_state.m_live.m_currentState = state;
				}
			}
			break;
		case EntityAnimationState::Attack:
			if (m_state.m_live.m_orientation == EntityOrientation::Left)
			{
				if (m_state.m_live.m_animations[EntityAnimationState::AttackLeft].activate())
				{
					setState(EntityAnimationState::AttackLeft);
				}
				else
				{
					m_state.m_live.m_currentState = state;
				}
			}
			else
			{
				if (m_state.m_live.m_animations[EntityAnimationState::AttackRight].activate())
				{
					setState(EntityAnimationState::AttackRight);
				}
				else
				{
					m_state.m_live.m_currentState = state;
				}
			}
			break;
			break;
		case EntityAnimationState::Hit:
			if (m_state.m_live.m_orientation == EntityOrientation::Left)
			{
				if (m_state.m_live.m_animations[EntityAnimationState::HitLeft].activate())
				{
					setState(EntityAnimationState::HitLeft);
				}
				else
				{
					m_state.m_live.m_currentState = state;
				}
			}
			else
			{
				if (m_state.m_live.m_animations[EntityAnimationState::HitRight].activate())
				{
					setState(EntityAnimationState::HitRight);
				}
				else
				{
					m_state.m_live.m_currentState = state;
				}
			}
			break;
		default:
			break;
		}
	}
}

EntityAnimation* Entity::getAnimation(EntityAnimationState::Enum state)
{
	if (m_state.m_live.m_animations[state].m_animation.size() != 0)
	{
		return &m_state.m_live.m_animations[state];
	}
	else
	{
		return NULL;
	}
}

const sf::FloatRect Entity::getLastGlobalBounds()
{ 
	return sf::FloatRect(m_state.m_live.m_lastPosition.sf(), Vector2(m_state.m_live.m_width * m_state.m_live.m_scale.x, m_state.m_live.m_height * m_state.m_live.m_scale.y).sf());
}

const sf::FloatRect Entity::getGlobalBounds() const
{
	return sf::FloatRect(m_state.m_live.m_currentPosition.x, 
							m_state.m_live.m_currentPosition.y, 
							m_state.m_live.m_width * m_state.m_live.m_scale.x, 
							m_state.m_live.m_height * m_state.m_live.m_scale.y);
}

void Entity::sleep() 
{ 
	if (!m_state.m_live.m_sleep)
	{
		if (m_state.m_live.m_sleepTime > 5.0f)
		{
			if (!m_edition)
			{
				m_state.m_live.m_sleep = true;
			}
		}
	}
}

void Entity::wakeUp()
{
	if (m_state.m_live.m_collidable)
	{
		m_state.m_live.m_sleepTime = 0;
		if (m_state.m_live.m_sleep)
		{
			m_state.m_live.m_collisionResolved = false;
			m_state.m_live.m_sleep = false;
			m_state.m_live.m_lastImpulse = Vector2(0.0f, 0.0f);
		}
	}
}

void Entity::addMotion(Vector2 motion)
{
	wakeUp();
	m_state.m_live.m_collisionResolved = false;
	m_state.m_live.m_motion += motion;
}

void Entity::rollback(Vector2 impulse)
{ 
	m_state.m_live.m_currentPosition += impulse;
	m_state.m_live.m_motion += impulse;
}

void Entity::retry()
{
	auto lastMotion = m_state.m_live.m_lastMotion;
	auto lastPosition = m_state.m_live.m_lastPosition;
	update(0.0f);
	m_state.m_live.m_lastMotion = lastMotion;
	m_state.m_live.m_lastPosition = lastPosition;
}

void Entity::setCollisionState(CollisionState::Enum state)
{
	m_state.m_live.m_collisionState = (CollisionState::Enum)(m_state.m_live.m_collisionState | state);
}

void Entity::resetCollisionState()
{ 
	m_state.m_live.m_motion = Vector2();
	m_state.m_live.m_collisionState = CollisionState::None; 
	m_state.m_live.m_collisionResolved = true; 
	m_state.m_live.m_collisionProceed = false; 
	if (m_state.m_live.m_sleep)
	{
		m_state.m_live.m_sleepTime = 0;
	}
}

void replaceJsonByPng(char* dest, const char* source)
{
	strcpy(dest, source);
	char* ext = strstr(dest, ".json");
	strncpy(ext, ".png", 5);
}

void replaceName(char* dest, const char* name, int sizeName)
{
	int i = 0;
	int lastSlashPos = 0;
	while (dest[i] != '\0')
	{
		if (dest[i] == '/' || dest[i] == '\\')
		{
			lastSlashPos = i + 1;
		}
		i++;
	}
	for (int i = 0; i < sizeName; i++)
	{
		dest[lastSlashPos + i] = name[i];
	}
	dest[lastSlashPos + sizeName] = '\0';
}

void Entity::build(const char* path)
{
	char* json;
	int jsonSize;

	FileMgr::ReadFile(path, (void**)&json, &jsonSize);

	json[jsonSize] = 0;

	rapidjson::Document document;
	document.Parse((char*)json);
	auto error = document.HasParseError();
	auto object = document.IsObject();
	if (error || !object)
	{
		return;
	}
	assert(!error);
	assert(object);

	m_state.m_live.clear();
	m_state.m_next = NULL;

	uint32_t* widthPtr = &m_state.m_live.m_width;
	uint32_t* heightPtr = &m_state.m_live.m_height;
	checkAndAffect(&document, "Width", ValueType::Int, (void**)&widthPtr);
	checkAndAffect(&document, "Height", ValueType::Int, (void**)&heightPtr);

	float* maxSpeedPtr = &m_state.m_live.m_maxSpeed;
	float defaultMaxSpeed = 1.0;
	checkAndAffect(&document, "MaxSpeed", ValueType::Float, (void**)&maxSpeedPtr, &defaultMaxSpeed);

	bool member;
	sf::Sprite spr;

	auto scalePtr = &m_state.m_live.m_scale;
	Vector2 defaultScale(1.0f, 1.0f);
	checkAndAffect(&document, "Scale", ValueType::Vector2, (void**)&scalePtr, (void*)&defaultScale);
	spr.setScale(m_state.m_live.m_scale.sf());

	if (document.HasMember("Layout"))
	{
		if (strcmp(document["Layout"].GetString(), "Center") == 0)
		{
			auto rdrSize = GameMgr::getSingleton()->getMainRenderWindow()->getSize();
			auto pos = Vector2((rdrSize.x / 2.0f) - (getWidth() / 2.0f), (rdrSize.y / 2.0f) - (getHeight() / 2.0f));
			setTarget(pos);
			setPosition(pos);
		}
	}

	if (document.HasMember("Element"))
	{
		m_state.m_live.m_element = ShootType::GetType(document["Element"].GetString());
	}

	if (document.HasMember("Sound"))
	{
		const rapidjson::Value& sound = document["Sound"];
		int soundID = randIntBorned(0, sound.GetArray().Size());
		SoundMgr::getSingleton()->addSound(sound[soundID].GetString());
	}

	if (document.HasMember("Target"))
	{
		m_state.m_live.m_targetName = document["Target"].GetString();
	}
	else
	{
		m_state.m_live.m_targetName = "";
	}

	char entPath[128];

	if (document.HasMember("Textures"))
	{
		const rapidjson::Value& textures = document["Textures"];
		int textureID = randIntBorned(0, textures.GetArray().Size());
		strcpy(entPath, path);
		replaceName(entPath, textures[textureID].GetString(), textures[textureID].GetStringLength());
	} else
	{
		replaceJsonByPng(entPath, path);
	}

	m_state.m_live.m_texturePath = entPath;

	auto load = m_state.m_live.m_texture.loadFromFile(entPath);
	assert(load);
	m_state.m_live.m_texture.setSmooth(false);
	spr.setTexture(m_state.m_live.m_texture);

	auto namePtr = &m_state.m_live.m_name;
	checkAndAffect(&document, "Name", ValueType::String, (void**)&namePtr, "NoName", sizeof("NoName"));

	auto speedPtr = &m_state.m_live.m_speed;
	float defaultSpeed = 1.0f;
	checkAndAffect(&document, "Speed", ValueType::Float, (void**)&speedPtr, (void*)&defaultSpeed);
	setSpeed(m_state.m_live.m_speed);

	auto animatedPtr = &m_state.m_live.m_animate;
	bool defaultAnimated = false;
	checkAndAffect(&document, "Animated", ValueType::Bool, (void**)&animatedPtr, (void*)&defaultAnimated);
	
	auto backgroundLevelPtr = &m_state.m_live.m_backgroundLevel;
	uint32_t defaultBackgroundLevel = 0;
	checkAndAffect(&document, "BackgroundLevel", ValueType::Uint, (void**)&backgroundLevelPtr, (void*)&defaultBackgroundLevel);

	m_state.m_live.m_orientation = EntityOrientation::Right;
	if (document.HasMember("Orientation"))
	{
		auto orientation = document["Orientation"].GetString();
		if (orientation == "RIGHT")
		{
			m_state.m_live.m_orientation = EntityOrientation::Right;
		}
		else
		{
			m_state.m_live.m_orientation = EntityOrientation::Left;
		}
	}

	auto collidablePtr = &m_state.m_live.m_collidable;
	bool defaultCollidable = true;
	checkAndAffect(&document, "Collidable", ValueType::Bool, (void**)&collidablePtr, (void*)&defaultCollidable);

	if (m_state.m_live.m_collidable)
	{
		PhysicMgr::getSingleton()->registerEntity(this);
	}

	member = document.HasMember("Animations");
	assert(member);
	const rapidjson::Value& animations = document["Animations"];

	for (auto& animation : animations.GetArray())
	{
		EntityAnimation anim;
		member = animation.HasMember("State");
		assert(member);

		member = animation.HasMember("Frame");
		assert(member);
		uint32_t nbrFrame = animation["Frame"].GetUint();

		member = animation.HasMember("Time");
		assert(member);
		anim.m_timePerFrame = animation["Time"].GetFloat();

		member = animation.HasMember("Line");
		assert(member);
		uint32_t line = animation["Line"].GetUint() - 1;

		anim.m_unstopable = false;
		if (animation.HasMember("Unstopable"))
		{
			bool unstopable = animation["Unstopable"].GetBool();
			anim.m_unstopable = unstopable;
		}

		bool reverse = false;
		if (animation.HasMember("Reverse"))
		{
			reverse = animation["Reverse"].GetBool();
		}

		anim.m_loop = false;
		anim.m_loopStart = 0;
		anim.m_loopEnd = nbrFrame - 1;
		if (animation.HasMember("LoopStart"))
		{
			anim.m_loop = true;
			anim.m_loopStart = animation["LoopStart"].GetInt() - 1;
		}
		if (animation.HasMember("LoopEnd"))
		{
			anim.m_loop = true;
			anim.m_loopEnd = animation["LoopEnd"].GetInt() - 1;
		}

		if (reverse)
		{
			for (int column = nbrFrame - 1; column >= 0; column--)
			{
				AnimationHandler animHandler;
				spr.setTextureRect(sf::IntRect(m_state.m_live.m_width * column, m_state.m_live.m_height * line, m_state.m_live.m_width, m_state.m_live.m_height));
				spr.setScale(m_state.m_live.m_scale.sf());
				animHandler.m_sprite = spr;
				anim.m_animation.push_back(animHandler);
			}
		}
		else
		{
			for (uint32_t column = 0; column < nbrFrame; column++)
			{
				AnimationHandler animHandler;
				spr.setTextureRect(sf::IntRect(m_state.m_live.m_width * column, m_state.m_live.m_height * line, m_state.m_live.m_width, m_state.m_live.m_height));
				spr.setScale(m_state.m_live.m_scale.sf());
				animHandler.m_sprite = spr;
				anim.m_animation.push_back(animHandler);
			}
		}
		

		if (animation.HasMember("Commands"))
		{
			const rapidjson::Value& commands = animation["Commands"];

			for (auto& command : commands.GetArray())
			{
				if (command.HasMember("Command"))
				{
					int commandID;
					std::string commandName = "Command";
					commandName += command["Command"].GetString();
					Command* cmd = CommandMgr::getSingleton()->getCommand(commandName.c_str(), &commandID);
					if (command.HasMember("Value"))
					{
						float value = command["Value"].GetFloat();
						cmd->init(this, (void*)&value);
					}
					else if (command.HasMember("Path"))
					{
						const rapidjson::Value& paths = command["Path"];
						int pathID = randIntBorned(0, paths.GetArray().Size());
						auto path = paths[pathID].GetString();
						cmd->init(this, (void*)path);
					} 
					else
					{
						cmd->init(this);
					}

					int start = 0;
					int end = nbrFrame;
					if (command.HasMember("StartFrame"))
					{
						start = command["StartFrame"].GetInt() - 1;
					}

					if (command.HasMember("EndFrame"))
					{
						end = command["EndFrame"].GetInt() - 1;
					}

					for (int frame = start; frame <= end; frame++)
					{
						anim.m_animation[frame].m_commands.push_back(cmd);
					}
				}
			}
		}
		anim.m_currentFrame = 0;
		addAnimation(stringToEntityAnimationState[animation["State"].GetString()], anim);
	}

	if (document.HasMember("Position"))
	{
		const rapidjson::Value& posArray = document["Position"];
		assert(posArray.Size() == 2);
		Vector2 pos;
		
		//pos.x = randIntBorned(0.0f, posArray[0].GetFloat());
		pos.x = posArray[0].GetFloat();
		pos.y = posArray[1].GetFloat();
		setTarget(pos);
		setPosition(pos);
	}

	if (document.HasMember("Type"))
	{
		m_state.m_live.m_type = stringToEntityType[document["Type"].GetString()];
	}
	else
	{
		m_state.m_live.m_type = EntityType::Anchor;
	}

	m_state.m_live.m_vx = 0.0f;
	m_state.m_live.m_vGrav = 0.40f;
	m_state.m_live.m_vJump = -15.0f;
	m_state.m_live.m_vy = 0.0f;
	m_state.m_live.m_vMax = 15.0f;
	if (document.HasMember("JumpParam"))
	{
		const rapidjson::Value& jumpParam = document["JumpParam"];

		for (auto& v : jumpParam.GetArray())
		{
			if (v.HasMember("vx"))
			{
				m_state.m_live.m_vx = v["vx"].GetFloat();
			}

			if (v.HasMember("vGrav"))
			{
				m_state.m_live.m_vGrav = v["vGrav"].GetFloat();
			}

			if (v.HasMember("vJump"))
			{
				m_state.m_live.m_vJump = v["vJump"].GetFloat();
			}

			if (v.HasMember("vy"))
			{
				m_state.m_live.m_vy = v["vy"].GetFloat();
			}

			if (v.HasMember("vMax"))
			{
				m_state.m_live.m_vMax = v["vMax"].GetFloat();
			}
		}
	}


	m_state.m_live.m_angle = 0.0f;
	member = document.HasMember("State");
	assert(member);
	setState(stringToEntityAnimationState[document["State"].GetString()]);
	setLive(true);
	m_onLoading = false;
	m_loaded = true;
	m_state.m_live.m_sleep = !m_state.m_live.m_collidable;
	FileMgr::CloseFile(json);
}

void Entity::updatePosition()
{
	if (!m_state.m_live.m_sleep)
	{
		if (m_state.m_live.m_collisionProceed)
		{
			m_state.m_live.m_lastPosition = m_state.m_live.m_currentPosition;
			m_state.m_live.m_lastMotion = m_state.m_live.m_motion - m_state.m_live.m_impulse;
			clampVector(&m_state.m_live.m_motion);
			clampVector(&m_state.m_live.m_lastMotion);

			m_state.m_live.m_currentPosition += m_state.m_live.m_motion;
			m_state.m_live.m_motion -= m_state.m_live.m_lastMotion + m_state.m_live.m_impulse;

			auto testSlepping = m_state.m_live.m_lastMotion + m_state.m_live.m_lastImpulse;
			auto nullVec = Vector2(0.0f, 0.0f);
			clampVector(&testSlepping);

			if (testSlepping == nullVec || (m_state.m_live.m_lastMotion == nullVec) || m_state.m_live.m_lastPosition == m_state.m_live.m_currentPosition)
			{
				if (++m_state.m_live.m_sleepTime > 5.0f)
				{
					sleep();
				}
			}
			else
			{
				m_state.m_live.m_sleepTime = 0;
			}
		}
		else
		{
			m_state.m_live.m_lastPosition = m_state.m_live.m_currentPosition;
			m_state.m_live.m_lastMotion = m_state.m_live.m_motion;
			clampVector(&m_state.m_live.m_motion);
			clampVector(&m_state.m_live.m_lastMotion);

			m_state.m_live.m_currentPosition += m_state.m_live.m_motion;
			m_state.m_live.m_motion -= m_state.m_live.m_lastMotion;

			if (m_edition)
			{
				setFall(true);
			}
			else
			{
				auto testFall = clamp(m_state.m_live.m_lastMotion.y + m_state.m_live.m_lastImpulse.y, -0.1f, 0.1f);
				if (testFall == 0 || m_state.m_live.m_lastMotion.y == 0 || m_state.m_live.m_lastPosition.y == m_state.m_live.m_currentPosition.y)
				{
					setFall(false);
				}
				else
				{
					setFall(true);
				}
			}
		}
	}
}

void Entity::proceedCollision()
{
	m_state.m_live.m_collisionProceed = true; 
	m_state.m_live.m_impulse = Vector2(0.0f, 0.0f);
	m_state.m_live.m_lastImpulse = Vector2(0.0f, 0.0f);
}

void Entity::release()
{
	if (m_state.m_live.m_collidable)
	{
		PhysicMgr::getSingleton()->unregisterEntity(this);
	}
	m_state.m_live.clear();
	m_loaded = false;
	setLive(false);
}

void Entity::showImGuiWindow()
{
	if (m_displayInfo)
	{
		bool collidable = m_state.m_live.m_collidable;
		std::string name = std::to_string(getUID()) + " : " + m_state.m_live.m_name;
		if (ImGui::Begin(name.c_str(), &m_displayInfo, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::InputFloat("Speed", &m_state.m_live.m_speed);
			int backgroundLevel = m_state.m_live.m_backgroundLevel;
			ImGui::InputInt("BackgroundLevel", &backgroundLevel);
			(backgroundLevel < 0) ? setBackgroundLevel(0) : setBackgroundLevel(backgroundLevel);
			ImGui::Checkbox("Collidable", &m_state.m_live.m_collidable);
			ImGui::Checkbox("Editable", &m_edition);

			int sizeType = entityTypeToString.size();
			int type = m_state.m_live.m_type;
			char** label = (char**)malloc(sizeof(char*)*sizeType);
			int i = 0;
			for (int i = 0; i < sizeType; i++)
			{
				label[i] = (char*)malloc(sizeof(char)*strlen(entityTypeToString[i]) + 1); // For null terminated
				strcpy(label[i], entityTypeToString[i]);
			}
			ImGui::Combo("Type", &type, (const char**)label, sizeType);
			m_state.m_live.m_type = static_cast<EntityType::Enum>(type);

			for (int i = 0; i < sizeType; i++)
			{
				free(label[i]);
			}
			free(label);

			ImGui::Text("Type : %s", entityTypeToString[m_state.m_live.m_type]);
			ImGui::Text("Collision : %i", m_state.m_live.m_collisionState);
			ImGui::Text("Sleep : %s - %fms", (m_state.m_live.m_sleep) ? "True" : "False", m_state.m_live.m_sleepTime);
			ImGui::SameLine();
			if (ImGui::Button("Wake up"))
			{
				m_state.m_live.m_sleepTime = 0;
				addMotion(Vector2(0.0f, -1.0f));
			}
			

			ImGui::Text("Fall : %s", (isFall()) ? "True" : "False");
			ImGui::Text("Last motion : x = %f | y = %f", m_state.m_live.m_lastMotion.x, m_state.m_live.m_lastMotion.y);
			ImGui::Text("Last impulse : x = %f | y = %f", m_state.m_live.m_lastImpulse.x, m_state.m_live.m_lastImpulse.y);
			ImGui::Text("Last Position : x = %f | y = %f", m_state.m_live.m_lastPosition.x, m_state.m_live.m_lastPosition.y);
			ImGui::Text("Position");
			float x = m_state.m_live.m_currentPosition.x;
			float y = m_state.m_live.m_currentPosition.y;
			auto winSize = GameMgr::getSingleton()->getMainRenderWindow()->getSize();
			ImGui::SliderFloat("x", &x, 0.0f, winSize.x);
			ImGui::SliderFloat("y", &y, 0.0f, winSize.y);
			float rot = m_state.m_live.m_angle * DEGTORAD;
			ImGui::SliderAngle("Rotation", &rot);
		
			if(ImGui::CollapsingHeader("JumpParam"))
			{
				ImGui::InputFloat("vx", &m_state.m_live.m_vx);
				ImGui::InputFloat("vGrav", &m_state.m_live.m_vGrav);
				ImGui::InputFloat("vJump", &m_state.m_live.m_vJump);
				ImGui::InputFloat("vy", &m_state.m_live.m_vy);
				ImGui::InputFloat("vMax", &m_state.m_live.m_vMax);
			}
			if (ImGui::Button("Jump"))
			{
				jump();
			}
			ImGui::SameLine();
			if (ImGui::Button("Attack"))
			{
				attack();
			}
			

			setPosition(Vector2(x, y));
			m_state.m_live.m_angle = rot * RADTODEG;

			char** items = (char**)malloc(sizeof(char*) * entityAnimationStateToString.size());
			for (unsigned int i = 0; i < entityAnimationStateToString.size(); i++)
			{
				items[i] = (char*)malloc(sizeof(char) * strlen(entityAnimationStateToString[i]) + 1); // For null terminated
				strcpy(items[i], entityAnimationStateToString[i]);
			}

			int currentState = m_state.m_live.m_currentState;

			ImGui::Combo("Animation state", &currentState, (const char**)items, entityAnimationStateToString.size());
			for (unsigned int i = 0; i < entityAnimationStateToString.size(); i++)
			{
				free(items[i]);
			}
			free(items);
			
			m_state.m_live.m_currentState = (EntityAnimationState::Enum)currentState;
			sf::Sprite* currentAnim = m_state.m_live.m_animations[m_state.m_live.m_currentState].getCurrentAnimation();
			ImGui::Text("Origin = x : %f | y : %f", currentAnim->getOrigin());
			ImGui::Image(*currentAnim);
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				auto textRect = currentAnim->getTextureRect();
				ImGui::Text("Text rect x : %i | y : %i | w : %i | h : %i", textRect.left, textRect.top, textRect.width, textRect.height);
				ImGui::Text("Global Bound top : %f | Left : %f | Widht : %f | Heigth : %f", getGlobalBounds().top, getGlobalBounds().left, getGlobalBounds().width, getGlobalBounds().height);
				ImGui::Text("Texture : %s", m_state.m_live.m_texturePath.c_str());
				ImGui::Image(*(currentAnim->getTexture()));
				ImGui::EndTooltip();
			}
			int current = m_state.m_live.m_animations[m_state.m_live.m_currentState].m_currentFrame;
			ImGui::SliderInt("Current", &current, 0, m_state.m_live.m_animations[m_state.m_live.m_currentState].m_animation.size() - 1);
			m_state.m_live.m_animations[m_state.m_live.m_currentState].m_currentFrame = current;

			if (ImGui::Button("Play"))
			{
				m_state.m_live.m_animate = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Pause"))
			{
				m_state.m_live.m_animate = false;
			}
			ImGui::InputFloat("Time between frame", &m_state.m_live.m_animations[m_state.m_live.m_currentState].m_timePerFrame, 0.01f, 1.0f);


		}
		ImGui::End();

		if (collidable != m_state.m_live.m_collidable)
		{
			if (m_state.m_live.m_collidable)
			{
				PhysicMgr::getSingleton()->registerEntity(this);
			}
			else
			{
				PhysicMgr::getSingleton()->unregisterEntity(this);
			}
		}
	}
	
}

void Entity::setPosition(Vector2 pos)
{
	m_state.m_live.m_lastPosition = pos;
	m_state.m_live.m_currentPosition = pos;
}

const float Entity::getDistance(Entity* ent)
{
	Vector2 centerEnt1, centerEnt2;
	auto globalBoundsEnt1 = getLastGlobalBounds();
	auto globalBoundsEnt2 = ent->getGlobalBounds();
	centerEnt1.x = globalBoundsEnt1.left + (globalBoundsEnt1.width / 2.0f);
	centerEnt1.y = globalBoundsEnt1.top + (globalBoundsEnt1.height / 2.0f);

	centerEnt2.x = globalBoundsEnt2.left + (globalBoundsEnt2.width / 2.0f);
	centerEnt2.y = globalBoundsEnt2.top + (globalBoundsEnt2.height / 2.0f);

	Vector2 distanceVector = centerEnt2 - centerEnt1;

	return sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);
}

const float Entity::getVX() const
{ 
	if (m_state.m_live.m_orientation == EntityOrientation::Right)
	{
		return m_state.m_live.m_vx; 
	}
	else
	{
		return -m_state.m_live.m_vx;
	}
}

void Entity::move(Vector2 motion)
{
	if (m_state.m_live.m_type != EntityType::Anchor)
	{
		if (isSleeping() && motion != Vector2())
		{
			wakeUp();
		}
		
		m_state.m_live.m_lastPosition = m_state.m_live.m_currentPosition;
		m_state.m_live.m_currentPosition += motion;
		m_state.m_live.m_motion += motion;
		if (!isFall() && motion.y < 0.0f)
		{
			setFall(true);
		}	
	}
	if (m_state.m_live.m_type == EntityType::Anchor && isEdition())
	{
		m_state.m_live.m_lastPosition = m_state.m_live.m_currentPosition;
		m_state.m_live.m_currentPosition += motion;
		m_state.m_live.m_motion = Vector2(0.0, 0.0);
	}
}


void Entity::jump() 
{
	m_state.m_live.m_action = EntityAction::Jump;
	if (m_state.m_live.m_currentState != EntityAnimationState::JumpLeft && m_state.m_live.m_currentState != EntityAnimationState::JumpRight && m_state.m_live.m_currentState != EntityAnimationState::Jump)
	{
		setState(EntityAnimationState::Jump);
	}
	else
	{
		m_state.m_live.m_vy = m_state.m_live.m_vJump;
	}
}

void Entity::setAction(EntityAction::Enum action, bool b)
{
	if (b)
	{
		m_state.m_live.m_action |= action;
	}
	else
	{
		if (isInAction(action))
		{
			m_state.m_live.m_action ^= action;
		}
	}
}

const bool Entity::isInAction(EntityAction::Enum action) const 
{ 
	auto test = (m_state.m_live.m_action & action) == action;
	return (m_state.m_live.m_action & action) == action; 
}

void Entity::onClick() 
{ 
	if (m_state.m_live.m_commandOnClick != NULL)
	{
		m_state.m_live.m_commandOnClick->execute();
	}
}

const EntityType::Enum Entity::getTypeByName(const char* name) const
{
	return stringToEntityType[name];
}
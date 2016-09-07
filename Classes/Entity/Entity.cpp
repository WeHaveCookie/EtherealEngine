#include "stdafx.h"
#include "Entity.h"

#include "EtherealDreamManagers.h"
#include "Manager/Render/RenderMgr.h"
#include "../../External/rapidjson/document.h"
#include "Utils/Random.h"
#include "Utils/VectorUtils.h"

std::map<std::string, EntityAnimationState::Enum> StateToEnum =
{
	{ "RIGHT", EntityAnimationState::Right },
	{ "LEFT", EntityAnimationState::Left },
	{ "IDLE", EntityAnimationState::Idle },
	{ "IDLE_RIGHT", EntityAnimationState::IdleRight },
	{ "IDLE_LEFT", EntityAnimationState::IdleLeft }
};

std::vector<const char*> EnumToString = {
	"RIGHT",
	"LEFT",
	"IDLE",
	"IDLE_RIGHT",
	"IDLE_LEFT"
};

Entity::Entity()
	:m_uid(newUID++)
{
	m_live = false;
	m_state.m_live.m_errorTexture.loadFromFile(ERROR_TEXTURE);
	m_state.m_live.m_errorTexture.setRepeated(true);
}

Entity::~Entity()
{

}

void Entity::move(sf::Vector2f motion)
{
	m_state.m_live.m_motion += motion;
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

void Entity::update(const float dt)
{
	updatePosition();

	m_state.m_live.m_animations[m_state.m_live.m_currentState].update(dt);
	sf::Sprite* currentAnim = m_state.m_live.m_animations[m_state.m_live.m_currentState].getCurrentAnimation();
	currentAnim->setPosition(m_state.m_live.m_currentPosition);
}

bool Entity::process(const float dt)
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
	m_state.m_live.m_animations[m_state.m_live.m_currentState].reset();
	m_state.m_live.m_currentState = state;
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

sf::FloatRect Entity::getGlobalBounds()
{
	return sf::FloatRect(m_state.m_live.m_currentPosition.x, m_state.m_live.m_currentPosition.y, m_state.m_live.m_width, m_state.m_live.m_height);
}

void Entity::roolback()
{ 
	m_state.m_live.m_currentPosition = m_state.m_live.m_lastPosition;
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

	FILE *f = fopen(path, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *json = (char *)malloc(fsize + 1);
	fread(json, fsize, 1, f);
	fclose(f);

	json[fsize] = 0;

	rapidjson::Document document;
	document.Parse(json);
	assert(!document.HasParseError());
	assert(document.IsObject());

	sf::Sprite spr;
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

	m_state.m_live.m_texture = sf::Texture();
	auto load = m_state.m_live.m_texture.loadFromFile(entPath);
	assert(load);
	m_state.m_live.m_texture.setSmooth(true);
	spr.setTexture(m_state.m_live.m_texture);

	assert(document.HasMember("Name"));
	setName(document["Name"].GetString());

	assert(document.HasMember("Speed"));
	setSpeed(document["Speed"].GetFloat());

	assert(document.HasMember("Width"));
	m_state.m_live.m_width = document["Width"].GetUint();

	assert(document.HasMember("Height"));
	m_state.m_live.m_height = document["Height"].GetUint();

	m_state.m_live.clear();
	m_state.m_next = NULL;

	if (document.HasMember("Collidable"))
	{
		m_state.m_live.m_collidable = document["Collidable"].GetBool();
	} else
	{
		m_state.m_live.m_collidable = true;
	}
	
	assert(document.HasMember("Animation"));
	const rapidjson::Value& animation = document["Animation"];

	for (auto& v : animation.GetArray())
	{
		EntityAnimation anim;
		assert(v.HasMember("State"));

		assert(v.HasMember("Frame"));
		uint32_t nbrFrame = v["Frame"].GetUint();

		assert(v.HasMember("Time"));
		anim.m_timePerFrame = v["Time"].GetFloat();

		assert(v.HasMember("Line"));
		uint32_t line = v["Line"].GetUint() - 1;

		for (uint32_t column = 0; column < nbrFrame; column++)
		{
			spr.setTextureRect(sf::IntRect(m_state.m_live.m_width * column, m_state.m_live.m_height * line, m_state.m_live.m_width, m_state.m_live.m_height));
			anim.m_animation.push_back(spr);
		}
		anim.m_currentFrame = 0;
		addAnimation(StateToEnum[v["State"].GetString()], anim);
	}

	if (document.HasMember("Position"))
	{
		const rapidjson::Value& posArray = document["Position"];
		assert(posArray.Size() == 2);
		sf::Vector2f pos;
		
		//pos.x = randIntBorned(0.0f, posArray[0].GetFloat());
		pos.x = posArray[0].GetFloat();
		pos.y = posArray[1].GetFloat();
		setPosition(pos);
	}

	assert(document.HasMember("State"));
	setState(StateToEnum[document["State"].GetString()]);

	m_live = true;
}

void Entity::updatePosition()
{
	m_state.m_live.m_lastPosition = m_state.m_live.m_currentPosition;
	m_state.m_live.m_lastMotion = m_state.m_live.m_motion;
	m_state.m_live.m_currentPosition += m_state.m_live.m_lastMotion;
	m_state.m_live.m_motion -= m_state.m_live.m_lastMotion;
}
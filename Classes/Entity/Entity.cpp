#include "stdafx.h"
#include "Entity.h"

#include "EtherealDreamManagers.h"
#include "Manager/Render/RenderMgr.h"
#include "../../External/rapidjson/document.h"

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

Entity::Entity(const char* path)
{
	m_id = -1;
	m_speed = 1.0f;
	m_currentState = EntityAnimationState::Idle;
	m_position = sf::Vector2f(0.0f, 0.0f);
	build(path);
}

Entity::~Entity()
{

}

void Entity::move(sf::Vector2f motion)
{

}

void Entity::paint()
{
	auto renderMgr = RENDER_MGR;
	auto rdrWin  = renderMgr->getMainRenderWindow();
	rdrWin->draw(*m_animations[m_currentState].getCurrentAnimation());
}

void Entity::update()
{
	sf::Sprite* currentAnim = m_animations[m_currentState].getCurrentAnimation();
	currentAnim->setPosition(m_position);
}

void Entity::addAnimation(EntityAnimationState::Enum entAnimState, EntityAnimation entAnim)
{
	m_animations[entAnimState] = entAnim;
}

void Entity::setSpeed(float speed)
{
	m_speed = std::max(speed,0.0f);
}

EntityAnimation* Entity::getAnimation(EntityAnimationState::Enum state)
{ 
	if (m_animations[state].m_animation.size() != 0)
	{
		return &m_animations[state]; 
	}
	else
	{
		return NULL;
	}
}

void getEntityPath(char* dest, const char* source)
{
	strcpy(dest, source);
	char* ext = strstr(dest, ".json");
	strncpy(ext, ".png", 5);
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
	getEntityPath(entPath, path);

	assert(m_texture.loadFromFile(entPath));
	m_texture.setSmooth(true);

	spr.setTexture(m_texture);

	assert(document.HasMember("ID"));
	setId(document["ID"].GetInt());

	assert(document.HasMember("Speed"));
	setSpeed(document["Speed"].GetFloat());

	assert(document.HasMember("Width"));
	unsigned int width = document["Width"].GetUint();

	assert(document.HasMember("Height"));
	unsigned int height = document["Height"].GetUint();

	assert(document.HasMember("State"));
	setState(StateToEnum[document["State"].GetString()]);

	assert(document.HasMember("Animation"));
	const rapidjson::Value& animation = document["Animation"];

	for (auto& v : animation.GetArray())
	{
		EntityAnimation anim;
		assert(v.HasMember("State"));

		assert(v.HasMember("Frame"));
		unsigned int nbrFrame = v["Frame"].GetUint();

		assert(v.HasMember("Time"));
		anim.m_timePerFrame = v["Time"].GetFloat();

		assert(v.HasMember("Line"));
		unsigned int line = v["Line"].GetUint() - 1;

		for (unsigned int column = 0; column < nbrFrame; column++)
		{
			spr.setTextureRect(sf::IntRect(width * column, height * line, width, height));
			anim.m_animation.push_back(spr);
		}
		anim.m_currentFrame = 0;
		addAnimation(StateToEnum[v["State"].GetString()], anim);
	}

	if (document.HasMember("Position"))
	{
		const rapidjson::Value& posArray = document["Position"];
		assert(posArray.Size() == 2);
		sf::Vector2f pos = { posArray[0].GetFloat(), posArray[1].GetFloat() };
		setPosition(pos);
	}
}
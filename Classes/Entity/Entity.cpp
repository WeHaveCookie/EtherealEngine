#include "stdafx.h"
#include "Entity.h"

#include "EtherealDreamManagers.h"
#include "Manager/Render/RenderMgr.h"
#include "Manager/Engine/PhysicMgr.h"
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
	setLive(false);
	m_loaded = false;
	m_onLoading = false;
	m_displayInfo = false;
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
	displayInfo();
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
	auto error = document.HasParseError();
	auto object = document.IsObject();
	assert(!error);
	assert(object);

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
	m_state.m_live.m_texturePath = entPath;
	m_state.m_live.m_texture = sf::Texture();
	auto load = m_state.m_live.m_texture.loadFromFile(entPath);
	assert(load);
	m_state.m_live.m_texture.setSmooth(true);
	spr.setTexture(m_state.m_live.m_texture);

	auto member = document.HasMember("Name");
	assert(member);
	setName(document["Name"].GetString());

	member = document.HasMember("Speed");
	assert(member);
	setSpeed(document["Speed"].GetFloat());

	member = document.HasMember("Width");
	assert(member);
	m_state.m_live.m_width = document["Width"].GetUint();

	member = document.HasMember("Height");
	assert(member);
	m_state.m_live.m_height = document["Height"].GetUint();

	
	m_state.m_next = NULL;

	if (document.HasMember("Collidable"))
	{
		m_state.m_live.m_collidable = document["Collidable"].GetBool();
	} else
	{
		m_state.m_live.m_collidable = true;
	}
	if (m_state.m_live.m_collidable)
	{
		PhysicMgr::getSingleton()->registerEntity(this);
	}
	
	member = document.HasMember("Animation");
	assert(member);
	const rapidjson::Value& animation = document["Animation"];

	for (auto& v : animation.GetArray())
	{
		EntityAnimation anim;
		member = v.HasMember("State");
		assert(member);

		member = v.HasMember("Frame");
		assert(member);
		uint32_t nbrFrame = v["Frame"].GetUint();

		member = v.HasMember("Time");
		assert(member);
		anim.m_timePerFrame = v["Time"].GetFloat();

		member = v.HasMember("Line");
		assert(member);
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

	member = document.HasMember("State");
	assert(member);
	setState(StateToEnum[document["State"].GetString()]);

	setLive(true);
	m_onLoading = false;
	m_loaded = true;
}

void Entity::updatePosition()
{
	m_state.m_live.m_lastPosition = m_state.m_live.m_currentPosition;
	m_state.m_live.m_lastMotion = m_state.m_live.m_motion;
	m_state.m_live.m_currentPosition += m_state.m_live.m_lastMotion;
	m_state.m_live.m_motion -= m_state.m_live.m_lastMotion;
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

void Entity::displayInfo()
{
	if (m_displayInfo)
	{
		bool collidable = m_state.m_live.m_collidable;
		std::string name = std::to_string(getUID()) + " : " + m_state.m_live.m_name;
		if (ImGui::Begin(name.c_str(), &m_displayInfo))
		{
			sf::Sprite* currentAnim = m_state.m_live.m_animations[m_state.m_live.m_currentState].getCurrentAnimation();
			ImGui::Image(*currentAnim);
			ImGui::InputFloat("Speed", &m_state.m_live.m_speed);
			ImGui::Checkbox("IsCollidable", &m_state.m_live.m_collidable);
			
			ImGui::Text("Position");
			ImGui::SliderFloat("x", &m_state.m_live.m_currentPosition.x, 0.0f, 1920.0f);
			ImGui::SliderFloat("y", &m_state.m_live.m_currentPosition.y, 0.0f, 1080.0f);
			ImGui::Text("Texture : %s", m_state.m_live.m_texturePath.c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Image(m_state.m_live.m_texture);
				ImGui::EndTooltip();
			}
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
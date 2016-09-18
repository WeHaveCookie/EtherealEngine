#include "stdafx.h"
#include "Entity.h"

#include "EtherealDreamManagers.h"
#include "Manager/Render/RenderMgr.h"
#include "Manager/Engine/PhysicMgr.h"
#include "Manager/Input/InputMgr.h"
#include "../../External/rapidjson/document.h"
#include "Utils/Random.h"
#include "Utils/VectorUtils.h"

#define ERROR_TEXTURE "Data/Texture/error.png"

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
	m_editable = false;
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
	displayInfo();
}

void Entity::update(const float dt)
{
	updatePosition();

	if (m_state.m_live.m_animate)
	{
		m_state.m_live.m_animations[m_state.m_live.m_currentState].update(dt);
	}
	sf::Sprite* currentAnim = m_state.m_live.m_animations[m_state.m_live.m_currentState].getCurrentAnimation();
	currentAnim->setPosition(m_state.m_live.m_currentPosition);
	currentAnim->setRotation(m_state.m_live.m_angle);
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
	m_state.m_live.m_animations[m_state.m_live.m_currentState].reset();
	m_state.m_live.m_currentState = state;
}

const EntityAnimation* Entity::getAnimation(EntityAnimationState::Enum state)
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

const sf::FloatRect Entity::getGlobalBounds() const
{
	return sf::FloatRect(m_state.m_live.m_currentPosition.x, 
							m_state.m_live.m_currentPosition.y, 
							m_state.m_live.m_width, 
							m_state.m_live.m_height);
}

void Entity::rollbackXAxis()
{ 
	m_state.m_live.m_currentPosition = m_state.m_live.m_lastPosition;
	m_state.m_live.m_motion = m_state.m_live.m_lastMotion;
	if (m_state.m_live.m_lastMotion.x < 1.0f && m_state.m_live.m_lastMotion.x > -1.0f)
	{
		m_state.m_live.m_motion.x = 0.0f;
		return;
	}
	else
	{
		m_state.m_live.m_motion.x = m_state.m_live.m_lastMotion.x * 0.90; // Get only 90% of the previous motion
	}
}

void Entity::rollbackYAxis()
{
	m_state.m_live.m_currentPosition = m_state.m_live.m_lastPosition;
	m_state.m_live.m_motion = m_state.m_live.m_lastMotion;
	if (m_state.m_live.m_lastMotion.y < 1.0f && m_state.m_live.m_lastMotion.y > -1.0f)
	{
		m_state.m_live.m_motion.y = 0.0f;
		return;
	}
	else
	{
		m_state.m_live.m_motion.y = m_state.m_live.m_lastMotion.y * 0.90; // Get only 90% of the previous motion
	}
}

void Entity::rollBackAllAxis()
{
	m_state.m_live.m_currentPosition = m_state.m_live.m_lastPosition;
	m_state.m_live.m_motion = m_state.m_live.m_lastMotion;
	if (m_state.m_live.m_lastMotion.y < 1.0f && m_state.m_live.m_lastMotion.y > -1.0f)
	{
		m_state.m_live.m_motion.y = 0.0f;
	}
	else
	{
		m_state.m_live.m_motion.y = m_state.m_live.m_lastMotion.y * 0.90; // Get only 90% of the previous motion
	}
	if (m_state.m_live.m_lastMotion.x < 1.0f && m_state.m_live.m_lastMotion.x > -1.0f)
	{
		m_state.m_live.m_motion.x = 0.0f;
	}
	else
	{
		m_state.m_live.m_motion.x = m_state.m_live.m_lastMotion.x * 0.90; // Get only 90% of the previous motion
	}

}

void Entity::retry()
{
	update(0.0f);
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

	auto member = document.HasMember("Width");
	assert(member);
	m_state.m_live.m_width = document["Width"].GetUint();

	member = document.HasMember("Height");
	assert(member);
	m_state.m_live.m_height = document["Height"].GetUint();
	
	m_state.m_live.clear();

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
	auto load = m_state.m_live.m_texture.loadFromFile(entPath);
	assert(load);
	m_state.m_live.m_texture.setSmooth(true);
	spr.setTexture(m_state.m_live.m_texture);

	member = document.HasMember("Name");
	assert(member);
	setName(document["Name"].GetString());

	member = document.HasMember("Speed");
	assert(member);
	setSpeed(document["Speed"].GetFloat());

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

	if (document.HasMember("Mass"))
	{
		m_state.m_live.m_mass = document["Mass"].GetFloat();
	}
	else
	{
		m_state.m_live.m_mass = 1.0f;
	}
	m_state.m_live.m_angle = 0.0f;
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
		if (ImGui::Begin(name.c_str(), &m_displayInfo, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::InputFloat("Speed", &m_state.m_live.m_speed);
			ImGui::Checkbox("IsCollidable", &m_state.m_live.m_collidable);
			ImGui::Checkbox("Is Editable", &m_editable);
			ImGui::Text("Position");
			float x = m_state.m_live.m_currentPosition.x;
			float y = m_state.m_live.m_currentPosition.y;
			ImGui::SliderFloat("x", &x, 0.0f, 1920.0f);
			ImGui::SliderFloat("y", &y, 0.0f, 1080.0f);
			float rot = m_state.m_live.m_angle * DEGTORAD;
			ImGui::SliderAngle("Rotation", &rot);

			m_state.m_live.m_currentPosition = sf::Vector2f(x, y);
			m_state.m_live.m_angle = rot * RADTODEG;

			char** items;
			items = (char**)malloc(sizeof(char*) * EnumToString.size());
			int i = 0;
			for (auto& state : EnumToString)
			{
				items[i] = (char*)malloc(sizeof(char) * strlen(state));
				strcpy(items[i], state);
				i++;
			}

			int currentState = m_state.m_live.m_currentState;

			ImGui::Combo("Animation state", &currentState, (const char**)items, EnumToString.size());
			free(items);
			
			m_state.m_live.m_currentState = (EntityAnimationState::Enum)currentState;
			sf::Sprite* currentAnim = m_state.m_live.m_animations[m_state.m_live.m_currentState].getCurrentAnimation();
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

void Entity::setPosition(sf::Vector2f pos)
{
	m_state.m_live.m_currentPosition = pos;
}
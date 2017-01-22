#pragma once
#include "Manager/Entity/EntityMgr.h"

class Entity;

struct Background {
	sf::Texture	m_texture;
	sf::Sprite	m_sprite;
	int			m_backgroundLevel;

	void paint();
};

class Level
{

	public:
		Level();
		~Level();

		void paint();
		bool load(const char* path);
		bool reload();
		void unload();

		const char* getName() const { return m_name.c_str(); }
		const sf::Vector2f getSize() const{ return m_size; }
		void registerEntity(Entity* ent);
		const char* getCharacterPath() const { return m_characterPath.c_str(); };
		sf::View getView() const { return m_view; }
		const bool cameraShake() const { return m_shake; }
		void shake(bool b);
		const sf::View getCamera() const { return m_view; }
		void processShake(const float dt);
		const bool isLoaded() const;
		void killEnemyType(ShootType::Enum type);
		void startExtermination();
		void endExtermination();
		const int getLocalSinusDead() const { return m_sinusLocalDead; }
		const int getTotalSinusDead() const { return m_sinusTotalDead; }
		const int getLocalSpikeDead() const { return m_spikeLocalDead; }
		const int getTotalSpikeDead() const { return m_spikeTotalDead; }
		const int getLocalTriangleDead() const { return m_triangleLocalDead; }
		const int getTotalTriangleDead() const { return m_triangleTotalDead; }
		const int getScore() const { return m_score; }
		void clearScore();

	private:

		std::string					m_path;
		std::string					m_name;
		std::string					m_characterPath;
		sf::Vector2f				m_size;
		std::vector<Entity*>		m_entitys;
		std::vector<Background*>	m_backgrounds;
		std::vector<Background*>	m_foregrounds;
		sf::View					m_view;
		bool						m_shake;
		float						m_shakeTime;
		float						m_timer;
		float						m_shakeFactor;
		float						m_maxShakeFactor;
		sf::Font					m_font;
		std::vector<sf::Text>		m_texts;
		uint32_t					m_sinusTotalDead;
		uint32_t					m_spikeTotalDead;
		uint32_t					m_triangleTotalDead;
		uint32_t					m_sinusLocalDead;
		uint32_t					m_spikeLocalDead;
		uint32_t					m_triangleLocalDead;
		uint32_t					m_score;
};


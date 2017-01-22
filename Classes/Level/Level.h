#pragma once

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
		uint32_t					m_sinusDead;
		uint32_t					m_spikeDead;
		uint32_t					m_triangleDead;
		uint32_t					m_score;
};


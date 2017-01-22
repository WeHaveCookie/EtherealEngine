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

	private:

		std::string					m_path;
		std::string					m_name;
		std::string					m_characterPath;
		sf::Vector2f				m_size;
		std::vector<Entity*>		m_entitys;
		std::vector<Background*>	m_backgrounds;
		std::vector<Background*>	m_foregrounds;
};


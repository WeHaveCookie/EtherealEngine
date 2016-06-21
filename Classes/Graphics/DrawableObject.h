#pragma once

class DrawableObject
{
    public:
        DrawableObject();
        virtual ~DrawableObject();
		virtual void move(sf::Vector2f motion) {};
        inline sf::Vector2f getPosition() {return m_position;}
        inline float getSpeed() {return m_speed;}
        inline sf::FloatRect getGlobalBounds() {return m_sprite.getGlobalBounds();}
        inline sf::FloatRect getLocalBounds() {return m_sprite.getLocalBounds();}
        inline sf::Sprite getSprite() {return m_sprite;}
        inline sf::Vector2f getOriginPosition() {return m_originPosition;}
        virtual void paint() {};
        virtual void update() {};
    protected:


        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Vector2f m_position;
        float m_speed;
        sf::Vector2f m_originPosition;
    private:
};

#pragma once

class DrawableObject
{
    public:
        DrawableObject();
        virtual ~DrawableObject();
        virtual void move(sf::Vector2f motion);
        inline sf::Vector2f getPosition() {return m_position;}
        inline void setState(MovingState st) {m_movingState = st;}
        inline MovingState getState() {return m_movingState;}
        inline float getSpeed() {return m_speed;}
        inline sf::FloatRect getGlobalBounds() {return m_sprite.getGlobalBounds();}
        inline sf::FloatRect getLocalBounds() {return m_sprite.getLocalBounds();}
        inline sf::Sprite getSprite() {return m_sprite;}
        inline sf::Vector2f getOriginPosition() {return m_originPosition;}
        virtual void draw(sf::RenderWindow* window) {};
        virtual void update(sf::RenderWindow* window) {};
    protected:


        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Vector2f m_position;
        float m_speed;
        MovingState m_movingState;
        sf::Vector2f m_originPosition;
    private:
};

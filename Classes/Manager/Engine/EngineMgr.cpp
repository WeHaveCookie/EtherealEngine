#include "stdafx.h"
#include "EngineMgr.h"

EngineMgr::EngineMgr()
:Manager(ManagerType::Enum::Engine)
{
}

EngineMgr::~EngineMgr()
{
}

void EngineMgr::init()
{

}

void EngineMgr::process(const float dt)
{

}

void EngineMgr::end()
{

}

bool EngineMgr::collisionCircle(sf::FloatRect box1, sf::CircleShape circle)
{
   float d2 = (box1.left-circle.getPosition().x)*(box1.left-circle.getPosition().x) + (box1.top-circle.getPosition().y)*(box1.top-circle.getPosition().y);
   if (d2>pow(circle.getRadius(),2))
      return false;
   else
      return true;
}

bool EngineMgr::collisionAABB(sf::FloatRect box1, sf::FloatRect box2)
{
    return !((box2.left >= box1.left + box1.width) ||
        (box2.left  + box2.width <= box1.left) ||
        (box2.top >= box1.top + box1.height) ||
        (box2.top + box2.height <= box1.top));
}

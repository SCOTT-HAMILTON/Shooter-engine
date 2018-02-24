#ifndef OBJECT_H
#define OBJECT_H

using idtype = unsigned long long int;

#include <SFML/Graphics.hpp>
#include "Displayer.h"

class Object
{
public:
    Object();
    virtual ~Object();
    virtual void drawExts(sf::RenderTarget &target){}
    virtual void preDrawUpdate(){
    }

    idtype getMyId();

    static void setDisplayer(const std::shared_ptr<Displayer<Object>> &displayer);
    const sf::Texture &getTexture();
    const sf::Vector2f &getPos();
    const sf::RenderStates &getRenderStates();

protected:
    sf::Vector2u rectSize;
    sf::Vector2f pos;
    sf::Texture base_texture;
    sf::RenderStates state;

    static idtype current_id;
    idtype myid;

    static std::shared_ptr<Displayer<Object>> displayer;
};


#endif // OBJECT_H

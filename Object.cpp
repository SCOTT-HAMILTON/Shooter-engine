/*
    Scott Hamilton alias ScottHDev (13 years old)
    24 / 02 / 2018

    Little 2D Shooter-Engine

    Big thank's to Frederic Legrand
    for his nice website :
    http://www.f-legrand.fr/scidoc/docmml/graphie/geometrie/polygone/polygone.html
    to give us nice and easy maths formulas like this one who
    divide a complexe polygone in little triangles.
*/

#include "Object.h"

idtype  Object::current_id = 0;
std::shared_ptr<Displayer<Object>> Object::displayer = nullptr;

Object::Object()
{
    myid = current_id;
    current_id++;
}

Object::~Object()
{
    displayer->removeObject(myid);
}

void Object::setDisplayer(const std::shared_ptr<Displayer<Object>> &displayer){
    Object::displayer = displayer;
}

idtype Object::getMyId(){
    return myid;
}

const sf::Texture &Object::getTexture(){
    return base_texture;
}
const sf::Vector2f &Object::getPos(){
    return pos;
}

const sf::RenderStates &Object::getRenderStates(){
    return state;
}

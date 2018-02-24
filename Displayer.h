#ifndef DISPLAYER_H
#define DISPLAYER_H

#include <SFML/Graphics.hpp>
#include "Object.h"
#include <memory>
#include <unordered_map>
#include <iostream>
#include <iterator>

template<class T>
class Displayer
{
public:
    Displayer<T>(){}
    virtual ~Displayer(){}
    void drawbjects(sf::RenderTarget &target){
        //std::cout << "    " << std::endl;
        for (auto it = objects.begin(); it != objects.end(); it++){
            it->second->preDrawUpdate();
            //std::cout << "texture : " << it->second->getTexture().getSize().x << ", " << it->second->getTexture().getSize().y << std::endl;
            sf::Sprite sp(it->second->getTexture());
            sp.setPosition(it->second->getPos());
            target.draw(sp, it->second->getRenderStates());
            it->second->drawExts(target);
        }
    }

    void addObject(T* obj){
        objects.insert(std::make_pair<idtype, T*> (obj->getMyId(), (T*)obj));
    }

    void removeObject(idtype id){
        objects.erase(objects.find(id));
    }

private:
    std::map<idtype, T*> objects;
};

#endif // DISPLAYER_H

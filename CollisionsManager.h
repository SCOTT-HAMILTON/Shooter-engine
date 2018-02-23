#ifndef COLLISIONSMANAGER_H
#define COLLISIONSMANAGER_H

#include <unordered_map>
#include <functional>
#include <memory>
#include <SFML/Graphics.hpp>

template<class Key, class T>
class CollisionsManager
{
public:
    CollisionsManager<Key, T>(const std::function<bool(T, T)> &fctCollide){
        this->fctCollide = fctCollide;
        fps_update = 30;
    }
    virtual ~CollisionsManager(){}

    void addObject(T *obj, Key key){
        objects.insert(std::make_pair<Key,T*>((const Key)key, (T*)obj));
    }

    void removeObject(unsigned long long int id){
        objects.remove(id);
    }

    int update(){
        if (last_update.getElapsedTime().asMilliseconds()<(1000/fps_update)) return 0;
        last_update.restart();
        for (auto i = objects.begin(); i != objects.end(); i++){
            i->second->inCollision = false;
        }
        for (auto i = objects.begin(); i != objects.end(); i++){
            for (auto j = i; j != objects.end(); j++){
                if (i != j){
                    std::cout << "first : " << i->second->getMyId() << std::endl;
                    std::cout << "second : " << j->second->getMyId() << std::endl;
                    auto tmp_dr1 = (*(i->second));
                    auto tmp_dr2 = (*(j->second));
                    if (fctCollide( tmp_dr1, tmp_dr2 )){
                        i->second->inCollision = true;
                        j->second->inCollision = true;
                    }
                }

            }
        }
    }

private:
    std::function<bool(T, T)> fctCollide;
    std::unordered_map<Key, T*> objects;
    sf::Clock last_update;
    int fps_update;
};

#endif // COLLISIONSMANAGER_H

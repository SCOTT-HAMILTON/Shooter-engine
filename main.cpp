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


#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "CollisionsManager.h"
#include "Displayer.h"

int main()
{

    std::shared_ptr<Displayer<Object>> displayer;
    displayer = std::make_shared<Displayer<Object>>();
    Object::setDisplayer(displayer);

    std::shared_ptr<CollisionsManager<idtype, Entity>> collisionManager;
    collisionManager = std::make_shared<CollisionsManager<idtype, Entity>>(&Entity::Collide);
    Entity::setCollisionManager(collisionManager);

    sf::RenderWindow fenetre(sf::VideoMode(800, 600), "SFML shooter");
    fenetre.setFramerateLimit(130);
    sf::Event event;

    sf::RectangleShape base(sf::Vector2f(fenetre.getSize().x, fenetre.getSize().y));
    Entity base_dr(base, {0, 0}, sf::Color(0, 0, 0), sf::Color(255, 255, 255));

    sf::CircleShape circle_base(100);
    Entity circle(circle_base, {400, 10}, sf::Color(255, 255, 255), sf::Color(0, 0, 0));

    sf::ConvexShape etoile_base;
    int scale = 100;
    etoile_base.setPointCount(10);
    etoile_base.setPoint(0, sf::Vector2f(2*scale, 0*scale));
    etoile_base.setPoint(1, sf::Vector2f(1*scale, 1*scale));
    etoile_base.setPoint(2, sf::Vector2f(0*scale, 1*scale));
    etoile_base.setPoint(3, sf::Vector2f(1*scale, 2*scale));
    etoile_base.setPoint(4, sf::Vector2f(0*scale, 3*scale));
    etoile_base.setPoint(5, sf::Vector2f(2*scale, 2*scale));
    etoile_base.setPoint(6, sf::Vector2f(4*scale, 3*scale));
    etoile_base.setPoint(7, sf::Vector2f(3*scale, 2*scale));
    etoile_base.setPoint(8, sf::Vector2f(4*scale, 1*scale));
    etoile_base.setPoint(9, sf::Vector2f(3*scale, 1*scale));
    Entity etoile(etoile_base, {300, 300}, sf::Color(50, 255, 25), sf::Color(0, 212, 50));

    sf::RectangleShape rect_base(sf::Vector2f(50 , 50));
    Entity rect(rect_base, {300, 300}, sf::Color(0, 212, 50), sf::Color(11, 66, 50));

    int Entity_sign = -1;
    float Entity_speed = 3;
    float y_speed = 5;
    float x_speed = y_speed;
    bool pause = false;

    while (fenetre.isOpen()){
        while (fenetre.pollEvent(event)){
            if (event.type == sf::Event::Closed) fenetre.close();
            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Up){
                    rect.move(0, -y_speed);
                }else if (event.key.code == sf::Keyboard::Down){
                    rect.move(0, y_speed);
                }else if (event.key.code == sf::Keyboard::Left){
                    rect.move(-x_speed, 0);
                }else if (event.key.code == sf::Keyboard::Right){
                    rect.move(x_speed, 0);
                }else if (event.key.code == sf::Keyboard::P){
                    pause = !pause;
                }
            }
        }

        Entity::updateCollisionManager();

        if (!pause)Entity::y_axe -= Entity_speed*Entity_sign;
        if (Entity::y_axe > fenetre.getSize().y || Entity::y_axe <= 0){
            Entity_sign = Entity_sign-(Entity_sign*2);
            Entity::y_axe -= Entity_speed*Entity_sign;
        }

        fenetre.clear(sf::Color::White);
        displayer->drawbjects(fenetre);
        fenetre.display();
    }

    return 0;
}

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Drawer.h"

int main()
{

    sf::RenderWindow fenetre(sf::VideoMode(800, 600), "SFML shooter");
    fenetre.setFramerateLimit(60);
    sf::Event event;

    sf::RectangleShape base(sf::Vector2f(fenetre.getSize().x, fenetre.getSize().y));
    Drawer base_dr(base, {0, 0}, sf::Color(0, 0, 0), sf::Color(255, 255, 255));

    //sf::CircleShape circle_base(100);
    sf::RectangleShape circle_base(sf::Vector2f(60, 100));
    Drawer circle(circle_base, {400, 10}, sf::Color(255, 255, 255), sf::Color(0, 0, 0));

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
    Drawer etoile(etoile_base, {300, 300}, sf::Color(50, 255, 25), sf::Color(0, 212, 50));

    sf::RectangleShape rect_base(sf::Vector2f(50 , 50));
    Drawer rect(rect_base, {300, 300}, sf::Color(0, 212, 50), sf::Color(11, 66, 50));

    int drawer_sign = -1;
    float drawer_speed = 3;
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

        Drawer::updateCollisionManager();

        if (!pause)Drawer::y_axe -= drawer_speed*drawer_sign;
        if (Drawer::y_axe > fenetre.getSize().y || Drawer::y_axe <= 0){
            drawer_sign = drawer_sign-(drawer_sign*2);
            Drawer::y_axe -= drawer_speed*drawer_sign;
        }

        if (Drawer::ptInside(static_cast<sf::Vector2f>(sf::Mouse::getPosition(fenetre)), etoile)){
            //std::cout << "in !!" << std::endl;
        }//else  std::cout << "out !!" << std::endl;

        fenetre.clear(sf::Color::White);
        base_dr.draw(fenetre);
        rect.draw(fenetre);
        etoile.draw(fenetre);
        circle.draw(fenetre);

        fenetre.display();
    }

    return 0;
}
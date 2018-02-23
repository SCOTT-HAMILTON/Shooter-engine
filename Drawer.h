#ifndef DRAWER_H
#define DRAWER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "CollisionsManager.h"

struct Triangle{
    sf::Vector2f p1;
    sf::Vector2f p2;
    sf::Vector2f p3;
};

struct Line{
    sf::Vector2f p1;
    sf::Vector2f p2;
};

enum CalculationMode{GRAPH, SHADER};

class Drawer
{
public:
    Drawer(sf::Shape &shape, const sf::Vector2f &pos = {0, 0}, const sf::Color &up = sf::Color::White, const sf::Color &down = sf::Color::Black);
    virtual ~Drawer();
    void draw(sf::RenderTarget &target);
    int update();

    static float y_axe;
    static sf::Color first_color;
    static sf::Color second_color;
    static CalculationMode mode;
    static sf::Shader drawer_shader;

    void setBase(sf::Shape &shape);
    void move(float x, float y);
    static void updateCollisionManager();

    static bool Collide(const Drawer &dr1, const Drawer &dr2);

    static bool segTouchSeg(const Line &line1, const Line &line2);
    static float detOfP2Line(const Line &l, const sf::Vector2f &p);
    static sf::Color collisionColor;
    static bool ptInside(const sf::Vector2f &pos, const Drawer &dr);

    static std::shared_ptr<Drawer> DrawersList;
    bool inCollision;

    unsigned long long int getMyId();
    static bool PointInTriangle(Triangle t, const sf::Vector2f &P);

private:
    sf::Vector2u rectSize;
    sf::Vector2f pos;

    sf::Texture first;
    sf::Texture second;

    sf::Texture base_text;
    sf::Sprite base;

    sf::Color up_color;
    sf::Color down_color;

    unsigned long long int myid;

    static CollisionsManager<unsigned long long int, Drawer> collisionManager;
    static unsigned long long int current_id;

    sf::Shape &base_shape;

    std::vector<Triangle> triangles;

    void precalc_values();

};


#endif // DRAWER_H

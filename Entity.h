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

#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "Object.h"
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

class Entity : public Object
{
public:
    Entity(sf::Shape &shape, const sf::Vector2f &pos = {0, 0}, const sf::Color &up = sf::Color::White, const sf::Color &down = sf::Color::Black);
    virtual ~Entity();
    virtual void drawExts(sf::RenderTarget &target);
    int update();

    static float y_axe;
    static sf::Color first_color;
    static sf::Color second_color;
    sf::Shader entity_shader;

    void setBase(sf::Shape &shape);
    void move(float x, float y);
    static void updateCollisionManager();

    static bool Collide(const Entity &dr1, const Entity &dr2);

    static bool segTouchSeg(const Line &line1, const Line &line2);
    static float detOfP2Line(const Line &l, const sf::Vector2f &p);
    static sf::Color collisionColor;
    static bool ptInside(const sf::Vector2f &pos, const Entity &dr);

    static void setCollisionManager(const std::shared_ptr<CollisionsManager<idtype, Entity>> &manager);

    static std::shared_ptr<Entity> EntitysList;
    bool inCollision;

    static bool PointInTriangle(Triangle t, const sf::Vector2f &P);

    virtual void preDrawUpdate();

private:
    sf::Color up_color;
    sf::Color down_color;
    sf::Shape &base_shape;

    std::vector<Triangle> triangles;

    void precalc_values();

    static std::shared_ptr<CollisionsManager<idtype, Entity>> collisionManager;
};


#endif // ENTITY_H

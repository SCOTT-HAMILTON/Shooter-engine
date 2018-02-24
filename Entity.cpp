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

#include "Entity.h"
#include <algorithm>

float Entity::y_axe = 100.0f;
sf::Color Entity::first_color = sf::Color(0, 0, 0);
sf::Color Entity::second_color = sf::Color(255, 255, 255);
sf::Color Entity::collisionColor = sf::Color(255, 0, 0);
std::shared_ptr<CollisionsManager<idtype, Entity>> Entity::collisionManager = nullptr;

Entity::Entity(sf::Shape &shape, const sf::Vector2f &pos, const sf::Color &up, const sf::Color &down) :
    Object(), base_shape(shape), inCollision(false), up_color(up), down_color(down)
{
    std::cout << "constructor !" << std::endl;
    this->pos = pos;
    entity_shader.loadFromFile("shader.frag", sf::Shader::Fragment);
    state.shader = &entity_shader;
    std::cout << "can Load Shaders : " << (int)sf::Shader::isAvailable() << std::endl;

    std::cout << "myid : " << myid << std::endl;
    if (myid){
        collisionManager->addObject(this, myid);
    }
    if (this->state.shader == nullptr)std::cout << "init noshader" << std::endl;
    else std::cout << "init with shader" << std::endl;
    setBase(shape);
    if (myid == 1)std::cout << "nb triangles : " << triangles.size() << std::endl;

    displayer->addObject(this);
}
Entity::~Entity()
{
}

void Entity::preDrawUpdate(){
    if (inCollision){
        entity_shader.setUniform("first_color", sf::Glsl::Vec4(collisionColor));
        entity_shader.setUniform("second_color", sf::Glsl::Vec4(collisionColor));
    }else{
        entity_shader.setUniform("first_color", sf::Glsl::Vec4(up_color));
        entity_shader.setUniform("second_color", sf::Glsl::Vec4(down_color));
    }
    entity_shader.setUniform("y_axe", y_axe);
    entity_shader.setUniform("texture", sf::Shader::CurrentTexture);
}

void Entity::drawExts(sf::RenderTarget &target){
    for (std::size_t i = 0; i < triangles.size(); i++){
        Triangle tmp_t = {
            {triangles[i].p1.x+pos.x, triangles[i].p1.y+pos.y},
            {triangles[i].p2.x+pos.x, triangles[i].p2.y+pos.y},
            {triangles[i].p3.x+pos.x, triangles[i].p3.y+pos.y}
        };
        sf::Color t_color = sf::Color(255, 0, 0, 255);
        sf::Vertex l1[] = {
            sf::Vertex(tmp_t.p1, t_color),
            sf::Vertex(tmp_t.p2, t_color)
        };

        sf::Vertex l2[] = {
            sf::Vertex(tmp_t.p2, t_color),
            sf::Vertex(tmp_t.p3, t_color)
        };

        sf::Vertex l3[] = {
            sf::Vertex(tmp_t.p3, t_color),
            sf::Vertex(tmp_t.p1, t_color)
        };

        target.draw(l1, 2, sf::Lines);
        target.draw(l2, 2, sf::Lines);
        target.draw(l3, 2, sf::Lines);

    }
}

int Entity::update(){
}

void Entity::setBase(sf::Shape &shape){
    sf::RenderTexture rend_t;
    rend_t.create(shape.getGlobalBounds().width, shape.getGlobalBounds().height);
    shape.setFillColor(sf::Color(0, 0, 255, 255));
    rend_t.clear(sf::Color::Black);
    rend_t.draw(shape);
    rend_t.display();
    base_texture = sf::Texture(rend_t.getTexture());
    sf::Image tmp_img;
    tmp_img = base_texture.copyToImage();
    tmp_img.saveToFile("img1.png");

    base_shape = shape;

    precalc_values();
}

void Entity::move(float x, float y){
    pos.x += x;
    pos.y += y;
}

bool Entity::Collide(const Entity &dr1, const Entity &dr2){
    std::vector<Line> dr1lines;

    if (!dr1.base_shape.getGlobalBounds().intersects(dr2.base_shape.getGlobalBounds()))return false;

    for (int i = 0; i < dr1.base_shape.getPointCount(); i++){
        sf::Vector2f pt(dr1.base_shape.getPoint(i).x+dr1.pos.x, dr1.base_shape.getPoint(i).y+dr1.pos.y);
        if (ptInside(pt, dr2)){
            return true;
        }
        sf::Vector2f p1, p2;
        p1 = dr1.base_shape.getPoint(i);
        if (i == dr1.base_shape.getPointCount()-1){
            p2 = dr1.base_shape.getPoint(0);
        }else{
            p2 = dr1.base_shape.getPoint(i+1);
        }
        dr1lines.push_back({{p1.x+dr1.pos.x, p1.y+dr1.pos.y}, {p2.x+dr1.pos.x, p2.y+dr1.pos.y}});

    }

    for (int i = 0; i < dr2.base_shape.getPointCount(); i++){
        sf::Vector2f pt(dr2.base_shape.getPoint(i).x+dr2.pos.x, dr2.base_shape.getPoint(i).y+dr2.pos.y);
        if (ptInside(pt, dr1)){
            return true;
        }
        sf::Vector2f p1, p2;
        p1 = dr2.base_shape.getPoint(i);
        if (i == dr2.base_shape.getPointCount()-1){
            p2 = dr2.base_shape.getPoint(0);
        }else{
            p2 = dr2.base_shape.getPoint(i+1);
        }
        Line l = {{p1.x+dr2.pos.x, p1.y+dr2.pos.y}, {p2.x+dr2.pos.x, p2.y+dr2.pos.y}};
        for (int j = 0; j < dr1lines.size(); j++){
            if (segTouchSeg(l, dr1lines[j]))return true;
        }
    }

    return false;
}

bool Entity::segTouchSeg(const Line &line1, const Line &line2){
    int a , b, c, d;
    a = static_cast<int>(detOfP2Line(line1, line2.p1));
    b = static_cast<int>(detOfP2Line(line1, line2.p2));

    c = static_cast<int>(detOfP2Line(line2, line1.p1));
    d = static_cast<int>(detOfP2Line(line2, line1.p2));

    if (a!=0)a/=std::abs(a);
    if (b!=0)b/=std::abs(b);
    if (c!=0)c/=std::abs(c);
    if (d!=0)d/=std::abs(d);

    if ((a == 0 && b == 0) || (c == 0 && d == 0))return false;

    if ( std::abs(a+b) == 0 && std::abs(c+d) == 0){
        return true;
    }
    return false;
}

float Entity::detOfP2Line(const Line &l, const sf::Vector2f &p){
    sf::Vector2f D, T;
    D.x = l.p2.x-l.p1.x;
    D.y = l.p2.y-l.p1.y;
    T.x = p.x-l.p1.x;
    T.y = p.y-l.p1.y;
    return D.x*T.y-D.y*T.x;
}

bool Entity::PointInTriangle(Triangle t, const sf::Vector2f &P)
{
    float a = detOfP2Line({t.p1, t.p2}, P);
    float b = detOfP2Line({t.p2, t.p3}, P);
    float c = detOfP2Line({t.p3, t.p1}, P);

    if (a != 0){
        if (a<0)return b < 0 && c < 0;
        else return b > 0 && c > 0;
    }else if (b != 0){
        if (c == 0) return true;
        return ((b > 0 && c > 0) || (b < 0 && c < 0));
    }
    return true;
}

bool Entity::ptInside(const sf::Vector2f &pPos, const Entity &dr){
    int nb_in = 0;
    for (std::size_t i = 0; i < dr.triangles.size(); i++){
        Triangle tmp_t = {
            {dr.triangles[i].p1.x+dr.pos.x, dr.triangles[i].p1.y+dr.pos.y},
            {dr.triangles[i].p2.x+dr.pos.x, dr.triangles[i].p2.y+dr.pos.y},
            {dr.triangles[i].p3.x+dr.pos.x, dr.triangles[i].p3.y+dr.pos.y}
        };
        if ( PointInTriangle(tmp_t, pPos) ) return true;
    }
    return false;
}

void Entity::updateCollisionManager(){
    collisionManager->update();
}

std::size_t sommet_gauche(const std::vector<sf::Vector2f> &polygone){
    std::size_t index;
    float min_x;
    for (std::size_t i = 0; i < polygone.size(); i++){
        if (i == 0 || polygone[i].x<min_x){
            min_x = polygone[i].x;
            index = i;
        }
    }
    return index;
}

std::size_t voisin_sommet(int n, int i, int di){
    int result = (i+di)%n;
    while (result<0)result+=n;
    return result;
}

int sommet_distance_maximale(const std::vector<sf::Vector2f> &polygone, const sf::Vector2f &P0, const sf::Vector2f &P1, const sf::Vector2f &P2, const std::vector<std::size_t> &indices){
    float distance = 0.0f;
    int j = -1;
    for (std::size_t i = 0; i < polygone.size(); i++){
        if (std::find(indices.begin(), indices.end(), i) == indices.end()){
            sf::Vector2f M = polygone[i];
            if (Entity::PointInTriangle({P0,P1,P2}, M)){
                float d = std::abs( Entity::detOfP2Line({P1,P2},M) );
                if (d > distance){
                    distance = d;
                    j = i;
                }
            }
        }
    }
    return j;
}

std::vector<sf::Vector2f> nouveau_polygone(const std::vector<sf::Vector2f> &polygone, std::size_t i_debut, std::size_t i_fin){
    std::vector<sf::Vector2f> p;
    std::size_t i = i_debut;
    while (i!=i_fin){
        p.push_back(polygone[i]);
        i = voisin_sommet(polygone.size(),i,1);
    }
    p.push_back(polygone[i_fin]);
    return p;
}

void trianguler(const std::vector<sf::Vector2f> &polygone, std::vector<Triangle> &triangles){

    std::size_t n = polygone.size();
    std::size_t j0 = sommet_gauche(polygone);
    std::size_t j1 = voisin_sommet(n,j0,1);
    std::size_t j2 = voisin_sommet(n,j0,-1);
    sf::Vector2f P0 = polygone[j0];
    sf::Vector2f P1 = polygone[j1];
    sf::Vector2f P2 = polygone[j2];
    std::vector<std::size_t> indices;
    indices.clear();

    indices.push_back(j0);indices.push_back(j1);indices.push_back(j2);
    int j = sommet_distance_maximale(polygone,P0,P1,P2,indices);
    std::vector<sf::Vector2f> polygone_1, polygone_2;

    if (j == -1){

        triangles.push_back({P0,P1,P2});
        polygone_1 = nouveau_polygone(polygone,j1,j2);
        if (polygone_1.size() == 3){
            triangles.push_back({polygone_1[0], polygone_1[1], polygone_1[2]});
        }else{
            trianguler(polygone_1, triangles);
        }
    }else{
        polygone_1 = nouveau_polygone(polygone,j0,j);
        polygone_2 = nouveau_polygone(polygone,j,j0);
        if (polygone_1.size() == 3){
            triangles.push_back({polygone_1[0], polygone_1[1], polygone_1[2]});
        }else{
            trianguler(polygone_1, triangles);
        }if (polygone_2.size() == 3){
            triangles.push_back({polygone_2[0], polygone_2[1], polygone_2[2]});
        }
        else{
            trianguler(polygone_2, triangles);
        }
    }
}

void Entity::precalc_values() {

    if (myid != 0){
        triangles.clear();
        std::vector<sf::Vector2f> polygone;
        for (std::size_t i = 0; i < base_shape.getPointCount(); i++){
            polygone.push_back(base_shape.getPoint(i));
        }
        trianguler(polygone, triangles);
        std::cout << "nb_triangles : " << triangles.size() << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << "" << std::endl;
    }
}

void Entity::setCollisionManager(const std::shared_ptr<CollisionsManager<idtype, Entity>> &manager){
    collisionManager = manager;
}

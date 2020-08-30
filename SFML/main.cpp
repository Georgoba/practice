#include <iostream>
#include <vector> 
#include <cmath>
#define NOMINMAX
#include <Windows.h> 
#include <SFML\Graphics.hpp> 
#include <SFML\System\Clock.hpp> 


using namespace sf;
struct ball {
    Vector2f pos;
    Vector2f vel;
    //void move(float dt); 
};
float dot(Vector2f a, Vector2f b) {
    return a.x * b.x + a.y * b.y;
}
float len(Vector2f v) {
    return sqrt(v.x * v.x + v.y * v.y);
}
float len2(Vector2f v) {
    return v.x * v.x + v.y * v.y;
}
void collide(ball& a, ball& b) {
    Vector2f collision = a.pos - b.pos;
    float distance = len(collision);
    if (distance == 0.0f) { // hack to avoid div by zero 
        collision = Vector2f(1.0f, 0.0f);
        distance = 1.0f;
    }
    collision = collision / distance;
    float aci = dot(a.vel, collision);
    float bci = dot(b.vel, collision);
    float acf = bci;
    float bcf = aci;
    a.vel += (acf - aci) * collision;
    b.vel += (bcf - bci) * collision;
}
std::vector<ball> units;
namespace geo {
    const int width = 750;
    const int height = 500;
}
int main() {
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(geo::width, geo::height), "balls");
    sf::CircleShape img;
    sf::Clock clock;
    const double M_PI = 3.14;
    for (int i = 0; i < 30; ++i) {
        float alfa = rand() * M_PI / RAND_MAX;
        units.push_back({ { rand() % geo::width * 1.0f, rand() % geo::height * 1.0f }, {0.1f * sin(alfa), 0.1f * cos(alfa) } });
    }
    img.setRadius(10.0f);
    img.setFillColor(sf::Color::Green);
    while (window.isOpen()) {
        window.clear();
        for (auto bunit : units) {
            img.setPosition(bunit.pos);
            window.draw(img);
        }
        window.display();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        Time dt = clock.restart();
        for (int i = 0; i < units.size() - 1; ++i)
            for (int j = i + 1; j < units.size(); ++j)
                if (len2(units[i].pos - units[j].pos) <= img.getRadius() * img.getRadius())
                    collide(units[i], units[j]);
        for (auto& b : units) {
            b.pos += b.vel * float(dt.asMilliseconds());
            if (b.pos.x > geo::width - img.getRadius() || b.pos.x < 0) {
                b.vel.x = -b.vel.x;
                b.pos.x += 2 * b.vel.x * float(dt.asMilliseconds());
            }
            if (b.pos.y > geo::height - img.getRadius() || b.pos.y < 0) {
                b.vel.y = -b.vel.y;
                b.pos.y += 2 * b.vel.y * float(dt.asMilliseconds());
            }
        }
        Sleep(10);
    }
    return 0;
}

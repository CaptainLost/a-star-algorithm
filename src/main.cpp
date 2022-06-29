#include "SFML/Graphics.hpp"
#include "Grid.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(512, 512), "Path finding, A* algorithm - Michal Kreft");
    window.setVerticalSyncEnabled(true);

    float zoom = .5f;
    sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(512.f * zoom, 512 * zoom));
    window.setView(view);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::Resized)
            {
                float aspectRatio = (float)window.getSize().x / (float)window.getSize().y;
                view.setSize({ 512.f * zoom * aspectRatio, 512.f * zoom });
                window.setView(view);
            }
            else if (event.type == sf::Event::MouseWheelMoved)
            {
                zoom += 0.1f * -event.mouseWheel.delta;

                if (zoom < 0.1f) zoom = 0.1f;
                else if (zoom > 2.f) zoom = 2.f;

                float aspectRatio = (float)window.getSize().x / (float)window.getSize().y;
                view.setSize({ 512.f * zoom * aspectRatio, 512.f * zoom });
                window.setView(view);
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::R)
                {
                    GridManager::Get()->ResetWalls();
                }
            }
        }

        window.clear(sf::Color(32, 32, 32, 255));
        GridManager::Update(window);
        window.display();
    }

    return 0;
}
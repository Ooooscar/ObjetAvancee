#ifndef _PIECEOUT_RENDERER
#define _PIECEOUT_RENDERER
#include "Level.hpp"
#include <SFML/Graphics.hpp>

class Renderer
{
private:
    // const std::vector<LevelData> levels;
    sf::RenderWindow window;
public:
    // Renderer(const std::vector<LevelData>& levels);
    Renderer();
    void run();
};

#endif
#ifndef _PIECEOUT_VECTOR_GRAPHICS_HELPER
#define _PIECEOUT_VECTOR_GRAPHICS_HELPER
#include <SFML/Graphics.hpp>
#include <vector>

class VectorGraphicsHelper
{
public:
    static void addSquareTo(std::vector<sf::Vertex>& vertexArray, float gridSizeInPixels, const sf::Vector2i& topLeftGridPos, const sf::Color& color);
    static void addRightPointingTrigTo(std::vector<sf::Vertex>& vertexArray, float gridSizeInPixels, const sf::Vector2i& topLeftGridPos, const sf::Color& color);
    static void addDownPointingTrigTo(std::vector<sf::Vertex>& vertexArray, float gridSizeInPixels, const sf::Vector2i& topLeftGridPos, const sf::Color& color);
    static void addLeftPointingTrigTo(std::vector<sf::Vertex>& vertexArray, float gridSizeInPixels, const sf::Vector2i& topLeftGridPos, const sf::Color& color);
    static void addTopPointingTrigTo(std::vector<sf::Vertex>& vertexArray, float gridSizeInPixels, const sf::Vector2i& topLeftGridPos, const sf::Color& color);
};

#endif
#include "VectorGraphicsHelper.hpp"

void VectorGraphicsHelper::addSquareTo(std::vector<sf::Vertex>& vertexArray, float gridSizeInPixels, const sf::Vector2i& topLeftGridPos, const sf::Color& color)
{
    sf::Vector2f topLeftPos{mapGridToPixel(topLeftGridPos)};
    float x0 = topLeftPos.x;
    float x1 = x0 + gridSizeInPixels;
    float y0 = topLeftPos.y;
    float y1 = y0 + gridSizeInPixels;
    vertexArray.emplace_back(sf::Vertex{{x0, y0}, color});
    vertexArray.emplace_back(sf::Vertex{{x1, y0}, color});
    vertexArray.emplace_back(sf::Vertex{{x0, y1}, color});
    vertexArray.emplace_back(sf::Vertex{{x1, y0}, color});
    vertexArray.emplace_back(sf::Vertex{{x0, y1}, color});
    vertexArray.emplace_back(sf::Vertex{{x1, y1}, color});
}
void VectorGraphicsHelper::addRightPointingTrigTo(std::vector<sf::Vertex>& vertexArray, float gridSizeInPixels, const sf::Vector2i& topLeftGridPos, const sf::Color& color);
void VectorGraphicsHelper::addDownPointingTrigTo(std::vector<sf::Vertex>& vertexArray, float gridSizeInPixels, const sf::Vector2i& topLeftGridPos, const sf::Color& color);
void VectorGraphicsHelper::addLeftPointingTrigTo(std::vector<sf::Vertex>& vertexArray, float gridSizeInPixels, const sf::Vector2i& topLeftGridPos, const sf::Color& color);
void VectorGraphicsHelper::addTopPointingTrigTo(std::vector<sf::Vertex>& vertexArray, float gridSizeInPixels, const sf::Vector2i& topLeftGridPos, const sf::Color& color);
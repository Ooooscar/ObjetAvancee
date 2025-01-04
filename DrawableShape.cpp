#include "DrawableShape.hpp"

DrawableShape::DrawableShape(const std::vector<sf::Vertex>& vertexArray, sf::PrimitiveType primitiveType)
    : vertexArray{vertexArray}
    , primitiveType{primitiveType}
{}
DrawableShape::DrawableShape(std::vector<sf::Vertex>&& vertexArray, sf::PrimitiveType primitiveType)
    : vertexArray{std::move(vertexArray)}
    , primitiveType{primitiveType}
{}

void DrawableShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(&vertexArray[0], vertexArray.size(), primitiveType, states);
}

void DrawableShape::acceptTransform(const sf::Transform& transform)
{
    for (sf::Vertex& vertex : vertexArray)
    {
        vertex.position = transform.transformPoint(vertex.position);
    }
}

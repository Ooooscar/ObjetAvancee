#ifndef _PIECEOUT_DRAWABLE_SHAPE
#define _PIECEOUT_DRAWABLE_SHAPE
#include <SFML/Graphics.hpp>

class DrawableShape : public sf::Drawable
{
protected:
    std::vector<sf::Vertex> vertexArray;
    sf::PrimitiveType primitiveType;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
    DrawableShape(const std::vector<sf::Vertex>& vertexArray, sf::PrimitiveType primitiveType);
    virtual void acceptTransform(const sf::Transform& transform);
};

#endif
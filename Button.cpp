#include "Button.hpp"
#include "GameStateMachine.hpp"
#include "ResourceManager.hpp"
#include <string>

Button::Button(const sf::Vector2f& worldPosTopLeft, float width, float height,
           const sf::Color& buttonColor, const sf::Color& buttonColorOnHover,
           const sf::String& buttonTextRaw, sf::Font& buttonTextFont, unsigned int buttonTextSize,
           const sf::Color& buttonTextColor, const sf::Color& buttonTextColorOnHover)
    : DrawableShape{
        std::vector<sf::Vertex>{
            {worldPosTopLeft},
            {{worldPosTopLeft.x + width, worldPosTopLeft.y}},
            {{worldPosTopLeft.x, worldPosTopLeft.y + height}},
            {{worldPosTopLeft.x + width, worldPosTopLeft.y + height}}
        },
        sf::TriangleStrip
    }
    , buttonColor{buttonColor}
    , buttonColorOnHover{buttonColorOnHover}
    , buttonText{buttonTextRaw, buttonTextFont, buttonTextSize}
    , buttonTextColor{buttonTextColor}
    , buttonTextColorOnHover{buttonTextColorOnHover}
    , mouseOver{false}
{
    buttonText.setPosition(worldPosTopLeft);
    update();
}

bool Button::contains(const sf::Vector2f& worldPos) const {
    return vertexArray[0].position.x <= worldPos.x
        && vertexArray[0].position.y <= worldPos.y
        && vertexArray[1].position.x >= worldPos.x
        && vertexArray[2].position.y >= worldPos.y;
}
void Button::onMouseHover() {
    mouseOver = true;
    update();
}
void Button::onMouseLeave() {
    mouseOver = false;
    update();
}
void Button::update() {
    if (mouseOver) {
        for (sf::Vertex& vertex : vertexArray) {
            vertex.color = buttonColorOnHover;
        }
        buttonText.setFillColor(buttonTextColorOnHover);
    } else {
        for (sf::Vertex& vertex : vertexArray) {
            vertex.color = buttonColor;
        }
        buttonText.setFillColor(buttonTextColor);
    }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    DrawableShape::draw(target, states);
    target.draw(buttonText, sf::RenderStates(states.transform.combine(buttonTextTransform)));
}
void Button::acceptTransform(const sf::Transform& transform) {
    DrawableShape::acceptTransform(transform);
    buttonTextTransform.combine(transform);
}


ButtonHello::ButtonHello(const sf::Vector2f& worldPosTopLeft)
    : Button{
        worldPosTopLeft, 1000.f, 300.f,
        sf::Color(0x0000FFFF), sf::Color(0x00FF00FF),
        L"Je veux jouer !", ResourceManager::getInstance().getTextFont(), 96U,
        sf::Color::Yellow, sf::Color::White
    }
{}
GameState* ButtonHello::onMouseClick() {
    return nullptr;
}
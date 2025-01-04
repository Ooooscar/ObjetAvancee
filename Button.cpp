#include "Button.hpp"
#include "GameStateMachine.hpp"
#include "ResourceManager.hpp"
#include <string>

Button::Button(const sf::Vector2f& topLeftWorldPos, float width, float height,
           const sf::Color& buttonColor, const sf::Color& buttonColorOnHover,
           const sf::String& buttonTextRaw, sf::Font& buttonTextFont, unsigned int buttonTextSize,
           const sf::Color& buttonTextColor, const sf::Color& buttonTextColorOnHover)
    : DrawableShape{
        std::vector<sf::Vertex>{
            {topLeftWorldPos},
            {{topLeftWorldPos.x + width, topLeftWorldPos.y}},
            {{topLeftWorldPos.x, topLeftWorldPos.y + height}},
            {{topLeftWorldPos.x + width, topLeftWorldPos.y + height}}
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
    updateTextPosition();
    updateColor();
}

bool Button::contains(const sf::Vector2f& worldPos) const {
    return vertexArray[0].position.x <= worldPos.x
        && vertexArray[0].position.y <= worldPos.y
        && vertexArray[1].position.x >= worldPos.x
        && vertexArray[2].position.y >= worldPos.y;
}
void Button::onMouseHover() {
    mouseOver = true;
    updateColor();
}
void Button::onMouseLeave() {
    mouseOver = false;
    updateColor();
}
void Button::updateColor() {
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
void Button::updateTextPosition() {
    sf::Vector2f textCenter{buttonText.getGlobalBounds().getSize() / 2.f};
    sf::Vector2f textLocalBounds{textCenter + buttonText.getLocalBounds().getPosition()};
    buttonText.setOrigin(textLocalBounds);
    sf::Vector2f rectCenter{(vertexArray[1].position.x - vertexArray[0].position.x) / 2.f,
                            (vertexArray[2].position.y - vertexArray[0].position.y) / 2.f};
    buttonText.setPosition(rectCenter);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    DrawableShape::draw(target, states);
    target.draw(buttonText, sf::RenderStates(states.transform.combine(buttonTextTransform)));
}
void Button::acceptTransform(const sf::Transform& transform) {
    DrawableShape::acceptTransform(transform);
    buttonTextTransform.combine(transform);
}


ButtonHello::ButtonHello(const sf::Vector2f& topLeftWorldPos)
    : Button{
        topLeftWorldPos, 1000.f, 300.f,
        sf::Color(0x0000FFFF), sf::Color(0x00FF00FF),
        L"Je veux jouer !", ResourceManager::getInstance().getTextFont(), 96U,
        sf::Color::Yellow, sf::Color::White
    }
{}
GameState* ButtonHello::onMouseClick() {
    return nullptr;
}
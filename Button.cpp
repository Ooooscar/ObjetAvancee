#include "Button.hpp"
#include "GameStateMachine.hpp"
#include "LevelManager.hpp"
#include "ResourceManager.hpp"

sf::Color Button::buttonTextColor{0xFFFFFCFF};

Button::Button(const sf::Vector2f& topLeftWorldPos, float width, float height,
            const sf::String& buttonTextRaw, sf::Font& buttonTextFont, unsigned int buttonTextSize,
            const sf::Color& buttonColor = sf::Color{0x333333FF},
            const sf::Color& buttonColorOnHover = sf::Color{0x444444FF},
            const sf::Color& buttonColorOnClick = sf::Color{0x555555FF}
        )
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
    , buttonColorOnClick{buttonColorOnClick}
    , buttonText{buttonTextRaw, buttonTextFont, buttonTextSize}
    , mouseOver{false}
{
    onMouseLeave();
}

bool Button::contains(const sf::Vector2f& worldPos) const {
    return vertexArray[0].position.x <= worldPos.x
        && vertexArray[0].position.y <= worldPos.y
        && vertexArray[1].position.x >= worldPos.x
        && vertexArray[2].position.y >= worldPos.y;
}

void Button::onMouseLeave() {
    mouseOver = false;
    buttonText.setStyle(sf::Text::Regular);
    updateTextPosition();
    for (sf::Vertex& vertex : vertexArray) {
        vertex.color = buttonColor;
    }
}
void Button::onMouseEnter() {
    mouseOver = true;
    buttonText.setStyle(sf::Text::Bold);
    updateTextPosition();
    for (sf::Vertex& vertex : vertexArray) {
        vertex.color = buttonColorOnHover;
    }
}
void Button::onMouseDown() {
    mouseOver = true;
    updateColor();
    for (sf::Vertex& vertex : vertexArray) {
        vertex.color = buttonColorOnClick;
    }
}
void Button::updateColor() {
    if (mouseOver) {
        for (sf::Vertex& vertex : vertexArray) {
            vertex.color = buttonColorOnHover;
        }
    } else {
        for (sf::Vertex& vertex : vertexArray) {
            vertex.color = buttonColor;
        }
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
        L"Je veux jouer !", ResourceManager::getInstance().getTextFont(), 96U
    }
{}
GameState* ButtonHello::activate() {
    return new LevelStateIdle{
        LevelManager::getInstance().loadLevel(GameStateMachine::getContext().window, 1),
        {-1, -1}
    };
}
#include "Button.hpp"
#include "GameStateMachine.hpp"
#include "LevelManager.hpp"
#include "ResourceManager.hpp"

sf::Color Button::buttonTextColor{0xFFFFFCFF};

Button::Button(const sf::Vector2f& topLeftWorldPos, float width, float height,
            const sf::String& buttonTextRaw, sf::Font& buttonTextFont, unsigned int buttonTextSize,
            const sf::Color& buttonColor = sf::Color{0x333333FF},
            const sf::Color& buttonColorOnHover = sf::Color{0x3C3C3FFF},
            const sf::Color& buttonColorOnClick = sf::Color{0x505055FF}
        )
    : DrawableShape{
        std::vector<sf::Vertex>{
            {topLeftWorldPos, buttonColor},
            {{topLeftWorldPos.x + width, topLeftWorldPos.y}, buttonColor},
            {{topLeftWorldPos.x + width, topLeftWorldPos.y + height}, buttonColor},
            {{topLeftWorldPos.x, topLeftWorldPos.y + height}, buttonColor}
        },
        sf::TriangleFan
    }
    , buttonColor{buttonColor}
    , buttonColorOnHover{buttonColorOnHover}
    , buttonColorOnClick{buttonColorOnClick}
    , buttonText{buttonTextRaw, buttonTextFont, buttonTextSize}
    , mouseOver{false}
{
    updateTextPosition();
}

/**
 * `Button` hérite de la classe `DrawableShape`, donc il faut que cette méthode fonctionnne
 * même si on applique des transformations affines sur le bouton;
 * l'algorithme suivant est adapté de `https://wrfranklin.org/Research/Short_Notes/pnpoly.html`.
 */
bool Button::contains(const sf::Vector2f& worldPos) const {
    bool res = 0;
    float vi_x, vi_y, vj_x, vj_y;
    float p_x = worldPos.x;
    float p_y = worldPos.y;

    for (int i = 0, j = 3; i < 4; j = i++) {
        vi_x = vertexArray[i].position.x;
        vi_y = vertexArray[i].position.y;
        vj_x = vertexArray[j].position.x;
        vj_y = vertexArray[j].position.y;

        if (((vi_y > p_y) != (vj_y > p_y))
            && (p_x < (vj_x - vi_x) * (p_y - vi_y) / (vj_y - vi_y) + vi_x))
        {
            res = !res;
        }
    }
    return res;
}

void Button::onMouseEnter() {
    mouseOver = true;
    for (sf::Vertex& vertex : vertexArray) {
        vertex.color = buttonColorOnHover;
    }
    buttonText.setStyle(sf::Text::Bold);
    updateTextPosition();
    // acceptTransform(sf::Transform{}.rotate(5));
}
void Button::onMouseLeave() {
    mouseOver = false;
    for (sf::Vertex& vertex : vertexArray) {
        vertex.color = buttonColor;
    }
    buttonText.setStyle(sf::Text::Regular);
    // acceptTransform(sf::Transform{}.rotate(-5));
    updateTextPosition();
}
void Button::onMouseDown() {
    mouseOver = true;
    for (sf::Vertex& vertex : vertexArray) {
        vertex.color = buttonColorOnClick;
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
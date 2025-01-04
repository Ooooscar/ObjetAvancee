#ifndef _PIECEOUT_BUTTON
#define _PIECEOUT_BUTTON
#include "DrawableShape.hpp"
#include <SFML/Graphics.hpp>

class GameState;

class Button : public DrawableShape
{
private:
    sf::Color buttonColor;
    sf::Color buttonColorOnHover;
    sf::Color buttonColorOnClick;
    sf::Text buttonText;
    sf::Transform buttonTextTransform;
    static sf::Color buttonTextColor;
    bool mouseOver;
protected:
    Button(const sf::Vector2f& topLeftWorldPos, float width, float height,
        const sf::String& buttonTextRaw, sf::Font& buttonTextFont, unsigned int buttonTextSize,
        const sf::Color& buttonColor,
        const sf::Color& buttonColorOnHover,
        const sf::Color& buttonColorOnClick
    );
public:
    bool contains(const sf::Vector2f& worldPos) const;
    virtual void onMouseEnter();
    virtual void onMouseLeave();
    virtual void onMouseDown();
    virtual GameState* activate() = 0;
    void updateTextPosition();
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void acceptTransform(const sf::Transform& transform) override;
};

class ButtonHello : public Button
{
public:
    ButtonHello(const sf::Vector2f& topLeftWorldPos);
    GameState* activate() override;
};

#endif
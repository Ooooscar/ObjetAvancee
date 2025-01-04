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
    sf::Text buttonText;
    sf::Transform buttonTextTransform;
    sf::Color buttonTextColor;
    sf::Color buttonTextColorOnHover;
    bool mouseOver;
protected:
    Button(const sf::Vector2f& topLeftWorldPos, float width, float height,
           const sf::Color& buttonColor, const sf::Color& buttonColorOnHover,
           const sf::String& buttonTextRaw, sf::Font& buttonTextFont, unsigned int buttonTextSize,
           const sf::Color& buttonTextColor, const sf::Color& buttonTextColorOnHover);
public:
    bool contains(const sf::Vector2f& worldPos) const;
    virtual GameState* onMouseClick() = 0;
    virtual void onMouseHover();
    virtual void onMouseLeave();
    void updateColor();
    void updateTextPosition();
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void acceptTransform(const sf::Transform& transform) override;
};

class ButtonHello : public Button
{
public:
    ButtonHello(const sf::Vector2f& topLeftWorldPos);
    GameState* onMouseClick() override;
};

#endif
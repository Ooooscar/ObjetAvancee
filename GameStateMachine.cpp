#include "GameStateMachine.hpp"
#include "Button.hpp"
#include "ResourceManager.hpp"
#include "LevelManager.hpp"
#include "Level.hpp"
#include <cmath>

GameStateMachine::GameStateMachine(sf::RenderWindow& window)
    : window{window}
{}
GameStateMachine::~GameStateMachine() {
    delete currentState;
}

GameStateMachine* GameStateMachine::context = nullptr;

void GameStateMachine::initializeStateMachine(sf::RenderWindow& window) {
    context = new GameStateMachine(window);
    context->currentState = new MainMenuState();
}
GameStateMachine& GameStateMachine::getContext() {
    if (!context) {
        std::cerr << "Erreur : la machine à états n'est pas initialisé !" << std::endl;
        std::terminate();
    }
    return *context;
}

void GameStateMachine::tick()
{
    currentState->onTick();

    sf::Vector2i mousePos{sf::Mouse::getPosition(window)};
    sf::Vector2f mouseWorldPos{window.mapPixelToCoords(mousePos)};
    currentState = currentState->onMousePositionUpdate(mouseWorldPos);

    // la gestion des événements
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed
            || (event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::Escape))
        {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed
            && event.mouseButton.button == sf::Mouse::Left)
        {
            currentState = currentState->onMouseClick();
        }

        if (event.type == sf::Event::MouseButtonReleased
            && event.mouseButton.button == sf::Mouse::Left)
        {
            currentState = currentState->onMouseRelease();
        }
    }

    // l'affichage
    window.clear();
    window.setView(window.getDefaultView());
    currentState->render(window);
    window.display();
}

GameState* GameState::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) { return this; }
GameState* GameState::onMouseClick() { return this; }
GameState* GameState::onMouseRelease() { return this; }
void GameState::onTick() {}
void GameState::render(sf::RenderWindow& window) const {}


MainMenuState::MainMenuState()
    : animationTimer{}
    , title{
        [&]() -> sf::Text{
            const sf::Vector2u& windowSize{GameStateMachine::getContext().window.getSize()};
            sf::Text tmp{
                L"Pièce Out",
                ResourceManager::getInstance().getTextFont(),
                static_cast<unsigned int>(std::max(windowSize.y * 0.1f, windowSize.x * 0.01f))
            };
            sf::Vector2f textCenter{tmp.getGlobalBounds().getSize() * 0.5f};
            sf::Vector2f textLocalBounds{textCenter + tmp.getLocalBounds().getPosition()};
            tmp.setOrigin(textLocalBounds);
            tmp.setPosition({windowSize.x * 0.5f, windowSize.y * 0.3f});
            return tmp;
        }()
    }
    , buttons{
        [&]() -> const std::vector<Button*>*{
            const sf::RenderWindow& window{GameStateMachine::getContext().window};
            std::vector<Button*>* btns = new std::vector<Button*>{};
            btns->reserve(6); // TODO
            for (int levelIdx = 1; levelIdx <= static_cast<int>(btns->capacity()); ++levelIdx)
                btns->emplace_back(new ButtonLevel{window, levelIdx});
            return btns;
        }()
    }
{}
MainMenuState::MainMenuState(MainMenuState&& state)
    : animationTimer{std::move(state.animationTimer)}
    , title{std::move(state.title)}
    , buttons{std::move(state.buttons)}
{}
MainMenuState::~MainMenuState() {
    for (Button* button : *buttons) {
        delete button;
    }
    delete buttons;
}
Button* MainMenuState::getButtonAt(const sf::Vector2f& worldPos) const {
    // on veut un std::reverse_iterator, car les derniers boutons affichés se trouvent au dessus
    for (auto it = buttons->rbegin(); it != buttons->rend(); ++it) {
        if ((*it)->contains(worldPos)) {
            return *it;
        }
    }
    return nullptr;
}
void MainMenuState::onTick() {
    int elapsedMilliseconds = animationTimer.getElapsedTime().asMilliseconds();
    float k = std::sin(elapsedMilliseconds * 0.002f) * 2.0f;
    title.setRotation(k);
    float s = 1 + std::abs(k * 0.05f);
    title.setScale(s, s);
}
GameState* MainMenuState::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    Button* button = getButtonAt(mouseWorldPos);
    if (button) {
        button->onMouseEnter();
        return new MainMenuStateButtonHover(std::move(*this), button);
    }
    return this;
}
void MainMenuState::render(sf::RenderWindow& window) const {
    window.draw(title);
    for (Button* button : *buttons) {
        window.draw(*button);
    }
}

MainMenuStateButtonHover::MainMenuStateButtonHover(MainMenuState&& state, Button* currentButton)
    : MainMenuState{std::move(state)}
    , currentButton{currentButton}
{}
GameState* MainMenuStateButtonHover::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    if (currentButton->contains(mouseWorldPos)) {
        return this;
    }
    currentButton->onMouseLeave();
    GameState* newState{MainMenuState::onMousePositionUpdate(mouseWorldPos)};
    return (newState == this) ? new MainMenuState(std::move(*this)) : newState;
}
GameState* MainMenuStateButtonHover::onMouseClick() {
    currentButton->onMouseDown();
    return new MainMenuStateButtonClicked{std::move(*this), currentButton};
}

MainMenuStateButtonClicked::MainMenuStateButtonClicked(MainMenuStateButtonHover&& state, Button* currentButton)
    : MainMenuState{std::move(state)}
    , currentButton{currentButton}
{}
GameState* MainMenuStateButtonClicked::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    if (currentButton->contains(mouseWorldPos)){
        return this;
    }
    currentButton->onMouseLeave();
    GameState* newState{MainMenuState::onMousePositionUpdate(mouseWorldPos)};
    return (newState == this) ? new MainMenuState(std::move(*this)) : newState;
}
GameState* MainMenuStateButtonClicked::onMouseRelease() {
    return (currentButton) ? currentButton->activate() : new MainMenuState(std::move(*this));
}


LevelState::LevelState(Level* level, const sf::Vector2i& currentGridPos)
    : GameState{}
    , level{level}
    , currentGridPos{currentGridPos}
{}
LevelState::~LevelState() {
    delete level;
}
bool LevelState::levelContainsCurrentPos() const {
    return currentGridPos.x != -1;
}
GameState* LevelState::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    if (level->contains(mouseWorldPos)) {
        currentGridPos_f = level->mapPixelToGrid(mouseWorldPos);
        currentGridPos = sf::Vector2i{currentGridPos_f};
    } else {
        currentGridPos.x = -1;
    }
    return this;
}
void LevelState::render(sf::RenderWindow& window) const {
    window.draw(*level);
}

LevelStateIdle::LevelStateIdle(Level* level, const sf::Vector2i& currentGridPos)
    : LevelState{level, currentGridPos}
{}
GameState* LevelStateIdle::onMouseClick() {
    if (levelContainsCurrentPos()) {
        int selectedPieceIdx = level->getCurrent(currentGridPos) - 2;
        std::cout << "Clicked on Piece " << selectedPieceIdx << std::endl;
        if (selectedPieceIdx >= 0) {
            return new LevelStatePieceClicked(level, selectedPieceIdx, currentGridPos);
        }
    }
    return this;
}

LevelStatePieceClicked::LevelStatePieceClicked(Level* level, int selectedPieceIdx, const sf::Vector2i& currentGridPos)
    : LevelState{level, currentGridPos}
    , selectedPieceIdx{selectedPieceIdx}
{}
GameState* LevelStatePieceClicked::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    lastGridPos = currentGridPos;
    LevelState::onMousePositionUpdate(mouseWorldPos);
    if (!levelContainsCurrentPos()) {
        return new LevelStateIdle{level, currentGridPos};
    }
    Direction dir;
    int d = currentGridPos.x - lastGridPos.x;
    if (d == 0) {
        d = currentGridPos.y - lastGridPos.y;
        if (d == 0) {
            return this;
        } else if (d > 0) {
            dir = S;
        } else {
            dir = N;
        }
    } else if (d > 0) {
        dir = E;
    } else {
        dir = W;
    }
    std::cout << "Now start sliding, currentGridPos = (" << currentGridPos.x << ", " << currentGridPos.y << "):\n";
    switch (level->pieces[selectedPieceIdx].onMouseSlide(dir)) {
    case REJECTED:
        return new LevelStatePieceSliding(level, selectedPieceIdx, nullptr, currentGridPos, lastGridPos);
    case ACCEPTED:
        return new LevelStatePieceSliding(level, selectedPieceIdx, new Direction{dir}, currentGridPos, lastGridPos);
    default:
        return this;
    }
}
GameState* LevelStatePieceClicked::onMouseRelease() {
    level->pieces[selectedPieceIdx].onMouseActivate(currentGridPos);
    if (level->isFinished()) {
        return new MainMenuState{};
    } else {
        return new LevelStateIdle{level, currentGridPos};
    }
}

LevelStatePieceSliding::LevelStatePieceSliding(Level* level, int selectedPieceIdx, Direction* direction, const sf::Vector2i& currentGridPos, const sf::Vector2i& startingGridPos)
    : LevelStatePieceClicked{level, selectedPieceIdx, currentGridPos}
    , direction{direction}
    , startingGridPos{startingGridPos}
{}
LevelStatePieceSliding::~LevelStatePieceSliding() {
    delete direction;
}
GameState* LevelStatePieceSliding::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    if (!direction) {
        return LevelStatePieceClicked::onMousePositionUpdate(mouseWorldPos);
    }
    lastGridPos = currentGridPos;
    LevelState::onMousePositionUpdate(mouseWorldPos);
    if (!levelContainsCurrentPos()) {
        return new LevelStateIdle{level, currentGridPos};
    }
    int d;
    switch (*direction) {
    case E:
    case W:
        d = currentGridPos.x - lastGridPos.x;
        if (d == 0) {
            return this;
        } else if (d > 0) {
            *direction = E;
        } else {
            *direction = W;
        }
        break;
    case S:
    case N:
        d = currentGridPos.y - lastGridPos.y;
        if (d == 0) {
            return this;
        } else if (d > 0) {
            *direction = S;
        } else {
            *direction = N;
        }
        break;
    }
    std::cout << "Now sliding, currentGridPos = (" << currentGridPos.x << ", " << currentGridPos.y << "):\n";
    switch (level->pieces[selectedPieceIdx].onMouseSlide(*direction)) {
    case ACCEPTED:
        if (currentGridPos == startingGridPos) {
            return new LevelStatePieceSliding(level, selectedPieceIdx, nullptr, currentGridPos, currentGridPos);
        }
    default:
        break;
    }
    return this;
}
GameState* LevelStatePieceSliding::onMouseRelease() {
    if (level->isFinished()) {
        return new MainMenuState{};
    } else {
        return new LevelStateIdle{level, currentGridPos};
    }
}

#include "GameStateMachine.hpp"
#include "Button.hpp"
#include "LevelManager.hpp"
#include "Level.hpp"

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
void GameState::render(sf::RenderWindow& window) const {}

MainMenuState::MainMenuState()
    : buttons{
        new ButtonHello{{20.f, 20.f}}
    }
{}
MainMenuState::~MainMenuState() {
    for (Button* button : buttons) {
        delete button;
    }
}
GameState* MainMenuState::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    // on veut un std::reverse_iterator, car les derniers boutons affichés se trouvent au dessus
    for (auto it = buttons.rbegin(); it != buttons.rend(); ++it) {
        if ((*it)->contains(mouseWorldPos)) {
            (*it)->onMouseHover();
        }
    }
    return this;
}
GameState* MainMenuState::onMouseClick() { return this; }
GameState* MainMenuState::onMouseRelease() {
    return new LevelStateIdle{
        LevelManager::getInstance().loadLevel(GameStateMachine::getContext().window, 1),
        {-1, -1}
    };
}
void MainMenuState::render(sf::RenderWindow& window) const {
    for (Button* button : buttons) {
        window.draw(*button);
    }
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
            return new LevelStatePieceSelected(level, selectedPieceIdx, currentGridPos);
        }
    }
    return this;
}

LevelStatePieceSelected::LevelStatePieceSelected(Level* level, int selectedPieceIdx, const sf::Vector2i& currentGridPos)
    : LevelState{level, currentGridPos}
    , selectedPieceIdx{selectedPieceIdx}
{}
GameState* LevelStatePieceSelected::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
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
GameState* LevelStatePieceSelected::onMouseRelease() {
    level->pieces[selectedPieceIdx].onMouseActivate(currentGridPos);
    return new LevelStateIdle{level, currentGridPos};
}

LevelStatePieceSliding::LevelStatePieceSliding(Level* level, int selectedPieceIdx, Direction* direction, const sf::Vector2i& currentGridPos, const sf::Vector2i& startingGridPos)
    : LevelStatePieceSelected{level, selectedPieceIdx, currentGridPos}
    , direction{direction}
    , startingGridPos{startingGridPos}
{}
LevelStatePieceSliding::~LevelStatePieceSliding() {
    delete direction;
}
GameState* LevelStatePieceSliding::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    if (!direction) {
        return LevelStatePieceSelected::onMousePositionUpdate(mouseWorldPos);
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
    return new LevelStateIdle(level, currentGridPos);
}

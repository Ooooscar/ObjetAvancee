#ifndef _PIECEOUT_LEVEL_MANAGER
#define _PIECEOUT_LEVEL_MANAGER
#include <SFML/Graphics.hpp>
#include <vector>

class LevelData;
class Level;

class LevelManager
{
private:
    static const std::vector<LevelData> levelList;
    static LevelManager instance;
    
    LevelManager() = default;
    virtual ~LevelManager() = default;
    LevelManager(const LevelManager&) = delete;
    LevelManager& operator=(const LevelManager&) = delete;
public:
    static LevelManager& getInstance();
    Level* loadLevel(const sf::RenderWindow& window, int levelIdx);
};

#endif
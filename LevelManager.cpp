#include "LevelManager.hpp"
#include "Level.hpp"

LevelManager LevelManager::instance{};

LevelManager& LevelManager::getInstance() {
    return instance;
}

Level* LevelManager::loadLevel(const sf::RenderWindow& window, int levelIdx) {
    sf::Vector2f centerCoords{
        window.mapPixelToCoords(sf::Vector2i{window.getSize()}) / 2.0f
    };
    return new Level{
        levelDataList[levelIdx - 1],
        centerCoords,
        std::min(centerCoords.x, centerCoords.y) / 6.0f,
    };
}

const std::vector<LevelData> LevelManager::levelDataList{
    {
        9, 8,
        {
            1,1,1,1,1,1,1,1,1,
            1,1,0,0,1,1,0,0,1,
            1,0,0,0,0,0,0,0,1,
            1,1,1,0,0,0,0,0,1,
            1,1,1,2,0,0,3,3,1,
            1,1,2,2,2,0,0,3,1,
            1,1,1,1,1,1,0,3,1,
            1,1,1,1,1,1,1,1,1,
        },
        {
            1,1,1,1,1,1,1,1,1,
            1,1,2,0,1,1,3,3,1,
            1,2,2,2,0,0,0,3,1,
            1,1,1,0,0,0,0,3,1,
            1,1,1,0,0,0,0,0,1,
            1,1,0,4,0,0,0,0,1,
            1,1,1,1,1,1,0,0,1,
            1,1,1,1,1,1,1,1,1,
        },
        {
            {MOV_W, {1,2}},
            {MOV_E, {3,2}},
            {ROT_CCW, {2,2}},
            {MOV_N, {7,1}},
            {MOV_S, {7,3}},
            {FLP_VRT, {7,2}},
            {MOV_E, {3,5}},
            {MOV_W, {3,5}},
            {MOV_S, {3,5}},
            {MOV_N, {3,5}},
        },
        {
            PieceColor::GREEN,
            PieceColor::BLUE,
            PieceColor::RED,
        }
    },
};
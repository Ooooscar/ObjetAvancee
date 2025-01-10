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
        levelList[levelIdx - 1],
        centerCoords,
        std::min(centerCoords.x, centerCoords.y) / 6.0f,
    };
}

const std::vector<LevelData> LevelManager::levelList{
    { // Niveau 1
        7, 7,
        {
            1,1,1,1,1,1,1,
            1,0,0,0,0,1,1,
            1,0,0,0,0,0,1,
            1,0,0,1,0,0,1,
            1,0,0,0,0,0,1,
            1,1,0,0,0,2,1,
            1,1,1,1,1,1,1,
        },
        {
            1,1,1,1,1,1,1,
            1,2,0,0,0,1,1,
            1,0,0,0,0,0,1,
            1,0,0,1,0,0,1,
            1,0,0,0,0,0,1,
            1,1,0,0,0,0,1,
            1,1,1,1,1,1,1,
        },
        {
            {MOV_W, {1,1}},
            {MOV_E, {1,1}},
            {MOV_S, {1,1}},
            {MOV_N, {1,1}},
        },
        {
            PieceColor::RED,
        },
    },
    { // Niveau 2
        8, 5,
        {
            1,1,1,1,1,1,1,1,
            1,0,0,0,0,0,0,1,
            1,0,0,0,0,0,0,1,
            1,2,3,4,5,6,7,1,
            1,1,1,1,1,1,1,1,
        },
        {
            1,1,1,1,1,1,1,1,
            1,5,7,2,4,6,3,1,
            1,0,0,0,0,0,0,1,
            1,0,0,0,0,0,0,1,
            1,1,1,1,1,1,1,1,
        },
        {
            {MOV_E, {1,1}}, {MOV_S, {1,1}}, {MOV_W, {1,1}}, {MOV_N, {1,1}},
            {MOV_E, {2,1}}, {MOV_S, {2,1}}, {MOV_W, {2,1}}, {MOV_N, {2,1}},
            {MOV_E, {3,1}}, {MOV_S, {3,1}}, {MOV_W, {3,1}}, {MOV_N, {3,1}},
            {MOV_E, {4,1}}, {MOV_S, {4,1}}, {MOV_W, {4,1}}, {MOV_N, {4,1}},
            {MOV_E, {5,1}}, {MOV_S, {5,1}}, {MOV_W, {5,1}}, {MOV_N, {5,1}},
            {MOV_E, {6,1}}, {MOV_S, {6,1}}, {MOV_W, {6,1}}, {MOV_N, {6,1}},
        },
        {
            PieceColor::RED,
            PieceColor::ORANGE,
            PieceColor::GREEN,
            PieceColor::CYAN,
            PieceColor::BLUE,
            PieceColor::PURPLE,
        },
    },
    { // Niveau 3
        9, 5,
        {
            1,1,1,1,1,1,1,1,1,
            1,1,1,1,0,1,1,1,1,
            1,1,1,0,0,0,1,1,1,
            1,0,0,0,0,0,0,3,1,
            1,1,1,1,1,1,1,1,1,
        },
        {
            1,1,1,1,1,1,1,1,1,
            1,1,1,1,0,1,1,1,1,
            1,1,1,2,2,2,1,1,1,
            1,0,3,0,2,0,0,0,1,
            1,1,1,1,1,1,1,1,1,
        },
        {
            {MOV_E, {2,3}},
            {ROT_CW, {4,2}},
        },
        {
            PieceColor::CYAN,
            PieceColor::RED,
        },
    },
    { // Niveau 4
        7, 6,
        {
            1,1,1,1,1,1,1,
            1,0,0,1,1,1,1,
            1,0,0,0,0,1,1,
            1,0,0,0,0,0,1,
            1,0,0,0,0,2,1,
            1,1,1,1,1,1,1,
        },
        {
            1,1,1,1,1,1,1,
            1,0,0,1,1,1,1,
            1,0,0,0,0,1,1,
            1,3,3,4,4,4,1,
            1,2,3,3,4,0,1,
            1,1,1,1,1,1,1,
        },
        {
            {MOV_E, {1,4}},
            {MOV_W, {1,4}},
            {ROT_CCW, {4,3}},
            {MOV_N, {2,3}},
            {MOV_S, {2,4}},
        },
        {
            PieceColor::RED,
            PieceColor::CYAN,
            PieceColor::PURPLE,
        },
    },
    { // Niveau 5
        8, 8,
        {
            1,1,1,1,1,1,1,1,
            1,1,0,0,1,2,2,1,
            1,1,0,0,1,2,2,1,
            1,1,0,0,0,0,0,1,
            1,0,0,0,0,0,0,1,
            1,0,0,0,0,0,0,1,
            1,1,1,0,1,1,1,1,
            1,1,1,1,1,1,1,1,
        },
        {
            1,1,1,1,1,1,1,1,
            1,1,0,0,1,0,0,1,
            1,1,0,0,1,0,0,1,
            1,1,4,4,3,3,3,1,
            1,2,2,4,0,0,0,1,
            1,2,2,4,0,0,0,1,
            1,1,1,0,1,1,1,1,
            1,1,1,1,1,1,1,1,
        },
        {
            {MOV_N, {2,4}},
            {MOV_E, {2,5}},
            {MOV_S, {1,5}},
            {MOV_W, {1,4}},
            {FLP_HOR, {4,3}},
            {MOV_N, {3,3}},
            {MOV_S, {3,5}},
        },
        {
            PieceColor::CYAN,
            PieceColor::BLUE,
            PieceColor::PURPLE,
        },
    },
    { // Niveau 6
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
        },
    },
};
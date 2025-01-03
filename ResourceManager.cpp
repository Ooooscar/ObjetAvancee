#include "ResourceManager.hpp"
#include <iostream>

ResourceManager ResourceManager::instance{};

ResourceManager::~ResourceManager() {
    delete textFont;
    delete symbolsFont;
}
ResourceManager& ResourceManager::getInstance() {
    return instance;
}

const sf::Font& ResourceManager::getTextFont() {
    if (!textFont) {
        textFont = new sf::Font();
    }
    if (!(*textFont).loadFromFile("resources/fonts/Montserrat-Medium.ttf")) {
        std::cerr << "Erreur lors du chargement des polices" << std::endl;
        std::terminate();
    }
    return *textFont;
}
const sf::Font& ResourceManager::getSymbolsFont() {
    if (!symbolsFont) {
        symbolsFont = new sf::Font();
    }
    if (!(*symbolsFont).loadFromFile("resources/fonts/NotoSansSymbols2-Regular.ttf")) {
        std::cerr << "Erreur lors du chargement des polices" << std::endl;
        std::terminate();
    }
    return *symbolsFont;
}

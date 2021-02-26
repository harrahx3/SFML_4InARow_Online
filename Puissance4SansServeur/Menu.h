#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

//#include "inc.h"

class Menu
{
public:
    Menu(std::vector <std::string> elements, int selected=-1, int right=150, int left=100, int top=10, int bot=10, int middle=10);
    void draw(sf::RenderWindow& window);
    std::string selected (sf::RenderWindow& window);
    std::string clicked ();
    void setElements(std::vector <std::string> elements);


private:
    std::vector <std::string> m_elements;
    int m_selected;
    sf::Text m_text;
    sf::Font m_fonte;
    int m_right;
    int m_left;
    int m_top;
    int m_bot;
    int m_middle;
};


#endif // MENU_H

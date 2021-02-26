//#include "Menu.h"
//#include "inc.h"

using namespace std;

Menu::Menu(vector <string> elements, int selected, int right, int left, int top, int bot, int middle) : m_elements ( elements), m_selected(selected),m_right(right), m_left(left), m_top(top), m_bot(bot), m_middle(middle)
{

    if (!m_fonte.loadFromFile("Fipps-Regular.otf"))       ///charger la fonte
        std::cout<<"erreur chargement fonte \n";
    else
        m_text.setFont(m_fonte);
    m_text.setColor(sf::Color::Black);
    m_text.setCharacterSize(10);
}

void Menu::draw(sf::RenderWindow& window)
{
    this->selected(window);
    for (unsigned i=0; i<m_elements.size(); i++)
    {
        sf::RectangleShape rect( sf::Vector2f( window.getSize().x - (m_right+m_left), window.getSize().y / m_elements.size() - (m_middle*(m_elements.size()-1)/m_elements.size() + m_bot/m_elements.size() + m_top/m_elements.size())));
        rect.setPosition (sf::Vector2f (m_left,(rect.getSize().y+m_middle)*i+m_top));
        if (i == m_selected)
            rect.setFillColor(sf::Color::Blue);
        else
            rect.setFillColor(sf::Color::Red);
        window.draw(rect);

        m_text.setString( m_elements[i] );
        m_text.setPosition( sf::Vector2f (m_left+20, (rect.getSize().y+m_middle)*i+m_top + 0.5*rect.getSize().y ));
        window.draw(m_text);
    }
}

string Menu::selected (sf::RenderWindow& window)
{
    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    if ( mouse.y > m_top && mouse.y < window.getSize().y-m_bot && mouse.x > m_left && mouse.x < window.getSize().x-m_right )
    {
     ///   m_selected = ( mouse.y - ( (mouse.y) % ( window.getSize().y / m_elements.size() - m_top ) ) ) / (window.getSize().y / m_elements.size() - m_middle);
       int t = window.getSize().y / m_elements.size() - (m_middle*(m_elements.size()-1)/m_elements.size() + m_bot/m_elements.size() + m_top/m_elements.size()) + m_middle;
        m_selected = ( mouse.y - ( (mouse.y - m_top) % t )) / t;

        return m_elements[m_selected];
    }
    else
        m_selected = -1;
    return "";
}

string Menu::clicked ()
{
    if (m_selected >= 0)
        return m_elements[m_selected];
    else
        return "";
}

void Menu::setElements(vector <string> elements)
{
    m_elements = elements;
}

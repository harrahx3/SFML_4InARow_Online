//#include <SFML/Graphics.hpp>

//#include "inc.h"

#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <ctime>
#include <fstream>

#include "Grid.h"

using namespace std;


Grid::Grid (unsigned short width, unsigned short height, int radius, int separation )
{
    ///m_grid { {YELLOW}, {YELLOW}},
    m_dimensions.x = width;
    m_dimensions.y = height;
    m_radius = radius;
    m_separation = separation;
    for (int i=0; i<m_dimensions.x+6; i++)
        m_grid.push_back(vector<Players>(m_dimensions.y+6, EMPTY)); ///On ajoute une ligne de 'largueur de la grille' cases toutes vides
}

int Grid::addPiece (int column, Players playeur)      ///ajoute une piece dans la grille
{
    column+=2;
    if (column < 3 || column > m_dimensions.x+2 || m_grid [column][3] != EMPTY)
        return -1;   ///false si invalide (colonne inexistante ou pleine)
    int row = 3;
    while (m_grid [column][row+1] == EMPTY && row != m_dimensions.y+2)
        row++;
    m_grid [column][row] = playeur;
    return row-2;
}

Players Grid::checkVictory ()
{
    for (int i=0; i<m_dimensions.x+6; i++)
        for (int j=0; j<m_dimensions.y+6; j++)
        {
            if ( m_grid [i][j] != EMPTY)
            {
                if ( m_grid [i][j] == m_grid [i+1][j]  && m_grid [i][j]  == m_grid [i+2][j] && m_grid [i][j]  == m_grid [i+3][j])
                    return m_grid[i][j];
                else if ( m_grid [i][j] == m_grid [i][j+1]  && m_grid [i][j]  == m_grid [i][j+2] && m_grid [i][j]  == m_grid [i][j+3])
                    return m_grid[i][j];
                else if ( m_grid [i][j] == m_grid [i+1][j+1]  && m_grid [i][j]  == m_grid [i+2][j+2] && m_grid [i][j]  == m_grid [i+3][j+3])
                    return m_grid[i][j];
                else if ( m_grid [i][j] == m_grid [i+1][j-1]  && m_grid [i][j]  == m_grid [i+2][j-2] && m_grid [i][j]  == m_grid [i+3][j-3])
                    return m_grid[i][j];
            }
        }
    return EMPTY;
}
string Grid::drawCGrid ()
{
    string strGrid = "\n";
    for (int i=0; i<m_dimensions.x+1; i++)
        strGrid += "##";
    strGrid += "\n";
    for (int j=3; j<m_dimensions.y+3; j++)
    {
        strGrid += "#";
        for (int i=3; i<m_dimensions.x+3; i++)
        {
            switch (m_grid [i][j])
            {
            case RED :
                strGrid += "X";
                break;
            case YELLOW :
                strGrid += "O";
                break;
            case EMPTY :
                strGrid += " ";
                break;
            }
            strGrid += " ";
        }
        strGrid += "#\n";
    }
    for (int i=0; i<m_dimensions.x+1; i++)
        strGrid += "##";
    strGrid += "\n";
    return strGrid;
}

bool Grid::isFull ()
{
    for (int j=3; j<m_dimensions.y+3; j++)
    {
        for (int i=3; i<m_dimensions.x+3; i++)
        {
            if (m_grid [i][j] == EMPTY)
                return false;
        }
    }
    return true;
}

sf::Vector2i Grid::getDimensions ()
{
    return m_dimensions;
}

void Grid::drawGrid(sf::RenderWindow& window )
{
    window.clear( sf::Color::Green);
    sf::RectangleShape grille;          ///un rectangle bleu pour la grille vide
    grille.setFillColor( sf::Color::Blue );
    grille.setSize( sf::Vector2f( (2*m_radius+m_separation)*m_dimensions.x+m_separation, (2*m_radius+m_separation)*m_dimensions.y+m_separation) );
    grille.setPosition( sf::Vector2f(0,0) );
    window.draw( grille );
    for (int j=0; j<m_dimensions.y; j++)
        for ( int i=0; i<m_dimensions.x; i++)
        {
            sf::CircleShape circle(m_radius);

            switch (this->m_grid[i+3][j+3])     /// +3 a cause de la bordure autour de la grille (cf. constructeur)
            {
            case YELLOW :
                circle.setFillColor( sf::Color::Yellow );   /// ronds colorés pour les pieces

                break;

            case RED :
                circle.setFillColor( sf::Color::Red );
                break;

            default :
                circle.setFillColor( sf::Color::White );    /// et des ronds blancs qui representent les cases vides
            }

            circle.setPosition( (2*m_radius+m_separation)*i+m_separation, (2*m_radius+m_separation)*j+m_separation ); ///50=r*2+d
            window.draw( circle );

        }
}

int Grid::play(sf::Vector2i mousePosition, Players player)
{
    if ( mousePosition.x < (2*m_radius+m_separation)*m_dimensions.x+m_separation )
        if ( mousePosition.y < (2*m_radius+m_separation)*m_dimensions.y+m_separation)   ///si on a clique dans la grille
        {
            this->addPiece((mousePosition.x - (mousePosition.x % (2*m_radius+m_separation))) / (2*m_radius+m_separation) +1, player); ///on determine la colonne
            return (mousePosition.x - (mousePosition.x % (2*m_radius+m_separation))) / (2*m_radius+m_separation) +1;
        }
    return 0; ///si clique invalide on renvoi faux
}

std::vector<float> Grid::linearise(Players player)
{
    std::vector <float> a;
    for(typename std::vector<std::vector<Players> >::iterator it = m_grid.begin(); it!=m_grid.end(); ++it)
    {
        for(typename std::vector<Players>::iterator ite = (*it).begin(); ite!=(*it).end(); ++ite)
        {
            if (*ite == player)
            {
                a.push_back(1.);
            }
            else if (*ite == EMPTY)
            {
                a.push_back(0.);
            }
            else
            {
                a.push_back(-1.);
            }
            /*switch (*ite)
            {
            case player :
                a.push_back(1.);
                break;
            case EMPTY :
                a.push_back(0.);
                break;
            default :
                a.push_back(-1.);
            }*/
        }
    }
    return a;
}


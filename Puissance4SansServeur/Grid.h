#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

//#include "inc.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

enum Players {EMPTY=0, YELLOW, RED};  ///enumeration des valeurs que peut prendre le joueur et les cases
enum Mode {SINGLE, LOCAL, NETWORK, AUTO};

class Grid
{
public:
    Grid (unsigned short width = 7, unsigned short height = 6, int radius=20, int separation=10);
    int addPiece (int column, Players playeur);
    Players checkVictory ();
    std::string drawCGrid ();
    void drawGrid (sf::RenderWindow& window );
    bool isFull ();
    sf::Vector2i getDimensions ();
    int play(sf::Vector2i mousePosition, Players player);
    std::vector<float> linearise(Players player);

private:
    std::vector < std::vector <Players> > m_grid;
    sf::Vector2i m_dimensions;///dimensions de la grille
    std::vector <sf::CircleShape> m_pieces; ///tous les pions qui ont ete joué depuis le debut
    int m_radius, m_separation; ///caracteristiques de la grille (taille et espacement des cases)
};


#endif // GRID_H_INCLUDED

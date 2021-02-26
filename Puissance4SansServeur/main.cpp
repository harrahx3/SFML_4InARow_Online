//#include <SFML/Graphics.hpp>
//#include <SFML/Network.hpp>

//#include "inc.h"


#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <ctime>
#include <fstream>
/*
#include "Grid.h"
#include "Network.h"
//#include "Grid.cpp"
#include "IA.cpp"


#include "Menu.h"
#include "Menu.cpp"*/

#include "Menu.h"
#include "Grid.h"
#include "Network.h"
#include "Menu.cpp"
#include "Grid.cpp"
//#include "Network.cpp"

using namespace std;

int playAuto (Grid grid, Network& net)
{
    for (int i=0 ; i<grid.getDimensions().x ; i++)
    {
        Grid grille = grid;
        cout << "test i=" << i << endl;
        grille.addPiece(i,YELLOW);
        if (grille.checkVictory())
        {
            cout << "IA plaid " << i << endl;
            return i;
        }
        grille=grid;
        grille.addPiece(i,RED);
        if (grille.checkVictory())
        {
            cout << "IA plaid " << i << endl;
            return i;
        }
    }
    cout << "nothing : IA plaid random" <<endl;
    return (rand()%grid.getDimensions().x) +1;
}

/*
void menu()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {

        cout << menu.clicked() << endl;

        if (menu.clicked() == "NETWORK")
        {
            mode = NETWORK;
        }
        else if (menu.clicked() == "LOCAL")
        {
            mode = LOCAL;
        }
        else if (menu.clicked() == "SINGLE")
        {
            mode = SINGLE;
        }
    }
}*/

void game()
{

}
/*

int main()
{
    srand(time(0));

    sf::Text text;
    sf::Font fonte;
    if (!fonte.loadFromFile("Fipps-Regular.otf"))       ///charger la fonte
        std::cout<<"erreur chargement fonte \n";
    else
        text.setFont(fonte);
    text.setString(" Welcome in this 'puissance 4' game !!");
    text.setPosition(10,350);
    text.setColor(sf::Color::Black);
    text.setCharacterSize(10);

    cout << "Puissance 4 !!" << endl;   ///send to both players
    Grid grid (7, 6);
    Players player = YELLOW;
    Mode mode = NETWORK;

    bool turnToPlay = true;

    sf::TcpSocket opponent;
    opponent.setBlocking(false);


    /// Create the main window
    sf::RenderWindow app(sf::VideoMode(800, 600), "SFML window");
    /// Start the game loop
    while (app.isOpen())
    {
        /// Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            /// Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
            if (event.type == sf::Event::Resized)
            {
                // on met à jour la vue, avec la nouvelle taille de la fenêtre
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                app.setView(sf::View(visibleArea));
            }

        }





        /// Draw the sprite
        menu.draw(app);

        /// Update the window
        app.display();
         /// Clear screen
        app.clear();
    }

    return EXIT_SUCCESS;
}

*/


int main()
{
    srand(time(0));

    std::vector <int> layers;
    layers.push_back(7*6);
    layers.push_back(20);
    layers.push_back(7);
    Network net = Network(layers);

    sf::Text text;
    sf::Font fonte;
    if (!fonte.loadFromFile("Fipps-Regular.otf"))       ///charger la fonte
        std::cout<<"erreur chargement fonte \n";
    else
        text.setFont(fonte);
    text.setString(" Welcome in this 'puissance 4' game !!");
    text.setPosition(10,350);
    text.setColor(sf::Color::Black);
    text.setCharacterSize(10);

    /// Create the main window
    sf::RenderWindow app(sf::VideoMode(800, 600), "choose game mode window");

    cout << "Puissance 4 !!" << endl;   ///send to both players
    Grid grid (7, 6);
    Players player = YELLOW;
    Mode mode = NETWORK;

    bool turnToPlay = true;

    sf::TcpSocket opponent;
    opponent.setBlocking(false);

    vector<string> elements;
    elements.push_back("SINGLE");
    elements.push_back("NETWORK");
    elements.push_back("LOCAL");
    elements.push_back("AUTO");
    Menu menu (elements);

    while (app.isOpen())
    {
        /// Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            /// Close app : exit
            if (event.type == sf::Event::Closed)
                app.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                cout << menu.clicked() << endl;

                if (menu.clicked() == "NETWORK")
                {
                    mode = NETWORK;
                    vector<string> elem;
                    elem.push_back("AS SERVOR");
                    elem.push_back("AS CLIENT");
                    menu.setElements(elem);
                }
                else if (menu.clicked() == "LOCAL")
                {
                    mode = LOCAL;
                    app.close();
                }
                else if (menu.clicked() == "SINGLE")
                {
                    mode = SINGLE;
                    app.close();
                }
                else if (menu.clicked() == "AUTO")
                {
                    mode = AUTO;
                    app.close();
                }
                else if (menu.clicked() == "AS SERVOR")
                {
                    sf::TcpListener listener;
                    listener.listen(53000);
                    ///listener.setBlocking(true);
                    cout << "waiting for a connecton" << endl;
                    if (!listener.accept(opponent) == sf::Socket::Done)
                        cout << "connection error" << endl;
                    app.close();
                }
                else if (menu.clicked() == "AS CLIENT")
                {
                    vector<string> servorIP;
                    servorIP.push_back("127.0.0.1");
                    servorIP.push_back("192.168.1.51");
                    menu.setElements(servorIP);
                }
                else if (menu.clicked() == "127.0.0.1" || menu.clicked() == "192.168.1.51")
                {
                    opponent.connect(menu.clicked(), 53000);
                    app.close();
                }
            }

            if (event.type == sf::Event::Resized) /// on met à jour la vue, avec la nouvelle taille de la fenêtre
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                app.setView(sf::View(visibleArea));
            }
        }

        /// Clear screen
        app.clear();

        /// Draw the sprite
        menu.draw(app);

        /// Update the app
        app.display();
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "game window");

    /// Start the game loop
    while (window.isOpen())
    {
        /// Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            /// Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized)
            {
                /// on met à jour la vue, avec la nouvelle taille de la fenêtre
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && turnToPlay == true)
            {
                UINT16 column = (UINT16) grid.play(sf::Mouse::getPosition(window), player);
                if(column)
                {
                    player = ( player == YELLOW ) ? RED : YELLOW;
                    if (mode != LOCAL)
                        turnToPlay = false;
                    if (mode == NETWORK)
                    {
                        sf::Packet packet;
                        packet << column;
                        opponent.send(packet);
                    }
                }
            }
        }

        /// Clear screen
        window.clear();

        grid.drawGrid(window);

        switch (mode)
        {
        case SINGLE :
            if (player == RED)
            {
                grid.addPiece(playAuto(grid, net), player);
                player = YELLOW;
                turnToPlay = true;
            }
            break;

        case AUTO :
            grid.addPiece(net.play(grid, player), player);
            player = ( player == YELLOW ) ? RED : YELLOW;
            break;

        case NETWORK :
            sf::Packet packet;
            opponent.receive(packet);
            UINT16 column;
            packet >> column;
            if (packet)
            {
                grid.addPiece(column, player);
                player = ( player == YELLOW ) ? RED : YELLOW;
                turnToPlay = true;
            }
            break;

        }

        if(grid.checkVictory())
        {
            string winner = ( grid.checkVictory() == RED ) ? "RED" : "YELLOW" ;
            text.setString(winner + " won");
            turnToPlay = false;

            if (mode == AUTO)
            {
                net.winner(grid, grid.checkVictory());
                grid = Grid();
            }
        }
        else
        {
            string str = ( player == RED ) ? "RED" : "YELLOW";
            text.setString(str + " is playing");
        }

        /// Draw the elements
        window.draw(text);

        /// Update the window
        //system("pause");

        window.display();
    }

    return EXIT_SUCCESS;
}

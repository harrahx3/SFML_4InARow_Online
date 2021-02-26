#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Grid
{
public:
    Grid (int x=7, int y=6, int radius=20, int separation=10)   ///constructeur
    {
        m_dimensions.x = x;
        m_dimensions.y = y;
        m_radius = radius;
        m_separation = separation;
    }

    void drawEmpty ( sf::RenderWindow& window ) ///dessiner la grille vide
    {
        window.clear( sf::Color::Green);
        sf::RectangleShape grille;          ///un rectangle bleu
        grille.setFillColor( sf::Color::Blue );
        grille.setSize( sf::Vector2f( (2*m_radius+m_separation)*m_dimensions.x+m_separation, (2*m_radius+m_separation)*m_dimensions.y+m_separation) );
        grille.setPosition( sf::Vector2f(0,0) );
        window.draw( grille );
        for (int i=0; i<m_dimensions.x; i++)
            for ( int j=0; j<m_dimensions.y; j++)
            {
                sf::CircleShape emptySpace(20);           /// et des ronds blancs qui representent les cases vides
                emptySpace.setFillColor( sf::Color::White );
                emptySpace.setPosition( (2*m_radius+m_separation)*i+m_separation, (2*m_radius+m_separation)*j+m_separation ); ///50=r*2+d
                window.draw( emptySpace );
            }
    }

    void drawPieces ( sf::RenderWindow& window )    ///draw all the pieces red and yellow in the grid
    {
        for (unsigned i=0; i<m_pieces.size(); i++)
        {
            window.draw(m_pieces[i]);
        }
    }

    void addPiece ( sf::TcpSocket& socket ) ///receive a new piece from servor and add it to the grid
    {
        socket.setBlocking(true);   ///on passe en mode bloquant
        UINT32 player, x, y;
        sf::Packet packet;
        socket.receive(packet);
        packet >> player;   ///which player plaid (for the color)
        socket.receive(packet);
        packet >> x;    ///where he plaid ->column
        socket.receive(packet);
        packet >> y;       ///->row
        sf::CircleShape piece ( m_radius );
        piece.setPosition( (x-1)*(2*m_radius+m_separation)+m_separation, (y-1)*(2*m_radius+m_separation)+m_separation );
        sf::Color couleur = (player == 1) ? sf::Color::Yellow : sf::Color::Red; ///set the color of the player
        piece.setFillColor( couleur );
        socket.setBlocking(false);  ///on enleve le mode bloquant car on retourne dans la boucle principale
        m_pieces.push_back(piece);
    }

    bool play ( sf::Vector2i mousePosition, sf::TcpSocket& servor ) ///envoyer au serveur dans quelle colonne on a clique
    {
        if ( mousePosition.x < (2*m_radius+m_separation)*m_dimensions.x+m_separation )
            if ( mousePosition.y < (2*m_radius+m_separation)*m_dimensions.y+m_separation)   ///si on a clique dans la grille
            {
                UINT32 column = (UINT32) ((mousePosition.x - (mousePosition.x % (2*m_radius+m_separation))) / (2*m_radius+m_separation) +1);///on determine la colonne
                sf::Packet packet;
                cout << (int) column << endl;
                packet << column;
                servor.send( packet );///et on l'envoi
                return true;
            }
        return false; ///si clique invalide on renvoi faux
    }

private:
    sf::Vector2i m_dimensions;///dimensions de la grille
    vector <sf::CircleShape> m_pieces; ///tous les pions qui ont ete joué depuis le debut
    int m_radius, m_separation; ///caracteristiques de la grille (taille et espacement des cases)
};


int main()
{
    string str;
    sf::TcpSocket servor;
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
    bool play = false;
    bool chooseNbPlayers = false;
    const sf::IpAddress servorAdress = "127.0.0.1";
    ///const sf::IpAddress servorAdress = "192.168.1.51";

    const unsigned short servorPort = 53000;
    servor.connect(servorAdress, servorPort);   /// on se connecte au serveur
    cout << "connected" << endl;
    servor.setBlocking(false);
    Grid grid;
    sf::String message = " Welcome in this 'puissance 4' game !!";

    sf::Sound soundPiece;
    sf::SoundBuffer bufferPiece;
    if (!bufferPiece.loadFromFile("sonPiece.wav"))
        cout<<"error chargement son"<<endl;

    soundPiece.setBuffer(bufferPiece);
    sf::RenderWindow window ( sf::VideoMode( 600, 600 ), "Puissance quatre game" ); /// une fenetre
    window.setFramerateLimit(60);

    while ( window.isOpen() )
    {
        sf::Event event;
        while (window.pollEvent(event)) /// evenements
        {
            if (event.type==sf::Event::Closed)
            {
                servor.disconnect();
                window.close(); ///fermer la fenetre si on clique sur la croix
            }

            if (event.type == sf::Event::Resized)
            {
                // on met à jour la vue, avec la nouvelle taille de la fenêtre
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && play == true)
            {
                if ( grid.play(sf::Mouse::getPosition(window), servor) )    /// si c'est a nous de jouer et que le joueur clique on verifie s'il a cliquer dans la grille et on envoi au serveur la colonne choisie
                {
                    play = false;   /// ce n'est plus a nous de jouer
                    message = "the opponent is playing, please wait.";
                }
                else
                {
                    message = "error select a column please";
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Numpad1 && chooseNbPlayers == true)
            {
                sf::Packet packe;
                cout << "1 player" << endl;
                packe << (UINT32) 1;
                servor.send( packe );///et on l'envoi
                chooseNbPlayers = false;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Numpad2 && chooseNbPlayers == true)
            {
                sf::Packet packe;
                cout << "2 player" << endl;
                packe << (UINT32) 2;
                servor.send( packe );///et on l'envoi
                chooseNbPlayers = false;

            }
        }

        sf::Packet packet;
        servor.receive(packet); /// on recoit et affiche les infos du serveur a chque tour
        packet >> str;
        cout << str;
        packet.clear();

        if (str == "1 ou 2 joueurs ?")
        {
            message = "1 ou 2 joueurs ?";
            chooseNbPlayers = true;
        }
        else if (str == "Wich num column ?") ///-> si on recoit ca, le serveur demande une reponse
        {
            play = true;    /// c'est au joueur de jouer il peut cliquer sur une colonne
            message = "It's your turn to play";
            ///sendInt(servor);
        }
        else if (str == "plaid") /// ->si on recoit ca, un joueur a joue et il faut ajoute son pion
        {
            soundPiece.play();  /// on joue le son
            grid.addPiece( servor );    /// on ajoute la nouvelle piece a la grille
        }
        else if (str == "won")
        {
            servor.receive(packet);
            packet >> str;
            message = str;
        }
        else if (str == "grid is full")
        {
            message = "grid is full";
        }

        window.clear( sf::Color::Green );
        grid.drawEmpty( window );    /// on redessine la grille
        grid.drawPieces(window);    ///et tous les pions
        text.setString(message);
        window.draw(text);  ///et on affiche le texte
        window.display();
    }
    return 0;
}

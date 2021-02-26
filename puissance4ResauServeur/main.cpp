#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <ctime>

using namespace std;

enum Players {EMPTY=0, YELLOW, RED};  ///enumeration des valeurs que peut prendre le joueur et les cases

class Grid
{
public:
    Grid (unsigned short m_width = 6, unsigned short m_height = 6) :  dimensions (m_width, m_height)
    {
        ///grid { {YELLOW}, {YELLOW}},
        for (int i=0; i<dimensions.first+6; i++)
            grid.push_back(vector<Players>(dimensions.second+6, EMPTY)); ///On ajoute une ligne de 'largueur de la grille' cases toutes vides
    }

    int play (int column, Players playeur)      ///ajoute une piece dans la grille
    {
        column+=2;
        if (column < 3 || column > dimensions.first+2 || grid [column][3] != EMPTY)
            return -1;   ///false si invalide (colonne inexistante ou pleine)
        int row = 3;
        while (grid [column][row+1] == EMPTY && row != dimensions.second+2)
            row++;
        grid [column][row] = playeur;
        return row-2;
    }
    bool checkVictory ()
    {
        for (int i=0; i<dimensions.first+6; i++)
            for (int j=0; j<dimensions.second+6; j++)
            {
                if ( grid [i][j] != EMPTY)
                {
                    if ( grid [i][j] == grid [i+1][j]  && grid [i][j]  == grid [i+2][j] && grid [i][j]  == grid [i+3][j])
                        return true;
                    else if ( grid [i][j] == grid [i][j+1]  && grid [i][j]  == grid [i][j+2] && grid [i][j]  == grid [i][j+3])
                        return true;
                    else if ( grid [i][j] == grid [i+1][j+1]  && grid [i][j]  == grid [i+2][j+2] && grid [i][j]  == grid [i+3][j+3])
                        return true;
                    else if ( grid [i][j] == grid [i+1][j-1]  && grid [i][j]  == grid [i+2][j-2] && grid [i][j]  == grid [i+3][j-3])
                        return true;
                }
            }
        return false;
    }
    string drawGrid ()
    {
        string strGrid = "\n";
        for (int i=0; i<dimensions.first+1; i++)
            strGrid += "##";
        strGrid += "\n";
        for (int j=3; j<dimensions.second+3; j++)
        {
            strGrid += "#";
            for (int i=3; i<dimensions.first+3; i++)
            {
                switch (grid [i][j])
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
        for (int i=0; i<dimensions.first+1; i++)
            strGrid += "##";
        strGrid += "\n";
        return strGrid;
    }

    bool isFull ()
    {
        for (int j=3; j<dimensions.second+3; j++)
        {
            for (int i=3; i<dimensions.first+3; i++)
            {
                if (grid [i][j] == EMPTY)
                    return false;
            }
        }
        return true;
    }

    pair <int, int> getDimensions ()
    {
        return dimensions;
    }

private:
    vector < vector <Players> > grid;
    pair <int, int> dimensions;
};


class Player
{
public:
    Player(bool isIA = false) : m_isIA(isIA)
    {
        if (m_isIA==false)
        {
            sf::TcpListener listener;
            listener.listen(53000);
            Connection(listener);
        }
    }

    bool Connection(sf::TcpListener& listener)
    {
        if (listener.accept(m_socket) == sf::Socket::Done)
            return true;
        return false;
    }

    template <typename T>
    void send (T message)
    {
        if (m_isIA==false)
        {
            sf::Packet packet;
            packet << message;
            m_socket.send(packet);
        }
    }

    template <typename T>
    T receive (Grid grid)
    {
        if (m_isIA==false)
        {
            sf::Packet packet;
            T data;
            m_socket.receive(packet);
            packet >> data;
            ///return atoi( data.c_str() );
            return data;
        }
        else
        {
            return this->playAuto(grid);
        }
    }

    int playAuto (Grid grid)
    {
        for (int i=0 ; i<grid.getDimensions().first ; i++)
        {
            Grid grille = grid;
            cout << "test i=" << i << endl;
            grille.play(i,YELLOW);
            if (grille.checkVictory())
            {
                cout << "IA plaid " << i << endl;
                return i;
            }
            grille=grid;
            grille.play(i,RED);
            if (grille.checkVictory())
            {
                cout << "IA plaid " << i << endl;
                return i;
            }
        }
        cout << "nothing : IA plaid random" <<endl;
        return (rand()%grid.getDimensions().first) +1;
    }

private:
    sf::TcpSocket m_socket;
    bool m_isIA;
};

int main()
{
    srand(time(0));
    Grid grid (7,6);
    Players player = YELLOW;
    cout << "wait for a connection" << endl;
    Player client1;
    cout << "one player connected" << endl;
    client1.send("1 ou 2 joueurs ?");
    UINT32 nbPlayers = client1.receive<UINT32>(grid);

    cout << "waiting for second connection" <<endl;

    bool isIA = (nbPlayers == 2) ? false : true;
    Player client2 (isIA);

    cout << "connection second player ok" << endl;
    client1.send("connection OK. game begin\n");
    client2.send("connection OK. game begin\n");

    do
    {
        client1.send( grid.drawGrid() );
        client2.send( grid.drawGrid() );
        cout << "grid sent" << endl;
        int column;
        int row;
        do
        {
            Player* curentPlayer = (player == YELLOW) ? &client2 : &client1;
            Player* notCurentPlayer = (player == YELLOW) ? &client1 : &client2;
            curentPlayer->send("Wich num column ?");
            notCurentPlayer->send("the opponent is playing...\n");
            cout << "question sent. waiting for answer" << endl;
            column = (int) curentPlayer->receive <INT32>(grid);
            row = grid.play(column, player);
        }
        while ( row == -1 );
        UINT32 curPlayer = ( player == YELLOW ) ? 1 : 2;
        client1.send("plaid");
        client1.send(curPlayer);
        client1.send( (UINT32)column );
        client1.send( (UINT32)row);
        client2.send("plaid");
        client2.send(curPlayer);
        client2.send( (UINT32)column );
        client2.send( (UINT32)row );

        player = ( player == YELLOW ) ? RED : YELLOW;
    }
    while ( !grid.checkVictory() && !grid.isFull() );

    if (grid.isFull())
    {
        client1.send("grid is full");
        client2.send("grid is full");
    }
    else
    {
        client1.send("won");
        client2.send("won");
        string winner = ( player == YELLOW ) ? "RED" : "YELLOW";
        client1.send(winner + " won!!");
        client2.send(winner + " won!!");
        cout << winner << " won!!" <<endl;
    }
    ///}
    return 0;
}

/**bool connection (sf::TcpListener& listener, sf::TcpSocket& client1, sf::TcpSocket& client2)
{
    sf::Packet packet;
    string str;
    const unsigned short port = 53000;
    if (listener.listen(port) == sf::Socket::Done) /// listen to a port{
        if (listener.accept(client1)== sf::Socket::Done)
        {
            cout << "a connection !"<<endl;
            str = "Hello ! Welcome in this game ! Waiting for a second player";
            packet << str;
            client1.send (packet);
            packet.clear();
            if (listener.accept (client2)== sf::Socket::Done)
            {
                cout << "a second connection !" <<endl;
                str = "Hello ! Welcome in this game ! another player is connected. ready to play";
                packet << str;
                client2.send (packet);
                packet.clear();
                return true;
            }
        }
    }
    return false;

}**/

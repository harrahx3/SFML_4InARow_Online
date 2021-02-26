#include <iostream>
#include <utility>
#include <vector>

using namespace std;

enum Players {NOTHING=0, CIRCLE, CROSS};  ///enumeration des valeurs que peut prendre le joueur et les cases

class Grid
{
public:
    Grid (unsigned short m_width = 6, unsigned short m_height = 6) :  dimensions (m_width, m_height)
    {
        ///grid { {CIRCLE}, {CIRCLE}},
        for (int i=0; i<dimensions.first+6; i++)
            grid.push_back(vector<Players>(dimensions.second+6, NOTHING)); ///On ajoute une ligne de 'largueur de la grille' cases toutes vides
    }

    bool play (int column, Players playeur)      ///ajoute une piece dans la grille
    {
        column+=2;
        if (column < 3 || column > dimensions.first+2 || grid [column][3] != NOTHING)
            return false;   ///false si invalide (colonne inexistante ou pleine)
        int row = 3;
        while (grid [column][row+1] == NOTHING && row != dimensions.second+2)
            row++;
        grid [column][row] = playeur;
        return true;
    }
    bool checkVictory ()    ///test si un des joueurs a gagne
    {
        for (int i=0; i<dimensions.first+6; i++)
            for (int j=0; j<dimensions.second+6; j++)   ///verification par rapport a toutes les cases
            {
                if ( grid [i][j] != NOTHING)
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
    void drawGrid ()        ///permet de dessiner la grille
    {
        for (int i=0; i<dimensions.first+1; i++)
            cout<<"##";
        cout<<endl;
        for (int j=3; j<dimensions.second+3; j++)
        {
            cout<<"#";
            for (int i=3; i<dimensions.first+3; i++)
            {
                switch (grid [i][j])
                {
                case CROSS :
                    cout << "X";
                    break;
                case NOTHING :
                    cout << " ";
                    break;
                case CIRCLE :
                    cout << "O";
                    break;
                }
                cout<<" ";
            }
            cout<<"#";
            cout<<endl;
        }
        for (int i=0; i<dimensions.first+1; i++)
            cout<<"##";
        cout<<endl;
    }

private:
    /// Players grid [][];
    vector < vector <Players> > grid;
    pair <int, int> dimensions;
};

int main()
{
    cout << "Puissance 4 !!" << endl;   ///send to both players
    Grid grid (7, 6);
    Players playeur = CIRCLE;
    int column;

    do
    {
        grid.drawGrid();    ///send to both
        do
        {
            cout << "Wich num column ?" << endl;    ///send to current player
            cin >> column;                          ///recieve from current player
        }
        while ( !grid.play( column, playeur) );
        playeur = ( playeur == CIRCLE ) ? CROSS : CIRCLE;
    }
    while( !grid.checkVictory() );
    grid.drawGrid();    ///send to both
    string winner = ( playeur == CIRCLE ) ? "CROSS" : "CIRCLE";
    cout << winner << "won !!";     ///send to both
    return 0;
}

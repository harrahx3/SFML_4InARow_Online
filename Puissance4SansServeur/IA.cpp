//#include <SFML/Graphics.hpp>
//#include <SFML/Network.hpp>

//#include "inc.h"
/*#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <ctime>
#include <fstream>*/

//#include 'Network.h'

/*int playAuto (Grid grid, Network& net)
{*/
    /*for (int i=0 ; i<grid.getDimensions().x ; i++)
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
    return (rand()%grid.getDimensions().x) +1;*/


   /* std::vector<std::vector<float> > o = net.evaluate(grid.linearise());
    std::vector<float> out = o[o.size()-1];
    int max = 0;
    for (int i=1; i < out.size(); i++)
    {
        if (out[i] > out[max])
            max = i;
    }
net.draw();
    std::vector <float> a = std::vector<float> (7,0);
    a[3]=1;
    net.learn(grid.linearise(), a, 10);
net.draw();
    return max+1;

}*/

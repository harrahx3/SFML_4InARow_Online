#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

//#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Grid.h"

class Network
{
public:
    Network(std::vector <int> dim);
    std::vector<std::vector <float> > evaluate(std::vector <float> x);
    float activation (float z);
    float derivatedActivation (float z);
    float inversedActivation (float a);
    void learn(std::vector <float> x, std::vector <float> y, float learningRate);
    std::vector <std::vector<float> > backPropagation(std::vector <float> y, std::vector<std::vector <float> > a);
    //std::vector<float> quadraticCost(std::vector <float> a, std::vector <float> y);
    //std::vector<float> derivatedQuadraticCost(std::vector <float> a, std::vector <float> y);
    float quadraticCost (float a, float y);
    float derivatedQuadraticCost (float a, float y);
    void draw();
    int play(Grid grid, Players player);
    void winner(Grid grid, Players player);

private:
    std::vector < std::vector <float> > m_bias;
    std::vector < std::vector < std::vector <float> > > m_weights;
    std::vector <int> m_dim;
    int m_previous;


};


#endif // NETWORK_H_INCLUDED

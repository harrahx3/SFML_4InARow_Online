#include "Network.h"

#include <vector>
#include <math.h>
#include <iostream>

//#include "inc.h"


using namespace std;

Network::Network(std::vector <int> dim) : m_dim(dim)
{
    for (int i=1; i<m_dim.size(); i++)
    {
        m_bias.push_back(vector<float>(m_dim[i], 0));
        m_weights.push_back(vector< vector<float> >(m_dim [i], std::vector<float>(m_dim[i-1],1)));
    }
    /*for(typename vector<int>::iterator it = m_dim.begin()+1; it!=m_dim.end(); ++it)
    {
        m_bias.push_back(vector<float>(*it, 0));
        m_weights.push_back(vector< vector<float> >(*it, std::vector<float>(*(it-1),1)));
    }*/
}

std::vector <std::vector<float> > Network::evaluate(std::vector<float> x)
{
    std::vector <std::vector <float> > a; //= std::vector <std::vector <float> >(1,x);
    a.push_back(x);

    //for(typename vector<int>::iterator it = m_dim.begin()+1; it!=m_dim.end(); ++it)
    for (int i=1; i<m_dim.size(); i++)
    {
        std::vector<float> newAct;
        for (int j=0; j<m_dim[i]; j++)
        {
            float z = m_bias[i-1][j];
            for (int k=0; k<m_dim[i-1]; k++)
            {
                z += m_weights[i-1][j][k] * a[i-1][k];
            }
            newAct.push_back(activation(z));
        }
        a.push_back(newAct);
    }

    return a;
}

float Network::activation (float z)
{
    return 1/(1+exp(-z));
}

float Network::derivatedActivation (float z)
{
    return exp(-z)/((1+exp(-z))*(1+exp(-z)));
}

float Network::inversedActivation (float a)
{
    return -log(1/a -1);
}


void Network::learn(std::vector <float> x, std::vector <float> y, float learningRate)
{
    std::vector <std::vector<float> > a = evaluate(x);
    std::vector <std::vector<float> > gradient = backPropagation(y, a);
    for (int i=1; i<m_dim.size(); i++)
    {
        for (int j=0; j<m_dim[i]; j++)
        {
            m_bias[i-1][j] -= learningRate * gradient[i-1][j];
            for (int k=0; k<m_dim[i-1]; k++)
            {
                m_weights[i-1][j][k] -= learningRate * gradient[i-1][j] * a[i][j];
            }
        }
    }
}

std::vector <std::vector<float> > Network::backPropagation(std::vector <float> y, std::vector<std::vector <float> > a)
{
    std::vector <std::vector<float> > gradient;
    for (int i=1; i<m_dim.size(); i++)
    {
        gradient.push_back(vector<float>(m_dim[i], 0));
    }

    for (int i=0; i<m_dim[m_dim.size()-1]; i++)
    {
        gradient [m_dim.size()-2][i] = derivatedQuadraticCost(a[a.size()-1][i], y[i]) * derivatedActivation(inversedActivation(a[a.size()-1][i]));
    }

    for (int i=m_dim.size()-2; i>0; i--)
    {
        for (int j=0; j<m_dim[i]; j++)
        {
            float add = 0;
            for (int k=0; k<m_dim[i+1]; k++)
            {
                add += gradient[i][k] * m_weights[i][k][j];
            }
            gradient[i-1][j] = add * derivatedActivation(inversedActivation(a[i][j]));
        }
    }
    return gradient;
}

/*std::vector<float> Network::quadraticCost(std::vector <float> a, std::vector <float> y)
{
    std::vector<float> cost;std::vector <float> y, std::vector<std::vector <float> > a
    for (int i=0; i<a.size(); i++)
    {
        float c = (y[i] - a[i])*(y[i] - a[i]) / 2;
        cost.push_back(c);
    }std::vector <float> y, std::vector<std::vector <float> std::vector <float> y, std::vector<std::vector <float> > a> a
    return c;std::vector <float> y, std::vector<std::vector <float> > a
}std::vector <float> y,std::vector <float> y, std::vector<std::vector <float> > a std::vector<std::vector <float> > a
std::vector<float> Network::derivatedQuadraticCosstd::vector <float> y, std::vector<std::vector <float> > at(std::vector <float> a, std::vector <float> y)
{std::vector <float> y, std::vstd::vector <float> y, std::vector<std::vector <float> > aector<std::vector <float> > astd::vector <float> y, std::vector<std::vector <float> > a
    std::vector<float> cost;std::vector <float> y, std::vector<sstd::vector <float> y, std::vector<std::vector <float> > atd::vector <float> > a
    for (int i=0; i<a.size(); i++)
    {std::vector <floastd::vector <float> y, std::vector<std::vector <float> > at> y, std::vector<std::vector <float> > std::vecstd::vector <float> y, std::vector<std::vector <float> > atostd::vector <float> y, std::vector<std::vector <float> > ar <float> y, std::vector<std::vector <float> > aa
        float c = (a[i] - y[std::vector <float> y, std::vector<std::vector <float> > ai]);std::vector <float> y, std::vector<std::vector <float> > a
        cost.push_back(c);std::vector <float> y, std::vstd::vector <float> y, std::vector<std::vector <float> > aector<std::vector <float> > a
    }std::vector <float> std::vector <float> y, std::vector<std::vector <float> > ay, std::vector<std::vectstd::vector <float> y, std::vector<std::vector <float> > aor <float> std::vector <float> y, std::vector<std::vector <float> > a> a
    return c;std::vector <float> y, std::vector<std::vector <float> > a
}*/

float Network::quadraticCost(float a, float y)
{
    return 1/2 * (y-a) *(y-a);
    //std::vector <float> y, std::vector<std::vector <float> > a
}
//std::vector <float> y, std::vector<std::vector <float> > a
float Network::derivatedQuadraticCost (float a, float y)
{
    return a-y;
}

void Network::draw()
{
    for(typename std::vector<std::vector<float> >::iterator it = m_bias.begin(); it!=m_bias.end(); ++it)
    {
        for(typename std::vector<float>::iterator ite = (*it).begin(); ite!=(*it).end(); ++ite)
            std::cout<<*ite<<std::endl;
    }
}

int Network::play(Grid grid, Players player)
{
    std::vector<std::vector<float> > o = evaluate(grid.linearise(player));
    std::vector<float> out = o[o.size()-1];
    std::vector<int> maxs;
    int max = 0;
    for (int i=0; i < out.size(); i++)
    {
        if (out[i] > out[max] + .5)
            max = i;
    }
    for (int i=0; i < out.size(); i++)
    {
        if (out[i] > out[max] - .5)
            maxs.push_back(i);
    }
    max = maxs[rand()%maxs.size()];

    draw();
   /* std::vector <float> a = std::vector<float> (7,0);
    a[3]=1;
    learn(grid.linearise(player), a, 10);
    draw();*/

    m_previous = max;
    return max+1;
}

void Network::winner(Grid grid, Players player)
{
    std::vector <float> a = std::vector<float> (7,0);
    a[m_previous]=1;
    learn(grid.linearise(player), a, 10);
}

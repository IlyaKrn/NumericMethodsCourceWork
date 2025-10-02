#include "../include/InterpolatedFunctions.h"
#include <fstream>

using namespace std;

vector<node> getNodes(string filePath){
    vector<node> nodes;
    ifstream file(filePath);
    long double x, y;
    while (file >> x >> y){
        nodes.push_back({x / 10, - y / 10});
    }
    return nodes;
}

vector<long double> linearInterpolation(long double x, vector<node> nodes){
    vector<long double> values;

    for (int i = 0; i < nodes.size() - 1; ++i){
        if (min(nodes[i].x, nodes[i + 1].x) <= x && max(nodes[i].x, nodes[i + 1].x) > x){
            values.push_back(((x - nodes[i].x) * (nodes[i + 1].y - nodes[i].y)) / (nodes[i + 1].x - nodes[i].x) + nodes[i].y);
        }
    }
    return values;
}

vector<long double> lagrangeInterpolation(long double x, vector<node> nodes){
    vector<long double> values;

    long double summValue = 0;
    for (int i = 0; i < nodes.size(); ++i) {
        long double proizv = 1;
        for (int j = 0; j < nodes.size(); ++j) {
            if(j != i && nodes[i].x != nodes[j].x){
                proizv *= (x - nodes[j].x) / (nodes[i].x - nodes[j].x);
            }
        }

        summValue += nodes[i].y * proizv;
    }
    values.push_back(summValue);
    return values;
}

vector<long double> newtonInterpolation(long double x, vector<node> nodes){

}
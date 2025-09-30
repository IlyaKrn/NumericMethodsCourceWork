#include "../include/InterpolatedFunctions.h"
#include <fstream>

using namespace std;

vector<node> getNodes(string filePath){
    vector<node> nodes;
    ifstream file(filePath);
    float x, y;
    while (file >> x >> y){
        nodes.push_back({x / 10, - y / 10});
    }
    return nodes;
}

vector<float> linearInterpolation(float x, vector<node> nodes){
    vector<float> values;

    for (int i = 0; i < nodes.size() - 1; ++i){
        if (min(nodes[i].x, nodes[i + 1].x) <= x && max(nodes[i].x, nodes[i + 1].x) > x){
            float val = ((x - nodes[i].x) * (nodes[i + 1].y - nodes[i].y)) / (nodes[i + 1].x - nodes[i].x) + nodes[i].y;
            values.push_back(val);
        }
    }
    return values;
}

vector<float> lagrangeInterpolation(float x, vector<node> nodes){

}

vector<float> newtonInterpolation(float x, vector<node> nodes){

}
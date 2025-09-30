#include <cstdlib>
#include "../include/InterpolatedFunctions.h"

using namespace std;

vector<float> linearInterpolation(float x, vector<node> nodes){
    vector<float> values;

    for (int i = 0; i < nodes.size() - 1; ++i){
        if (nodes[i].x <= x && nodes[i + 1].x > x){
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
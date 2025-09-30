#pragma once
#include <vector>


struct node {
    float x;
    float y;
};

std::vector<float> linearInterpolation(float x, std::vector<node> nodes);
std::vector<float> lagrangeInterpolation(float x, std::vector<node> nodes);
std::vector<float> newtonInterpolation(float x, std::vector<node> nodes);
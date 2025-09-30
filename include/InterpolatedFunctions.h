#pragma once
#include <vector>
#include <string>


struct node {
    float x;
    float y;
};

std::vector<node> getNodes(std::string filePath);

std::vector<float> linearInterpolation(float x, std::vector<node> nodes);
std::vector<float> lagrangeInterpolation(float x, std::vector<node> nodes);
std::vector<float> newtonInterpolation(float x, std::vector<node> nodes);
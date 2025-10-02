#pragma once
#include <vector>
#include <string>


struct node {
    long double x;
    long double y;
};

std::vector<node> getNodes(std::string filePath);

std::vector<long double> linearInterpolation(long double x, std::vector<node> nodes);
std::vector<long double> lagrangeInterpolation(long double x, std::vector<node> nodes);
std::vector<long double> newtonInterpolation(long double x, std::vector<node> nodes);
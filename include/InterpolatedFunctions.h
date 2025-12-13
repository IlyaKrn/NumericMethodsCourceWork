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
std::vector<long double> splineInterpolation(long double x, std::vector<node> nodes);
std::vector<long double> diffForward(long double x, std::vector<node> nodes);
std::vector<long double> diffBackward(long double x, std::vector<node> nodes);
std::vector<long double> diffCentral(long double x, std::vector<node> nodes);
std::vector<long double> hdiffForward(long double x, std::vector<node> nodes);
std::vector<long double> hdiffBackward(long double x, std::vector<node> nodes);
std::vector<long double> hdiffCentral(long double x, std::vector<node> nodes);
std::vector<long double> integralLeft(long double x, std::vector<node> nodes);
std::vector<long double> integralRight(long double x, std::vector<node> nodes);
std::vector<long double> integralCentral(long double x, std::vector<node> nodes);
std::vector<long double> integralTrap(long double x, std::vector<node> nodes);
std::vector<long double> integralSimpson(long double x, std::vector<node> nodes);
std::vector<long double> splineAndDiffInterpolation(long double x, std::vector<node> nodes);

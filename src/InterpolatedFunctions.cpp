#include <cstdlib>
#include <valarray>
#include "../include/InterpolatedFunctions.h"

float* linearInterpolation(float x, float* nodes, int nodeCount, int* valuesCount){
    *valuesCount = 2;
    return new float[2]{(float)sin(x), (float)cos(x)};
}

float* lagrangeInterpolation(float x, float* nodes, int nodeCount, int* valuesCount){

}

float* newtonInterpolation(float x, float* nodes, int nodeCount, int* valuesCount){

}
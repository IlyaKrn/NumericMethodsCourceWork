#pragma once

float* linearInterpolation(float x, float* nodes, int nodesCount, int* valuesCount);
float* lagrangeInterpolation(float x, float* nodes, int nodesCount, int* valuesCount);
float* newtonInterpolation(float x, float* nodes, int nodesCount, int* valuesCount);
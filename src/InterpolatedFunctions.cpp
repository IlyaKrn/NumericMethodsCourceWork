#include "../include/InterpolatedFunctions.h"
#include <fstream>
#include <iostream>

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
//    values.push_back(summValue);
    return values;
}

vector<long double> newtonInterpolation(long double x, vector<node> nodes){
    vector<long double> values;

    long double summValue = nodes[0].y;
    for (int i = 1; i < nodes.size(); ++i) {
        long double proizv = 1;
        for (int j = 0; j < i; ++j) {
            proizv *= x - nodes[j].x;
        }
        long double razdRaz = 0;
        for (int j = 0; j <= i; ++j) {
            long double znam = 1;
            for (int k = 0; k <= i; ++k) {
                if(j != k) {
                    znam *= nodes[j].x - nodes[k].x;
                }
            }
            razdRaz += nodes[j].y / znam;
        }

        summValue += proizv * razdRaz;
    }
//    values.push_back(summValue);
    return values;
}

vector<long double> splineInterpolation(long double x, vector<node> nodes){
    vector<long double> values;


    //инициализируем матрицу
    vector<vector<long double>> slau;
    for (int i = 0; i < nodes.size(); ++i) {
        vector<long double> row;
        for (int j = 0; j < nodes.size(); ++j) {
            row.push_back(0);
        }
        slau.push_back(row);
    }
    //заполняем матрицу
    slau[0][0] = 1;
    slau[nodes.size() - 1][nodes.size() - 1] = 1;
    for (int i = 1; i < nodes.size() - 1; ++i) {
        slau[i][i-1] = nodes[i].x - nodes[i - 1].x;
        slau[i][i+1] = nodes[i + 1].x - nodes[i].x;
        slau[i][i] = 2 * (slau[i][i-1] + slau[i][i+1]);
    }

    //считаем правую часть и добавляем в матрицу
    slau[0].push_back(0);
    for (int i = 1; i < nodes.size() - 1; ++i){
        slau[i].push_back(3 * ((nodes[i + 1].y - nodes[i].y)/ (nodes[i + 1].x - nodes[i].x) - (nodes[i].y - nodes[i - 1].y) / (nodes[i].x - nodes[i - 1].x)));
    }
    slau[slau.size() - 1].push_back(0);

    //зануляем столбцы
    for (int row = 1; row < nodes.size(); ++row) {
        for (int i = row; i < slau.size(); ++i) {
            long double mnozh = slau[i][row-1] / slau[row-1][row-1];
            for (int j = row-1; j < slau[0].size(); ++j) {
                slau[i][j] -= mnozh * slau[row-1][j];
            }
            slau[i][row-1] = 0;
        }
    }

    vector<long double> c;
    for (int i = 0; i < nodes.size(); ++i) {
        c.push_back(0);
    }

    for (int i = nodes.size() - 1; i >= 0; --i) {
        long double sum = 0;
        for (int j = 0; j < c.size(); ++j) {
            sum += c[j] * slau[i][j];
        }
        c[i] = (slau[i][slau[i].size() - 1] - sum) / slau[i][i];
    }

    for (int i = 0; i < nodes.size() - 1; ++i){
        if (min(nodes[i].x, nodes[i + 1].x) <= x && max(nodes[i].x, nodes[i + 1].x) > x){
            long double d = (c[i+1] - c[i]) / (3 * (nodes[i + 1].x - nodes[i].x));
            long double b = (nodes[i+1].y - nodes[i].y) / (nodes[i + 1].x - nodes[i].x) - (nodes[i + 1].x - nodes[i].x) * (2 * c[i] + c[i+1]) / 3;
            values.push_back(nodes[i].y + b * (x - nodes[i].x) + c[i] * (x - nodes[i].x) * (x - nodes[i].x) + d * (x - nodes[i].x) * (x - nodes[i].x) * (x - nodes[i].x));
        }
    }

    return values;
}
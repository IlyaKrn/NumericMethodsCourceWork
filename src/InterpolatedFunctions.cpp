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

vector<long double> diffForward(long double x, vector<node> nodes){
    vector<long double> values;
    vector<long double> diffs;

    for (int i = 0; i < nodes.size() - 1; ++i) {
        diffs.push_back((nodes[i + 1].y - nodes[i].y) / (nodes[i + 1].x - nodes[i].x));
    }

    for (int i = 0; i < diffs.size() - 1; ++i){
        if (nodes[i].x <= x && nodes[i + 1].x > x){
            values.push_back(((x - nodes[i].x) * (diffs[i + 1] - diffs[i])) / (nodes[i + 1].x - nodes[i].x) + diffs[i]);
        }
    }
    return values;
}

vector<long double> diffBackward(long double x, vector<node> nodes){
    vector<long double> values;
    vector<long double> diffs;

    for (int i = 1; i < nodes.size(); ++i) {
        diffs.push_back((nodes[i].y - nodes[i - 1].y) / (nodes[i].x - nodes[i - 1].x));
    }

    for (int i = 1; i < diffs.size(); ++i){
        if (nodes[i].x <= x && nodes[i + 1].x > x){
            values.push_back(((x - nodes[i].x) * (diffs[i] - diffs[i - 1])) / (nodes[i + 1].x - nodes[i].x) + diffs[i - 1]);
        }
    }
    return values;
}

vector<long double> diffCentral(long double x, vector<node> nodes){
    vector<long double> values;
    vector<long double> diffs;

    for (int i = 1; i < nodes.size() - 1; ++i) {
        diffs.push_back((nodes[i + 1].y - nodes[i - 1].y) / (nodes[i + 1].x - nodes[i - 1].x));
    }

    for (int i = 1; i < diffs.size(); ++i){
        if (nodes[i].x <= x && nodes[i + 1].x > x){
            values.push_back(((x - nodes[i].x) * (diffs[i] - diffs[i - 1])) / (nodes[i + 1].x - nodes[i].x) + diffs[i - 1]);
        }
    }
    return values;
}

vector<long double> hdiffForward(long double x, vector<node> nodes){
    vector<long double> values;
    long double h = 0.00001;

    vector<long double> y0 = splineInterpolation(x, nodes);
    vector<long double> y1 = splineInterpolation(x + h, nodes);
    if(!y0.empty() && !y1.empty())
        values.push_back((y1[0]-y0[0])/h);
    return values;
}

vector<long double> hdiffBackward(long double x, vector<node> nodes){
    vector<long double> values;
    long double h = 0.00001;

    vector<long double> y0 = splineInterpolation(x - h, nodes);
    vector<long double> y1 = splineInterpolation(x, nodes);
    if(!y0.empty() && !y1.empty())
        values.push_back((y1[0]-y0[0])/h);
    return values;
}

vector<long double> hdiffCentral(long double x, vector<node> nodes){
    vector<long double> values;
    long double h = 0.00001;

    vector<long double> y0 = splineInterpolation(x - h, nodes);
    vector<long double> y1 = splineInterpolation(x + h, nodes);
    if(!y0.empty() && !y1.empty())
        values.push_back((y1[0]-y0[0])/(2*h));
    return values;
}

vector<long double> integralLeft(long double x, vector<node> nodes){
    vector<long double> values;
    long double quadr = 0;

    for (int i = 0; i < nodes.size() - 1; ++i) {
        quadr += nodes[i].y * (nodes[i + 1].x - nodes[i].x);
    }
    cout << "left quadr = " << quadr << endl;

    for (int i = 0; i < nodes.size() - 1; ++i){
        if (nodes[i].x <= x && nodes[i + 1].x > x){
//            values.push_back(nodes[i].y);
        }
    }
    return values;
}

vector<long double> integralRight(long double x, vector<node> nodes){
    vector<long double> values;
    long double quadr = 0;

    for (int i = 0; i < nodes.size() - 1; ++i) {
        quadr += nodes[i + 1].y * (nodes[i + 1].x - nodes[i].x);
    }
    cout << "right quadr = " << quadr << endl;

    for (int i = 0; i < nodes.size() - 1; ++i){
        if (nodes[i].x <= x && nodes[i + 1].x > x){
//            values.push_back(nodes[i + 1].y);
        }
    }
    return values;
}

vector<long double> integralCentral(long double x, vector<node> nodes){
    vector<long double> values;
    long double quadr = 0;

    for (int i = 0; i < nodes.size() - 2; i+=2) {
        quadr += nodes[i + 1].y * (nodes[i + 2].x - nodes[i].x);
    }
    cout << "central quadr = " << quadr << endl;

    for (int i = 1; i < nodes.size() - 1; i+=2){
        if (nodes[i - 1].x <= x && nodes[i + 1].x > x){
//            values.push_back(nodes[i].y);
        }
    }
    return values;
}

vector<long double> integralTrap(long double x, vector<node> nodes){
    vector<long double> values;
    long double quadr = 0;

    for (int i = 0; i < nodes.size() - 1; ++i) {
        quadr += (nodes[i].y + nodes[i + 1].y) * (nodes[i + 1].x - nodes[i].x) / 2;
    }
    cout << "trapetz quadr = " << quadr << endl;

    for (int i = 0; i < nodes.size() - 1; ++i){
        if (min(nodes[i].x, nodes[i + 1].x) <= x && max(nodes[i].x, nodes[i + 1].x) > x){
//            values.push_back(((x - nodes[i].x) * (nodes[i + 1].y - nodes[i].y)) / (nodes[i + 1].x - nodes[i].x) + nodes[i].y);
        }
    }
    return values;
}

vector<long double> integralSimpson(long double x, vector<node> nodes){
    vector<long double> values;
    long double quadr = 0;

    for (int i = 0; i < nodes.size() - 2; i+=2) {
        quadr += (nodes[i + 2].x - nodes[i].x) * (nodes[i].y + 4 * nodes[i + 1].y + nodes[i + 2].y) / 6;
    }
    cout << "simpson quadr = " << quadr << endl;

    for (int i = 1; i < nodes.size() - 1; i+=2){
        if (nodes[i - 1].x <= x && nodes[i + 1].x > x){

            long double slau[3][4] = {
                    {nodes[i].x * nodes[i].x             ,nodes[i].x        , 1,     nodes[i].y        },
                    {nodes[i + 1].x * nodes[i + 1].x     ,nodes[i + 1].x    , 1,     nodes[i + 1].y    },
                    {nodes[i - 1].x * nodes[i - 1].x     ,nodes[i - 1].x    , 1,     nodes[i - 1].y    },
            };

            long double a0 = 0;
            long double a1 = 0;
            long double a2 = 0;

            //решаем слау

            //зануляем столбцы
            for (int i = 1; i < 3; ++i) {
                long double mnozh = slau[i][0] / slau[0][0];
                for (int j = 0; j < 4; ++j) {
                    slau[i][j] -= mnozh * slau[0][j];
                }
                slau[i][0] = 0;
            }
            for (int i = 2; i < 3; ++i) {
                long double mnozh = slau[i][1] / slau[1][1];
                for (int j = 1; j < 4; ++j) {
                    slau[i][j] -= mnozh * slau[1][j];
                }
                slau[i][1] = 0;
            }

            //считаем ответ
            a0 = (slau[2][3]) / slau[2][2];
            a1 = (slau[1][3] - a0 *  slau[1][2]) / slau[1][1];
            a2 = (slau[0][3] - a0 *  slau[0][2] - a1 *  slau[0][1]) / slau[0][0];


//            values.push_back(a0 + a1*x + a2*x*x);
        }
    }
    return values;
}
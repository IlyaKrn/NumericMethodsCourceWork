#include <iostream>
#include <vector>
#include "build/libs/sdl2/include/SDL.h"
#include "include/InterpolatedFunctions.h"

using namespace std;

const int COLOR_WHITE = 2147483647;
const int COLOR_GRAY = 1002159035;
const int COLOR_RED = 16711680;
const int COLOR_BLACK = 0;

//рисуем белый фон
void drawSurface(Uint32* pixels, int h, int w){
    for (int i = 0; i < h * w; ++i) {
        pixels[i] = COLOR_WHITE;
    }
}

//рисуем клетки и оси
void drawField(Uint32* pixels, int h, int w, int s){
    for (int i = 0; i < h / 2 - 1; ++i) {
        if(i % s == 0) {
            for (int j = 0; j < w; ++j) {
                pixels[(i + h / 2) * w + j] = COLOR_GRAY;
                pixels[(-i + h / 2) * w + j] = COLOR_GRAY;
            }
        }
    }
    for (int i = 0; i < w / 2 - 1; ++i) {
        if(i % s == 0) {
            for (int j = 0; j < h; ++j) {
                pixels[j * w + ((i + w / 2 - 1))] = COLOR_GRAY;
                pixels[j * w + ((-i + w / 2 - 1))] = COLOR_GRAY;
            }
        }
    }
    for (int i = 0; i < w; ++i) {
        pixels[(h / 2) * w + i] = COLOR_BLACK;
    }
    for (int i = 0; i < h; ++i) {
        pixels[i * w + s] = COLOR_BLACK;
    }

    //рисуем буквы
    int size = s * 6 / 10;

    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            for (int i = 0; i < size; i++) {
                pixels[(s / 5 + i + j - 2) * w + k + w / 2 + s / 5 + i] = COLOR_BLACK;
                pixels[(s / 5 + i + j - 2) * w + k + w / 2 + s / 5 + size - 1 - i] = COLOR_BLACK;
            }
        }
    }

    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            for (int i = 0; i < size / 2; i++) {
                pixels[(h / 2 + s / 5 + i + j - 2) * w + k + s / 5 + i] = COLOR_BLACK;
                pixels[(h / 2 + s / 5 + i + j - 2) * w + k + s / 5 + size - 1 - i] = COLOR_BLACK;
            }
            for (int i = size / 2; i < size; i++) {
                pixels[(h / 2 + s / 5 + i + j - 2) * w + k + s / 5 + size / 2] = COLOR_BLACK;
            }
        }
    }


}

int main(int argc, char** argv) {

    //размеры окна и размер клетки
    int HEIGHT = 1000;
    int WIDTH = 1500;
    int SCALE = 25;

    //перезаписываем параметры окна из аргументов программы
    if(argc == 3){
        try{ HEIGHT = stoi(argv[0]); } catch(...) {}
        try{ WIDTH = stoi(argv[1]);  } catch(...) {}
        try{ SCALE = stoi(argv[2]);  } catch(...) {}
    }
    cout << "field: " << HEIGHT << "x" << WIDTH << "\ncell= " << SCALE << endl;

    //получаем список узлов
    vector<node> nodes = getNodes("/home/ilyakrn/CLionProjects/NumericMethodsCourceWork/nodes.txt");

    //открываем окно и получаем матрицу пикселей
    SDL_Window* window = SDL_CreateWindow("Курсовая работа Численные методы", 0, 0, WIDTH, HEIGHT, 0);
    Uint32* pixels = (Uint32*) SDL_GetWindowSurface(window)->pixels;

    //рисуем белый фон
    drawSurface(pixels, HEIGHT, WIDTH);

    //рисуем графики и считываем действия пользователя
    while (true){
        //читаем действия пользователя
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    SDL_Quit();
                    return 0;
            }
        }

        //рисуем клетки и оси
        drawField(pixels, HEIGHT, WIDTH, SCALE);

        long double lastLinear = 0;
        long double lastLagrange = 0;
        long double lastNewton = 0;
        long double lastSpline = 0;
        long double lastDiffF = 0;
        long double lastDiffB = 0;
        long double lastDiffC = 0;
        long double lastHDiffF = 0;
        long double lastHDiffB = 0;
        long double lastHDiffC = 0;
        long double lastIntF = 0;
        long double lastIntB = 0;
        long double lastIntC = 0;
        long double lastIntT = 0;
        long double lastIntS = 0;


        for (int x = 0; x < WIDTH; ++x) {
            //считаем текущую координату x с поправкой на смещение осей
            float curX = x - SCALE;

            //получаем координаты точек для текущего x
            vector<long double> linear = linearInterpolation(curX / SCALE, nodes);
            vector<long double> lagrange = lagrangeInterpolation(curX / SCALE, nodes);
            vector<long double> newton = newtonInterpolation(curX / SCALE, nodes);
            vector<long double> spline = splineInterpolation(curX / SCALE, nodes);
            vector<long double> diffF = diffForward(curX / SCALE, nodes);
            vector<long double> diffB = diffBackward(curX / SCALE, nodes);
            vector<long double> diffC = diffCentral(curX / SCALE, nodes);
            vector<long double> hdiffF = hdiffForward(curX / SCALE, nodes);
            vector<long double> hdiffB = hdiffBackward(curX / SCALE, nodes);
            vector<long double> hdiffC = hdiffCentral(curX / SCALE, nodes);
            vector<long double> intF = integralLeft(curX / SCALE, nodes);
            vector<long double> intB = integralRight(curX / SCALE, nodes);
            vector<long double> intC = integralCentral(curX / SCALE, nodes);
            vector<long double> intT = integralTrap(curX / SCALE, nodes);
            vector<long double> intS = integralSimpson(curX / SCALE, nodes);

            //рисуем точки функции
            for (int i = 0; i < linear.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = linear[i] * SCALE + HEIGHT / 2;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastLinear); j <= max(y, lastLinear); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        for (int k = -1; k < 2; ++k) {
                            for (int l = -1; l < 2; ++l) {
                                pixels[((int) j + k) * WIDTH + x + l] = COLOR_RED;
                            }
                        }
                    }
                }
                lastLinear = y;
            }
            for (int i = 0; i < lagrange.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = lagrange[i] * SCALE + SCALE;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastLagrange); j <= max(y, lastLagrange); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                lastLagrange = y;
            }
            for (int i = 0; i < newton.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = newton[i] * SCALE + SCALE;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastNewton); j <= max(y, lastNewton); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                lastNewton = y;
            }
            for (int i = 0; i < spline.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = spline[i] * SCALE + SCALE;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastSpline); j <= max(y, lastSpline); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        for (int k = -1; k < 1; ++k) {
                            for (int l = -1; l < 1; ++l) {
                                pixels[((int) j + k) * WIDTH + x + l] = COLOR_BLACK;
                            }
                        }
                    }
                }
                lastSpline = y;
            }
            for (int i = 0; i < diffF.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = diffF[i] * SCALE + HEIGHT / 2;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastDiffF); j <= max(y, lastDiffF); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                lastDiffF = y;
            }
            for (int i = 0; i < diffB.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = diffB[i] * SCALE + HEIGHT / 2;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastDiffB); j <= max(y, lastDiffB); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                lastDiffB = y;
            }
            for (int i = 0; i < diffC.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = diffC[i] * SCALE + HEIGHT / 2;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastDiffC); j <= max(y, lastDiffC); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                lastDiffC = y;
            }
            for (int i = 0; i < hdiffF.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = hdiffF[i] * SCALE + HEIGHT / 2;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastHDiffF); j <= max(y, lastHDiffF); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                lastHDiffF = y;
            }
            for (int i = 0; i < hdiffB.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = hdiffB[i] * SCALE + HEIGHT / 2;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastHDiffB); j <= max(y, lastHDiffB); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                lastHDiffB = y;
            }
            for (int i = 0; i < hdiffC.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = hdiffC[i] * SCALE + HEIGHT / 2;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastHDiffC); j <= max(y, lastHDiffC); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                lastHDiffC = y;
            }
            for (int i = 0; i < intF.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = intF[i] * SCALE + HEIGHT / 2;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastIntF); j <= max(y, lastIntF); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                //рисуем полосы до нуля
                for (int j = min(y, (long double) HEIGHT / 2); j <= max(y, (long double) HEIGHT / 2); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                lastIntF = y;
            }
            for (int i = 0; i < intB.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = intB[i] * SCALE + HEIGHT / 2;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastIntB); j <= max(y, lastIntB); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                //рисуем полосы до нуля
                for (int j = min(y, (long double) HEIGHT / 2); j <= max(y, (long double) HEIGHT / 2); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                lastIntB = y;
            }
            for (int i = 0; i < intC.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = intC[i] * SCALE + HEIGHT / 2;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastIntC); j <= max(y, lastIntC); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                //рисуем полосы до нуля
                for (int j = min(y, (long double) HEIGHT / 2); j <= max(y, (long double) HEIGHT / 2); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                lastIntC = y;
            }
            for (int i = 0; i < intT.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = intT[i] * SCALE + HEIGHT / 2;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastIntT); j <= max(y, lastIntT); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                //рисуем полосы до нуля
                for (int j = min(y, (long double) HEIGHT / 2); j <= max(y, (long double) HEIGHT / 2); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                lastIntT = y;
            }
            for (int i = 0; i < intS.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                long double y = intS[i] * SCALE + HEIGHT / 2;
                if (y < 0) y = 0;
                if (y >= HEIGHT) y = HEIGHT - 1;
                //рисуем если помещается на поле
                for (int j = min(y, lastIntS); j <= max(y, lastIntS); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                //рисуем полосы до нуля
                for (int j = min(y, (long double) HEIGHT / 2); j <= max(y, (long double) HEIGHT / 2); ++j) {
                    if(j >= 0 && j < HEIGHT) {
                        pixels[((int) j) * WIDTH + x] = COLOR_BLACK;
                    }
                }
                lastIntS = y;
            }

            SDL_UpdateWindowSurface(window);
        }





        SDL_UpdateWindowSurface(window);
    }

}
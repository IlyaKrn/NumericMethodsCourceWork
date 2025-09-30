#include <iostream>
#include <vector>
#include "build/libs/sdl2/include/SDL.h"
#include "include/InterpolatedFunctions.h"

using namespace std;

const int COLOR_WHITE = 2147483647;
const int COLOR_GRAY = 1002159035;
const int COLOR_BLACK = 0;

//рисуем белый фон
void drawSurface(Uint32* pixels, int h, int w){
    for (int i = 0; i < h * w; ++i) {
        pixels[i] = COLOR_WHITE;
    }
}

//рисуем клетки и оси
void drawField(Uint32* pixels, int h, int w, int s){
    for (int i = 1; i < h / 2 - 1; ++i) {
        if(i % s == 0) {
            for (int j = 0; j < w; ++j) {
                pixels[(i + h / 2) * w + j] = COLOR_GRAY;
                pixels[(-i + h / 2) * w + j] = COLOR_GRAY;
            }
        }
    }
    for (int i = 1; i < w / 2 - 1; ++i) {
        if(i % s == 0) {
            for (int j = 0; j < h; ++j) {
                pixels[j * w + ((i + w / 2 - 1))] = COLOR_GRAY;
                pixels[j * w + ((-i + w / 2 - 1))] = COLOR_GRAY;
            }
        }
    }
    for (int i = 0; i < h; ++i) {
        pixels[i * w + w / 2] = COLOR_BLACK;
    }
    for (int i = 0; i < w; ++i) {
        pixels[(h / 2 - 1) * w + i] = COLOR_BLACK;
    }
}

int main(int argc, char** argv) {

    //размеры окна и размер клетки
    int HEIGHT = 1000;
    int WIDTH = 1500;
    int SCALE = 50;

    //перезаписываем параметры окна из аргументов программы
    if(argc == 3){
        try{ HEIGHT = stoi(argv[0]); } catch(...) {}
        try{ WIDTH = stoi(argv[1]);  } catch(...) {}
        try{ SCALE = stoi(argv[2]);  } catch(...) {}
    }
    cout << "field: " << HEIGHT << "x" << WIDTH << "\ncell= " << SCALE << endl;

    //получаем список узлов
    vector<node> nodes = getNodes("/home/ilyakrn/CLionProjects/NumericalMethodsCourceWork/nodes.txt");

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


        for (int x = 0; x < WIDTH; ++x) {
            //считаем текущую координату x с поправкой на смещение осей
            float curX = x - SCALE;

            //получаем координаты точек для текущего x
            vector<float> values = linearInterpolation(curX / SCALE, nodes);

            //рисуем точки функции
            for (int i = 0; i < values.size(); ++i) {
                //вычисляем значение функции с поправкой на смещение осей
                float y = - values[i] * SCALE + SCALE;
                //рисуем если помещается на поле
                if(y >= 0 && y < HEIGHT) {
                    pixels[((int) y) * WIDTH + x] = COLOR_BLACK;
                }
            }
        }





        SDL_UpdateWindowSurface(window);
    }

}
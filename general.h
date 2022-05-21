//
// Created by yPhysicist on 14.05.2022.
//
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <Windows.h>

using namespace sf;
using namespace std;

#ifndef KURSOVIK1_GENERAL_H
#define KURSOVIK1_GENERAL_H

sf::Color HSVtoRGB(float H, float S, float V);
double normalize(double value, double localMin, double localMax, double min, double max);
double mandelIter(double cx, double cy, int maxIter);
sf::Texture mandelbrot(int width, int height, double xmin, double xmax, double ymin, double ymax, int iterations);

#endif //KURSOVIK1_GENERAL_H

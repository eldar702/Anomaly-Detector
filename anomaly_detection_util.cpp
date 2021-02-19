//
// Created by Eldar on 14/10/2020.
//

#include "anomaly_detection_util.h"
#include <iostream>
#include <cmath>
using namespace std;
// return the life expectancy which needed for calculate the variance
float lifeExpect(const float* x, int size)
{
    if (size == 0) { return 0.0f; }
    float sumX = 0.0f;
    for (int i = 0; i < size; ++i) {
        sumX += x[i];
    }
    return sumX * (1.0f / (float) size);
}
// returns the variance of X and Y
float var(float* x, int size)
{
    if (size == 0) { return 0.0f; } // extreme case
    float u = lifeExpect(x, size); // u = the life expectancy
    float sumX = 0.0f; // the totaling of x[i]
    for (int i = 0; i < size; ++i) {
        sumX += (x[i] * x[i]);
    }
    return ((1.0f / float(size)) * sumX - (u * u));
}
// returns the covariance of X and Y
float cov(float* x, float* y, int size)
{
    if(size == 0) { return 0.0f; }
    float eX = lifeExpect(x, size);
    float eY = lifeExpect(y, size);
    float eXY = 0.0f;
    for (int i = 0; i < size; ++i) {
        eXY += (x[i] * y[i]);
    }
    return ((1.0f / (float)size) * eXY) - (eX * eY);
}

float pearson(float *x, float *y, int size)
{
    float cov_xy = cov(x, y, size);
    float sum_x = var(x, size);
    float sum_y = var(y, size);

    if(size == 0 || sum_y == 0 || sum_x == 0)
    {
        return 0.0f;
    }

    return (cov_xy / (sqrt(sum_x) * sqrt(sum_y)));
}

// performs a linear regression and return s the line equation
Line linear_reg(Point** points, int size)
{
    float x[size], y[size];
    float varX = var(x, size);
    if ((varX == 0.0f) || (size == 0)) // extreme cases - dev in 0
    {
        Line *line = nullptr;
        return *line;
    }
    for (int i = 0; i < size; ++i)
    {
        Point *point = points[i];
        x[i] = point->x;
        y[i] = point->y;
    }
    return linear_reg(x, y, size);
}
// override - get two performs a linear regression and return s the line equation
Line linear_reg(float* x, float* y, int size)
{
    if(size == 0)
    {
        Line* line = nullptr;
        return *line;
    }
    float a = cov(x, y, size) / var(x, size);
    float m_x = lifeExpect(x, size);
    float m_y = lifeExpect(y, size);
    float b = m_y - (a * m_x);
    Line line = Line(a, b);
    return line;
}
// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size)
{
    if (size == 0) { return 0.0f; }
    Line line = linear_reg(points, size);
    return dev(p, line);
}

// returns the deviation between point p and the line
float dev(Point p,Line l)
{
    float expectedX = l.f(p.x);
    return abs((expectedX - p.y));
}


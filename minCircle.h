// 20561663 31655464
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include "anomaly_detection_util.h"


using namespace std;

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};


/*
 * Function role is to return the distance between two given points (using the known formula).
 */
float findDistance(const Point& g, const Point& h);
/*
 * Function role is to calculate the distance (not exactly for 2 points..).
 */
float findDistance(const float x, const float x1, const float y, const float y1);
/*
 * Function role is to define if a given point is inside the circle.
 */
bool insideCircle(const Point& point, const Circle& circle);
/*
 * Function role is to create a circle by given 2 points .
 */
Circle createCircle(const Point& p1, const Point& p2);
/*
 * Function role is determined if the given circle contain the points.
 */
bool isValid(const Circle& circle, const vector<Point>& points);
/*
 * Function role is to solve the determinants of the 3 matrix (3X3):
 * case 0 - calc the determinant of matrix A, built from the 3 points.
 * case 1 - calc the determinant of matrix B, built from multiplying the points and the y's values.
 * case 2 - calc the determinant of matrix B, built from multiplying the points and the x's values.
 */
float calcDet(int flag, const float x1, const float y1, const float x2, const float y2, const float x3, const float y3);


/*
 * Function role: create a circle when giving 3 points, using the formula: (x-x')^2 + (y-y')^2 -(r^2) = 0.
 * After multiplied out, we will need to calc 4 elements:
 * the determinants of 3 matrix (3X3) with another function, and create the circle.
 */
Circle createCircle(const Point& p1, const Point& p2, const Point& p3);
/*
 * Function role is to find minimum circle of num of point less or equal to 3.
 * if there is 0 points = return circle equal to none.
 * if there is 1 point - return circle equal only to that point.
 * if there is 2 points - create the minimum circle that included those points (both will be on the circle border).
 * if there is 3 points - create the minimum circle that included those points (2 of them will be on the border of
 * the circle and 1 inside the border).
 */
Circle calcCircle(vector<Point>& points);
/*
 * Function role: Choose 3 points and create the circle where they are on the circle boundary. After that the algorithm
 * Get the next point and check if it is inside or on the circle:
 * If it is enclosed - the circle is good for those points, so he keep checking for the other points.
 * If it is not enclosed - the alg' will create new circle contain the checked points.
 */
Circle theAlgorithm(vector<Point>& originalVec, vector<Point> newVec, int size);

Circle findMinCircle(Point** points,size_t size);

Circle findMinCircle(vector<float> &feature1, vector<float> &feature2,size_t size);
#endif /* MINCIRCLE_H_ */
#pragma once

#include "riptide_simulink_ros/Vector3d.h"
 
class Matrix3d{
    public:
        double a;
        double b;
        double c;
        double d;
        double e;
        double f;
        double g;
        double h;
        double i;

        // a b c
        // d e f
        // h i j
    
    Matrix3d(double A, double B, double C, double D, double E, double F, double G, double H, double I);

    Vector3d premultiplyVector(Vector3d v);
};
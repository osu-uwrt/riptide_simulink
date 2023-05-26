#include "riptide_simulink_ros/Matrix3d.h"

Matrix3d::Matrix3d(double A, double B, double C, double D, double E, double F, double G, double H, double I){
    a = A;
    b = B;
    c = C;
    d = D;
    e = E;
    f = F;
    g = G;
    h = H;
    i = I;
}

Vector3d Matrix3d::premultiplyVector(Vector3d v){
    double x = this->a * v.x + this->b * v.y + this->c * v.z;
    double y = this->d * v.x + this->e * v.y + this->f * v.z;
    double z = this->g * v.x + this->h * v.y + this->i * v.z;
    
    return Vector3d(x, y, z);
}


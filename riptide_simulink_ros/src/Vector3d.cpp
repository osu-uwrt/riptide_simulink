#include "riptide_simulink_ros/Vector3d.h"

Vector3d::Vector3d(double X, double Y, double Z){
    x = X;
    y = Y;
    z = Z;
}
Vector3d::Vector3d(){
    x = 0;
    y = 0;
    z = 0;
}

Vector3d Vector3d::MultiplyByScalar(double s){
    return Vector3d(this->x * s, this->y * s, this->z *s);
}

Vector3d Vector3d::AddVector(Vector3d v){
    return Vector3d(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vector3d Vector3d::SubtractVector(Vector3d v){
    return Vector3d(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vector3d Vector3d::CrossVector(Vector3d v){
    return Vector3d(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x);
}

double Vector3d::DotVector(Vector3d v){
    return this->x * v.x + this->y * v.y + this->z * v.z;
}

bool Vector3d::IsEqual(Vector3d v){
    if(this->x == v.x && this->y == v.y && this->z == v.z){
        return true;
    }

    return false;
}
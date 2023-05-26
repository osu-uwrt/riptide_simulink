#pragma once

class Vector3d{
    public:
        double x;
        double y;
        double z;

    Vector3d();
    Vector3d(double X, double Y, double Z);

    Vector3d MultiplyByScalar(double s);
    Vector3d AddVector(Vector3d v);
    Vector3d SubtractVector(Vector3d v);

    double DotVector(Vector3d v);
    Vector3d CrossVector(Vector3d v);

    bool IsEqual(Vector3d v);
};
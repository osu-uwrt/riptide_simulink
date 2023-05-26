#pragma once

#include "riptide_simulink_ros/Vector3d.h"
#include "riptide_simulink_ros/Matrix3d.h"


#include <vector>
class IntegrationZone{
    private:
    public:

        //bounds of the zone on the surface plane
        std::vector<Vector3d> upper_bound;
        std::vector<Vector3d> lower_bound;

        double total_volume = 0;
        double total_force = 0;

        //how much bouyant force is provided per volume displaced N/m^3
        double bouyant_density;

        //plane cofficients [cx, cy, cz, k] : cx*x + cy*y + cz*z = k
        std::vector<double> plane_coefficents;
        
        Vector3d bouyant_force_acting_location;

        IntegrationZone(std::vector<Vector3d> upper, std::vector<Vector3d> lower, std::vector<double> plane, double bouyantDensity);

        void integrate();
        void invertForceDirection();
        void sumBouyantForces(std::vector<double> forces, std::vector<Vector3d> centroids, double *sum_forces, Vector3d *overall_centroid);

        double integrate_eq(double Mymax, double Mymin, double Bymax, double Bymin, double Xmax, double Xmin, double k, double Cx, double Cy, double Cz);
        double calculateYCentroid(double c, double h, double d, double j, double a, double b, double k, double l, double m, double o, double volume);
        double calculateXCentroid(double c, double h, double d, double j, double a, double b, double k, double l, double m, double o, double volume);

        Vector3d pointsToMxPlusB(Vector3d p1, Vector3d p2);

        Vector3d calculateCentroid(double Mymax, double Mymin, double Bymax, double Bymin, double Xmax, double Xmin, double k, double Cx, double Cy, double Cz, double volume);



};


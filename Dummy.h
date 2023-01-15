#ifndef __Dummy__H
#define __Dummy__H

#include <vector>
#include <Eigen/Dense>


using Eigen::Vector3d;
using Eigen::Matrix3d;


class Dummy{
    private:
        //if the absolute vectors have been calculated -- allows intersection calculation to run
        bool absolute_vectors_calculated = false;

    public:

        double location; // the location of the Dummy
        double vector_from_center;
        double rotated_vector_from_center; 

        //connections to other vertices
        double c1_relative;
        double c2_relative;
        double c3_relative;

        double c1_absolute;
        double c2_absolute;
        double c3_absolute;

        //connections to intersections
        std::vector<double> intersections;

        //Constructor and Destructor
        Dummy(double l, double c1, double c2, double c3);


        //calculate the absolute vectors between adjoining vert
};

#endif
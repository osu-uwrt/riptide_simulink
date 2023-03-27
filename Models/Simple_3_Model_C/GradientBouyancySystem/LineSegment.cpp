#include "LineSegment.h"
#include <iostream>
#include <cmath>

using namespace std;

LineSegment::LineSegment(Vector3d p1, Vector3d p2):
vertex_one{Vertex(p1,p1,p1,p1)},
vertex_two{Vertex(p1,p1,p1,p1)}
{
    this->end_point_one = p1;
    this->end_point_two = p2;

    // initialized vertex_one and two to junk -> don't mark them as there
}

LineSegment::LineSegment(Vertex p1, Vertex p2):
vertex_one{p1},
vertex_two{p2}
{
    this->end_point_one = p1.location;
    this->end_point_two = p2.location;

    //vetices have been filled out
    this->includes_vertices = true;
}

bool LineSegment::checkForDuplicate(LineSegment other){
    // check to see if this line segment is the same as the passed
    // false: other and self are different
    // true: other and self are the same

    if (this->end_point_one.IsEqual(other.end_point_one)){
        if(this->end_point_two.IsEqual(other.end_point_two)){
            return true;
        }
    } else if (this->end_point_one.IsEqual(other.end_point_two)){
        if(this->end_point_two.IsEqual(other.end_point_one)){
            return true;
        }
    }
    
    return false;
}

double LineSegment::getLength(){
    return pow(pow((this->end_point_one.x - this->end_point_two.x),2) + pow((this->end_point_one.y - this->end_point_two.y),2) + pow((this->end_point_one.z- this->end_point_two.z),2),0.5);
}

double LineSegment::getProjectedLength(){
    //return the length of the line segment one it has been projected onto the surface plane
    return pow(pow((this->end_point_one.x - this->end_point_two.x),2) + pow((this->end_point_one.y- this->end_point_two.y),2),0.5);
}

double LineSegment::returnXMax(){
    if(this->end_point_one.x > this->end_point_two.x){
        return this->end_point_one.x;
    }

    return this->end_point_two.x;
}

bool LineSegment::isInXBound(double xValue){
    //If the xValue is in the range of the line segment
    if(this->end_point_one.x > this->end_point_two.x){
        if((xValue < this->end_point_one.x) && (xValue > this->end_point_two.x)){
            return true;
        }
    }else{
        if((xValue > this->end_point_one.x) && (xValue < this->end_point_two.x)){
            return true;
        }
    }

    return false;

}

bool LineSegment::returnInterpolatedPoint(double xValue, Vector3d *interpolated_point){
    //returns true if an interpolated point is found -- and fills out interpolated_point
    //returns false if the point is out of x bounds

    if(! this->isInXBound(xValue)){
        return false;
    }

    if(this->end_point_two.x  == this->end_point_one.x){
        //prevent divide by zero
        *interpolated_point = this->end_point_one;
        return true;
    }

    double porportion_of_segment = (xValue - this->end_point_one.x) / (this->end_point_two.x - this->end_point_one.x);
    Vector3d vector_from_one = this->end_point_two.SubtractVector(this->end_point_one).MultiplyByScalar(porportion_of_segment);

    *interpolated_point = this->end_point_one.AddVector(vector_from_one);
    return true;

}
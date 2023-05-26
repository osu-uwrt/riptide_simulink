#pragma once

#include "riptide_simulink_ros/Vertex.h"
#include "riptide_simulink_ros/LineSegment.h"
#include "riptide_simulink_ros/IntegrationZone.h"

#include <bits/stdc++.h>
#include <vector>
#include <iostream>
#include "riptide_simulink_ros/Vector3d.h"
#include "riptide_simulink_ros/Matrix3d.h"
#include <chrono>

struct GradientBouyancyResults {
    double force;
    Vector3d location;
};

void calculateBoundingPaths(std::vector<Vector3d> points, std::vector<Vector3d> *upper_path, std::vector<Vector3d> *lower_path);
void sumBouyantForces(std::vector<double> forces, std::vector<Vector3d> centroids, double *sum_forces, Vector3d *overall_centroid);

GradientBouyancyResults calculateBouyancy(double x, double y, double z, double roll, double pitch, double yaw);
void calculateBouyancy(std::vector<Vertex> raw_vertices, Vector3d euler_rotation, Vector3d position, double bouyant_density, double bouyant_force, double *calculated_force, Vector3d *acting_loccation);

bool isCoplanar(Vector3d point, std::vector<double> plane);
bool determineIfAdditiveArea(Vector3d izv1, Vector3d izv2, Vector3d izi1, Vector3d izi2, Vector3d izo1, Vector3d izo2);
bool determineAdditiveArea(std::vector<double> plane, Vector3d other_surfaced_vertice);

Matrix3d rotationFromEuler(double x, double y, double z);

std::vector<double> pointsToPlane(Vector3d p1, Vector3d p2, Vector3d p3);

std::vector<Vertex> findOtherVertices(std::vector<Vertex> surfaced_vertices, std::vector<Vertex>vertices);
std::vector<Vertex> generateRawVertices(std::vector<double> bounding_box);
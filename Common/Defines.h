#pragma once

#include <vector>

using namespace std;

#define PAD 10

typedef struct Point3D
{
    Point3D()
    {
        X = Y = Z = 0.0f;
    }

    Point3D(const Point3D &point)
    {
        X = point.X;
        Y = point.Y;
        Z = point.Z;
    }

    Point3D(double x, double y, double z)
    {
        X = x;
        Y = y;
        Z = z;
    }

    Point3D operator+(const Point3D& point) const
    {
        return Point3D(X + point.X, Y + point.Y, Z + point.Z);
    }

    Point3D operator-(const Point3D& point) const
    {
        return Point3D(X - point.X, Y - point.Y, Z - point.Z);
    }

    bool Equals(const Point3D& point)
    {
        return (X == point.X) && (Y == point.Y) && (Z == point.Z);
    }

    double X;
    double Y;
    double Z;
} Position, Velocity, Acceleration;

typedef vector<Position> Path;

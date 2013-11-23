#pragma once
#include <afxwin.h>

#include <vector>
using namespace std;

#include <GdiPlus.h>
using namespace Gdiplus;

#define PAD 10

#define TARGET_TRACK_WIDTH 2
#define TARGET_RADIUS 4
#define TARGET_TITLE_PAD 2
#define TARGET_TITLE_OFFSET 15

#define LIST_ITEM_WIDTH 90

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

enum TargetColor
{
    TargetColorRed = 0,
    TargetColorOrange,
    TargetColorYellow,
    TargetColorGreen,
    TargetColorCyan,
    TargetColorBlue,
    TargetColorPurple,
    TargetColorLast,
};

extern Color TargetColors[TargetColorLast];
extern CString TargetColorNames[TargetColorLast];

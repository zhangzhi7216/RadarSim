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

typedef struct __declspec(dllexport) Point3D
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

    Point3D &operator+=(const Point3D& point)
    {
        X += point.X;
        Y += point.Y;
        Z += point.Z;
        return *this;
    }

    Point3D operator-(const Point3D& point) const
    {
        return Point3D(X - point.X, Y - point.Y, Z - point.Z);
    }

    Point3D &operator-=(const Point3D& point)
    {
        X -= point.X;
        Y -= point.Y;
        Z -= point.Z;
        return *this;
    }

    Point3D &operator*=(double n)
    {
        X *= n;
        Y *= n;
        Z *= n;
        return *this;
    }

    Point3D &operator/=(double n)
    {
        X /= n;
        Y /= n;
        Z /= n;
        return *this;
    }

    bool Equals(const Point3D& point)
    {
        return (X == point.X) && (Y == point.Y) && (Z == point.Z);
    }

    double X;
    double Y;
    double Z;
} Position, Velocity, Acceleration;

__declspec(dllexport) CArchive & operator << (CArchive &ar, Position &pos);

__declspec(dllexport) CArchive & operator >> (CArchive &ar, Position &pos);

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

enum StateMapBackground
{
    StateMapBackground0 = 0,
    StateMapBackground1,
    StateMapBackground2,
    StateMapBackground3,
    StateMapBackground4,
    StateMapBackgroundLast,
};

extern Image *StateMapBackgrounds[StateMapBackgroundLast];
extern CString StateMapBackgroundNames[StateMapBackgroundLast];

enum TargetType
{
    TargetTypeHeli = 0,
    TargetTypeFighter,
    TargetTypeAew,
    TargetTypeBomber,
    TargetTypeShipboard,
    TargetTypeTank,
    TargetTypePanzer,
    TargetTypeMissile,
    TargetTypeLast,
};

extern Image *TargetTypeImages[TargetTypeLast];
extern CString TargetTypeNames[TargetTypeLast];

#define PLANE_COUNT 3
#define TARGET_COUNT 3
#define DATA_CENTER_ADDR TEXT("localhost")
#define DATA_CENTER_PORT 10086

enum PacketType
{
    PacketTypeImFusion = 0,
    PacketTypeFusionAddr,
    PacketTypeReset,
    PacketTypePlane,
    PacketTypeTarget,
    PacketTypeRadar,
    PacketTypeEsm,
    PacketTypeInfrared,
    PacketTypeStateMap,
    PacketTypeTrueData,
    PacketTypeNoiseData,
    PacketTypeFusionData,
    PacketTypeControlData,
};

void __declspec(dllexport) GlobalInit();
void __declspec(dllexport) GlobalShut();

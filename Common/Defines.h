#pragma once
#include <afxwin.h>

#include <vector>

#include <fstream>
#include <iostream>

using namespace std;

#include <GdiPlus.h>
using namespace Gdiplus;

#define PAD 10

#define TARGET_TRACK_WIDTH 2
#define TARGET_RADIUS 4
#define TARGET_TITLE_PAD 2
#define TARGET_TITLE_OFFSET 15

#define LIST_ITEM_WIDTH 90

#ifdef _DEV
#define MISSILE_HIT_THRESHOLD 10000
#else
#define MISSILE_HIT_THRESHOLD 5000
#endif

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

    Point3D operator*(double n) const
    {
        return Point3D(X * n, Y * n, Z * n);
    }
    Point3D &operator*=(double n)
    {
        X *= n;
        Y *= n;
        Z *= n;
        return *this;
    }

    Point3D operator/(double n) const
    {
        return Point3D(X / n, Y / n, Z / n);
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

__declspec(dllexport) wofstream & operator << (wofstream &os, Position &pos);
__declspec(dllexport) wifstream & operator >> (wifstream &is, Position &pos);

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

extern Color __declspec(dllimport) TargetColors[TargetColorLast];
extern CString __declspec(dllimport) TargetColorNames[TargetColorLast];

enum StateMapBackground
{
    StateMapBackground0 = 0,
    StateMapBackground1,
    StateMapBackground2,
    StateMapBackground3,
    StateMapBackground4,
    StateMapBackgroundLast,
};

extern Image __declspec(dllimport) *StateMapBackgrounds[StateMapBackgroundLast];
extern CString __declspec(dllimport) StateMapBackgroundNames[StateMapBackgroundLast];

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

extern Image __declspec(dllimport) *TargetTypeImages[TargetTypeLast];
extern CString __declspec(dllimport) TargetTypeNames[TargetTypeLast];

enum ExplosionType
{
    ExplosionType0 = 0,
    ExplosionType1,
    ExplosionType2,
    ExplosionTypeLast,
};

extern Image __declspec(dllimport) *ExplosionTypeImages[ExplosionTypeLast];
extern CString __declspec(dllimport) ExplosionTypeNames[ExplosionTypeLast];

enum TargetMoveType
{
    TargetMoveTypeUniVel = 0,
    TargetMoveTypeUniAcc,
    TargetMoveTypeUniCir,
    TargetMoveTypeLast,
};

enum TargetState
{
    TargetStateAlive = 0,
    TargetStateExploding,
    TargetStateDestroyed,
    TargetStateLast,
};

extern CString __declspec(dllimport) TargetMoveTypeNames[TargetMoveTypeLast];

#ifdef _DEV
#define PLANE_COUNT 3
#else
#define PLANE_COUNT 4
#endif

#define TARGET_COUNT_MAX 5

#define MATLAB_DRAW_TRUE_DATA_SIZE 3
#define MATLAB_DRAW_FUSION_DATA_SIZE 9

#define MATLAB_FUSION_TRUE_DATA_SIZE 11
#define MATLAB_CONTROL_DATA_SIZE 3
#define MATLAB_FUSION_NOISE_DATA_SIZE 8

#define GLOBAL_VAR_FRAME_SIZE 625

#define DATA_CENTER_ADDR TEXT("localhost")
#define DATA_CENTER_PORT 10086

enum PacketType
{
    PacketTypeImFusion = 0,
    PacketTypeFusionAddr,
    PacketTypeImRadar,
    PacketTypeImDetect,
    PacketTypeReset,
    PacketTypePlane,
    PacketTypeTarget,
    PacketTypeSensor1,
    PacketTypeSensor2,
    PacketTypeStateMap,
    PacketTypeFusionAlgo,
    PacketTypeNaviAlgo,
    PacketTypeGlobalData,
    PacketTypeTrueData,
    PacketTypeNoiseData,
    PacketTypeFusionData,
};

enum NoiseType
{
    NoiseTypeWhite = 0,
    NoiseTypeColor,
    NoiseTypeMult,
    NoiseTypeLast,
};
extern CString __declspec(dllimport) NoiseTypeNames[NoiseTypeLast];

enum SensorId
{
    SensorIdRadar = 0,
    SensorIdEsm,
    SensorIdTong,
    SensorIdLei,
    SensorIdLast,
};
extern CString __declspec(dllimport) SensorIdNames[SensorIdLast];

enum FusionAlgoType
{
    FusionAlgoTypeLocal = 0,
    FusionAlgoTypeVc,
    FusionAlgoTypeLast,
};

enum FusionLocalAlgoType
{
    FusionLocalAlgoTypeTest1 = 0,
    FusionLocalAlgoTypeTest2,
    FusionLocalAlgoTypeLast,
};

extern CString __declspec(dllimport) ConfigFileName;

void __declspec(dllimport) GlobalInit();
void __declspec(dllimport) GlobalShut();

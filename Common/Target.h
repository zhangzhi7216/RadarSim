#pragma once

#include <afxwin.h>
#include <GdiPlus.h>
using namespace Gdiplus;

#include <map>
using namespace std;

class Target
{
public:
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
    
    static Image *TargetTypeImages[TargetTypeLast];

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

    static Color TargetColors[TargetColorLast];
    static CString TargetColorNames[TargetColorLast];

    Target(Target *plane = NULL);
    virtual ~Target(void);

    int m_Id;
    TargetType m_Type;
    TargetColor m_Color;

    Position m_Position;
    Path m_Path;

    map<int, Target> m_Targets;
    map<int, Path> m_RelPositionPaths;
    map<int, vector<double>> m_DistancePaths, m_ThetaPaths, m_PhiPaths;

    void AddTarget(Target &target);
    void MoveTo(const Position &position);
};

typedef Target Plane;


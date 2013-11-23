#pragma once

#include "Defines.h"

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

    Target();
    virtual ~Target(void);

    int m_Id;
    TargetType m_Type;

    Position m_Position;
    Path m_Path;

    vector<Target> m_Targets;
    vector<Path> m_RelPositionPaths;
    vector<vector<double>> m_DistancePaths, m_ThetaPaths, m_PhiPaths;

    void AddTarget(Target &target);
    void MoveTo(const Position &position);

    void Reset();
};

typedef Target Plane;


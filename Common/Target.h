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

    void Reset();
};

typedef Target Plane;


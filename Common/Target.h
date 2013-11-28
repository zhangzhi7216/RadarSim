#pragma once

class __declspec(dllexport) Target
{
public:
    Target();
    virtual ~Target(void);

    int m_Id;
    TargetType m_Type;

    Position m_Position;

    void Reset();
};

typedef Target Plane;


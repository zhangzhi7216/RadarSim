#pragma once

class __declspec(dllexport) Target
{
public:
    Target();
    virtual ~Target(void);

    int m_Id;
    TargetType m_Type;

    Position m_Position;
    Velocity m_Vel;
    Acceleration m_Acc;

    Point3D m_HeadDir;

    TargetMoveType m_MoveType;

    TargetColor m_Color;

    void Move(double t);
    void MoveTo(const Position &pos);

    void Reset();
};

typedef Target Plane;

__declspec(dllexport) CArchive & operator << (CArchive &ar, Target &target);

__declspec(dllexport) CArchive & operator >> (CArchive &ar, Target &target);

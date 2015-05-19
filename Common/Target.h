#pragma once

#include "Sensor.h"

class __declspec(dllexport) Target
{
public:
    Target();
    virtual ~Target(void);

    Target &operator =(const Target &);

    int m_Id;
    TargetType m_Type;

    Position m_InitPosition;
    Velocity m_InitVel;
    Acceleration m_InitAcc;

    Position m_Position;
    Velocity m_Vel;
    Acceleration m_Acc;

    // Բ���˶�����
    Position m_Center; // ��ʼ��Ϊm_Position
    double m_Pal; // ���ٶ�
    double m_Radius; // �뾶

    Point3D m_HeadDir;

    TargetMoveType m_MoveType;

    TargetColor m_Color;

    TargetState m_State;

    bool m_IsKeyTarget;

    Sensor m_Radar, m_Ais, m_Tong, m_Lei;

    void Move(double t);
    void MoveTo(const Position &pos);

    void Reset();
};

typedef Target Plane;
typedef Target Missile;

__declspec(dllexport) CArchive & operator << (CArchive &ar, Target &target);

__declspec(dllexport) CArchive & operator >> (CArchive &ar, Target &target);

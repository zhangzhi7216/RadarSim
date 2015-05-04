#include "stdafx.h"

#include "Defines.h"
#include "DataPacket.h"
#include <mclmcrrt.h>

Color __declspec(dllexport) TargetColors[] =
{
    Color::Red,
    Color::Orange,
    Color::Yellow,
    Color::Green,
    Color::Cyan,
    Color::Blue,
    Color::Purple,
};

CString __declspec(dllexport) TargetColorNames[] =
{
    TEXT("红色"),
    TEXT("橙色"),
    TEXT("黄色"),
    TEXT("绿色"),
    TEXT("青色"),
    TEXT("蓝色"),
    TEXT("紫色"),
};

Image __declspec(dllexport) *StateMapBackgrounds[];

CString __declspec(dllexport) StateMapBackgroundNames[] = 
{
    TEXT("地图0"),
    TEXT("地图1"),
    TEXT("地图2"),
    TEXT("地图3"),
    TEXT("地图4"),
};

Image __declspec(dllexport) *TargetTypeImages[] =
{
    Image::FromFile(TEXT("Heli.png")),
    Image::FromFile(TEXT("Fighter.png")),
    Image::FromFile(TEXT("Aew.png")),
    Image::FromFile(TEXT("Bomber.png")),
    Image::FromFile(TEXT("Shipboard.png")),
    Image::FromFile(TEXT("Tank.png")),
    Image::FromFile(TEXT("Panzer.png")),
    Image::FromFile(TEXT("Missile.png")),
};

Image __declspec(dllexport) *TargetTypeMaskImages[] =
{
    Image::FromFile(TEXT("HeliMask.png")),
    Image::FromFile(TEXT("HeliMask.png")),
    Image::FromFile(TEXT("HeliMask.png")),
    Image::FromFile(TEXT("HeliMask.png")),
    Image::FromFile(TEXT("HeliMask.png")),
    Image::FromFile(TEXT("HeliMask.png")),
    Image::FromFile(TEXT("HeliMask.png")),
    Image::FromFile(TEXT("HeliMask.png")),
};

CString __declspec(dllexport) TargetTypeNames[] =
{
    TEXT("直升机"),
    TEXT("歼敌机"),
    TEXT("预警机"),
    TEXT("轰炸机"),
    TEXT("舰载机"),
    TEXT("坦克"),
    TEXT("装甲车"),
    TEXT("导弹"),
};

Image __declspec(dllexport) *ExplosionTypeImages[] =
{
    Image::FromFile(TEXT("Explosion0.png")),
    Image::FromFile(TEXT("Explosion1.png")),
    Image::FromFile(TEXT("Explosion2.png")),
};

CString __declspec(dllexport) ExplosionTypeNames[] =
{
    TEXT("爆炸0"),
    TEXT("爆炸1"),
    TEXT("爆炸2"),
};

CString __declspec(dllexport) TargetMoveTypeNames[] =
{
    TEXT("匀速直线运动"),
    TEXT("匀加速直线运动"),
    TEXT("匀速圆周运动"),
};

CString __declspec(dllexport) NoiseTypeNames[] =
{
    TEXT("白噪声"),
    TEXT("色噪声"),
    TEXT("乘性噪声"),
};

CString __declspec(dllexport) SensorIdNames[] =
{
    TEXT("雷达"),
    TEXT("AIS"),
    TEXT("通侦"),
    TEXT("雷侦"),
};

CArchive & operator << (CArchive &ar, Position &pos)
{
    ar << pos.X << pos.Y << pos.Z;

    return ar;
}

CArchive & operator >> (CArchive &ar, Position &pos)
{
    ar >> pos.X >> pos.Y >> pos.Z;

    return ar;
}

wofstream & operator << (wofstream &os, Position &pos)
{
    os << pos.X << TEXT(" ") << pos.Y << TEXT(" ") << pos.Z;

    return os;
}

wifstream & operator >> (wifstream &is, Position &pos)
{
    is >> pos.X >> pos.Y >> pos.Z;

    return is;
}

CString __declspec(dllexport) ConfigFileName(TEXT("config.ini"));

void __declspec(dllexport) GlobalInit()
{
    srand((unsigned int)time(NULL));

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    StateMapBackgrounds[StateMapBackground0] = Image::FromFile(TEXT("StateMapBackground0.bmp"));
    StateMapBackgrounds[StateMapBackground1] = Image::FromFile(TEXT("StateMapBackground1.bmp"));
    StateMapBackgrounds[StateMapBackground2] = Image::FromFile(TEXT("StateMapBackground2.bmp"));
    StateMapBackgrounds[StateMapBackground3] = Image::FromFile(TEXT("StateMapBackground3.bmp"));
    StateMapBackgrounds[StateMapBackground4] = Image::FromFile(TEXT("StateMapBackground4.bmp"));

    TargetTypeImages[TargetTypeHeli] = Image::FromFile(TEXT("Heli.png"));
    TargetTypeImages[TargetTypeFighter] = Image::FromFile(TEXT("Fighter.png"));
    TargetTypeImages[TargetTypeAew] = Image::FromFile(TEXT("Aew.png"));
    TargetTypeImages[TargetTypeBomber] = Image::FromFile(TEXT("Bomber.png"));
    TargetTypeImages[TargetTypeShipboard] = Image::FromFile(TEXT("Shipboard.png"));
    TargetTypeImages[TargetTypeTank] = Image::FromFile(TEXT("Tank.png"));
    TargetTypeImages[TargetTypePanzer] = Image::FromFile(TEXT("Panzer.png"));
    TargetTypeImages[TargetTypeMissile] = Image::FromFile(TEXT("Missile.png"));

    TargetTypeMaskImages[TargetTypeHeli] = Image::FromFile(TEXT("HeliMask.png"));
    TargetTypeMaskImages[TargetTypeFighter] = Image::FromFile(TEXT("HeliMask.png"));
    TargetTypeMaskImages[TargetTypeAew] = Image::FromFile(TEXT("HeliMask.png"));
    TargetTypeMaskImages[TargetTypeBomber] = Image::FromFile(TEXT("HeliMask.png"));
    TargetTypeMaskImages[TargetTypeShipboard] = Image::FromFile(TEXT("HeliMask.png"));
    TargetTypeMaskImages[TargetTypeTank] = Image::FromFile(TEXT("HeliMask.png"));
    TargetTypeMaskImages[TargetTypePanzer] = Image::FromFile(TEXT("HeliMask.png"));
    TargetTypeMaskImages[TargetTypeMissile] = Image::FromFile(TEXT("HeliMask.png"));

    ExplosionTypeImages[ExplosionType0] = Image::FromFile(TEXT("Explosion0.png"));
    ExplosionTypeImages[ExplosionType1] = Image::FromFile(TEXT("Explosion1.png"));
    ExplosionTypeImages[ExplosionType2] = Image::FromFile(TEXT("Explosion2.png"));

    g_GlobalVar = new GlobalVarFrame[PLANE_COUNT + TARGET_COUNT_MAX];
    for (int i = 0; i < PLANE_COUNT + TARGET_COUNT_MAX; ++i)
    {
        for (int j = 0; j < GLOBAL_VAR_FRAME_SIZE; ++j)
        {
           g_GlobalVar[i].m_G[j] = 0.0;
        }
    }

    mclInitializeApplication(NULL, 0);
}

void __declspec(dllexport) GlobalShut()
{
    delete[] g_GlobalVar;

    for (int i = StateMapBackground0; i < StateMapBackgroundLast; ++i)
    {
        delete StateMapBackgrounds[i];
    }
    for (int i = TargetTypeHeli; i < TargetTypeLast; ++i)
    {
        delete TargetTypeImages[i];
    }
    for (int i = ExplosionType0; i < ExplosionTypeLast; ++i)
    {
        delete ExplosionTypeImages[i];
    }
    ULONG_PTR gdiplusToken = NULL; 
    GdiplusShutdown(gdiplusToken);

    mclTerminateApplication();
}

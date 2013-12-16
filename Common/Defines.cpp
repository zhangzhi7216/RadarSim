#include "stdafx.h"

#include "Defines.h"
#include <mclmcrrt.h>

Color TargetColors[] =
{
    Color::Red,
    Color::Orange,
    Color::Yellow,
    Color::Green,
    Color::Cyan,
    Color::Blue,
    Color::Purple,
};

CString TargetColorNames[] =
{
    TEXT("红色"),
    TEXT("橙色"),
    TEXT("黄色"),
    TEXT("绿色"),
    TEXT("青色"),
    TEXT("蓝色"),
    TEXT("紫色"),
};

Image *StateMapBackgrounds[];

CString StateMapBackgroundNames[] = 
{
    TEXT("地图0"),
    TEXT("地图1"),
    TEXT("地图2"),
    TEXT("地图3"),
    TEXT("地图4"),
};

Image *TargetTypeImages[] =
{
    Image::FromFile(TEXT("Heli.ico")),
    Image::FromFile(TEXT("Fighter.ico")),
    Image::FromFile(TEXT("Aew.ico")),
    Image::FromFile(TEXT("Bomber.ico")),
    Image::FromFile(TEXT("Shipboard.ico")),
    Image::FromFile(TEXT("Tank.ico")),
    Image::FromFile(TEXT("Panzer.ico")),
    Image::FromFile(TEXT("Missile.ico")),
};

CString TargetTypeNames[] =
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

void GlobalInit()
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

    TargetTypeImages[TargetTypeHeli] = Image::FromFile(TEXT("Heli.ico"));
    TargetTypeImages[TargetTypeFighter] = Image::FromFile(TEXT("Fighter.ico"));
    TargetTypeImages[TargetTypeAew] = Image::FromFile(TEXT("Aew.ico"));
    TargetTypeImages[TargetTypeBomber] = Image::FromFile(TEXT("Bomber.ico"));
    TargetTypeImages[TargetTypeShipboard] = Image::FromFile(TEXT("Shipboard.ico"));
    TargetTypeImages[TargetTypeTank] = Image::FromFile(TEXT("Tank.ico"));
    TargetTypeImages[TargetTypePanzer] = Image::FromFile(TEXT("Panzer.ico"));
    TargetTypeImages[TargetTypeMissile] = Image::FromFile(TEXT("Missile.ico"));

    mclInitializeApplication(NULL, 0);
}

void GlobalShut()
{
    for (int i = StateMapBackground0; i < StateMapBackgroundLast; ++i)
    {
        delete StateMapBackgrounds[i];
    }
    for (int i = TargetTypeHeli; i < TargetTypeLast; ++i)
    {
        delete TargetTypeImages[i];
    }
    ULONG_PTR gdiplusToken = NULL; 
    GdiplusShutdown(gdiplusToken);

    mclTerminateApplication();
}

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
    TEXT("��ɫ"),
    TEXT("��ɫ"),
    TEXT("��ɫ"),
    TEXT("��ɫ"),
    TEXT("��ɫ"),
    TEXT("��ɫ"),
    TEXT("��ɫ"),
};

Image __declspec(dllexport) *StateMapBackgrounds[];

CString __declspec(dllexport) StateMapBackgroundNames[] = 
{
    TEXT("��ͼ0"),
    TEXT("��ͼ1"),
    TEXT("��ͼ2"),
    TEXT("��ͼ3"),
    TEXT("��ͼ4"),
};

Image __declspec(dllexport) *TargetTypeImages[] =
{
    Image::FromFile(TEXT("Land.png")),
    Image::FromFile(TEXT("Sea.png")),
    Image::FromFile(TEXT("Air.png")),
};

Image __declspec(dllexport) *TargetTypeMaskImages[] =
{
    Image::FromFile(TEXT("LandMask.png")),
    Image::FromFile(TEXT("SeaMask.png")),
    Image::FromFile(TEXT("AirMask.png")),
};

CString __declspec(dllexport) TargetTypeNames[] =
{
    TEXT("·��Ŀ��"),
    TEXT("����Ŀ��"),
    TEXT("����Ŀ��"),
};

Image __declspec(dllexport) *ExplosionTypeImages[] =
{
    Image::FromFile(TEXT("Explosion0.png")),
    Image::FromFile(TEXT("Explosion1.png")),
    Image::FromFile(TEXT("Explosion2.png")),
};

CString __declspec(dllexport) ExplosionTypeNames[] =
{
    TEXT("��ը0"),
    TEXT("��ը1"),
    TEXT("��ը2"),
};

CString __declspec(dllexport) TargetMoveTypeNames[] =
{
    TEXT("����ֱ���˶�"),
    TEXT("�ȼ���ֱ���˶�"),
    TEXT("����Բ���˶�"),
};

CString __declspec(dllexport) NoiseTypeNames[] =
{
    TEXT("������"),
    TEXT("ɫ����"),
    TEXT("��������"),
};

CString __declspec(dllexport) SensorIdNames[] =
{
    TEXT("�״�"),
    TEXT("AIS"),
    TEXT("ͨ��"),
    TEXT("����"),
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

    TargetTypeImages[TargetTypeLand] = Image::FromFile(TEXT("Land.png"));
    TargetTypeImages[TargetTypeSea] = Image::FromFile(TEXT("Sea.png"));
    TargetTypeImages[TargetTypeAir] = Image::FromFile(TEXT("Air.png"));

    TargetTypeMaskImages[TargetTypeLand] = Image::FromFile(TEXT("LandMask.png"));
    TargetTypeMaskImages[TargetTypeSea] = Image::FromFile(TEXT("SeaMask.png"));
    TargetTypeMaskImages[TargetTypeAir] = Image::FromFile(TEXT("AirMask.png"));

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
    for (int i = TargetTypeLand; i < TargetTypeLast; ++i)
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

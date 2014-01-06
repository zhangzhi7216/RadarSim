/**
* @file
* @brief MatLab辅助函数头文件.
* @author ruoxi
*
* 定义了MatLab辅助函数.
*/

#pragma once

#include <mclmcrrt.h>

#include <vector>
#include <string>

#include <engine.h>

using namespace std;

typedef mxArray Array;

namespace MatlabHelper
{
    typedef bool (*RealFunc)(int, Array *[], int, Array *[]);

    typedef bool (*InitializeFunc)(void);
    /** @brief 实际在DLL中的算法初始化函数名前缀. */
    extern LPCSTR InitializeFuncPrefix;

    /** @brief 实际在DLL中的入口函数名前缀. */
    extern LPCSTR FuncPrefix;

    typedef void (*PrintStackFunc)(void);
    extern LPCSTR PrintStackFuncPrefix;

    typedef void (*TerminateFunc)(void);
    /** @brief 实际在DLL中的算法销毁函数名前缀. */
    extern LPCSTR TerminateFuncPrefix;

    Array *CreateDoubleArray(INT32 x, INT32 y,
        const unsigned char *content,
        INT32 width, INT32 height,
        INT32 startX, INT32 startY);
    Array *CreateDoubleArray(INT32 x, INT32 y,
        const double *content,
        INT32 width, INT32 height,
        INT32 startX, INT32 startY);
    Array *CreateDoubleArray(INT32 x, INT32 y,
        const unsigned char *content,
        INT32 size, INT32 start);
    Array *CreateDoubleArray(INT32 x, INT32 y,
        const double *content,
        INT32 size, INT32 start);
    void DestroyArray(Array *a);

    bool RunFunc(LPCWSTR dllFileName, LPCWSTR funcName, vector<Array *> &outputList, vector<Array *> &inputList);
    bool RealRunFunc(HINSTANCE algorithmDllHandle, const string &fullFuncName, vector<Array *> &outputList, vector<Array *> &inputList, bool &result);
};


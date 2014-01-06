/**
* @file
* @brief MatLab辅助函数cpp文件.
* @author ruoxi
*
* 实现了MatLab辅助函数.
*/
#include "stdafx.h"

#include "MatlabHelper.h"
#include <shlwapi.h>
#include "utility.h"

namespace MatlabHelper
{

LPCSTR InitializeFuncPrefix = "Initialize";

LPCSTR FuncPrefix = "mlx";

LPCSTR PrintStackFuncPrefix = "PrintStackTrace";

LPCSTR TerminateFuncPrefix = "Terminate";

Array *CreateDoubleArray(INT32 x, INT32 y,
                         const unsigned char *content,
                         INT32 width, INT32 height,
                         INT32 startX, INT32 startY)
{
    Array *a = mxCreateDoubleMatrix(x, y, mxREAL);

    double *p = mxGetPr(a);
    memset(p, 0, x * y * sizeof(double));
    if (content)
    {
        if (width && height)
        {
            for (INT32 yy = max(0, -startY); yy < min(y, height - startY); ++yy)
            {
                for (INT32 xx = max(0, -startX); xx < min(x, width - startX); ++xx)
                {
                    p[yy * x + xx] = content[(startY + yy) * width + startX + xx];
                }
            }
        }
        else
        {
            for (INT32 i = 0; i < x * y; ++i)
            {
                p[i] = content[i];
            }
        }
    }

    return a;
}

Array *CreateDoubleArray(INT32 x, INT32 y,
                         const double *content,
                         INT32 width, INT32 height,
                         INT32 startX, INT32 startY)
{
    Array *a = mxCreateDoubleMatrix(x, y, mxREAL);

    double *p = mxGetPr(a);
    memset(p, 0, x * y * sizeof(double));
    if (content)
    {
        if (width && height)
        {
            for (INT32 yy = max(0, -startY); yy < min(y, height - startY); ++yy)
            {
                for (INT32 xx = max(0, -startX); xx < min(x, width - startX); ++xx)
                {
                    p[yy * x + xx] = content[(startY + yy) * width + startX + xx];
                }
            }
        }
        else
        {
            for (INT32 i = 0; i < x * y; ++i)
            {
                p[i] = content[i];
            }
        }
    }

    return a;
}

Array *CreateDoubleArray(INT32 x, INT32 y,
                         const unsigned char *content,
                         INT32 size, INT32 start)
{
    Array *a = mxCreateDoubleMatrix(x, y, mxREAL);

    double *p = mxGetPr(a);
    memset(p, 0, x * y * sizeof(double));
    if (content)
    {
        if (size)
        {
            for (INT32 i = max(0, -start); i < min(x * y, size - start); ++i)
            {
                p[i] = content[start + i];
            }
        }
        else
        {
            for (INT32 i = 0; i < x * y; ++i)
            {
                p[i] = content[i];
            }
        }
    }

    return a;
}

Array *CreateDoubleArray(INT32 x, INT32 y,
                         const double *content,
                         INT32 size, INT32 start)
{
    Array *a = mxCreateDoubleMatrix(x, y, mxREAL);

    double *p = mxGetPr(a);
    memset(p, 0, x * y * sizeof(double));
    if (content)
    {
        if (size)
        {
            for (INT32 i = max(0, -start); i < min(x * y, size - start); ++i)
            {
                p[i] = content[start + i];
            }
        }
        else
        {
            for (INT32 i = 0; i < x * y; ++i)
            {
                p[i] = content[i];
            }
        }
    }

    return a;
}

void DestroyArray(Array *a)
{
    mxDestroyArray(a);
}

bool RunFunc(LPCWSTR dllFileName, LPCWSTR funcName, vector<Array *> &outputList, vector<Array *> &inputList)
{
    HINSTANCE algorithmDllHandle = LoadLibrary(dllFileName);

    if (!algorithmDllHandle)
    {
        return false;
    }

    bool result;
    wstring wsFuncName(funcName);
    string sFuncName(wsFuncName.begin(), wsFuncName.end());

    string initializeFuncName = sFuncName;
    initializeFuncName += InitializeFuncPrefix;

    InitializeFunc initializeFunc = (InitializeFunc)GetProcAddress(
        (HMODULE)algorithmDllHandle,
        initializeFuncName.c_str());

    if (!initializeFunc)
    {
        return false;
    }

    result = initializeFunc();

    if (!result)
    {
        return false;
    }

    string fullFuncName = sFuncName;
    fullFuncName[0] = toupper(fullFuncName[0]);
    fullFuncName = FuncPrefix + fullFuncName;

    if(!RealRunFunc(algorithmDllHandle, fullFuncName, outputList, inputList, result))
    {
        return false;
    }

    if (!result)
    {
        string printStackFuncName = sFuncName;
        printStackFuncName += PrintStackFuncPrefix;

        PrintStackFunc printStackFunc = (PrintStackFunc)GetProcAddress(
            (HMODULE)algorithmDllHandle,
            printStackFuncName.c_str());

        printStackFunc();

        return false;
    }

    string terminateFuncName = sFuncName;
    terminateFuncName += TerminateFuncPrefix;

    TerminateFunc terminateFunc = (TerminateFunc)GetProcAddress(
        (HMODULE)algorithmDllHandle,
        terminateFuncName.c_str());

    if (!terminateFunc)
    {
        return false;
    }

    // terminateFunc();

    return true;
}

/**
* @param algorithmDllHandle 算法DLL文件句柄.
* @param fullFuncName 算法入口函数全名.
* @param paramList 算法参数列表.
* @param output 算法输出.
* @param result 算法调用结果.
* @return 结果代码.
*
* 该函数被算法运行辅助函数调用,
* 用来实现对一个算法入口函数的具体调用.
* 该函数接收算法DLL文件句柄以及算法入口函数全名(通常MatLab导出的函数会添加前缀),
* 接收可变个数的参数,
* 对参数个数进行遍历,
* 实现对参数列表无差别调用(目前最大支持5个参数, 如有需要可随意增加),
* 并且记录算法调用结果.
*/
bool RealRunFunc(HINSTANCE algorithmDllHandle, const string &fullFuncName, vector<Array *> &outputList, vector<Array *> &inputList, bool &result)
{
    UINT32 outputCount = outputList.size();
    UINT32 inputCount = inputList.size();

    RealFunc func = (RealFunc)GetProcAddress(
        algorithmDllHandle,
        fullFuncName.c_str());
    if (!func)
    {
        return false;
    }
    result = func(outputCount, &outputList[0], inputCount, &inputList[0]);

    return true;
}

}

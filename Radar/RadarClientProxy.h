#pragma once

class __declspec(dllexport) RadarClientProxy
{
public:
    RadarClientProxy(void);
    virtual ~RadarClientProxy(void);

    virtual void OnClose() = 0;
    virtual void DrawTargets() = 0;
    virtual void BlendAll() = 0;
    virtual void Invalidate(BOOL bErase = 1) = 0;
    virtual BOOL RedrawWindow(LPCRECT lpRectUpdate = NULL,
		CRgn* prgnUpdate = NULL,
		UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE) = 0;
};

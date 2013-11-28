#pragma once
#include "afxsock.h"
class ClientSocket :
    public CSocket
{
public:
    ClientSocket(void);
    ~ClientSocket(void);
};


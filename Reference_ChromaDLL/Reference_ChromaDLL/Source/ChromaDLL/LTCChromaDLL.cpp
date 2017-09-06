// SWChroma.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "LTCChromaDLL.h"
#include "ChromaSDKImpl.h"

LTCChromaDLL_API LONG TriggerEvent(DWORD dwEventId, DWORD dwParameter)
{
    HMODULE hModule = GetModuleHandle(CHROMASDKDLL);
    if (hModule == NULL)
    {
        InitChromaSDK();
    }

    OnEvent(dwEventId, dwParameter);

    return 0;
}


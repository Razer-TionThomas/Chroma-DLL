#pragma once

#ifndef _CHROMASDKIMPL_H_
#define _CHROMASDKIMPL_H_

#ifndef _WINDOWS_
#include <Windows.h>
#endif

#ifndef _IOSTREAM_
#include <iostream>
#endif

#ifndef __ATLTRACE_H__
#include <atltrace.h>
#endif

#include "RzChromaSDKTypes.h"
#include "RzErrors.h"

#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif

using namespace ChromaSDK;

typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEHEADSETEFFECT)(Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEMOUSEPADEFFECT)(Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEMOUSEEFFECT)(Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYPADEFFECT)(Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATECHROMALINKEFFECT)(ChromaLink::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*DELETEEFFECT)(RZEFFECTID EffectId);

// {E416A2EF-6DF7-4230-8442-4B714960AE7E}
static const GUID CHROMA_MODULE_32 =
{ 0xe416a2ef, 0x6df7, 0x4230,{ 0x84, 0x42, 0x4b, 0x71, 0x49, 0x60, 0xae, 0x7e } };

const COLORREF BLACK = 0xFF000000 | RGB(0, 0, 0);
const COLORREF WHITE = 0xFF000000 | RGB(255, 255, 255);
const COLORREF RED = 0xFF000000 | RGB(255, 0, 0);
const COLORREF GREEN = 0xFF000000 | RGB(0, 255, 0);
const COLORREF BLUE = 0xFF000000 | RGB(0, 0, 255);
const COLORREF YELLOW = 0xFF000000 | RGB(255, 255, 0);
const COLORREF PURPLE = 0xFF000000 | RGB(128, 0, 128);
const COLORREF CYAN = 0xFF000000 | RGB(00, 255, 255);
const COLORREF ORANGE = 0xFF000000 | RGB(255, 165, 00);
const COLORREF PINK = 0xFF000000 | RGB(255, 192, 203);
const COLORREF GREY = 0xFF000000 | RGB(125, 125, 125);
const COLORREF OLIVEDRAB = 0xFF000000 | RGB(107, 142, 35);
const COLORREF INDIGO = 0xFF000000 | RGB(75, 0, 130);
const COLORREF VIOLET = 0xFF000000 | RGB(238, 130, 238);
const COLORREF LIGHTGREEN = 0xFF000000 | RGB(144, 238, 144);
const COLORREF ORANGERED = 0xFF000000 | RGB(255, 69, 0);


HMODULE g_hModule = NULL;
HANDLE g_hBaseEffect = NULL;

const COLORREF BASECOLOR = GREY;
const COLORREF ACTIVEKEYCOLOR = ORANGERED;
const COLORREF WAVECOLOR = OLIVEDRAB;
const COLORREF LOADINGCOLOR = OLIVEDRAB;
const COLORREF LOWHEALTHCOLOR = RED;
const COLORREF HITDETECTCOLOR = ORANGE;
const COLORREF OVERHEATCOLOR = YELLOW;
const COLORREF COMBOMETERCOLOR = GREEN;


Keyboard::CUSTOM_KEY_EFFECT_TYPE g_KeyboardEffect = {};

void InitChromaSDK(void)
{
    if (g_hModule == NULL)
    {
        g_hModule = LoadLibrary(CHROMASDKDLL);
    }

    if (g_hModule != NULL)
    {
        INIT Init = (INIT)GetProcAddress(g_hModule, "Init");
        if (Init != nullptr)
        {
            if (Init() == RZRESULT_SUCCESS)
            {
                for (UINT r = 0; r < Keyboard::MAX_ROW; r++)
                {
                    for (UINT c = 0; c < Keyboard::MAX_COLUMN; c++)
                    {
						g_KeyboardEffect.Color[r][c] = BLACK;
						;
                    }
                }

                g_KeyboardEffect.Key[HIBYTE(Keyboard::RZKEY_W)][LOBYTE(Keyboard::RZKEY_W)] = ACTIVEKEYCOLOR | 0x01000000;
                g_KeyboardEffect.Key[HIBYTE(Keyboard::RZKEY_A)][LOBYTE(Keyboard::RZKEY_A)] = ACTIVEKEYCOLOR | 0x01000000;
                g_KeyboardEffect.Key[HIBYTE(Keyboard::RZKEY_S)][LOBYTE(Keyboard::RZKEY_S)] = ACTIVEKEYCOLOR | 0x01000000;
                g_KeyboardEffect.Key[HIBYTE(Keyboard::RZKEY_D)][LOBYTE(Keyboard::RZKEY_D)] = ACTIVEKEYCOLOR | 0x01000000;
                g_KeyboardEffect.Key[HIBYTE(Keyboard::RZKEY_LSHIFT)][LOBYTE(Keyboard::RZKEY_LSHIFT)] = ACTIVEKEYCOLOR | 0x01000000;
                g_KeyboardEffect.Key[HIBYTE(Keyboard::RZKEY_LCTRL)][LOBYTE(Keyboard::RZKEY_LCTRL)] = ACTIVEKEYCOLOR | 0x01000000;
                g_KeyboardEffect.Key[HIBYTE(Keyboard::RZKEY_SPACE)][LOBYTE(Keyboard::RZKEY_SPACE)] = ACTIVEKEYCOLOR | 0x01000000;
            }
        }
    }
	else {
		MessageBox(NULL,
			L"Chroma Load Library Failed. Please connect Chroma devices or toggle Chroma to OFF in the settings menu",
			L"Chroma LoadLibrary",
			MB_OK);
	}
}

void UnInitChromaSDK(void)
{
    if (g_hModule != NULL)
    {
        UNINIT UnInit = (UNINIT)GetProcAddress(g_hModule, "UnInit");
        if (UnInit != nullptr)
        {
            UnInit();
        }

        FreeLibrary(g_hModule);
        g_hModule = NULL;
    }
}

DWORD WINAPI _Thread_ClearEffects(LPVOID lpParameter)
{
	CREATEKEYBOARDEFFECT fnCreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(g_hModule, "CreateKeyboardEffect");
	CREATEMOUSEEFFECT fnCreateMouseEffect = (CREATEMOUSEEFFECT)GetProcAddress(g_hModule, "CreateMouseEffect");
	CREATEMOUSEPADEFFECT fnCreateMousepadEffect = (CREATEMOUSEPADEFFECT)GetProcAddress(g_hModule, "CreateMousepadEffect");
	CREATEHEADSETEFFECT fnCreateHeadsetEffect = (CREATEHEADSETEFFECT)GetProcAddress(g_hModule, "CreateHeadsetEffect");
	CREATECHROMALINKEFFECT fnCreateChromaLinkEffect = (CREATECHROMALINKEFFECT)GetProcAddress(g_hModule, "CreateChromaLinkEffect");

	fnCreateKeyboardEffect(Keyboard::CHROMA_NONE, NULL, NULL);
	fnCreateMouseEffect(Mouse::CHROMA_NONE, NULL, NULL);
	fnCreateMousepadEffect(Mousepad::CHROMA_NONE, NULL, NULL);
	fnCreateHeadsetEffect(Headset::CHROMA_NONE, NULL, NULL);
	fnCreateChromaLinkEffect(ChromaLink::CHROMA_NONE, NULL, NULL);

	return 0;
}

DWORD WINAPI _Thread_RenderBase(LPVOID lpParameter)
{
    CREATEKEYBOARDEFFECT fnCreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(g_hModule, "CreateKeyboardEffect");
    CREATEMOUSEEFFECT fnCreateMouseEffect = (CREATEMOUSEEFFECT)GetProcAddress(g_hModule, "CreateMouseEffect");
    CREATEMOUSEPADEFFECT fnCreateMousepadEffect = (CREATEMOUSEPADEFFECT)GetProcAddress(g_hModule, "CreateMousepadEffect");
    CREATEHEADSETEFFECT fnCreateHeadsetEffect = (CREATEHEADSETEFFECT)GetProcAddress(g_hModule, "CreateHeadsetEffect");
    CREATECHROMALINKEFFECT fnCreateChromaLinkEffect = (CREATECHROMALINKEFFECT)GetProcAddress(g_hModule, "CreateChromaLinkEffect");
    CREATEEFFECT fnCreateEffect = (CREATEEFFECT)GetProcAddress(g_hModule, "CreateEffect");

	// Set base colors for keyboard
	
    Keyboard::CUSTOM_KEY_EFFECT_TYPE KeyboardEffect = {};

    KeyboardEffect.Key[HIBYTE(Keyboard::RZKEY_W)][LOBYTE(Keyboard::RZKEY_W)] = ACTIVEKEYCOLOR | 0x01000000;
    KeyboardEffect.Key[HIBYTE(Keyboard::RZKEY_A)][LOBYTE(Keyboard::RZKEY_A)] = ACTIVEKEYCOLOR | 0x01000000;
    KeyboardEffect.Key[HIBYTE(Keyboard::RZKEY_S)][LOBYTE(Keyboard::RZKEY_S)] = ACTIVEKEYCOLOR | 0x01000000;
    KeyboardEffect.Key[HIBYTE(Keyboard::RZKEY_D)][LOBYTE(Keyboard::RZKEY_D)] = ACTIVEKEYCOLOR | 0x01000000;
    KeyboardEffect.Key[HIBYTE(Keyboard::RZKEY_LSHIFT)][LOBYTE(Keyboard::RZKEY_LSHIFT)] = ACTIVEKEYCOLOR | 0x01000000;
    KeyboardEffect.Key[HIBYTE(Keyboard::RZKEY_LCTRL)][LOBYTE(Keyboard::RZKEY_LCTRL)] = ACTIVEKEYCOLOR | 0x01000000;
    KeyboardEffect.Key[HIBYTE(Keyboard::RZKEY_SPACE)][LOBYTE(Keyboard::RZKEY_SPACE)] = ACTIVEKEYCOLOR | 0x01000000;

	memcpy_s(&KeyboardEffect, sizeof(KeyboardEffect), &g_KeyboardEffect, sizeof(g_KeyboardEffect));

	for (UINT r = 0; r < Keyboard::MAX_ROW; r++)
	{
	    for (UINT c = 0; c < Keyboard::MAX_COLUMN; c++)
	    {
	        KeyboardEffect.Color[r][c] = RGB(GetRValue(BASECOLOR)*0.5, GetGValue(BASECOLOR)*0.5, GetBValue(BASECOLOR)*0.5);
	    }
	}

	// Set base colors for mouse

    Mouse::CUSTOM_EFFECT_TYPE2 MouseEffect = {};

    for (UINT r = 0; r < Mouse::MAX_ROW; r++)
    {
        for (UINT c = 0; c < Mouse::MAX_COLUMN; c++)
        {
            MouseEffect.Color[r][c] = BASECOLOR;
        }
    }

	// Set base colors for mousepad

    Mousepad::CUSTOM_EFFECT_TYPE MousepadEffect = {};

    for (UINT i = 0; i < Mousepad::MAX_LEDS; i++)
    {
        MousepadEffect.Color[i] = BASECOLOR;
    }

	// Set base colors for headset

    Headset::CUSTOM_EFFECT_TYPE HeadsetEffect = {};

    for (UINT i = 0; i < Headset::MAX_LEDS; i++)
    {
        HeadsetEffect.Color[i] = BASECOLOR;
    }

	// Set base colors for chroma link

	ChromaLink::CUSTOM_EFFECT_TYPE ChromaLinkEffect = {};

	for (UINT i = 0; i < ChromaLink::MAX_LEDS; i++)
	{
		ChromaLinkEffect.Color[i] = BASECOLOR;
	}

    fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &KeyboardEffect, NULL);
    fnCreateMouseEffect(Mouse::CHROMA_CUSTOM2, &MouseEffect, NULL);
    fnCreateMousepadEffect(Mousepad::CHROMA_CUSTOM, &MousepadEffect, NULL);
    fnCreateHeadsetEffect(Headset::CHROMA_CUSTOM, &HeadsetEffect, NULL);
	fnCreateChromaLinkEffect(ChromaLink::CHROMA_NONE, &ChromaLinkEffect, NULL);

    return 0;
}

DWORD WINAPI _Thread_RenderBaseEffectWave(LPVOID lpParameter)
{
	CREATEKEYBOARDEFFECT fnCreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(g_hModule, "CreateKeyboardEffect");
	CREATEHEADSETEFFECT fnCreateHeadsetEffect = (CREATEHEADSETEFFECT)GetProcAddress(g_hModule, "CreateHeadsetEffect");
	CREATEMOUSEPADEFFECT fnCreateMousepadEffect = (CREATEMOUSEPADEFFECT)GetProcAddress(g_hModule, "CreateMousepadEffect");
	CREATEMOUSEEFFECT fnCreateMouseEffect = (CREATEMOUSEEFFECT)GetProcAddress(g_hModule, "CreateMouseEffect");
	CREATECHROMALINKEFFECT fnCreateChromaLinkEffect = (CREATECHROMALINKEFFECT)GetProcAddress(g_hModule, "CreateChromaLinkEffect");
	CREATEEFFECT fnCreateEffect = (CREATEEFFECT)GetProcAddress(g_hModule, "CreateEffect");

	Keyboard::CUSTOM_KEY_EFFECT_TYPE KeyboardEffect = {};

	for (UINT c = 0; c < Keyboard::MAX_COLUMN-3; c+=3)
	{
		//memcpy_s(&KeyboardEffect, sizeof(KeyboardEffect), &g_KeyboardEffect, sizeof(g_KeyboardEffect));
		KeyboardEffect = {};

		for (int r = Keyboard::MAX_ROW - 1; r >= 0; r--)
		{
			if ((r == 0) && (c == 20))
				continue;   // Skip the Razer logo.
			KeyboardEffect.Color[r][c] = WAVECOLOR;
			KeyboardEffect.Color[r][c+1] = WAVECOLOR;
			KeyboardEffect.Color[r][c+2] = WAVECOLOR;
		}

		fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &KeyboardEffect, NULL);

		Sleep(50);
	}

	for (int c = Keyboard::MAX_COLUMN - 1; c >= 3; c-=3)
	{
		//memcpy_s(&KeyboardEffect, sizeof(KeyboardEffect), &g_KeyboardEffect, sizeof(g_KeyboardEffect));
		KeyboardEffect = {};

		for (int r = Keyboard::MAX_ROW - 1; r >= 0; r--)
		{
			if ((r == 0) && (c == 20))
				continue;   // Skip the Razer logo.
			KeyboardEffect.Color[r][c] = WAVECOLOR;
			KeyboardEffect.Color[r][c - 1] = WAVECOLOR;
			KeyboardEffect.Color[r][c - 2] = WAVECOLOR;
		}

		fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &KeyboardEffect, NULL);

		Sleep(50);
	}

	_Thread_RenderBase(NULL);
	return 0;

}

DWORD WINAPI _Thread_RenderLowHealth(LPVOID lpParameter)
{
    CREATEKEYBOARDEFFECT fnCreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(g_hModule, "CreateKeyboardEffect");
    CREATEHEADSETEFFECT fnCreateHeadsetEffect = (CREATEHEADSETEFFECT)GetProcAddress(g_hModule, "CreateHeadsetEffect");
    CREATEMOUSEPADEFFECT fnCreateMousepadEffect = (CREATEMOUSEPADEFFECT)GetProcAddress(g_hModule, "CreateMousepadEffect");
    CREATEMOUSEEFFECT fnCreateMouseEffect = (CREATEMOUSEEFFECT)GetProcAddress(g_hModule, "CreateMouseEffect");
    CREATECHROMALINKEFFECT fnCreateChromaLinkEffect = (CREATECHROMALINKEFFECT)GetProcAddress(g_hModule, "CreateChromaLinkEffect");
    CREATEEFFECT fnCreateEffect = (CREATEEFFECT)GetProcAddress(g_hModule, "CreateEffect");

    double Brightness[11] = { 1.0, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0 };

    Keyboard::CUSTOM_KEY_EFFECT_TYPE KeyboardEffect = {};

    memcpy_s(&KeyboardEffect, sizeof(KeyboardEffect), &g_KeyboardEffect, sizeof(g_KeyboardEffect));

    Mouse::CUSTOM_EFFECT_TYPE2 MouseEffect = {};
    Mousepad::CUSTOM_EFFECT_TYPE MousepadEffect = {};
    Headset::CUSTOM_EFFECT_TYPE HeadsetEffect = {};
	ChromaLink::CUSTOM_EFFECT_TYPE ChromaLinkEffect = {};

	UINT index = 0;
	UINT count = 0;
    while (count < 55)		// Loop through 5 times (5 * number of brightness values (i.e 11))
    {
        if (index > 10)
            index = 0;

        double brightness = Brightness[index];

		// Set low health colors for keyboard

        for (UINT r = 0; r < Keyboard::MAX_ROW; r++)
        {
            for (UINT c = 0; c < Keyboard::MAX_COLUMN; c++)
            {
                KeyboardEffect.Color[r][c] = RGB(GetRValue(LOWHEALTHCOLOR) * brightness,
                    GetGValue(LOWHEALTHCOLOR) * brightness,
                    GetBValue(LOWHEALTHCOLOR) * brightness);
            }
        }

        memcpy_s(&g_KeyboardEffect, sizeof(g_KeyboardEffect), &KeyboardEffect, sizeof(KeyboardEffect));

		// Set low health colors for mouse

        for (UINT r = 0; r < Mouse::MAX_ROW; r++)
        {
            for (UINT c = 0; c < Mouse::MAX_COLUMN; c++)
            {
                MouseEffect.Color[r][c] = RGB(GetRValue(LOWHEALTHCOLOR) * brightness,
                    GetGValue(LOWHEALTHCOLOR) * brightness,
                    GetBValue(LOWHEALTHCOLOR) * brightness);
            }
        }

		// Set low health colors for mousepad

        for (UINT i = 0; i < Mousepad::MAX_LEDS; i++)
        {
            MousepadEffect.Color[i] = RGB(GetRValue(LOWHEALTHCOLOR) * brightness,
                GetGValue(LOWHEALTHCOLOR) * brightness,
                GetBValue(LOWHEALTHCOLOR) * brightness);
        }

		// Set low health colors for headset

        for (UINT i = 0; i < Headset::MAX_LEDS; i++)
        {
            HeadsetEffect.Color[i] = RGB(GetRValue(LOWHEALTHCOLOR) * brightness,
                GetGValue(LOWHEALTHCOLOR) * brightness,
                GetBValue(LOWHEALTHCOLOR) * brightness);
        }

		// Set low health colors for ChromaLink

		for (UINT i = 0; i < ChromaLink::MAX_LEDS; i++)
		{
			ChromaLinkEffect.Color[i] = RGB(GetRValue(LOWHEALTHCOLOR) * brightness,
				GetGValue(LOWHEALTHCOLOR) * brightness,
				GetBValue(LOWHEALTHCOLOR) * brightness);
		}

        fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &KeyboardEffect, NULL);
        fnCreateMouseEffect(Mouse::CHROMA_CUSTOM2, &MouseEffect, NULL);
        fnCreateMousepadEffect(Mousepad::CHROMA_CUSTOM, &MousepadEffect, NULL);
        fnCreateHeadsetEffect(Headset::CHROMA_CUSTOM, &HeadsetEffect, NULL);
		fnCreateChromaLinkEffect(ChromaLink::CHROMA_CUSTOM, &ChromaLinkEffect, NULL);

        Sleep(100);

        index++;
		count++;
    };

	_Thread_RenderBase(NULL);
    return 0;
}

DWORD WINAPI _Thread_RenderOverHeat(LPVOID lpParameter)
{
	CREATEKEYBOARDEFFECT fnCreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(g_hModule, "CreateKeyboardEffect");
	CREATEHEADSETEFFECT fnCreateHeadsetEffect = (CREATEHEADSETEFFECT)GetProcAddress(g_hModule, "CreateHeadsetEffect");
	CREATEMOUSEPADEFFECT fnCreateMousepadEffect = (CREATEMOUSEPADEFFECT)GetProcAddress(g_hModule, "CreateMousepadEffect");
	CREATEMOUSEEFFECT fnCreateMouseEffect = (CREATEMOUSEEFFECT)GetProcAddress(g_hModule, "CreateMouseEffect");
	CREATECHROMALINKEFFECT fnCreateChromaLinkEffect = (CREATECHROMALINKEFFECT)GetProcAddress(g_hModule, "CreateChromaLinkEffect");
	CREATEEFFECT fnCreateEffect = (CREATEEFFECT)GetProcAddress(g_hModule, "CreateEffect");

	double Brightness[11] = { 1.0, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0 };

	Keyboard::CUSTOM_KEY_EFFECT_TYPE KeyboardEffect = {};

	memcpy_s(&KeyboardEffect, sizeof(KeyboardEffect), &g_KeyboardEffect, sizeof(g_KeyboardEffect));

	Mouse::CUSTOM_EFFECT_TYPE2 MouseEffect = {};
	Mousepad::CUSTOM_EFFECT_TYPE MousepadEffect = {};
	Headset::CUSTOM_EFFECT_TYPE HeadsetEffect = {};
	ChromaLink::CUSTOM_EFFECT_TYPE ChromaLinkEffect = {};

	UINT index = 0;
	UINT count = 0;
	while (count < 55)		// Loop through 5 times (5 * number of brightness values (i.e 11))
	{
		if (index > 10)
			index = 0;

		double brightness = Brightness[index];

		// Set overheat colors for keyboard

		for (UINT r = 0; r < Keyboard::MAX_ROW; r++)
		{
			for (UINT c = 0; c < Keyboard::MAX_COLUMN; c++)
			{
				KeyboardEffect.Color[r][c] = RGB(GetRValue(OVERHEATCOLOR) * brightness,
					GetGValue(OVERHEATCOLOR) * brightness,
					GetBValue(OVERHEATCOLOR) * brightness);
			}
		}

		memcpy_s(&g_KeyboardEffect, sizeof(g_KeyboardEffect), &KeyboardEffect, sizeof(KeyboardEffect));

		// Set overheat colors for mouse

		for (UINT r = 0; r < Mouse::MAX_ROW; r++)
		{
			for (UINT c = 0; c < Mouse::MAX_COLUMN; c++)
			{
				MouseEffect.Color[r][c] = RGB(GetRValue(OVERHEATCOLOR) * brightness,
					GetGValue(OVERHEATCOLOR) * brightness,
					GetBValue(OVERHEATCOLOR) * brightness);
			}
		}

		// Set overheat colors for mousepad

		for (UINT i = 0; i < Mousepad::MAX_LEDS; i++)
		{
			MousepadEffect.Color[i] = RGB(GetRValue(OVERHEATCOLOR) * brightness,
				GetGValue(OVERHEATCOLOR) * brightness,
				GetBValue(OVERHEATCOLOR) * brightness);
		}

		// Set overheat colors for headset

		for (UINT i = 0; i < Headset::MAX_LEDS; i++)
		{
			HeadsetEffect.Color[i] = RGB(GetRValue(OVERHEATCOLOR) * brightness,
				GetGValue(OVERHEATCOLOR) * brightness,
				GetBValue(OVERHEATCOLOR) * brightness);
		}

		// Set overheat colors for ChromaLink

		for (UINT i = 0; i < ChromaLink::MAX_LEDS; i++)
		{
			ChromaLinkEffect.Color[i] = RGB(GetRValue(OVERHEATCOLOR) * brightness,
				GetGValue(OVERHEATCOLOR) * brightness,
				GetBValue(OVERHEATCOLOR) * brightness);
		}

		fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &KeyboardEffect, NULL);
		fnCreateMouseEffect(Mouse::CHROMA_CUSTOM2, &MouseEffect, NULL);
		fnCreateMousepadEffect(Mousepad::CHROMA_CUSTOM, &MousepadEffect, NULL);
		fnCreateHeadsetEffect(Headset::CHROMA_CUSTOM, &HeadsetEffect, NULL);
		fnCreateChromaLinkEffect(ChromaLink::CHROMA_CUSTOM, &ChromaLinkEffect, NULL);

		Sleep(100);

		index++;
		count++;
	};

	_Thread_RenderBase(NULL);
	return 0;
}


DWORD WINAPI _Thread_RenderHitDetect(LPVOID lpParameter)
{
    CREATEKEYBOARDEFFECT fnCreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(g_hModule, "CreateKeyboardEffect");
    CREATEHEADSETEFFECT fnCreateHeadsetEffect = (CREATEHEADSETEFFECT)GetProcAddress(g_hModule, "CreateHeadsetEffect");
    CREATEMOUSEPADEFFECT fnCreateMousepadEffect = (CREATEMOUSEPADEFFECT)GetProcAddress(g_hModule, "CreateMousepadEffect");
    CREATEMOUSEEFFECT fnCreateMouseEffect = (CREATEMOUSEEFFECT)GetProcAddress(g_hModule, "CreateMouseEffect");
    CREATECHROMALINKEFFECT fnCreateChromaLinkEffect = (CREATECHROMALINKEFFECT)GetProcAddress(g_hModule, "CreateChromaLinkEffect");
    CREATEEFFECT fnCreateEffect = (CREATEEFFECT)GetProcAddress(g_hModule, "CreateEffect");

    Keyboard::CUSTOM_KEY_EFFECT_TYPE KeyboardEffect = {};

    memcpy_s(&KeyboardEffect, sizeof(KeyboardEffect), &g_KeyboardEffect, sizeof(g_KeyboardEffect));

	// Set hit detect colors for keyboard

    for (UINT r = 0; r < Keyboard::MAX_ROW; r++)
    {
        for (UINT c = 0; c < Keyboard::MAX_COLUMN; c++)
        {
            KeyboardEffect.Color[r][c] = HITDETECTCOLOR;
        }
    }

	// Set hit detect colors for mouse

    Mouse::CUSTOM_EFFECT_TYPE2 MouseEffect = {};

    for (UINT r = 0; r < Mouse::MAX_ROW; r++)
    {
        for (UINT c = 0; c < Mouse::MAX_COLUMN; c++)
        {
            MouseEffect.Color[r][c] = HITDETECTCOLOR;
        }
    }

	// Set hit detect colors for mousepad

    Mousepad::CUSTOM_EFFECT_TYPE MousepadEffect = {};

    for (UINT i = 0; i < Mousepad::MAX_LEDS; i++)
    {
        MousepadEffect.Color[i] = HITDETECTCOLOR;
    }

	// Set hit detect colors for headset

    Headset::CUSTOM_EFFECT_TYPE HeadsetEffect = {};

    for (UINT i = 0; i < Headset::MAX_LEDS; i++)
    {
        HeadsetEffect.Color[i] = HITDETECTCOLOR;
    }

	// Set hit detect colors for ChromaLink

	ChromaLink::CUSTOM_EFFECT_TYPE ChromaLinkEffect = {};

	for (UINT i = 0; i < ChromaLink::MAX_LEDS; i++)
	{
		ChromaLinkEffect.Color[i] = HITDETECTCOLOR;
	}

	// Flash 3 times
    fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &KeyboardEffect, NULL);
	fnCreateMouseEffect(Mouse::CHROMA_CUSTOM2, &MouseEffect, NULL);
	fnCreateMousepadEffect(Mousepad::CHROMA_CUSTOM, &MousepadEffect, NULL);
	fnCreateHeadsetEffect(Headset::CHROMA_CUSTOM, &HeadsetEffect, NULL);
	fnCreateChromaLinkEffect(ChromaLink::CHROMA_CUSTOM, &ChromaLinkEffect, NULL);
	Sleep(150);
	fnCreateKeyboardEffect(Keyboard::CHROMA_NONE, NULL, NULL);
	fnCreateMouseEffect(Mouse::CHROMA_NONE, NULL, NULL);
	fnCreateMousepadEffect(Mousepad::CHROMA_NONE, NULL, NULL);
	fnCreateHeadsetEffect(Headset::CHROMA_NONE, NULL, NULL);
	fnCreateChromaLinkEffect(ChromaLink::CHROMA_NONE, NULL, NULL);
	Sleep(150);

	fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &KeyboardEffect, NULL);
	fnCreateMouseEffect(Mouse::CHROMA_CUSTOM2, &MouseEffect, NULL);
	fnCreateMousepadEffect(Mousepad::CHROMA_CUSTOM, &MousepadEffect, NULL);
	fnCreateHeadsetEffect(Headset::CHROMA_CUSTOM, &HeadsetEffect, NULL);
	fnCreateChromaLinkEffect(ChromaLink::CHROMA_CUSTOM, &ChromaLinkEffect, NULL);
	Sleep(150);
	fnCreateKeyboardEffect(Keyboard::CHROMA_NONE, NULL, NULL);
	fnCreateMouseEffect(Mouse::CHROMA_NONE, NULL, NULL);
	fnCreateMousepadEffect(Mousepad::CHROMA_NONE, NULL, NULL);
	fnCreateHeadsetEffect(Headset::CHROMA_NONE, NULL, NULL);
	fnCreateChromaLinkEffect(ChromaLink::CHROMA_NONE, NULL, NULL);
	Sleep(150);

	fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &KeyboardEffect, NULL);
	fnCreateMouseEffect(Mouse::CHROMA_CUSTOM2, &MouseEffect, NULL);
	fnCreateMousepadEffect(Mousepad::CHROMA_CUSTOM, &MousepadEffect, NULL);
	fnCreateHeadsetEffect(Headset::CHROMA_CUSTOM, &HeadsetEffect, NULL);
	fnCreateChromaLinkEffect(ChromaLink::CHROMA_CUSTOM, &ChromaLinkEffect, NULL);
	Sleep(150);
	fnCreateKeyboardEffect(Keyboard::CHROMA_NONE, NULL, NULL);
	fnCreateMouseEffect(Mouse::CHROMA_NONE, NULL, NULL);
	fnCreateMousepadEffect(Mousepad::CHROMA_NONE, NULL, NULL);
	fnCreateHeadsetEffect(Headset::CHROMA_NONE, NULL, NULL);
	fnCreateChromaLinkEffect(ChromaLink::CHROMA_NONE, NULL, NULL);

    Sleep(100);

    _Thread_RenderBase(NULL);

    return 0;
}

DWORD WINAPI _Thread_RenderLoadingAnimation(LPVOID lpParameter)
{
    CREATEKEYBOARDEFFECT fnCreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(g_hModule, "CreateKeyboardEffect");
    CREATEHEADSETEFFECT fnCreateHeadsetEffect = (CREATEHEADSETEFFECT)GetProcAddress(g_hModule, "CreateHeadsetEffect");
    CREATEMOUSEPADEFFECT fnCreateMousepadEffect = (CREATEMOUSEPADEFFECT)GetProcAddress(g_hModule, "CreateMousepadEffect");
    CREATEMOUSEEFFECT fnCreateMouseEffect = (CREATEMOUSEEFFECT)GetProcAddress(g_hModule, "CreateMouseEffect");
    CREATECHROMALINKEFFECT fnCreateChromaLinkEffect = (CREATECHROMALINKEFFECT)GetProcAddress(g_hModule, "CreateChromaLinkEffect");
    CREATEEFFECT fnCreateEffect = (CREATEEFFECT)GetProcAddress(g_hModule, "CreateEffect");

	fnCreateKeyboardEffect(Keyboard::CHROMA_NONE, NULL, NULL);
	fnCreateMouseEffect(Mouse::CHROMA_NONE, NULL, NULL);
	fnCreateMousepadEffect(Mousepad::CHROMA_NONE, NULL, NULL);

	// Loading Animation on Keyboard
    Keyboard::CUSTOM_KEY_EFFECT_TYPE KeyboardEffect = {};

	for (UINT r = 0; r<Keyboard::MAX_ROW; r++)
	{
		for (UINT c = 0; c<3; c++)
		{
			KeyboardEffect.Color[r][c] = LOADINGCOLOR;
		}
	}

	fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM, &KeyboardEffect, NULL);
	Sleep(400);

	for (UINT r = 0; r<Keyboard::MAX_ROW; r++)
	{
		for (UINT c = 0; c<6; c++)
		{
			KeyboardEffect.Color[r][c] = LOADINGCOLOR;
		}
	}

	fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM, &KeyboardEffect, NULL);
	Sleep(400);

	for (UINT r = 0; r<Keyboard::MAX_ROW; r++)
	{
		for (UINT c = 0; c<9; c++)
		{
			KeyboardEffect.Color[r][c] = LOADINGCOLOR;
		}
	}

	fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM, &KeyboardEffect, NULL);
	Sleep(400);

	for (UINT r = 0; r<Keyboard::MAX_ROW; r++)
	{
		for (UINT c = 0; c<12; c++)
		{
			KeyboardEffect.Color[r][c] = LOADINGCOLOR;
		}
	}

	fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM, &KeyboardEffect, NULL);
	Sleep(400);

	for (UINT r = 0; r<Keyboard::MAX_ROW; r++)
	{
		for (UINT c = 0; c<15; c++)
		{
			KeyboardEffect.Color[r][c] = LOADINGCOLOR;
		}
	}

	KeyboardEffect.Color[HIBYTE(Keyboard::RZLED_LOGO)][LOBYTE(Keyboard::RZLED_LOGO)] = LOADINGCOLOR;

	fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM, &KeyboardEffect, NULL);
	Sleep(400);

	for (UINT r = 0; r<Keyboard::MAX_ROW; r++)
	{
		for (UINT c = 0; c<18; c++)
		{
			KeyboardEffect.Color[r][c] = LOADINGCOLOR;
		}
	}

	fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM, &KeyboardEffect, NULL);
	Sleep(400);

	Keyboard::STATIC_EFFECT_TYPE StaticEffect = {};
	StaticEffect.Color = LOADINGCOLOR;

	fnCreateKeyboardEffect(Keyboard::CHROMA_STATIC, &StaticEffect, NULL);
	Sleep(400);
	
	// Loading Animation on Mouse

	Mouse::CUSTOM_EFFECT_TYPE2 MouseEffect = {};

	for (UINT i = 0; i < Mouse::MAX_ROW; i++)
	{
		for (UINT j = 0; j < Mouse::MAX_COLUMN; j++)
		{
			MouseEffect.Color[Mouse::MAX_ROW - 1 - i][j] = LOADINGCOLOR;
		}

		fnCreateMouseEffect(Mouse::CHROMA_CUSTOM2, &MouseEffect, NULL);
		Sleep(400);
	}

	// Loading Animation on Mousepad

	Mousepad::CUSTOM_EFFECT_TYPE MousePadEffect = {};

	for (UINT i = 0; i<Mousepad::MAX_LEDS; i++)
	{
		MousePadEffect.Color[Mousepad::MAX_LEDS - 1 - i] = LOADINGCOLOR;

		fnCreateMousepadEffect(Mousepad::CHROMA_CUSTOM, &MousePadEffect, NULL);

		Sleep(400);
	}

	// Loading Animation on Headset

	Headset::CUSTOM_EFFECT_TYPE HeadsetEffect = {};

	for (UINT i = 0; i < Headset::MAX_LEDS; i++)
	{
		HeadsetEffect.Color[Headset::MAX_LEDS - 1 - i] = LOADINGCOLOR;

		fnCreateHeadsetEffect(Headset::CHROMA_CUSTOM, &HeadsetEffect, NULL);

		Sleep(400);
	}

	// Loading Animation on ChromaLink

	ChromaLink::CUSTOM_EFFECT_TYPE ChromaLinkEffect = {};

	for (UINT i = 0; i < Headset::MAX_LEDS; i++)
	{
		ChromaLinkEffect.Color[Headset::MAX_LEDS - 1 - i] = LOADINGCOLOR;

		fnCreateChromaLinkEffect(ChromaLink::CHROMA_CUSTOM, &ChromaLinkEffect, NULL);

		Sleep(400);
	}

    _Thread_RenderBase(NULL);

    return 0;
}

DWORD WINAPI _Thread_RenderComboMeter(LPVOID lpParameter)
{
	CREATEKEYBOARDEFFECT fnCreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(g_hModule, "CreateKeyboardEffect");
	CREATEEFFECT fnCreateEffect = (CREATEEFFECT)GetProcAddress(g_hModule, "CreateEffect");

	Keyboard::CUSTOM_KEY_EFFECT_TYPE KeyboardEffect = {};

	//memcpy_s(&KeyboardEffect, sizeof(KeyboardEffect), &g_KeyboardEffect, sizeof(g_KeyboardEffect));

	UINT HpLevel = Keyboard::RZKEY_F1;
	for (UINT i = HpLevel; i < HpLevel+12; i++)
	{
		// F1 key starts from row 0, column 3 to column column 15
		KeyboardEffect.Color[0][i] = COMBOMETERCOLOR;

		fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM, &KeyboardEffect, NULL);
		Sleep(300);
	}

	//_Thread_RenderBase(NULL);

	return 0;
}

DWORD WINAPI _Thread_RenderMaxCombo(LPVOID lpParameter)
{
	CREATEKEYBOARDEFFECT fnCreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(g_hModule, "CreateKeyboardEffect");
	CREATEHEADSETEFFECT fnCreateHeadsetEffect = (CREATEHEADSETEFFECT)GetProcAddress(g_hModule, "CreateHeadsetEffect");
	CREATEMOUSEPADEFFECT fnCreateMousepadEffect = (CREATEMOUSEPADEFFECT)GetProcAddress(g_hModule, "CreateMousepadEffect");
	CREATEMOUSEEFFECT fnCreateMouseEffect = (CREATEMOUSEEFFECT)GetProcAddress(g_hModule, "CreateMouseEffect");
	CREATECHROMALINKEFFECT fnCreateChromaLinkEffect = (CREATECHROMALINKEFFECT)GetProcAddress(g_hModule, "CreateChromaLinkEffect");
	CREATEEFFECT fnCreateEffect = (CREATEEFFECT)GetProcAddress(g_hModule, "CreateEffect");

	COLORREF rainbowArray[] = { RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET };

	Keyboard::CUSTOM_KEY_EFFECT_TYPE KeyboardEffect = {};
	Mouse::CUSTOM_EFFECT_TYPE2 MouseEffect = {};
	Mousepad::CUSTOM_EFFECT_TYPE MousePadEffect = {};
	Headset::CUSTOM_EFFECT_TYPE HeadsetEffect = {};
	ChromaLink::CUSTOM_EFFECT_TYPE ChromaLinkEffect = {};

	int colorIndex = 0, count = 0;

	while (count < 21) {		// Cycle 3 times
		if (colorIndex == 7)
			colorIndex = 0;

		// Keybaord Spectrum Cycle
		for (UINT r = 0; r < Keyboard::MAX_ROW; r++)
		{
			for (UINT c = 0; c < Keyboard::MAX_COLUMN; c++)
			{
				KeyboardEffect.Color[r][c] = rainbowArray[colorIndex];
			}
		}

		// Mouse Spectrum Cycle
		for (UINT i = 0; i < Mouse::MAX_ROW; i++)
		{
			for (UINT j = 0; j < Mouse::MAX_COLUMN; j++)
			{
				MouseEffect.Color[Mouse::MAX_ROW - 1 - i][j] = rainbowArray[colorIndex];
			}
		}

		// Mousepad Spectrum Cycle
		for (UINT i = 0; i<Mousepad::MAX_LEDS; i++)
		{
			MousePadEffect.Color[Mousepad::MAX_LEDS - 1 - i] = rainbowArray[colorIndex];
		}

		// Headset Spectrum Cycle
		for (UINT i = 0; i < Headset::MAX_LEDS; i++)
		{
			HeadsetEffect.Color[Headset::MAX_LEDS - 1 - i] = rainbowArray[colorIndex];
		}

		// Chroma Link Spectrum Cycle
		for (UINT i = 0; i < Headset::MAX_LEDS; i++)
		{
			ChromaLinkEffect.Color[Headset::MAX_LEDS - 1 - i] = rainbowArray[colorIndex];
		}

		fnCreateKeyboardEffect(Keyboard::CHROMA_CUSTOM, &KeyboardEffect, NULL);
		fnCreateMouseEffect(Mouse::CHROMA_CUSTOM2, &MouseEffect, NULL);
		fnCreateMousepadEffect(Mousepad::CHROMA_CUSTOM, &MousePadEffect, NULL);
		fnCreateHeadsetEffect(Headset::CHROMA_CUSTOM, &HeadsetEffect, NULL);
		fnCreateChromaLinkEffect(ChromaLink::CHROMA_CUSTOM, &ChromaLinkEffect, NULL);
		Sleep(100);

		colorIndex++;
		count++;
	}

	_Thread_RenderBase(NULL);

	return 0;
}

void __cdecl OnEvent(DWORD dwEventId, DWORD dwParameter=0)
{
    switch (dwEventId)
    {
	case EVENT_CLEAR:
		{
			HANDLE hThread = CreateThread(NULL, 0, _Thread_ClearEffects, NULL, 0, NULL);
			CloseHandle(hThread);
		}
		break;
    case EVENT_BASE:
        {
            HANDLE hThread = CreateThread(NULL, 0, _Thread_RenderBase, NULL, 0, NULL);
            CloseHandle(hThread);
        }
        break;
	case EVENT_BASEWAVE:
		{
			HANDLE hThread = CreateThread(NULL, 0, _Thread_RenderBaseEffectWave, NULL, 0, NULL);
			CloseHandle(hThread);
		}
		break;
    case EVENT_LOW_HEALTH_ACTIVE:
        {
            HANDLE hThread = CreateThread(NULL, 0, _Thread_RenderLowHealth, NULL, 0, NULL);
			CloseHandle(hThread);
        }
        break;
    case EVENT_HIT_DETECT:
        {
            HANDLE hThread = CreateThread(NULL, 0, _Thread_RenderHitDetect, NULL, 0, NULL);
            CloseHandle(hThread);
        }
        break;
    case EVENT_LOADING_ANIMATION:
        {
            HANDLE hThread = CreateThread(NULL, 0, _Thread_RenderLoadingAnimation, NULL, 0, NULL);
            CloseHandle(hThread);
        }
        break;
	case EVENT_OVERHEAT:
		{
			HANDLE hThread = CreateThread(NULL, 0, _Thread_RenderOverHeat, NULL, 0, NULL);
			CloseHandle(hThread);
		}
		break;
	case EVENT_COMBO_METER:
	{
		HANDLE hThread = CreateThread(NULL, 0, _Thread_RenderComboMeter, NULL, 0, NULL);
		CloseHandle(hThread);
	}
		break;
	case EVENT_MAX_COMBO:
		{
			HANDLE hThread = CreateThread(NULL, 0, _Thread_RenderMaxCombo, NULL, 0, NULL);
			CloseHandle(hThread);
		}
		break;
    }
}

#endif

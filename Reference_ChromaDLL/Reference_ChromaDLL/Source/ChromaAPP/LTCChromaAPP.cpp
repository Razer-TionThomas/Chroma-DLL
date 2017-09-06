// LTCChromaAPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include "..\LTCChromaDLL\LTCChromaDLL.h"

using namespace std;

typedef LONG(*TRIGGEREVENT)(DWORD, PVOID);

int main()
{
    HMODULE hModule = LoadLibrary(_T("LTCChromaDLL.dll"));
    if (hModule == NULL) return -1;

    TRIGGEREVENT TriggerEvent = (TRIGGEREVENT)GetProcAddress(hModule, "TriggerEvent");
    if (TriggerEvent == NULL) return -1;

    int i;
    while (TRUE)
    {
        cout << "0. Exit." << endl;
		cout << "1. Clear Effects." << endl;
        cout << "2. Base Effect." << endl;
		cout << "3. Base Effect With Wave" << endl;
        cout << "4. Low health Effect" << endl;
        cout << "5. Hit detect Effect" << endl;
        cout << "6. Loading Animation Effect" << endl;
		cout << "7. Overheat Effect" << endl;
		cout << "8. Combo Meter Effect" << endl;
		cout << "9. Max Combo Effect" << endl;
        cout << "Choose an effect : ";
        cin >> i;
        cout << endl;

		if (i != 0)
		{
			switch (i)
			{
			case 1:
				TriggerEvent(EVENT_CLEAR, NULL);
				break;
			case 2:
				TriggerEvent(EVENT_BASE, NULL);
				break;
			case 3:
				TriggerEvent(EVENT_BASEWAVE, NULL);
				break;
			case 4:
				TriggerEvent(EVENT_LOW_HEALTH_ACTIVE, NULL);
				break;
			case 5:
				TriggerEvent(EVENT_HIT_DETECT, NULL);
				break;
			case 6:
				TriggerEvent(EVENT_LOADING_ANIMATION, NULL);
				break;
			case 7:
				TriggerEvent(EVENT_OVERHEAT, NULL);
				break;
			case 8:
				TriggerEvent(EVENT_COMBO_METER, NULL);
				break;
			case 9:
				TriggerEvent(EVENT_MAX_COMBO, NULL);
				break;
			}
		}
        else
        {
            break;
        }

        cout << endl;
    }

    return 0;
}


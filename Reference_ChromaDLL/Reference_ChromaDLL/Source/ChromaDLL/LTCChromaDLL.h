// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SWCHROMA_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// REFERENCECHROMADLL_EXPORTS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LTCChromaDLL_EXPORTS
#define LTCChromaDLL_API __declspec(dllexport)
#else
#define LTCChromaDLL_API __declspec(dllimport)
#endif

typedef enum _EVENTTYPE
{
	EVENT_CLEAR = 1,
    EVENT_BASE,
	EVENT_BASEWAVE,
    EVENT_LOW_HEALTH_ACTIVE,
    EVENT_HIT_DETECT,
    EVENT_LOADING_ANIMATION,
	EVENT_OVERHEAT,
	EVENT_COMBO_METER,
	EVENT_MAX_COMBO,
    EVENT_END
} EVENTTYPE;

#ifdef __cplusplus
extern "C" {
#endif

	LTCChromaDLL_API LONG TriggerEvent(DWORD dwEventId, DWORD dwParameter);

#ifdef __cplusplus
}
#endif


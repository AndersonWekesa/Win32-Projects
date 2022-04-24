#pragma once


#define BTN_CLOSE 1


#include "resource.h"



class TME {
	public:
		BOOL tracking;

	TME() {
		tracking = FALSE;
	}

	void startTracking(HWND hButton) {
		TRACKMOUSEEVENT tme;

		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER;
		tme.dwHoverTime = 9000;
		tme.hwndTrack = hButton;

		tracking = TRUE;

	}

	void stopTracking() {
		tracking = FALSE;
	}

};

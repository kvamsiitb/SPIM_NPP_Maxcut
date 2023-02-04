/************************ \cond COPYRIGHT *****************************
 *                                                                    *
 * Copyright (C) 2020 HOLOEYE Photonics AG. All rights reserved.      *
 * Contact: https://holoeye.com/contact/                              *
 *                                                                    *
 * This file is part of HOLOEYE SLM Display SDK.                      *
 *                                                                    *
 * You may use this file under the terms and conditions of the        *
 * "HOLOEYE SLM Display SDK Standard License v1.0" license agreement. *
 *                                                                    *
 **************************** \endcond ********************************/


// This file is used by all examples to open the SLM preview window. Opening the preview can of course also be done
// directly in the code, but with this file you can set a position for all examples in one place.
// To make this script work, we need to import the SLM Display SDK into MATLAB, call heds_types as well as
// heds_slm_init(), like done in the examples.

// If using this file in combination with the SLM Display SDK C++ API, i.e. when also including
//#include <holoeye_slmdisplaysdk.hpp>
// in your application, you must include this file inside the holoeye namespace, i.e. after the line
//using namespace holoeye;
// See our example files for more information.
// This way you can use this file also in pure C environments.

#include <holoeye_slmdisplaysdk.h>

heds_errorcode show_slm_preview(float scale HOLOEYE_DEFARG(0.0), heds_slmpreviewflags_t flags HOLOEYE_DEFARG(0))
{
    heds_errorcode error = HEDSERR_NoError;

    // Open the SLM preview window. This might have an impact on performance if fast data playback is desired.
    // Typically, the "Realtime preview" mode (default) does not impact performance too much,
    // but the "Capture SLM screen" mode will have an impact on playback performance.
    error = heds_utils_slmpreview_show(true);
    if (error != HEDSERR_NoError)
        return error;

    // Set scale in SLM preview:
    // 0.0 means "Fit" scale mode which shows whole SLM screen.
    // "Fit" is the default scale mode if not using this function.
    // Use "scale = 1.0" to show data without any interpolation.
    // Down-scaling interpolation can make data look completely different,
    // esp. for phase data with small structures.
    // Please see SLM Display SDK manual for more information.
    // The flags parameter is used here to be able to add additional flags to the
    // HEDSSLMPF_OnTop option, which we want to set by default in all examples.
    error = heds_utils_slmpreview_set(HEDSSLMPF_OnTop + flags, scale);
    if (error != HEDSERR_NoError)
        return error;

    // Move the preview window out of main display center position:
    // (x, y, w=0, h=0); x and y are the screen coordinates, w=0 and h=0 does
    // not change width and height.
    // Please adapt or comment (disable) this line if preview window
    // moves to wrong position or is not visible.
    error = heds_utils_slmpreview_move(150, 75, 0, 0);
    if (error != HEDSERR_NoError)
        return error;

    return error;
}

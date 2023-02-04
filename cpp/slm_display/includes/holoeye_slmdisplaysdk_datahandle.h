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


#ifndef HOLOEYE_SLMDISPLAYSDK_DATAHANDLE_H
#define HOLOEYE_SLMDISPLAYSDK_DATAHANDLE_H

#include "holoeye_slmdisplaysdk_types.h"

/** \ingroup SLMDisplayCAdvanced */
/** The format of the slm data. */
typedef enum
{
    /** No format was specified. Mainly used for default values. */
    HEDSDHFMT_Undefined = 0,

    /** A grayscale 8-bit data field. */
    HEDSDHFMT_Grayscale8 = 1,

    /** A grayscale float data field. The values must be between 0..1. */
    HEDSDHFMT_GrayscaleFloat = 3,

    /** A grayscale double data field. The values must be between 0..1. */
    HEDSDHFMT_GrayscaleDouble = 4,

    /** A 24bit data field with three color channels. */
    HEDSDHFMT_RGB24 = 5,

    /** A 32bit data field with four color channels. */
    HEDSDHFMT_RGBA32 = 6
} heds_datahandle_format;

/** \ingroup SLMDisplayCAdvanced */
/** The current state of the data. */
typedef enum
{
    /** The data was just created. */
    HEDSDHST_Issued = 0,

    /** The given filename or data is waiting for processing. */
    HEDSDHST_WaitingForProcessing = 1,

    /** The given filename is being loaded. */
    HEDSDHST_LoadingFile = 2,

    /** The given or loaded data needs to be converted. Performance Warning! */
    HEDSDHST_ConvertingData = 3,

    /** The data is being processed for display. This is not about conversion but about processing the data. */
    HEDSDHST_ProcessingData = 4,

    /** The data is waiting to be transferred to the gpu. */
    HEDSDHST_WaitingForTransfer = 5,

    /** The data is uploaded to the gpu. */
    HEDSDHST_TransferringData = 6,

    /** The data is ready to be rendered. This is the end state of the loading process. */
    HEDSDHST_ReadyToRender = 7,

    /** The data is waiting to be rendered. This is the first state when showing data. */
    HEDSDHST_WaitingForRendering = 8,

    /** The data is being rendered. This is about the actual effort needed to render the data. */
    HEDSDHST_Rendering = 9,

    /** The data is waiting to become visible on the SLM. Only applies to deferred rendering. */
    HEDSDHST_WaitingToBecomeVisible = 10,

    /** The data is currently visible on the slm. */
    HEDSDHST_Visible = 11,

    /** The data has been shown and is now no longer visible. */
    HEDSDHST_Finished = 12,

    /** An error occured. Check error code. */
    HEDSDHST_Error = 13
} heds_datahandle_state;

/** \ingroup SLMDisplayCAdvanced */
/** Represents the values provided by \ref heds_datahandle_statistics(). */
typedef enum
{
    /** The time between issuing a command it being picked up by the process in milliseconds. Is -1 when it has not been picked up by the process yet. */
    HEDSSTAT_DelayTimeMs = 0,

    /** The loading time of the handle in milliseconds. Is -1 when no loading happened. */
    HEDSSTAT_LoadingTimeMs = 1,

    /** The conversion time of the handle in milliseconds. Is -1 when no conversion happened. */
    HEDSSTAT_ConversionTimeMs = 2,

    /** The processing time of the handle in milliseconds. Is -1 when no processing happened. */
    HEDSSTAT_ProcessingTimeMs = 3,

    /** The transfer time of the handle in milliseconds. Is -1 when no transfer happened. */
    HEDSSTAT_TransferTimeMs = 4,

    /** The render time of the handle in milliseconds. Is -1 when no rendering happened. */
    HEDSSTAT_RenderTimeMs = 5,

    /** The time between rendering the handle and becoming visible on the SLM in milliseconds. Is -1 when no rendering was done before. */
    HEDSSTAT_BecomeVisibleTimeMs = 6,

    /** The visible time of the handle since it was last rendered in milliseconds. Is -1 when the handle was never shown. */
    HEDSSTAT_VisibleFrameTimeMs = 7,

    /** The visible time of the handle in milliseconds. Is -1 when the handle was never shown. */
    HEDSSTAT_VisibleTimeMs = 8,

    /** The number of statistics available. */
    HEDSSTAT_COUNT = 9
} heds_datahandle_stats;

/** \ingroup SLMDisplayCAdvanced */
/** Represents the flags for \ref heds_datahandle_applyvalue. */
typedef unsigned int heds_datahandle_applyvalues;

/** \ingroup SLMDisplayCAdvanced */
/** Represents the different datahandle values which can be applied/written during an update. */
typedef enum
{
    /** No values will be applied. */
    HEDSDHAV_None = 0,

    /** The value of \ref heds_datahandle.durationInFrames will be applied. */
    HEDSDHAV_DurationInFrames = 1,

    /** The value of \ref heds_datahandle.dataFlags will be applied. */
    HEDSDHAV_ShowFlags = 2,

    /** The value of \ref heds_datahandle.phaseWrap will be applied. */
    HEDSDHAV_PhaseWrap = 4,

    /** The value of \ref heds_datahandle.transformShiftX, \ref heds_datahandle.transformShiftY and \ref heds_datahandle.transformScale will be applied. */
    HEDSDHAV_Transform = 8,

    /** The value of \ref heds_datahandle.beamSteerX, \ref heds_datahandle.beamSteerY and \ref heds_datahandle.beamLens will be applied. */
    HEDSDHAV_BeamManipulation = 16,

    /** The value of \ref heds_datahandle.gamma will be applied. */
    HEDSDHAV_Gamma = 32,

    /** The value of \ref heds_datahandle.valueOffset will be applied. */
    HEDSDHAV_ValueOffset = 64
} heds_datahandle_applyvalue;

/** \ingroup SLMDisplayCAdvanced */
/** Represents an id for a \ref heds_datahandle object. Use \ref heds_datahandle_update to retrieve it. */
typedef unsigned int heds_datahandle_id;

/** \ingroup SLMDisplayCAdvanced */
/** Represents an action for \ref heds_datahandle that did not happen yet or was not required. */
static const unsigned short heds_datahandle_notdone = 0xFFFF;

#pragma pack(push, 1)
/** Represents data has been loaded to the slm. This handle can be used to show and release data.
 * \ingroup SLMDisplayCAdvanced */
typedef struct
{
    /** The id of the data handle. The id distinguishes one handle from another. */
    heds_datahandle_id id;

    /** The state of the data handle. When the state is \ref HEDSDHST_Error, then the error is specified in \ref errorCode.
     *  See \ref heds_datahandle_state for more info. */
    unsigned char state;

    /** The error code of the data handle. The value of \p state will always be \ref HEDSDHST_Error when there is an error.
     *  See \ref heds_errorcode for more info. */
    unsigned char errorCode;

    /** The canvas on which the data is shown. */
    char canvas;


    /** \cond INTERNALDOC */

    /** For memory alignment. */
    unsigned char __padding1__;


    /** \endcond */

    /** Specified for how many frames the data should be shown. This is the minimum duration.
     *  The value is based on the refresh rate of the device, so setting a duration of 4 frames means it will be shown for (4 / refresh_rate Hz). */
    unsigned char durationInFrames;

    /** The format of the data. Refer to \ref heds_datahandle_format for allowed values. */
    unsigned char dataFormat;

    /** The width of the data in pixels. */
    unsigned short dataWidth;

    /** The height of the data in pixels. */
    unsigned short dataHeight;

    /** The pitch of the data in bytes per line of pixels. */
    unsigned short dataPitch;

    /** The phase wrap of the provided data, by default 2Pi. */
    float phaseWrap;

    /** Stores how the data was loaded and how it should be presented on the slm. */
    heds_dataflags_t dataFlags;

    /** The horizontal shift of the data when displayed on the slm. */
    short transformShiftX;

    /** The vertical shift of the data when displayed on the slm. */
    short transformShiftY;

    /** The scaling of the data when displayed on the slm. */
    float transformScale;

    /** A blazed grating is added to the displayed data when this parameter is not zero. The blazed grating will steer the incident light in x-direction. Please use values in the range [-1.0, 1.0]. For values out of this range the result does not make sense due to the pixel size of the SLM, i.e. for values 1.0 and -1.0 a binary grating is addressed. 
     *  Please use the function \ref heds_utils_beam_steer_from_angle_rad or \ref heds_utils_beam_steer_from_angle_deg to calculate this parameter out of the deviation angle. The reverse calculation can be done with \ref heds_utils_beam_steer_to_angle_rad and \ref heds_utils_beam_steer_to_angle_deg. */
    float beamSteerX;

    /** A blazed grating is added to the displayed data when this parameter is not zero. The blazed grating will steer the incident light in y-direction. Please use values in the range [-1.0, 1.0]. For values out of this range the result does not make sense due to the pixel size of the SLM, i.e. for values 1.0 and -1.0 a binary grating is addressed. 
     *  Please use the function \ref heds_utils_beam_steer_from_angle_rad or \ref heds_utils_beam_steer_from_angle_deg to calculate this parameter out of the deviation angle. The reverse calculation can be done with \ref heds_utils_beam_steer_to_angle_rad and \ref heds_utils_beam_steer_to_angle_deg. */
    float beamSteerY;

    /** A Fresnel zone lens is added to the displayed data when this parameter is not zero. The Fresnel zone lens will defocus the incident light. The lens power is defined so that values in range [-1.0, 1.0] produces pixel-correct results, and out of this range (e.g. 2.0) Moire-effects occurs which starts in the SLM-edges and grow with an increasing value. Therefore, depending on your application, values out of the recommended range may still produce valid lens function, but are in general not pixel-correct. 
     *  Please use the function \ref heds_utils_beam_lens_from_focal_length_mm to calculate this parameter out of the desired focal length. The reverse calculation can be done with \ref heds_utils_beam_lens_to_focal_length_mm. */
    float beamLens;

    /** The gamma curve applied when displayed on the slm. */
    float gamma;

    /** This value is added to the data when displayed on the slm. */
    float valueOffset;

    /** The time between issuing the command and it being picked up by the process. */
    unsigned short delayTimeMs;

    /** The time in milliseconds the data had to wait to be processed. */
    unsigned short processingWaitTimeMs;

    /** The time in milliseconds it took to load the data when loaded from a file. If no file was loaded, the value is \ref heds_datahandle_notdone. */
    unsigned short loadingTimeMs;

    /** The time in milliseconds it took to convert the data for displaying.
     *  If not conversion was required the value is \ref heds_datahandle_notdone.
     *  Ideally this is always the case since this value represents only type conversions. */
    unsigned short conversionTimeMs;

    /** The time in milliseconds it took to process the data. Meaning mathematical operations
     *  required to show the data like converting phase values into grayscale data.
     *  If not processing was required the value will be \ref heds_datahandle_notdone. */
    unsigned short processingTimeMs;

    /** The time in milliseconds the data had to wait after being processed, to be sent to the graphics card.
     *  If the data is not ready yet to be ransferred, the value will be \ref heds_datahandle_notdone. */
    unsigned short transferWaitTimeMs;

    /** The time in milliseconds it took to transfer the data onto the gpu.
     *  If the data has not yet been transferred or an error occured, the value will be \ref heds_datahandle_notdone. */
    unsigned short transferTimeMs;

    /** The time in milliseconds the data had to wait to be rendered.
     *  If show function has been called yet, the value will be \ref heds_datahandle_notdone. */
    unsigned short renderWaitTimeMs;

    /** The time in milliseconds it took to render the data. This is not the time it was visible but the actual time the rendering took.
     *  The value always represents the first frame the data was rendered.
     *  If the data has not yet been rendered or an error occured, the value will be \ref heds_datahandle_notdone. */
    unsigned short renderTimeMs;

    /** The time in milliseconds the data had to wait to become visible on the SLM. Only applies to deferred rendering.
     *  If the data has not yet been waiting to become visible or an error occured, the value will be \ref heds_datahandle_notdone. */
    unsigned short becomeVisibleTimeMs;

    /** The current time in milliseconds the data is visible on the SLM.
     *  If the data has not yet been shown or an error occured, the value will be \ref heds_datahandle_notdone. */
    unsigned short visibleTimeMs;

    /** The time in milliseconds the data is visible on the SLM, since it was last rendered.
     *  If the data has not yet been shown or an error occured, the value will be \ref heds_datahandle_notdone. */
    unsigned short visibleFrameTimeMs;

} heds_datahandle;
#pragma pack(pop)

#endif // HOLOEYE_SLMDISPLAYSDK_DATAHANDLE_H

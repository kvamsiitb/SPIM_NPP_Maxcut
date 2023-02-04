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


#ifndef HOLOEYE_SLMDISPLAYSDK_TYPES_H
#define HOLOEYE_SLMDISPLAYSDK_TYPES_H

/** \ingroup SLMDisplayC */
/** The configuration that must be used to initialize the SLM. */
typedef struct
{
    /** The software package using the SDK. */
    int api;

    /** The version of the software package. Required. */
    char apiname[32];
} heds_configuration;

/** \ingroup SLMDisplayC */
/** Flags which define how data should be loaded and presented on the slm.
 * \see ShowFlags
 * \see LoadFlags */
typedef unsigned int heds_dataflags_t;

/** \ingroup SLMDisplayC */
/** A list of available load flags. */
enum heds_loadflags
{
    /** The default load behavior, optimized for performance. */
    HEDSLDF_Default = 0,

    /** Generate a colortable for the provided data. Does not apply to real data and phasevalues. */
    HEDSLDF_CreateColorTable = 65536,

    /** Shows the data after loading it and transfers the data right before showing it. Reduces wait time when showing data in real-time. */
    HEDSLDF_ShowImmediately = 65536 * 2,

    /** This load flag inverts the transpose option in showflags. Please use this when the API data is stored in Fortran memory layout instead of C memory layout. */
    HEDSLDF_TransposeData = 65536 * 4,
};

/** \ingroup SLMDisplayC */
/** A list of available show flags. */
enum heds_showflags
{
    /** Shows two-dimensional data unscaled at the center of the slm. One-dimensional data is shown as a grating. */
    HEDSSHF_PresentAutomatic = 0,

    /** The data is shown unscaled at the center of the slm.
     *  Free areas are filled with zeroes. The data is cropped to the slm size. */
    HEDSSHF_PresentCentered = 1,

    /** If set, the data will fit the slm so all data is visible and the aspect ratio is kept.
     *  Free areas on the top/bottom or left/right will be filled with zeroes.
     *  Only one of the present flags may be set.
     *  This option changes the scale of the displayed data. Therefore this show flag overwrites the transformScale option in a data handle. */
    HEDSSHF_PresentFitWithBars = 2,

    /** If set, the data will fit the slm so the slm is completely filled with data but the aspect ratio is kept.
     *  Some data might not be visible. Only one of the present flags may be set.
     *  This option changes the scale of the displayed data. Therefore this show flag overwrites the transformScale option in a data handle. */
    HEDSSHF_PresentFitNoBars = 4,

    /** If set, the data will completely fill the slm. The aspect ratio will not be kept.
     *  In short the data is shown fullscreen. Only one of the present flags may be set.
     *  This option changes the scale of the displayed data. Therefore this show flag overwrites the transformScale option in a data handle. */
    HEDSSHF_PresentFitScreen = 8,

    /** Shows the given data in a tiling pattern. The pattern is tiled around the center of the slm. */
    HEDSSHF_PresentTiledCentered = 16,

    /** If set, the rows and columns will be switched. */
    HEDSSHF_TransposeData = 32,

    /** If set, the data will be flipped horizontally. */
    HEDSSHF_FlipHorizontal = 64,

    /** If set, the data will be flipped vertically. */
    HEDSSHF_FlipVertical = 128,

    /** If set, the data will be inverted. */
    HEDSSHF_InvertValues = 256
};

/** The error code type.
 * \ingroup SLMDisplayC
 * \see heds_error_codes */
typedef int heds_errorcode;

/** \ingroup SLMDisplayC */
/** The code for any error that occured. */
enum heds_errorcodes
{
    /** No error. */
    HEDSERR_NoError = 0,

    /** The given SLM instance does not exist. */
    HEDSERR_NoSLMConnected = 1,

    /** The given filename is zero or too long. */
    HEDSERR_InvalidFilename = 2,

    /** A filename was given, but the file does not exist. */
    HEDSERR_FileNotFound = 3,

    /** A filename was given and the file exists, but the file format is not supported. */
    HEDSERR_UnsupportedFileFormat = 4,

    /** The given data is zero or too long. */
    HEDSERR_InvalidDataPointer = 5,

    /** The given data has a width less than one. */
    HEDSERR_InvalidDataWidth = 6,

    /** The given data has a height less than one. */
    HEDSERR_InvalidDataHeight = 7,

    /** A valid and supported filename or data was given, but the contained format is not supported. */
    HEDSERR_UnsupportedDataFormat = 8,

    /** The renderer had an internal error, for example when updating the window or sprite. */
    HEDSERR_InternalRendererError = 9,

    /** There is not enough system memory left to process the given filename or data. */
    HEDSERR_OutOfSystemMemory = 10,

    /** Data transfer into video memory failed. There is either not enough video memory left on the GPU, or the maximum number of data handles with data on the GPU has been reached. Please release unused data handles and try again. */
    HEDSERR_OutOfVideoMemory = 11,

    /** The current handle is invalid. */
    HEDSERR_InvalidHandle = 12,

    /** The provided duration in frames is less than one or higher than 255. */
    HEDSERR_InvalidDurationInFrames = 13,

    /** The given phase wrap must be greater than zero. */
    HEDSERR_InvalidPhaseWrap = 14,

    /** Waiting for a datahandle to reach a certain state timed out and failed. */
    HEDSERR_WaitForHandleTimedOut = 15,

    /** The number of Zernike values must be between zero and \ref HEDSZER_COUNT. */
    HEDSERR_InvalidZernikeValueSize = 19,

    /** The scale needs to be greater than zero. */
    HEDSERR_InvalidTransformScale = 20,

    /** The value of a given enum is invalid. */
    HEDSERR_InvalidEnumValue = 21,

    /** One of the arguments is invalid. */
    HEDSERR_InvalidArgument = 22,

    /** The specified canvas does not exist. */
    HEDSERR_InvalidCanvas = 23,

    /** The data is locked to another canvas. */
    HEDSERR_LockedToOtherCanvas = 24,

    /** The specified custom shader could not be found. */
    HEDSERR_CustomShaderNotFound = 25,

    /** The specified custom shader ha no data function. */
    HEDSERR_CustomShaderHasNoDataFunction = 26,

    /** The custom shader could not be compiled. */
    HEDSERR_CustomerShaderFailedToCompile = 27,

    /** Failed to connect to host. */
    HEDSERR_ConnectionFailed = 28,

    /** Internal network timeout occurred. */
    HEDSERR_ConnectionTimedOut = 29,

    /** There was an internal error during the connection. */
    HEDSERR_ConnectionInternalError = 30,

    /** The handle does not belong to the given instance. */
    HEDSERR_HandleInstanceMismatch = 31,

    /** The canvas does not belong to the given instance. */
    HEDSERR_CanvasInstanceMismatch = 32,

    /** The given data was not a two-dimensional array. */
    HEDSERR_InvalidDataDimensions = 100,

    /** The provided value for width_a is zero or less. */
    HEDSERR_GratingWidthAInvalid = 101,

    /** The provided value for width_b is zero or less. */
    HEDSERR_GratingWidthBInvalid = 102
};

/** \ingroup SLMDisplayC */
/** A list of the supported Zernike functions and their position in the list of values. */
enum heds_zernikevalues
{
    /** The radius is always the first argument and is required. It is provided in pixels. */
    HEDSZER_RadiusPx = 0,

    /** The Tilt X function. f = x */
    HEDSZER_TiltX = 1,

    /** The Tilt Y function. f = y */
    HEDSZER_TiltY = 2,

    /** The Astig 45deg function. f = 2xy */
    HEDSZER_Astig45 = 3,

    /** The Defocus function. f = 1-2x^2-2y^2 */
    HEDSZER_Defocus = 4,

    /** The Astig 0deg function. f = y^2-x^2 */
    HEDSZER_Astig0 = 5,

    /** The Trifoil X function. f = 3xy^2-x^3 */
    HEDSZER_TrifoilX = 6,

    /** The Coma X function. f = -2x+3xy^2+3x^3 */
    HEDSZER_ComaX = 7,

    /** The Coma Y function. f = -2y+3y^3+3x^2y */
    HEDSZER_ComaY = 8,

    /** The Trifoil Y function. f = y^3-3x^2y */
    HEDSZER_TrifoilY = 9,

    /** The Quadrafoil X function. f = 4y^3x-4x^3y */
    HEDSZER_QuadrafoilX = 10,

    /** The Astig 2nd 45deg function. f = -6xy+8y^3x+8x^3y */
    HEDSZER_Astig2nd45 = 11,

    /** The Spherical ABB function. f = 1-6y^2-6x^2+6y^4+12x^2y^2+6x^4 */
    HEDSZER_SphericalABB = 12,

    /** The Astig 2nd 0deg function. f = -3y^2+3x^2+4y^4-4x^2y^2-4x^4 */
    HEDSZER_Astig2nd0 = 13,

    /** The Quadrafoil Y function. f = y^4-6x^2y^2+x^4 */
    HEDSZER_QuadrafoilY = 14,

    /** The number of supported Zernike values. */
    HEDSZER_COUNT = 15
};

/** \ingroup SLMDisplayC */
/** Represents the flags for the preview window. \see SLMPreviewFlags */
typedef unsigned int heds_slmpreviewflags_t;

/** \ingroup SLMDisplayC */
/** Represents the different settings for the preview window. */
enum heds_slmpreviewflags
{
    /** No settings will be applied. */
    HEDSSLMPF_None = 0,

    /** Disables the border of the preview window. */
    HEDSSLMPF_NoBorder = 1,

    /** Makes sure the window is on top of other windows. */
    HEDSSLMPF_OnTop = 2,

    /** Shows the Zernike radius. */
    HEDSSLMPF_ShowZernikeRadius = 4,

    /** Show the wavefront compensation in the preview. */
    HEDSSLMPF_ShowWavefrontCompensation = 8
};

/** \ingroup SLMDisplayC */
/** Represents the flags for the wavefront compensation. */
typedef unsigned int heds_wavefrontcompensationflags_t;

/** \ingroup SLMDisplayC */
/** Represents the different settings for the wavefront compensation. \see WavefrontcompensationFlags */
enum heds_wavefrontcompensationflags
{
    /** No flags will be applied. */
    HEDSWCF_None = 0,

    /** Flips the wavefront compensation horizontally. */
    HEDSWCF_FlipHorizontally = 1,

    /** Flips the wavefront compensation vertically. */
    HEDSWCF_FlipVertically = 2
};

#endif // HOLOEYE_SLMDISPLAYSDK_TYPES_H

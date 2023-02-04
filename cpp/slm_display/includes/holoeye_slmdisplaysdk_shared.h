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


#ifndef HOLOEYE_SLMDISPLAYSDK_SHARED_H
#define HOLOEYE_SLMDISPLAYSDK_SHARED_H

#include "holoeye_platforms.h"
#include "holoeye_slmdisplaysdk_types.h"
#include <string.h>

#if HOLOEYE_WINDOWS
    #include <wchar.h>
#endif

#define HEDS_CONFIGURATION_STR_2(value) #value
#define HEDS_CONFIGURATION_STR(value) HEDS_CONFIGURATION_STR_2(value)
#define HEDS_CONFIGURATION_SETAPINAME(config, str) memcpy(&config.apiname, str, strlen(str));

#if defined(_MSC_VER)
    #define HEDS_CONFIGURATION_APINAME(config) HEDS_CONFIGURATION_SETAPINAME(config, "MSVC " HEDS_CONFIGURATION_STR(_MSC_VER))
#elif defined(__GNUC__)
    #define HEDS_CONFIGURATION_APINAME(config) HEDS_CONFIGURATION_SETAPINAME(config, "GCC " HEDS_CONFIGURATION_STR(__GNUC__) "." HEDS_CONFIGURATION_STR(__GNUC_MINOR__) "." HEDS_CONFIGURATION_STR(__GNUC_PATCHLEVEL__))
#elif defined(__clang_major__)
    #define HEDS_CONFIGURATION_APINAME(config) HEDS_CONFIGURATION_SETAPINAME(config, "clang " HEDS_CONFIGURATION_STR(__clang_major__) "." HEDS_CONFIGURATION_STR(__clang_minor__) "." HEDS_CONFIGURATION_STR(__clang_patchlevel__))
#elif defined(__MINGW32_MAJOR_VERSION)
    #define HEDS_CONFIGURATION_APINAME(config) HEDS_CONFIGURATION_SETAPINAME(config, "MinGW " HEDS_CONFIGURATION_STR(__MINGW32_MAJOR_VERSION) "." HEDS_CONFIGURATION_STR(__MINGW32_MINOR_VERSION))
#elif defined(__MINGW64_MAJOR_VERSION)
    #define HEDS_CONFIGURATION_APINAME(config) HEDS_CONFIGURATION_SETAPINAME(config, "MinGW-w64 " HEDS_CONFIGURATION_STR(__MINGW64_MAJOR_VERSION) "." HEDS_CONFIGURATION_STR(__MINGW64_MINOR_VERSION))
#else
    #define HEDS_CONFIGURATION_APINAME(config)
#endif

#define HEDS_CONFIGURATION_DEFAULT(config) \
    heds_configuration config; \
    memset(&config, 0, sizeof(heds_configuration)); \
    config.api = 1; \
    HEDS_CONFIGURATION_APINAME(config)

HOLOEYE_CCODE_BEGIN()

#if HOLOEYE_WINDOWS
    /** The character type of the file system.
     * \ingroup SLMDisplayC */
    typedef wchar_t fschar;

    /** A 16-bit unicode character.
     * \ingroup SLMDisplayC */
    typedef wchar_t ucchar;

    /** The delimiter for paths for the current system.
     * \ingroup SLMDisplayC */
    #define fsdelimiter L'\\'
#else
    /** The character type of the file system.
     * \ingroup SLMDisplayC */
    typedef char fschar;

    /** A 16-bit unicode character.
     * \ingroup SLMDisplayC */
    typedef short ucchar;

    /** The delimiter for paths for the current system.
     * \ingroup SLMDisplayC */
    #define fsdelimiter '/'
#endif

/** A small struct that provides the size of the SLM in pixels.
 * \ingroup SLMDisplayC */
typedef struct
{
    /** The width of the SLM in pixels. */
    int width;

    /** The height of the SLM in pixels. */
    int height;
} heds_slm_size_t;

/** A small struct that provides the size of the SLM's active area in millimeters.
 * \ingroup SLMDisplayC */
typedef struct
{
    /** The width of the SLM's active area in millimeters. */
    double width;

    /** The height of the SLM's active area in millimeters. */
    double height;
} heds_slm_size_mm_t;

/** Provides an error string in ASCII for a given error.
 * \ingroup SLMDisplayC
 * \see heds_errorcodes
 * \param error The error code to provide the string for.
 * \return An error string for the given error. Zero when the given error is invalid. */
HOLOEYE_API const char* heds_error_string_ascii(heds_errorcode error);

/** Provides an error string in Unicode for a given error.
 *  Unlike \ref heds_error_string_ascii, this function is not thread-safe and only exists for convenience reasons.
 * \ingroup SLMDisplayC
 * \see heds_errorcodes
 * \param error The error code to provide the string for.
 * \return An error string for the given error. Zero when the given error is invalid. */
HOLOEYE_API const wchar_t* heds_error_string_unicode(heds_errorcode error);

/** Initialises the detection and selection of the used SLM device. Must be called before using most functions of SLM Display SDK.
 * \ingroup SLMDisplayC
 * \see heds_errorcodes
 * \param configuration The configuration used to init the SLM. This is a required input argument.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_slm_init(const heds_configuration *configuration);

/** Closes the slm window.
 * \ingroup SLMDisplayC */
HOLOEYE_API void heds_slm_close();

/** Provides the version of the SDK as a string in ASCII.
 * \ingroup SLMDisplayC
 * \return The version string. */
HOLOEYE_API const char* heds_info_version_string_ascii();

/** Provides the version of the SDK as a string in Unicode.
 * \ingroup SLMDisplayC
 * \return The version string. */
HOLOEYE_API const wchar_t* heds_info_version_string_unicode();

/** Provides the major version of the SDK.
 *  The version format is major.minor.hotfix.revision.
 * \ingroup SLMDisplayC
 * \return The major number of the version. */
HOLOEYE_API int heds_info_version_major();

/** Provides the minor version of the SDK.
 *  The version format is major.minor.hotfix.revision.
 * \ingroup SLMDisplayC
 * \return The minor number of the version. */
HOLOEYE_API int heds_info_version_minor();

/** Provides the hotfix version of the SDK.
 *  The version format is major.minor.hotfix.revision.
 * \ingroup SLMDisplayC
 * \return The hotfix number of the version. */
HOLOEYE_API int heds_info_version_hotfix();

/** Provides the revision of the SDK.
 *  The version format is major.minor.hotfix.revision.
 * \ingroup SLMDisplayC
 * \return The revision number of the version. */
HOLOEYE_API int heds_info_version_revision();

/** Provides the version of the API implemented by the SDK.
 *  The API version is not directly related to the SDK version.
 * \ingroup SLMDisplayC
 * \return The API version of the SDK. */
HOLOEYE_API int heds_info_version_api();

/** Checks if the used SDK provides the correct API version required by the user's code.
 * If the function fails, text will be written to the console window. In addition a messagebox can be shown when \p showMessagebox is HOLOEYE_TRUE.
 * \ingroup SLMDisplayC
 * \param requiredApiVersion The API required by your code.
 * \param showMessagebox Defines if a messagebox will be shown, if there is an error.
 * \return Return HOLOEYE_TRUE when the API version of the SDK is the same as the provided \p requiredApiVersion. */
HOLOEYE_API HOLOEYE_BOOL heds_requires_version(int requiredApiVersion, HOLOEYE_BOOL showMessagebox);

/** Waits for a given amount of time.
 * \ingroup SLMDisplayC
 * \param millisecondsToWait The number of milliseconds to wait. */
HOLOEYE_API void heds_utils_wait_ms(int millisecondsToWait);

/** Waits for a given amount of time.
 * \ingroup SLMDisplayC
 * \param secondsToWait The number of seconds to wait. */
HOLOEYE_API void heds_utils_wait_s(double secondsToWait);

/** Waits for a given amount of time. Ends when the SLM process is closed.
 * \ingroup SLMDisplayC
 * \param millisecondsToWait The number of milliseconds to wait.
 * \return HEDSERR_NoError when the desired wait time was reached. */
HOLOEYE_API heds_errorcode heds_utils_wait_checked_ms(int millisecondsToWait);

/** Waits for a given amount of time. Ends when the SLM process is closed.
 * \ingroup SLMDisplayC
 * \param secondsToWait The number of seconds to wait.
 * \return HEDSERR_NoError when the desired wait time was reached. */
HOLOEYE_API heds_errorcode heds_utils_wait_checked_s(double secondsToWait);

/** Waits until the SLM process is closed.
 * \ingroup SLMDisplayC
 * \return HEDSERR_NoError when the SLM was closed. */
HOLOEYE_API heds_errorcode heds_utils_wait_until_closed();

/** Show or hide the SLM preview window. SLM must be initialized before opening the preview window.
 * \ingroup SLMDisplayC
 * \param show Pass HOLOEYE_TRUE (1) to open the preview window and HOLOEYE_FALSE (0) to close it.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_utils_slmpreview_show(HOLOEYE_BOOL show HOLOEYE_DEFARG(HOLOEYE_TRUE));

/** Sets settings and the zoom factor of the preview window.
 * \ingroup SLMDisplayC
 * \param flags The preview flags to set. Refer to \ref heds_slmpreviewflags for details.
 * \param zoom The zoom factor of the preview window. Use zero to make the data fit the screen.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_utils_slmpreview_set(heds_slmpreviewflags_t flags, float zoom);

/** Changes the position and size of the preview window.
 * \ingroup SLMDisplayC
 * \param posX The horizontal position of the window on the desktop.
 * \param posY The vertical position of the window on the desktop.
 * \param width The width of the window. If \p width or \p height is zero, the size will not be changed.
 * \param height The height of the window. If \p width or \p height is zero, the size will not be changed.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_utils_slmpreview_move(int posX, int posY, int width HOLOEYE_DEFARG(0), int height HOLOEYE_DEFARG(0));

/** Utilities function for calculating proper values for the beam manipulation parameters in heds_datahandle.
 *  The function takes the beam steer value from the data handle property as an input and calculates the corresponding
 *  steering angle of the incident light in radian.
 *  The SLM must be initialized properly in order to return the correct value.
 *  In case of an error due to uninitialized SLM the function returns 0.0f.
 * \ingroup SLMDisplayC
 * \param wavelength_nm The wavelength of the incident light in SI-unit nanometer.
 * \param beam_steer The parameter passed to the data handle property "beamSteerX" or "beamSteerY". Best optical blaze results are gained for values between -1.0f and 1.0f.
 * \return Returns the corresponding deviation angle in radian (full circle is 2*pi rad). */
HOLOEYE_API float heds_utils_beam_steer_to_angle_rad(float wavelength_nm, float beam_steer);

/** Utilities function for calculating proper values for the beam manipulation parameters in heds_datahandle.
 *  The function takes the beam steer value from the data handle property as an input and calculates the corresponding
 *  steering angle of the incident light in degree.
 *  The SLM must be initialized properly in order to return the correct value.
 *  In case of an error due to uninitialized SLM the function returns 0.0f.
 * \ingroup SLMDisplayC
 * \param wavelength_nm The wavelength of the incident light in SI-unit nanometer.
 * \param beam_steer The parameter passed to the data handle property "beamSteerX" or "beamSteerY". Best optical blaze results are gained for values between -1.0f and 1.0f.
 * \return Returns the corresponding deviation angle in degree (full circle is 360 degree). */
HOLOEYE_API float heds_utils_beam_steer_to_angle_deg(float wavelength_nm, float beam_steer);

/** Utilities function for calculating proper values for the beam manipulation parameters in heds_datahandle.
 *  The function takes the desired steering angle of the incident light in radian as an input and calculates the
 *  corresponding beam steer parameter to be passed into data handle. The beam steer parameter is normalized to
 *  meaningful values in the range from -1.0f to +1.0f. The value corresponds to steering from one side of the
 *  unit cell to the other side in the far field of a holographic projection.
 *  The SLM must be initialized properly in order to return the correct value.
 *  In case of an error due to uninitialized SLM the function returns 0.0f.
 * \ingroup SLMDisplayC
 * \param wavelength_nm The wavelength of the incident light in SI-unit nanometer.
 * \param steering_angle_rad Desired steering angle of the incident light in radian (full circle is 2*pi rad).
 * \return Returns the corresponding beam steer parameter to be passed into data handle. Values in range [-1.0f, 1.0f] are recommended. */
HOLOEYE_API float heds_utils_beam_steer_from_angle_rad(float wavelength_nm, float steering_angle_rad);

/** Utilities function for calculating proper values for the beam manipulation parameters in heds_datahandle.
 *  The function takes the desired steering angle of the incident light in degree as an input and calculates the
 *  corresponding beam steer parameter to be passed into data handle. The beam steer parameter is normalized to
 *  meaningful values in the range from -1.0f to +1.0f. The value corresponds to steering from one side of the
 *  unit cell to the other side in the far field of a holographic projection.
 *  The SLM must be initialized properly in order to return the correct value.
 *  In case of an error due to uninitialized SLM the function returns 0.0f.
 * \ingroup SLMDisplayC
 * \param wavelength_nm The wavelength of the incident light in SI-unit nanometer.
 * \param steering_angle_deg Desired steering angle of the incident light in degree (full circle is 360 degree).
 * \return Returns the corresponding beam steer parameter to be passed into data handle. Values in range [-1.0f, 1.0f] are recommended. */
HOLOEYE_API float heds_utils_beam_steer_from_angle_deg(float wavelength_nm, float steering_angle_deg);

/** Utilities function for calculating proper values for the beam manipulation parameters in heds_datahandle.
 *  The function takes the beam lens parameter value from the data handle property as an input and calculates
 *  the corresponding focal length of the Fresnel zone lens addressed with the given beam lens parameter.
 *  The beam lens parameter is proportional to the lens power (1/f) and is scaled so that for values in the range
 *  between -1.0f and +1.0f the addressed phase function has no artifacts due to the pixel size of the SLM.
 *  Higher absolute values might still produce valid optical lens results, but the quality of the addressed lens
 *  phase function will degrade with an increasing absolute value above 1.0f.
 *  The SLM must be initialized properly in order to return the correct value.
 *  In case of an error due to uninitialized SLM the function returns 0.0f.
 * \ingroup SLMDisplayC
 * \param wavelength_nm The wavelength of the incident light in SI-unit nanometer.
 * \param beam_lens The parameter passed to the data handle property "beamLens". Values in range [-1.0f, 1.0f] are recommended.
 * \return Returns the corresponding focal length of the Fresnel zone lens in SI-unit mm. */
HOLOEYE_API float heds_utils_beam_lens_to_focal_length_mm(float wavelength_nm, float beam_lens);

/** Utilities function for calculating proper values for the beam manipulation parameters in heds_datahandle.
 *  The function takes the desired folcal length as an input and calculates the corresponding beam lens parameter.
 *  The beam lens parameter is proportional to the lens power (1/f) and is scaled so that for values in the range
 *  between -1.0f and +1.0f the addressed phase function has no artifacts due to the pixel size of the SLM.
 *  Higher absolute values might still produce valid optical lens results, but the quality of the addressed lens
 *  phase function will degrade with an increasing absolute value above 1.0f.
 *  The SLM must be initialized properly in order to return the correct value.
 *  In case of an error due to uninitialized SLM the function returns 0.0f.
 * \ingroup SLMDisplayC
 * \param wavelength_nm The wavelength of the incident light in SI-unit nanometer.
 * \param focal_length_mm Desired focal length in SI-unit millimeter.
 * \return Returns the corresponding "beamLens" parameter to be passed into data handle. Values in range [-1.0f, 1.0f] are recommended. */
HOLOEYE_API float heds_utils_beam_lens_from_focal_length_mm(float wavelength_nm, float focal_length_mm);

/** Provides the pixelsize of the connected SLM device. The unit of the returned pixel size is micro-meter.
 * \ingroup SLMDisplayC
 * \see heds_slm_pixelsize_m()
 * \return The SLM's pixelsize. Returns zero when the device was not initialized. */
HOLOEYE_API double heds_slm_pixelsize_um();

/** Provides the pixelsize of the connected SLM device. The unit of the returned pixel size is the SI unit meter.
 * \ingroup SLMDisplayC
 * \see heds_slm_pixelsize_um()
 * \return The SLM's pixelsize. Returns zero when the device was not initialized. */
HOLOEYE_API double heds_slm_pixelsize_m();

/** Provides the refresh rate of the connected device. The unit of the returned refresh rate is is derived SI unit Hz.
 * \ingroup SLMDisplayC
 * \return The SLM's current refreshrate. Returns zero when the device was not initialized. */
HOLOEYE_API float heds_slm_refreshrate_hz();

/** Provides the width and height of the connected SLM device in number of pixels.
 * \ingroup SLMDisplayC
 * \return Zero width and height when no device is set up yet. A width and height greater than zero represents the SLM's size in pixel. */
HOLOEYE_API heds_slm_size_t heds_slm_size_px();

/** Provides the width of the connected SLM device in number of pixels.
 * \ingroup SLMDisplayC
 * \return Zero width when no device is set up yet. A width greater than zero represents the SLM's width in pixel. */
HOLOEYE_API int heds_slm_width_px();

/** Provides the height of the connected SLM device in number of pixels.
 * \ingroup SLMDisplayC
 * \return Zero height when no device is set up yet. A height greater than zero represents the SLM's height in pixel. */
HOLOEYE_API int heds_slm_height_px();

/** Provides the width and height of the SLM's active area in millimeters.
 * \ingroup SLMDisplayC
 * \return Zero width and height when no device is set up yet. A width and height greater than zero represents the SLM's size in millimeters. */
HOLOEYE_API heds_slm_size_mm_t heds_slm_size_mm();

/** Provides the width of the SLM's active area in millimeters.
 * \ingroup SLMDisplayC
 * \return Zero width when no device is set up yet. A width greater than zero represents the SLM's width in millimeters. */
HOLOEYE_API double heds_slm_width_mm();

/** Provides the height of the SLM's active area in millimeters.
 * \ingroup SLMDisplayC
 * \return Zero height when no device is set up yet. A height greater than zero represents the SLM's height in millimeters. */
HOLOEYE_API double heds_slm_height_mm();

/** Sets Zernike parameter values for the SLM.
 * \ingroup SLMDisplayC
 * \param values An array of Zernike values. Refer to \ref heds_zernikevalues for more details. Set to zero to disable Zernike again.
 * \param valuesCount The number of floats in \p values. Must be zero or greater and less than \ref HEDSZER_COUNT. Set to zero to disable Zernike again.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_slm_zernike(const float *values, int valuesCount);

/** Loads Zernike parameters from a HOLOEYE Zernike parameter file, for example saved by HOLOEYE SLM Pattern Generator.
 *  The loaded Zernike parameters are directly applied to the SLM.
 *  The filename parameter takes a Latin-1 ASCII string.
 * \ingroup SLMDisplayC
 * \param filename The Zernike parameter filename to load.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_slm_zernike_load_latin1(const char *filename);

/** Loads Zernike parameters from a HOLOEYE Zernike parameter file, for example saved by HOLOEYE SLM Pattern Generator.
 *  The loaded Zernike parameters are directly applied to the SLM.
 *  The filename parameter takes a UTF-8 string.
 * \ingroup SLMDisplayC
 * \param filename The Zernike parameter filename to load.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_slm_zernike_load_utf8(const char *filename);

/** Loads Zernike parameters from a HOLOEYE Zernike parameter file, for example saved by HOLOEYE SLM Pattern Generator.
 *  The loaded Zernike parameters are directly applied to the SLM.
 *  The filename parameter takes a unicode string.
 * \ingroup SLMDisplayC
 * \param filename The Zernike parameter filename to load.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_slm_zernike_load_unicode(const wchar_t *filename);

/** Loads Zernike parameters from a HOLOEYE Zernike parameter file, for example saved by HOLOEYE SLM Pattern Generator.
 *  The loaded Zernike parameters are directly applied to the SLM.
 *  The filename parameter takes a 16-bit unicode string.
 * \ingroup SLMDisplayC
 * \param filename The Zernike parameter filename to load.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_slm_zernike_load_unicode16(const ucchar *filename);

/** Loads and sets a wavefront compensation, using a Latin-1 ASCII string.
 * \ingroup SLMDisplayC
 * \param filename The wavefront compensation H5 file to load.
 * \param wavelength_nm The wavelength in nanometers to load the wavefront compensation for.
 * \param flags Modify how the wavefront compensation will be shown.
 * \param shift_x Shift the wavefront compensation field in x direction.
 * \param shift_y Shift the wavefront compensation field in y direction.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_slm_wavefrontcompensation_load_latin1(const char *filename, double wavelength_nm, heds_wavefrontcompensationflags_t flags HOLOEYE_DEFARG(HEDSWCF_None), int shift_x HOLOEYE_DEFARG(0), int shift_y HOLOEYE_DEFARG(0));

/** Loads and sets a wavefront compensation, using a UTF-8 string.
 * \ingroup SLMDisplayC
 * \param filename The wavefront compensation H5 file to load.
 * \param wavelength_nm The wavelength in nanometers to load the wavefront compensation for.
 * \param flags Modify how the wavefront compensation will be shown.
 * \param shift_x Shift the wavefront compensation field in x direction.
 * \param shift_y Shift the wavefront compensation field in y direction.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_slm_wavefrontcompensation_load_utf8(const char *filename, double wavelength_nm, heds_wavefrontcompensationflags_t flags HOLOEYE_DEFARG(HEDSWCF_None), int shift_x HOLOEYE_DEFARG(0), int shift_y HOLOEYE_DEFARG(0));

/** Loads and sets a wavefront compensation, using a unicode string.
 * \ingroup SLMDisplayC
 * \param filename The wavefront compensation H5 file to load.
 * \param wavelength_nm The wavelength in nanometers to load the wavefront compensation for.
 * \param flags Modify how the wavefront compensation will be shown.
 * \param shift_x Shift the wavefront compensation field in x direction.
 * \param shift_y Shift the wavefront compensation field in y direction.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_slm_wavefrontcompensation_load_unicode(const wchar_t *filename, double wavelength_nm, heds_wavefrontcompensationflags_t flags HOLOEYE_DEFARG(HEDSWCF_None), int shift_x HOLOEYE_DEFARG(0), int shift_y HOLOEYE_DEFARG(0));

/** Loads and sets a wavefront compensation, using a 16-bit unicode string.
 * \ingroup SLMDisplayC
 * \param filename The wavefront compensation H5 file to load.
 * \param wavelength_nm The wavelength in nanometers to load the wavefront compensation for.
 * \param flags Modify how the wavefront compensation will be shown.
 * \param shift_x Shift the wavefront compensation field in x direction.
 * \param shift_y Shift the wavefront compensation field in y direction.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_slm_wavefrontcompensation_load_unicode16(const ucchar *filename, double wavelength_nm, heds_wavefrontcompensationflags_t flags HOLOEYE_DEFARG(HEDSWCF_None), int shift_x HOLOEYE_DEFARG(0), int shift_y HOLOEYE_DEFARG(0));

/** Clears any loaded wavefront compensation.
 * \ingroup SLMDisplayC
 * \return ERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_slm_wavefrontcompensation_clear();

HOLOEYE_CCODE_END()

#endif // HOLOEYE_SLMDISPLAYSDK_SHARED_H

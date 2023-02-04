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


#ifndef HOLOEYE_SLMDISPLAYSDK_HANDLES_H
#define HOLOEYE_SLMDISPLAYSDK_HANDLES_H

#include "holoeye_platforms.h"
#include "holoeye_slmdisplaysdk_shared.h"
#include "holoeye_slmdisplaysdk_types.h"
#include "holoeye_slmdisplaysdk_datahandle.h"

HOLOEYE_CCODE_BEGIN()

/** Loads the given data to be shown on the SLM. Each unsigned character represents a single pixel.
 * \ingroup SLMDisplayCAdvanced
 * \see heds_errorcodes
 * \param handle A pointer to an unused data handle. The resulting handle will be stored here.
 * \param width The width of the given data field.
 * \param height The height of the given data field.
 * \param data The pointer to grayscale values. The size must be at least \p width * \p height bytes.
 * \param pitch The size of each line in bytes. Pass zero to use the default pitch.
 * \param loadFlags The flags used when loading the data.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_load_data_grayscale_uchar(heds_datahandle *handle, int width, int height, const unsigned char *data,
                                                          int pitch HOLOEYE_DEFARG(0), heds_dataflags_t loadFlags HOLOEYE_DEFARG(HEDSLDF_Default));

/** Loads the given data to be shown on the SLM. The values are expected to be in blocks of three unsigned characters
 *  representing a single RGB values for each pixel.
 * \ingroup SLMDisplayCAdvanced
 * \see heds_errorcodes
 * \param handle A pointer to an unused data handle. The resulting handle will be stored here.
 * \param width The width of the given data field.
 * \param height The height of the given data field.
 * \param data The pointer to RGB values. The size must be at least \p width * \p height * 3 bytes.
 * \param pitch The size of each line in bytes. Pass zero to use the default pitch.
 * \param loadFlags The flags used when loading the data.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_load_data_rgb24(heds_datahandle *handle, int width, int height, const unsigned char *data, int pitch HOLOEYE_DEFARG(0), heds_dataflags_t loadFlags HOLOEYE_DEFARG(HEDSLDF_Default));

/** Loads the given data to be shown on the SLM. Each unsigned integer represents a full RGBA value, in that order. The alpha value is ignored.
 * \ingroup SLMDisplayCAdvanced
 * \see heds_errorcodes
 * \param handle A pointer to an unused data handle. The resulting handle will be stored here.
 * \param width The width of the given data field.
 * \param height The height of the given data field.
 * \param data The pointer to RGBA values. The size must be at least \p width * \p height * 4 bytes.
 * \param pitch The size of each line in bytes. Pass zero to use the default pitch.
 * \param loadFlags The flags used when loading the data.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_load_data_rgba32(heds_datahandle *handle, int width, int height, const unsigned int *data, int pitch HOLOEYE_DEFARG(0), heds_dataflags_t loadFlags HOLOEYE_DEFARG(HEDSLDF_Default));

/** Loads the given data to be shown on the SLM. Each float represents a single pixel. Values outside 0.0f and 1.0f are clamped.
 * \ingroup SLMDisplayCAdvanced
 * \see heds_errorcodes
 * \param handle A pointer to an unused data handle. The resulting handle will be stored here.
 * \param width The width of the given data field.
 * \param height The height of the given data field.
 * \param data The pointer to grayscale values. The size must be at least \p width * \p height * sizeof(float) bytes.
 * \param pitch The size of each line in bytes. Pass zero to use the default pitch.
 * \param loadFlags The flags used when loading the data.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_load_data_grayscale_single(heds_datahandle *handle, int width, int height, const float *data,
                                                           int pitch HOLOEYE_DEFARG(0), heds_dataflags_t loadFlags HOLOEYE_DEFARG(HEDSLDF_Default));

/** Loads the given data to be shown on the SLM. Each double represents a single pixel. Values outside 0.0 and 1.0 are clamped.
 * \ingroup SLMDisplayCAdvanced
 * \see heds_errorcodes
 * \param handle A pointer to an unused data handle. The resulting handle will be stored here.
 * \param width The width of the given data field.
 * \param height The height of the given data field.
 * \param data The pointer to grayscale values. The size must be at least \p width * \p height * sizeof(double) bytes.
 * \param pitch The size of each line in bytes. Pass zero to use the default pitch.
 * \param loadFlags The flags used when loading the data.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_load_data_grayscale_double(heds_datahandle *handle, int width, int height, const double *data,
                                                           int pitch HOLOEYE_DEFARG(0), heds_dataflags_t loadFlags HOLOEYE_DEFARG(HEDSLDF_Default));

/** Loads data from a file for the SLM, using a Latin-1 or ASCII string.
 * \ingroup SLMDisplayCAdvanced
 * \param handle A pointer to an unused data handle. The resulting handle will be stored here.
 * \param filepath The path to an image file you want to load.
 * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
 * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
 * \param loadFlags The flags used when loading the data.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_load_data_fromfile_latin1(heds_datahandle *handle, const char *filepath,
                                                          heds_dataflags_t loadFlags HOLOEYE_DEFARG(HEDSLDF_Default));

/** Loads data from a file for the SLM, using a UTF-8 string.
 * \ingroup SLMDisplayCAdvanced
 * \param handle A pointer to an unused data handle. The resulting handle will be stored here.
 * \param filepath The path to an image file you want to load.
 * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
 * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
 * \param loadFlags The flags used when loading the data.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_load_data_fromfile_utf8(heds_datahandle *handle, const char *filepath,
                                                        heds_dataflags_t loadFlags HOLOEYE_DEFARG(HEDSLDF_Default));

/** Loads data from a file for the SLM, using a unicode string.
 * \ingroup SLMDisplayCAdvanced
 * \param handle A pointer to an unused data handle. The resulting handle will be stored here.
 * \param filepath The path to an image file you want to load.
 * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
 * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
 * \param loadFlags The flags used when loading the data.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_load_data_fromfile_unicode(heds_datahandle *handle, const wchar_t *filepath,
                                                           heds_dataflags_t loadFlags HOLOEYE_DEFARG(HEDSLDF_Default));

/** Loads data from a file for the SLM, using a 16-bit unicode string.
 *  For Windows, this is the same as calling \ref heds_load_data_fromfile_unicode.
 * \ingroup SLMDisplayCAdvanced
 * \param handle A pointer to an unused data handle. The resulting handle will be stored here.
 * \param filepath The path to an image file you want to load.
 * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
 * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
 * \param loadFlags The flags used when loading the data.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_load_data_fromfile_unicode16(heds_datahandle *handle, const ucchar *filepath,
                                                             heds_dataflags_t loadFlags HOLOEYE_DEFARG(HEDSLDF_Default));

/** Loads the given phase values to be shown on the SLM. The expected data type is single.
 * \ingroup SLMDisplayCAdvanced
 * \see heds_errorcodes
 * \param handle A pointer to an unused data handle. The resulting handle will be stored here.
 * \param width The width of the given phase value field.
 * \param height The height of the given phase value field.
 * \param phaseValues The pointer to the given phase values. The size must be at least \p width * \p height * sizeof(float) bytes.
 * \param pitch The size of each line in bytes. Pass zero to use the default pitch.
 * \param phaseWrap The phase wrap applied to the data, basically a modulo. A value of zero means there is no phase wrap applied. This is the phase value of a full circle, by default 2pi for phase values in radian.
 * \param loadFlags The flags used when loading the data.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_load_phasevalues_single(heds_datahandle *handle, int width, int height, const float *phaseValues,
                                                        int pitch HOLOEYE_DEFARG(0), float phaseWrap HOLOEYE_DEFARG(2.0f * HOLOEYE_PIF),
                                                        heds_dataflags_t loadFlags HOLOEYE_DEFARG(HEDSLDF_Default));

/** Loads the given phase values to be shown on the SLM. The expected data type is double.
 * \ingroup SLMDisplayCAdvanced
 * \see heds_errorcodes
 * \param handle A pointer to an unused data handle. The resulting handle will be stored here.
 * \param width The width of the given phase value field.
 * \param height The height of the given phase value field.
 * \param phaseValues The pointer to the given phase values. The size must be at least \p width * \p height * sizeof(double) bytes.
 * \param pitch The size of each line in bytes. Pass zero to use the default pitch.
 * \param phaseWrap The phase wrap applied to the data, basically a modulo. A value of zero means there is no phase wrap applied. This is the phase value of a full circle, by default 2pi for phase values in radian.
 * \param loadFlags The flags used when loading the data.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_load_phasevalues_double(heds_datahandle *handle, int width, int height, const double *phaseValues,
                                                        int pitch HOLOEYE_DEFARG(0), float phaseWrap HOLOEYE_DEFARG(2.0f * HOLOEYE_PIF),
                                                        heds_dataflags_t loadFlags HOLOEYE_DEFARG(HEDSLDF_Default));

/** Shows the given data on the SLM. Each unsigned character represents a single pixel.
 * \ingroup SLMDisplayCAdvanced
 * \see heds_showflags
 * \see heds_errorcodes
 * \param handle The handle whose data will be shown on the SLM.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_datahandle(heds_datahandle *handle, heds_dataflags_t showFlags HOLOEYE_DEFARG(HEDSSHF_PresentAutomatic));

/** Shows the given data on the SLM. Each unsigned character represents a single pixel.
 * \ingroup SLMDisplayCAdvanced
 * \see heds_showflags
 * \see heds_errorcodes
 * \param handle_id The hanlde id whose data will be shown on the SLM.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_datahandle_id(heds_datahandle_id handle_id, heds_dataflags_t showFlags HOLOEYE_DEFARG(HEDSSHF_PresentAutomatic));

/** Initializes a handle with correct data. The resulting handle will not hold any data or memory.
 * \ingroup SLMDisplayCAdvanced
 * \see heds_errorcodes
 * \param handle The handle that will be initialized.
 * \param handle_id The id that will be set on the handle. This must either be zero or an already existing handle id.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_datahandle_init(heds_datahandle *handle, heds_datahandle_id handle_id HOLOEYE_DEFARG(0));

/** Releases a given data handle.
 * \ingroup SLMDisplayCAdvanced
 * \param handle The handle whose data will be released. */
HOLOEYE_API void heds_datahandle_release(heds_datahandle *handle);

/** Releases a given data handle.
 * \ingroup SLMDisplayCAdvanced
 * \param handle_id The handle id whose data will be released. */
HOLOEYE_API void heds_datahandle_release_id(heds_datahandle_id handle_id);

/** Releases all data handles available at the moment. Can be used as a clean-up action.
 * \ingroup SLMDisplayCAdvanced */
HOLOEYE_API void heds_datahandle_release_all();

/** Updates a handle with the latest information.
 * \ingroup SLMDisplayCAdvanced
 * \param handle The handle we want to update.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_datahandle_update(heds_datahandle *handle);

/** Updates a handle with the latest information and applies/writes the flagged values.
 * \ingroup SLMDisplayCAdvanced
 * \param handle The handle whose values we want to apply.
 * \param appliedValues Specifies the values which will be applied.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_datahandle_apply(heds_datahandle *handle, heds_datahandle_applyvalues appliedValues);

/** Waits until a data handle has reached a certain state.
 * \ingroup SLMDisplayCAdvanced
 * \param handle The handle we want to wait for.
 * \param state The state to wait for.
 * \param timeOutInMs The time in milliseconds to wait before returning with the error HEDSERR_WaitForHandleTimedOut.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_datahandle_waitfor(heds_datahandle *handle, heds_datahandle_state state, int timeOutInMs HOLOEYE_DEFARG(4000));

/** Get the statistics from a handle in a more user friendly format. The handle is not updated.
 * \ingroup SLMDisplayCAdvanced
 * \param handle The handle we want to get the statistics for.
 * \param statistics The statistics from the handle. If a value is \ref heds_datahandle_notdone then it is represented as -1.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_datahandle_statistics(const heds_datahandle *handle, int statistics[HEDSSTAT_COUNT]);

HOLOEYE_CCODE_END()

#endif // HOLOEYE_SLMDISPLAYSDK_HANDLES_H

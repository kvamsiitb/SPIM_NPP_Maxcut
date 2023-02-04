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


#ifndef HOLOEYE_SLMDISPLAYSDK_H
#define HOLOEYE_SLMDISPLAYSDK_H

#include "holoeye_platforms.h"
#include "holoeye_slmdisplaysdk_shared.h"

/** Provides the instance of a canvas. */
#define HOLOEYE_CANVAS_INSTANCE(canvas) ((canvas >> 8) & 0xFF)

HOLOEYE_CCODE_BEGIN()

/** Shows a given image file on the SLM, using a Latin-1 or ASCII string.
 * \ingroup SLMDisplayC
 * \see heds_showflags
 * \see heds_errorcodes
 * \param filepath A zero-terminated string holding an image file path.
 * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
 * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_data_fromfile_latin1(const char *filepath, heds_dataflags_t showFlags HOLOEYE_DEFARG(HEDSSHF_PresentAutomatic));

/** Shows a given image file on the SLM, using a UTF-8 string.
 * \ingroup SLMDisplayC
 * \see heds_showflags
 * \see heds_errorcodes
 * \param filepath A zero-terminated string holding an image file path.
 * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
 * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_data_fromfile_utf8(const char *filepath, heds_dataflags_t showFlags HOLOEYE_DEFARG(HEDSSHF_PresentAutomatic));

/** Shows a given image file on the SLM, using a unicode string.
 * \ingroup SLMDisplayC
 * \see heds_showflags
 * \see heds_errorcodes
 * \param filepath A zero-terminated string holding an image file path.
 * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
 * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_data_fromfile_unicode(const wchar_t *filepath, heds_dataflags_t showFlags HOLOEYE_DEFARG(HEDSSHF_PresentAutomatic));

/** Shows a given image file on the SLM, using a 16-bit unicode string.
 *  For Windows, this is the same as calling \ref heds_show_data_fromfile_unicode.
 * \ingroup SLMDisplayC
 * \see heds_showflags
 * \see heds_errorcodes
 * \param filepath A zero-terminated string holding an image file path.
 * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
 * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_data_fromfile_unicode16(const ucchar *filepath, heds_dataflags_t showFlags HOLOEYE_DEFARG(HEDSSHF_PresentAutomatic));

/** Shows the given data on the SLM. Each unsigned character represents a single pixel.
 * \ingroup SLMDisplayC
 * \see heds_showflags
 * \see heds_errorcodes
 * \param width The width of the given data field.
 * \param height The height of the given data field.
 * \param data The pointer to grayscale values. The size must be at least \p width * \p height bytes.
 * \param pitch The size of each line in bytes. Pass zero to use the default pitch.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_data_grayscale_uchar(int width, int height, const unsigned char *data, int pitch HOLOEYE_DEFARG(0), heds_dataflags_t showFlags HOLOEYE_DEFARG(HEDSSHF_PresentAutomatic));

/** Shows the given data on the SLM. The values are expected to be in blocks of three unsigned characters
 *  representing a single RGB values for each pixel.
 * \ingroup SLMDisplayC
 * \see heds_showflags
 * \see heds_errorcodes
 * \param width The width of the given data field.
 * \param height The height of the given data field.
 * \param data The pointer to RGB values. The size must be at least \p width * \p height * 3 bytes.
 * \param pitch The size of each line in bytes. Pass zero to use the default pitch.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_data_rgb24(int width, int height, const unsigned char *data, int pitch HOLOEYE_DEFARG(0), heds_dataflags_t showFlags HOLOEYE_DEFARG(HEDSSHF_PresentAutomatic));

/** Shows the given data on the SLM. Each unsigned integer represents a full RGBA value, in that order. The alpha value is ignored.
 * \ingroup SLMDisplayC
 * \see heds_showflags
 * \see heds_errorcodes
 * \param width The width of the given data field.
 * \param height The height of the given data field.
 * \param data The pointer to RGBA values. The size must be at least \p width * \p height * 4 bytes.
 * \param pitch The size of each line in bytes. Pass zero to use the default pitch.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_data_rgba32(int width, int height, const unsigned int *data, int pitch HOLOEYE_DEFARG(0), heds_dataflags_t showFlags HOLOEYE_DEFARG(HEDSSHF_PresentAutomatic));

/** Shows the given data on the SLM. Each float represents a single pixel. The values must be between 0.0f and 1.0f. Values out of this range are cut off.
 * \ingroup SLMDisplayC
 * \see heds_showflags
 * \see heds_errorcodes
 * \param width The width of the given data field.
 * \param height The height of the given data field.
 * \param data The pointer to grayscale values. The size must be at least \p width * \p height * sizeof(float) bytes.
 * \param pitch The size of each line in bytes. Pass zero to use the default pitch.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_data_grayscale_single(int width, int height, const float *data, int pitch HOLOEYE_DEFARG(0), heds_dataflags_t showFlags HOLOEYE_DEFARG (HEDSSHF_PresentAutomatic));

/** Shows the given data on the SLM. Each double represents a single pixel. The values must be between 0.0f and 1.0f. Values out of this range are cut off.
 * \ingroup SLMDisplayC
 * \see heds_showflags
 * \see heds_errorcodes
 * \param width The width of the given data field.
 * \param height The height of the given data field.
 * \param data The pointer to grayscale values. The size must be at least \p width * \p height * sizeof(double) bytes.
 * \param pitch The size of each line in bytes. Pass zero to use the default pitch.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_data_grayscale_double(int width, int height, const double *data, int pitch HOLOEYE_DEFARG(0), heds_dataflags_t showFlags HOLOEYE_DEFARG(HEDSSHF_PresentAutomatic));

/** Shows an array of phase values on the SLM. The unit of the phase values is the same as for \p phaseWrap. By default radians. The expected data type is single (32 bit floating point number).
 * \ingroup SLMDisplayC
 * \see heds_showflags
 * \see heds_errorcodes
 * \param width The width of the given phase value field.
 * \param height The height of the given phase value field.
 * \param phaseValues The pointer to the given phase values. The size must be at least \p width * \p height * sizeof(float) bytes.
 * \param pitch The size of each line in bytes. Pass zero to use the default pitch.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \param phaseWrap The phase wrap applied to the data, basically a modulo. A value of zero means there is no phase wrap applied. This is the phase value of a full circle, by default 2pi for phase values in radian.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_phasevalues_single(int width, int height, const float *phaseValues, int pitch HOLOEYE_DEFARG(0), heds_dataflags_t showFlags HOLOEYE_DEFARG(HEDSSHF_PresentAutomatic), float phaseWrap HOLOEYE_DEFARG(2.0f * HOLOEYE_PIF));

/** Shows an array of phase values on the SLM. The unit of the phase values is the same as for \p phaseWrap. By default radians. The expected data type is double (64 bit floating point number).
 * \ingroup SLMDisplayC
 * \see heds_showflags
 * \see heds_errorcodes
 * \param width The width of the given phase value field.
 * \param height The height of the given phase value field.
 * \param phaseValues The pointer to the given phase values. The size must be at least \p width * \p height * sizeof(double) bytes.
 * \param pitch The size of each line in bytes. Pass zero to use the default pitch.
 * \param phaseWrap The phase wrap applied to the data, basically a modulo. A value of zero means there is no phase wrap applied. This is the phase value of a full circle, by default 2pi for phase values in radian.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_phasevalues_double(int width, int height, const double *phaseValues, int pitch HOLOEYE_DEFARG(0), heds_dataflags_t showFlags HOLOEYE_DEFARG(HEDSSHF_PresentAutomatic), float phaseWrap HOLOEYE_DEFARG (2.0f * HOLOEYE_PIF));

/** Shows a blank screen with a constant gray value.
 * \ingroup SLMDisplayC
 * \see heds_errorcodes
 * \param gray_value The gray value which is addressed to the full SLM. The value is automatically wrapped to the range 0-255.
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_blankscreen(int gray_value HOLOEYE_DEFARG(128));

/** Shows a vertical binary grating. The grating consists of two gray values \p a_gray_value and \p b_gray_value which will be addressed to the SLM.
 * The gray values have the data type int and will be wrapped internally to an unsigned char with a range of 0 to 255.
 * The width of each area with the gray value \p a_gray_value and \p b_gray_value is defined by \p a_width and \p b_width, respectively.
 * Each pair of gray values is repeated so that the SLM is completely filled.
 * \ingroup SLMDisplayC
 * \code
 * heds_show_grating_vertical_binary(2,2);
 * \endcode
 * \see heds_errorcodes
 * \param a_width The width of the first block with the value of \p a_gray_value. This parameter is mandatory.
 * \param b_width The width of the second block with the value of \p b_gray_value. This parameter is mandatory.
 * \param a_gray_value The addressed gray value of the first block. [default value = 0].
 * \param b_gray_value The addressed gray value of the second block. [default value = 128].
 * \param shift_x The horizontal offset applied to both blocks. [default value = 0].
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_grating_vertical_binary(int a_width, int b_width, int a_gray_value HOLOEYE_DEFARG(0), int b_gray_value HOLOEYE_DEFARG(128), int shift_x HOLOEYE_DEFARG (0));

/** Shows a horizontal binary grating. The grating consists of two gray values \p a_gray_value and \p b_gray_value.
 * The gray values have the data type int and will be wrapped internally to an unsigned char with a range of 0 to 255.
 * Each pair of gray values is repeated so that the SLM is completely filled.
 * The width of each area with the gray value \p a_gray_value and \p b_gray_value is defined by \p a_width and \p b_width, respectively.
 * \ingroup SLMDisplayC
 * \code
 * heds_show_grating_horizontal_binary(2,2);
 * \endcode
 * \see heds_errorcodes
 * \param a_width The width of the first block with the value of \p a_gray_value. This parameter is mandatory.
 * \param b_width The width of the second block with the value of \p b_gray_value. This parameter is mandatory.
 * \param a_gray_value The addressed gray value of the first block. [default value = 0].
 * \param b_gray_value The addressed gray value of the second block. [default value = 128].
 * \param shift_y The vertical offset applied to both blocks. [default value = 0].
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_grating_horizontal_binary(int a_width, int b_width, int a_gray_value HOLOEYE_DEFARG(0), int b_gray_value HOLOEYE_DEFARG(128), int shift_y HOLOEYE_DEFARG (0));

/** Shows a vertical blazed grating on the SLM.
 * \ingroup SLMDisplayC
 * \code
 * // shows a blazed grating with a period of 8 pixel on the SLM:
 * heds_show_grating_vertical_blaze(8);
 * \endcode
 * \param period The grating period in SLM pixels. The value is mandatory. Can be either positive or negative for an inverted grating profile. Please note that the phase can also be inverted by the \p phase_scale. If both values are negative, the invertions will superimpose to non invertion.
 * \param shift_x The horizontal offset applied to the grating. [default value = 0].
 * \param phase_scale Scales all phase values of this phase function. The value can be negative to invert the phase function. Other values than 1.0 and -1.0 are meant to optimize diffraction efficiency. Absolute values greater than 1.0 would lead to gray level saturation artifacts and are therefore limited to the range from -1.0 to +1.0. In case of limitation, a warning message will be shown. The scaling is done after wrapping phase values into the gray levels of the SLM. [default value = 1.0].
 * \param phase_offset Applies an offset to the phase values of this phase function. The unit of this value is in radian. The offset will retain the phase profile, but will change the actual used gray levels on the SLM. When this value is 0, the phase function will be centered into the gray value range on the SLM. After the offset was applied, wrapping to the gray values is done. [default value = 0.0].
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_grating_vertical_blaze(int period, int shift_x HOLOEYE_DEFARG(0), double phase_scale HOLOEYE_DEFARG(1.0), double phase_offset HOLOEYE_DEFARG(0.0));

/** Shows a horizontal blazed grating on the SLM.
 * \ingroup SLMDisplayC
 * \code
 * // shows a blazed grating with a period of 8 pixel on the SLM:
 * heds_show_grating_horizontal_blaze(8);
 * \endcode
 * \param period The grating period in SLM pixels. The value is mandatory. Can be either positive or negative for an inverted grating profile.
 * \param shift_y The vertical offset applied to the grating. [default value = 0].
 * \param phase_scale Scales all phase values of this phase function. The value can be negative to invert the phase function. Other values than 1.0 and -1.0 are meant to optimize diffraction efficiency. Absolute values greater than 1.0 would lead to gray level saturation artifacts and are therefore limited to the range from -1.0 to +1.0. In case of limitation, a warning message will be shown. The scaling is done after wrapping phase values into the gray levels of the SLM. [default value = 1.0].
 * \param phase_offset Applies an offset to the phase values of this phase function. The unit of this value is in radian. The offset will retain the phase profile, but will change the actual used gray levels on the SLM. When this value is 0, the phase function will be centered into the gray value range on the SLM. After the offset was applied, wrapping to the gray values is done. [default value = 0.0].
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_grating_horizontal_blaze(int period, int shift_y HOLOEYE_DEFARG(0), double phase_scale HOLOEYE_DEFARG(1.0), double phase_offset HOLOEYE_DEFARG(0.0));

/** Shows two areas on the SLM with two different gray values. The function is intended to be used for phase measurements of the SLM in which one half of the SLM can be used as a reference to the other half.
 * The screen will be split along the vertical (y) axis. This means that the gray levels a and b are painted to the left and right side of the SLM, resp.
 * \ingroup SLMDisplayC
 * \param screen_divider The ratio by which the SLM screen should be divided. Meaningful values are between 0.0 and 1.0. [default value = 0.5]
 * \param a_gray_value The gray value which will be adressed on the first side of the SLM. Values are wrapped to 0-255 range. [default value = 0]
 * \param b_gray_value The gray value which will be adressed on the second side of the SLM. Values are wrapped to 0-255 range. [default value = 255]
 * \param flipped If set to true, the first side will addressed with \p b_gray_value and the second side will be set to a_gray_value. [default value = false]
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_dividedscreen_vertical(int a_gray_value HOLOEYE_DEFARG(0), int b_gray_value HOLOEYE_DEFARG(255), float screen_divider HOLOEYE_DEFARG(0.5f), HOLOEYE_BOOL flipped HOLOEYE_DEFARG(HOLOEYE_FALSE));

/** Shows two areas on the SLM with two different gray values. The function is intended to be used for phase measurements of the SLM in which one half of the SLM can be used as a reference to the other half.
 * The screen will be split along the horizontal (x) axis. This means that the gray levels a and b are painted to the upper and lower side of the SLM, resp.
 * \ingroup SLMDisplayC
 * \param screen_divider The ratio by which the SLM screen should be divided. Meaningful values are between 0.0 and 1.0. [default value = 0.5]
 * \param a_gray_value The gray value which will be adressed on the first side of the SLM. Values are wrapped to 0-255 range. [default value = 0]
 * \param b_gray_value The gray value which will be adressed on the second side of the SLM. Values are wrapped to 0-255 range. [default value = 255]
 * \param flipped If set to true, the first side will addressed with \p b_gray_value and the second side will be set to a_gray_value. [default value = false]
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_dividedscreen_horizontal(int a_gray_value HOLOEYE_DEFARG(0), int b_gray_value HOLOEYE_DEFARG(255), float screen_divider HOLOEYE_DEFARG(0.5f), HOLOEYE_BOOL flipped HOLOEYE_DEFARG(HOLOEYE_FALSE));

/** Shows an axicon. The phase has a conical shape.
 * \ingroup SLMDisplayC
 * \code
 * // shows an axicon with 2pi phase at the border of the SLM in respect to the phase at the center of the optical function, if the height is small side of the SLM.
 * heds_show_phasefunction_axicon(heds_slm_height_px()/2);
 * \endcode
 * \see heds_errorcodes
 * \param inner_radius_px The radius in number of SLM pixel where the axicon phase function reached 2pi for the first time in respect to the center of the axicon.
 * \param center_x Horizontal shift of the center of the optical function on the SLM in number of pixel. [default value = 0].
 * \param center_y Vertical shift of the center of the optical function on the SLM in number of pixel. [default value = 0].
 * \param phase_scale Scales all phase values of this phase function. The value can be negative to invert the phase function. Other values than 1.0 and -1.0 are meant to optimize diffraction efficiency. Absolute values greater than 1.0 would lead to gray level saturation artifacts and are therefore limited to the range from -1.0 to +1.0. In case of limitation, a warning message will be shown. The scaling is done after wrapping phase values into the gray levels of the SLM. [default value = 1.0].
 * \param phase_offset Applies an offset to the phase values of this phase function. The unit of this value is in radian. The offset will retain the phase profile, but will change the actual used gray levels on the SLM. When this value is 0, the phase function will be centered into the gray value range on the SLM. After the offset was applied, wrapping to the gray values is done. [default value = 0.0].
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_phasefunction_axicon(int inner_radius_px, int center_x HOLOEYE_DEFARG(0), int center_y HOLOEYE_DEFARG(0), double phase_scale HOLOEYE_DEFARG(1.0), double phase_offset HOLOEYE_DEFARG(0.0));

/** Shows a lens phase function. The phase has a parabolic shape.
 * The resulting focal length can be calculated as f [m] = (\p inner_radius_px * \ref heds_slm_pixelsize_um()*1.0E-6) ^2 / (2.0*lambda),
 * with the incident optical wavelength lambda.
 * \ingroup SLMDisplayC
 * \code
 * // shows a lens with 2pi phase at the border of the SLM in respect to the phase at the center of the optical function, if the height is small side of the SLM.
 * heds_show_phasefunction_lens(heds_slm_height_px()/2);
 * \endcode
 * \see heds_errorcodes
 * \param inner_radius_px The radius in number of SLM pixel where the lens phase function reached 2pi for the first time in respect to the center of the lens. This value is related to the focal length f of the lens phase function by f = (inner_radius_px * heds_slm_pixelsize())^2 / (2*lambda).
 * \param center_x Horizontal shift of the center of the optical function on the SLM in number of pixel. [default value = 0].
 * \param center_y Vertical shift of the center of the optical function on the SLM in number of pixel. [default value = 0].
 * \param phase_scale Scales all phase values of this phase function. The value can be negative to invert the phase function. Other values than 1.0 and -1.0 are meant to optimize diffraction efficiency. Absolute values greater than 1.0 would lead to gray level saturation artifacts and are therefore limited to the range from -1.0 to +1.0. In case of limitation, a warning message will be shown. The scaling is done after wrapping phase values into the gray levels of the SLM. [default value = 1.0].
 * \param phase_offset Applies an offset to the phase values of this phase function. The unit of this value is in radian. The offset will retain the phase profile, but will change the actual used gray levels on the SLM. When this value is 0, the phase function will be centered into the gray value range on the SLM. After the offset was applied, wrapping to the gray values is done. [default value = 0.0].
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_phasefunction_lens(int inner_radius_px, int center_x HOLOEYE_DEFARG(0), int center_y HOLOEYE_DEFARG(0), double phase_scale HOLOEYE_DEFARG(1.0), double phase_offset HOLOEYE_DEFARG(0.0));

/** Shows an optical vortex phase function on the SLM. The phase has a helical shape.
 * \ingroup SLMDisplayC
 * \code
 * // shows a vortex with order of 1 centered to the SLM:
 * heds_show_phasefunction_vortex();
 * \endcode
 * \see heds_errorcodes
 * \param vortex_order The order of the optical vortex. If the order is one, the phase goes from 0 to 2pi over the full angle of 360 degree. For higher orders, 2pi phase shift is reached at angles of 360 degree divided by the given \p vortex_order. [default value = 1].
 * \param inner_radius_px The radius at the sigularity which will be set to gray value 0 on the SLM. [default value = 0].
 * \param center_x Horizontal shift of the center of the optical function on the SLM in number of pixel. [default value = 0].
 * \param center_y Vertical shift of the center of the optical function on the SLM in number of pixel. [default value = 0].
 * \param phase_scale Scales all phase values of this phase function. The value can be negative to invert the phase function. Other values than 1.0 and -1.0 are meant to optimize diffraction efficiency. Absolute values greater than 1.0 would lead to gray level saturation artifacts and are therefore limited to the range from -1.0 to +1.0. In case of limitation, a warning message will be shown. The scaling is done after wrapping phase values into the gray levels of the SLM. [default value = 1.0].
 * \param phase_offset Applies an offset to the phase values of this phase function. The unit of this value is in radian. The offset will retain the phase profile, but will change the actual used gray levels on the SLM. When this value is 0, the phase function will be centered into the gray value range on the SLM. After the offset was applied, wrapping to the gray values is done. [default value = 0.0].
 * \return HEDSERR_NoError when there is no error. */
HOLOEYE_API heds_errorcode heds_show_phasefunction_vortex(int vortex_order HOLOEYE_DEFARG(1), int inner_radius_px HOLOEYE_DEFARG(0), int center_x HOLOEYE_DEFARG(0), int center_y HOLOEYE_DEFARG(0), double phase_scale HOLOEYE_DEFARG(1.0), double phase_offset HOLOEYE_DEFARG(0.0));

HOLOEYE_CCODE_END()

#endif // HOLOEYE_SLMDISPLAYSDK_H

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


#ifndef HOLOEYE_SLMDISPLAYSDK_CPP_H
#define HOLOEYE_SLMDISPLAYSDK_CPP_H

#include "holoeye_platforms.h"

#if !HOLOEYE_CPP11_OR_HIGHER
    #error This header requires a C++ 11 compiler.
#endif

#ifdef HOLOEYE_SLMDISPLAYSDK_H
    #error Please do not include "holoeye_slmdisplaysdk.h" separately
#endif

#include "holoeye_slmdisplaysdk_shared.hpp"

#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <assert.h>

namespace holoeye {

#include <holoeye_slmdisplaysdk.h>

/** Shows a given image file on the SLM.
 * \ingroup SLMDisplayCPP
 * \see heds_showflags
 * \see heds_errorcodes
 * \param filepath A zero-terminated string holding an image file path.
 * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
 * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
 * \param encoding The encoding of the \p filepath string.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
heds_errorcode heds_show_data_fromfile(const std::string &filepath, heds_encoding encoding = HEDSENC_Default, heds_dataflags_t showFlags = HEDSSHF_PresentAutomatic);

/** Shows a given image file on the SLM. This function takes the string based on the filesystem.
 * \ingroup SLMDisplayCPP
 * \see heds_showflags
 * \see heds_errorcodes
 * \param filepath A zero-terminated string holding an image file path.
 * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
 * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
heds_errorcode heds_show_data_fromfile_fs(const fsstring &filepath, heds_dataflags_t showFlags = HEDSSHF_PresentAutomatic);

/** Shows a given image file on the SLM, using a unicode string.
 * \ingroup SLMDisplayCPP
 * \see heds_showflags
 * \see heds_errorcodes
 * \param filepath A zero-terminated string holding an image file path.
 * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
 * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
heds_errorcode heds_show_data_fromfile(const std::wstring &filepath, heds_dataflags_t showFlags = HEDSSHF_PresentAutomatic);

/** Shows a given image file on the SLM, using a utf-16 string.
 * \ingroup SLMDisplayCPP
 * \see heds_showflags
 * \see heds_errorcodes
 * \param filepath A zero-terminated string holding an image file path.
 * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
 * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
heds_errorcode heds_show_data_fromfile(const std::u16string &filepath, heds_dataflags_t showFlags = HEDSSHF_PresentAutomatic);

/** Shows the given data on the SLM. Each unsigned character represents a single pixel.
 * \ingroup SLMDisplayCPP
 * \see heds_showflags
 * \see heds_errorcodes
 * \param field The field containing the data to show.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
extern heds_errorcode heds_show_data(const field<unsigned char>::ptr &field, heds_dataflags_t showFlags = HEDSSHF_PresentAutomatic);

/** Shows the given data on the SLM. Each float represents a single pixel. The values must be between 0.0f and 1.0f. Values out of this range are cut off.
 * \ingroup SLMDisplayCPP
 * \see heds_showflags
 * \see heds_errorcodes
 * \param field The field containing the data to show.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
extern heds_errorcode heds_show_data(const field<float>::ptr &field, heds_dataflags_t showFlags = HEDSSHF_PresentAutomatic);

/** Shows the given data on the SLM. Each double represents a single pixel. The values must be between 0.0f and 1.0f. Values out of this range are cut off.
 * \ingroup SLMDisplayCPP
 * \see heds_showflags
 * \see heds_errorcodes
 * \param field The field containing the data to show.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
extern heds_errorcode heds_show_data(const field<double>::ptr &field, heds_dataflags_t showFlags = HEDSSHF_PresentAutomatic);

/** Shows an array of phase values on the SLM. The unit of the phase values is the same as for \p phaseWrap. By default radians. The expected data type is single (32 bit floating point number).
 * \ingroup SLMDisplayCPP
 * \see heds_showflags
 * \see heds_errorcodes
 * \param field The field containing the phase values to show.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \param phaseWrap The phase wrap applied to the data, basically a modulo. A value of zero means there is no phase wrap applied.
 * \return HEDSERR_NoError when there is no error. */
extern heds_errorcode heds_show_phasevalues(const field<float>::ptr &field, heds_dataflags_t showFlags = HEDSSHF_PresentAutomatic, float phaseWrap = 2.0f * HOLOEYE_PIF);

/** Shows an array of phase values on the SLM. The unit of the phase values is the same as for \p phaseWrap. By default radians. The expected data type is double (64 bit floating point number).
 * \ingroup SLMDisplayCPP
 * \see heds_showflags
 * \see heds_errorcodes
 * \param field The field containing the phase values to show.
 * \param phaseWrap The phase wrap applied to the data, basically a modulo. A value of zero means there is no phase wrap applied.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
extern heds_errorcode heds_show_phasevalues(const field<double>::ptr &field, heds_dataflags_t showFlags = HEDSSHF_PresentAutomatic, float phaseWrap = 2.0f * HOLOEYE_PIF);

}

#endif // HOLOEYE_SLMDISPLAYSDK_CPP_H

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


#ifndef HOLOEYE_SLMDISPLAYSDK_HANDLES_CPP_H
#define HOLOEYE_SLMDISPLAYSDK_HANDLES_CPP_H

#include "holoeye_platforms.h"

#if !HOLOEYE_CPP11_OR_HIGHER
    #error This header requires a C++ 11 compiler.
#endif

#ifdef HOLOEYE_SLMDISPLAYSDK_HANDLES_H
    #error Please do not include "holoeye_slmdisplaysdk_handles.h" separately
#endif

#include "holoeye_slmdisplaysdk_shared.hpp"

#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <assert.h>

namespace holoeye {

#include <holoeye_slmdisplaysdk_handles.h>

/** A helper class to release datahandles when they are no longer needed.
 * \ingroup SLMDisplayCPPAdvanced */
class heds_datahandle_t : public heds_datahandle
{
private:

    /** The filename associated with this datahandle.
     * This only applies to filepaths with the correct encoding. */
    fsstring _filename;

    /** Creates a new instance of the handle. Use \ref holoeye::heds_datahandle_t::create to create a new handle. */
    heds_datahandle_t()
    {
    }

public:

    /** Defines a shared pointer for the datahandle. */
    typedef std::shared_ptr<heds_datahandle_t> ptr;

    /** Creates a new datahandle. Usually this is not required. All functions create new datahandles as needed.
     * \see heds_datahandle::id
     * \param id The id of the datahandle to be created. This can be used to create a datahandle from an existing handle id.
     * \return A pointer to a new datahandle instance. */
    static ptr create(int id = 0)
    {
        assert(id >= 0);

        ptr p(new heds_datahandle_t());

        heds_errorcode error = heds_datahandle_init(p.get(), id);

        if(error != HEDSERR_NoError)
            return ptr();

        return p;
    }

    /** \cond internal */
    /** Creates a new datahandle. Do not call this function. Use \ref holoeye::heds_load_data_fromfile_fs instead.
     * \param filename The filename to set on the datahandle.
     * \return A pointer to a new datahandle instance. */
    static ptr create(const fsstring &filename)
    {
        assert(filename.size() > 0);

        ptr p(new heds_datahandle_t());

        heds_errorcode error = heds_datahandle_init(p.get());

        if(error != HEDSERR_NoError)
            return ptr();

        p->_filename = filename;

        return p;
    }

#if HOLOEYE_WINDOWS
    /** Creates a new datahandle. Do not call this function. Use \ref holoeye::heds_load_data_fromfile instead.
     * \param filename The filename to set on the datahandle.
     * \return A pointer to a new datahandle instance. */
    static ptr create(const std::u16string &filename)
    {
        auto wstr = (std::wstring*) &filename;

        return create(*wstr);
    }
#endif
    /** \endcond */

    /** Release the datahandle and the associated data. */
    ~heds_datahandle_t()
    {
        heds_datahandle_release(this);
    }

    /** The filename stored in the handle.
     * \return The stored filename */
    const fsstring& filename() const { return _filename; }
};

/** Adds functions for handling data to the SLM instance.
 * \ingroup SLMDisplayCPPAdvanced */
class heds_instance_handles : public heds_instance
{
public:


    /** Loads the given data to be shown on the SLM. Each unsigned character represents a single pixel.
     * \see heds_errorcodes
     * \param handle An empty datahandle pointer. The resulting handle will be stored here.
     * \param field The field containing the data to load.
     * \return HEDSERR_NoError when there is no error. */
    heds_errorcode load_data(heds_datahandle_t::ptr &handle, const field<unsigned char>::ptr &field);

    /** Loads the given data to be shown on the SLM. Each float represents a single pixel. The values must be between 0.0f and 1.0f.
     * \see heds_errorcodes
     * \param handle An empty datahandle pointer. The resulting handle will be stored here.
     * \param field The field containing the data to load.
     * \return HEDSERR_NoError when there is no error. */
    heds_errorcode load_data(heds_datahandle_t::ptr &handle, const field<float>::ptr &field);

    /** Loads the given data to be shown on the SLM. Each double represents a single pixel. The values must be between 0.0 and 1.0.
     * \see heds_errorcodes
     * \param handle An empty datahandle pointer. The resulting handle will be stored here.
     * \param field The field containing the data to load.
     * \return HEDSERR_NoError when there is no error. */
    heds_errorcode load_data(heds_datahandle_t::ptr &handle, const field<double>::ptr &field);

    /** Loads data from a file for the SLM.
     * \param handle An empty datahandle pointer. The resulting handle will be stored here.
     * \param filepath The path to an image file you want to load.
     * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
     * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
     * \param encoding The encoding of the \p filepath string.
     * \return HEDSERR_NoError when there is no error. */
    heds_errorcode load_data_fromfile(heds_datahandle_t::ptr &handle, const std::string &filepath, heds_encoding encoding = HEDSENC_Default);

    /** Loads data from a file for the SLM. This function takes the string based on the filesystem.
     * \param handle An empty datahandle pointer. The resulting handle will be stored here.
     * \param filepath The path to an image file you want to load.
     * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
     * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
     * \return HEDSERR_NoError when there is no error. */
    heds_errorcode load_data_fromfile_fs(heds_datahandle_t::ptr &handle, const fsstring &filepath);

    /** Loads data from a file for the SLM, using a unicode string.
     * \param handle An empty datahandle pointer. The resulting handle will be stored here.
     * \param filepath The path to an image file you want to load.
     * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
     * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
     * \return HEDSERR_NoError when there is no error. */
    heds_errorcode load_data_fromfile(heds_datahandle_t::ptr &handle, const std::wstring &filepath);

    /** Loads data from a file for the SLM, using a 16-bit unicode string.
     * \param handle An empty datahandle pointer. The resulting handle will be stored here.
     * \param filepath The path to an image file you want to load.
     * <br>Supported image file formats are: bmp, cur, dds, gif, icns, ico, jpeg, jpg, pbm, pgm, png, ppm, svg, svgz, tga, tif, tiff, wbmp, webp, xbm, xpm.
     * <br>For holographic data, we recommend not to use any lossy compressed formats, like jpg. Instead please use uncompressed formats (e.g. bmp) or lossless compressed formats (e.g. png).
     * \return HEDSERR_NoError when there is no error. */
    heds_errorcode load_data_fromfile(heds_datahandle_t::ptr &handle, const std::u16string &filepath);

    /** Loads the given phase values to be shown on the SLM. The expected data type is single.
     * \see heds_errorcodes
     * \param handle An empty datahandle pointer. The resulting handle will be stored here.
     * \param field The field containing the phase values to load.
     * \param phaseWrap The phase wrap applied to the data, basically a modulo. A value of zero means there is no phase wrap applied.
     * \return HEDSERR_NoError when there is no error. */
    heds_errorcode load_phasevalues(heds_datahandle_t::ptr &handle, const field<float>::ptr &field, float phaseWrap = 2.0f * HOLOEYE_PIF);

    /** Loads the given phase values to be shown on the SLM. The expected data type is double.
     * \see heds_errorcodes
     * \param handle An empty datahandle pointer. The resulting handle will be stored here.
     * \param field The field containing the phase values to load.
     * \param phaseWrap The phase wrap applied to the data, basically a modulo. A value of zero means there is no phase wrap applied.
     * \return HEDSERR_NoError when there is no error. */
    heds_errorcode load_phasevalues(heds_datahandle_t::ptr &handle, const field<double>::ptr &field, float phaseWrap = 2.0f * HOLOEYE_PIF);
};

/** Shows the data associated with the \p handle on the SLM.
 * \ingroup SLMDisplayCPPAdvanced
 * \see heds_showflags
 * \see heds_errorcodes
 * \param handle The handle whose data will be shown on the SLM.
 * \param showFlags Flags that define how the data is shown on the SLM.
 * \return HEDSERR_NoError when there is no error. */
extern heds_errorcode heds_show_datahandle(const heds_datahandle_t::ptr &handle, heds_dataflags_t showFlags = HEDSSHF_PresentAutomatic);

/** Updates a handle with the latest information.
 * \ingroup SLMDisplayCPPAdvanced
 * \param handle The handle we want to update.
 * \return HEDSERR_NoError when there is no error. */
extern heds_errorcode heds_datahandle_update(const heds_datahandle_t::ptr &handle);

/** Updates a handle with the latest information and applies/writes the flagged values.
 * \ingroup SLMDisplayCPPAdvanced
 * \param handle The handle whose values we want to apply.
 * \param appliedValues Specifies the values which will be applied.
 * \return HEDSERR_NoError when there is no error. */
extern heds_errorcode heds_datahandle_apply(const heds_datahandle_t::ptr &handle, heds_datahandle_applyvalues appliedValues);

/** Waits until a data handle has reached a certain state.
 * \ingroup SLMDisplayCPPAdvanced
 * \param handle The handle we want to wait for.
 * \param state The state to wait for.
 * \param timeOutInMs The time in milliseconds to wait before returning with the error HEDSERR_WaitForHandleTimedOut.
 * \return HEDSERR_NoError when there is no error. */
extern heds_errorcode heds_datahandle_waitfor(const heds_datahandle_t::ptr &handle, heds_datahandle_state state, int timeOutInMs = 4000);

/** Get the statistics from a handle in a more user friendly format. The handle is not updated.
 * \ingroup SLMDisplayCPPAdvanced
 * \param handle The handle we want to get the statistics for.
 * \param statistics The statistics from the handle. If a value is \ref heds_datahandle_notdone then it is represented as -1.
 * \return HEDSERR_NoError when there is no error. */
extern heds_errorcode heds_datahandle_statistics(const heds_datahandle_t::ptr &handle, int statistics[HEDSSTAT_COUNT]);

/** Creates a copy of a handle, without copying the underlying data.
 *  The new handle stored in \p to will have a new \ref apiID and will be as if you had loaded the same data twice.
 *  This will fail, when the handle \p from does not at least have the state of \ref HEDSST_ReadyToRender, or if it is \ref HEDSST_Error.
 * \param from The handle we will clone.
 * \param to The cloned handle will be stored here.
 * \param copySettings Defines if the settings will be copied to the new handle or not.
 * \return HEDSERR_NoError when there is no error. */
extern heds_errorcode heds_datahandle_clone(const heds_datahandle_t::ptr &from, heds_datahandle_t::ptr &to, bool copySettings = true);

}

#endif // HOLOEYE_SLMDISPLAYSDK_HANDLES_CPP_H

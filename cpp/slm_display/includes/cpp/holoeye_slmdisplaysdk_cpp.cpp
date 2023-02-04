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


#include <cpp/holoeye_slmdisplaysdk_shared.hpp>
#include <cpp/holoeye_slmdisplaysdk.hpp>
#include <cpp/holoeye_slmdisplaysdk_handles.hpp>

namespace holoeye {

bool heds_requires_version(int requiredApiVersion, bool showMessageBox)
{
    return heds_requires_version(requiredApiVersion, showMessageBox ? HOLOEYE_TRUE : HOLOEYE_FALSE) != HOLOEYE_FALSE;
}

heds_errorcode heds_show_data_fromfile(const std::string &filepath, heds_encoding encoding, heds_dataflags_t showFlags)
{
    if(encoding == HEDSENC_UTF8)
        return heds_show_data_fromfile_utf8(filepath.c_str(), showFlags);
    else
        return heds_show_data_fromfile_latin1(filepath.c_str(), showFlags);
}

heds_errorcode heds_show_data_fromfile_fs(const fsstring &filepath, heds_dataflags_t showFlags)
{
#if HOLOEYE_WINDOWS
    return heds_show_data_fromfile_unicode(filepath.c_str(), showFlags);
#else
    return heds_show_data_fromfile_utf8(filepath.c_str(), showFlags);
#endif
}

heds_errorcode heds_show_data_fromfile(const std::wstring &filepath, heds_dataflags_t showFlags)
{
    return heds_show_data_fromfile_unicode(filepath.c_str(), showFlags);
}

heds_errorcode heds_show_data_fromfile(const std::u16string &filepath, heds_dataflags_t showFlags)
{
    return heds_show_data_fromfile_unicode16((const ucchar*) filepath.c_str(), showFlags);
}

heds_errorcode heds_show_data(const field<unsigned char>::ptr &field, heds_dataflags_t showFlags)
{
    return heds_show_data_grayscale_uchar(field->width(), field->height(), field->data(), 0, showFlags);
}

heds_errorcode heds_show_data(const field<float>::ptr &field, heds_dataflags_t showFlags)
{
    return heds_show_data_grayscale_single(field->width(), field->height(), field->data(), 0, showFlags);
}

heds_errorcode heds_show_data(const field<double>::ptr &field, heds_dataflags_t showFlags)
{
    return heds_show_data_grayscale_double(field->width(), field->height(), field->data(), 0, showFlags);
}

heds_errorcode heds_show_phasevalues(const field<float>::ptr &field, heds_dataflags_t showFlags, float phaseWrap)
{
    return heds_show_phasevalues_single(field->width(), field->height(), field->data(), 0, showFlags, phaseWrap);
}

heds_errorcode heds_show_phasevalues(const field<double>::ptr &field, heds_dataflags_t showFlags, float phaseWrap)
{
    return heds_show_phasevalues_double(field->width(), field->height(), field->data(), 0, showFlags, phaseWrap);
}

heds_errorcode heds_show_datahandle(const heds_datahandle_t::ptr &handle, heds_dataflags_t showFlags)
{
    return heds_show_datahandle(handle.get(), showFlags);
}

heds_errorcode heds_datahandle_update(const heds_datahandle_t::ptr &handle)
{
    return heds_datahandle_update(handle.get());
}

heds_errorcode heds_datahandle_apply(const heds_datahandle_t::ptr &handle, heds_datahandle_applyvalues appliedValues)
{
    return heds_datahandle_apply(handle.get(), appliedValues);
}

heds_errorcode heds_datahandle_waitfor(const heds_datahandle_t::ptr &handle, heds_datahandle_state state, int timeOutInMs)
{
    return heds_datahandle_waitfor(handle.get(), state, timeOutInMs);
}

heds_errorcode heds_datahandle_statistics(const heds_datahandle_t::ptr &handle, int statistics[HEDSSTAT_COUNT])
{
    return heds_datahandle_statistics(handle.get(), statistics);
}

const fschar* heds_error_string_fs(heds_errorcode error)
{
#if HOLOEYE_WINDOWS
    return heds_error_string_unicode(error);
#else
    return heds_error_string_ascii(error);
#endif
}

heds_errorcode heds_instance::open()
{
    HEDS_CONFIGURATION_DEFAULT(config);

    heds_errorcode err = heds_slm_init(&config);

    if (err != HEDSERR_NoError)
        return err;

    _size = heds_slm_size_px();
    _size_mm = heds_slm_size_mm();
    _pixelsize = heds_slm_pixelsize_m();
    _refreshrate = heds_slm_refreshrate_hz();

    return HEDSERR_NoError;
}

void heds_instance::close()
{
    heds_slm_close();
}

heds_errorcode heds_instance::utils_slmpreview_show(bool show)
{
    return heds_utils_slmpreview_show(show ? HOLOEYE_TRUE : HOLOEYE_FALSE);
}

heds_errorcode heds_instance::slm_zernike(const std::vector<float> &values)
{
    return heds_slm_zernike(values.data(), (int)values.size());
}

heds_errorcode heds_instance_handles::load_data(heds_datahandle_t::ptr &handle, const field<unsigned char>::ptr &field)
{
    handle = heds_datahandle_t::create();

    return heds_load_data_grayscale_uchar(handle.get(), field->width(), field->height(), field->data(), 0);
}

heds_errorcode heds_instance_handles::load_data(heds_datahandle_t::ptr &handle, const field<float>::ptr &field)
{
    handle = heds_datahandle_t::create();

    return heds_load_data_grayscale_single(handle.get(), field->width(), field->height(), field->data(), 0);
}

heds_errorcode heds_instance_handles::load_data(heds_datahandle_t::ptr &handle, const field<double>::ptr &field)
{
    handle = heds_datahandle_t::create();

    return heds_load_data_grayscale_double(handle.get(), field->width(), field->height(), field->data(), 0);
}

heds_errorcode heds_instance_handles::load_data_fromfile(heds_datahandle_t::ptr &handle, const std::string &filepath, heds_encoding encoding)
{
    if (encoding == HEDSENC_UTF8)
    {
#if HOLOEYE_WINDOWS
        handle = heds_datahandle_t::create();
#else
        handle = heds_datahandle_t::create(filepath);
#endif

        return heds_load_data_fromfile_utf8(handle.get(), filepath.c_str());
    }
    else
    {
#if HOLOEYE_WINDOWS
        std::wstring fname(filepath.begin(), filepath.end());

        handle = heds_datahandle_t::create(fname);

        return heds_load_data_fromfile_unicode(handle.get(), fname.c_str());
#else
        handle = heds_datahandle_t::create();

        return heds_load_data_fromfile_latin1(handle.get(), filepath.c_str());
#endif
    }
}

heds_errorcode heds_instance_handles::load_data_fromfile_fs(heds_datahandle_t::ptr &handle, const fsstring &filepath)
{
    handle = heds_datahandle_t::create(filepath);

#if HOLOEYE_WINDOWS
    return heds_load_data_fromfile_unicode(handle.get(), filepath.c_str());
#else
    return heds_load_data_fromfile_utf8(handle.get(), filepath.c_str());
#endif
}

heds_errorcode heds_instance_handles::load_data_fromfile(heds_datahandle_t::ptr &handle, const std::wstring &filepath)
{
#if HOLOEYE_WINDOWS
    handle = heds_datahandle_t::create(filepath);
#else
    handle = heds_datahandle_t::create();
#endif

    return heds_load_data_fromfile_unicode(handle.get(), filepath.c_str());
}

heds_errorcode heds_instance_handles::load_data_fromfile(heds_datahandle_t::ptr &handle, const std::u16string &filepath)
{
#if HOLOEYE_WINDOWS
    handle = heds_datahandle_t::create(filepath);
#else
    handle = heds_datahandle_t::create();
#endif

    return heds_load_data_fromfile_unicode16(handle.get(), (const ucchar*)filepath.c_str());
}

heds_errorcode heds_instance_handles::load_phasevalues(heds_datahandle_t::ptr &handle, const field<float>::ptr &field, float phaseWrap)
{
    handle = heds_datahandle_t::create();

    return heds_load_phasevalues_single(handle.get(), field->width(), field->height(), field->data(), 0, phaseWrap);
}

heds_errorcode heds_instance_handles::load_phasevalues(heds_datahandle_t::ptr &handle, const field<double>::ptr &field, float phaseWrap)
{
    handle = heds_datahandle_t::create();

    return heds_load_phasevalues_double(handle.get(), field->width(), field->height(), field->data(), 0, phaseWrap);
}

}

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


#ifndef HOLOEYE_SLMDISPLAYSDK_SHARED_CPP_H
#define HOLOEYE_SLMDISPLAYSDK_SHARED_CPP_H

#include "holoeye_platforms.h"

#if !HOLOEYE_CPP11_OR_HIGHER
    #error This header requires a C++ 11 compiler.
#endif

#ifdef HOLOEYE_SLMDISPLAYSDK_SHARED_H
    #error Please do not include "holoeye_slmdisplaysdk_shared.h" separately
#endif

#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <assert.h>

#if HOLOEYE_WINDOWS
    /** The default encoding for non-unicode strings for the current system.
     * \ingroup SLMDisplayCPP */
    #define HEDSENC_Default HEDSENC_Latin1
#else
    /** The default encoding for non-unicode strings for the current system.
     * \ingroup SLMDisplayCPP */
    #define HEDSENC_Default HEDSENC_UTF8
#endif

namespace holoeye {

#include <holoeye_slmdisplaysdk_shared.h>

#if HOLOEYE_WINDOWS
    /** The string type of the file system.
     * \ingroup SLMDisplayCPP */
    typedef std::wstring fsstring;
#else
    /** The string type of the file system.
     * \ingroup SLMDisplayCPP */
    typedef std::string fsstring;
#endif

/** This enum determines the encoding of the provided string.
 * \ingroup SLMDisplayCPP */
enum heds_encoding
{
    /** The provided string is a UTF-8 string. */
    HEDSENC_UTF8,

    /** The provided string has a latin1 encoding. */
    HEDSENC_Latin1
};

/** A helper class to easily allocate two-dimensional fields. Please use \ref holoeye::field::create to create a new instance.
 * \ingroup SLMDisplayCPP */
template<typename T> class field
{
private:

    /** The width of the field in pixels. */
    int _width;

    /** The height of the field in pixels. */
    int _height;

    /** The pointer to the data of the field. */
    T _data[1];

    /** Initializes a new field. Please use \ref holoeye::field::create to create a new instance.
     * \param width The width of the field in pixels.
     * \param height The height of the field in pixels. */
    field(int width, int height) :
        _width(width),
        _height(height)
    {
        assert(width > 0 && height > 0);

        std::memset(_data, 0, sizeof(width * height * sizeof(T)));
    }

    /** \cond internal */
    field(const field &b) = delete;
    field& operator=(const field &b) = delete;

    static void* operator new(size_t, size_t count)
    {
        return std::malloc(sizeof(int) * 2 + count * sizeof(T));
    }
    /** \endcond */

public:

    /** Defines a shared pointer for the field. */
    typedef std::shared_ptr<field<T>> ptr;

    /** \cond internal */
    static void operator delete(void *ptr)
    {
        std::free(ptr);
    }

    static void operator delete(void *ptr, size_t)
    {
        std::free(ptr);
    }
    /** \endcond */

    /** Allocates a new field with a given size.
     * \param width The width in pixels of the field to be allocated.
     * \param height The height in pixels of the field to be allocated.
     * \return Returns a pointer to the allocated field. */
    static ptr create(int width, int height)
    {
        assert(width > 0 && height > 0);

        return ptr(new(width * height) field(width, height));
    }

    /** Provides the width of the field in pixels.
     * \return The width in pixels. */
    int width() const { return _width; }

    /** Provides the height of the field in pixels.
     * \return The height in pixels. */
    int height() const { return _height; }

    /** Provides the size of the field in bytes.
     * \return The sata size in bytes. */
    size_t size() const { return (size_t) _width * (size_t) _height * sizeof(T); }

    /** Provides a pointer to the stored data.
     * \return The data pointer. */
    T* data() { return _data; }

    /** Provides a pointer to the stored data.
     * \return The data pointer. */
    const T* data() const { return _data; }

    /** Provides a pointer to a specific row of the data.
     * \param row The row to get the pointer for.
     * \return The pointer to the beginning of \p row. */
    T* row(int row)
    {
        assert(row >= 0 && row < _height);

        return _data + row * _width;
    }

    /** Provides a pointer to a specific row of the data.
     * \param row The row to get the pointer for.
     * \return The pointer to the beginning of \p row. */
    const T* row(int row) const
    {
        assert(row >= 0 && row < _height);

        return _data + row * _width;
    }

    /** Accesses an element of the field as a one-dimensional array.
     * \param n The index of the value inside the data.
     * \return A reference to the value at position \p n. */
    T& operator[](int n)
    {
        assert(n >= 0 && n < _width * _height);

        return *(_data + n);
    }

    /** Accesses an element of the field as a one-dimensional array.
     * \param n The index of the value inside the data.
     * \return The value at position \p n. */
    T operator[](int n) const
    {
        assert(n >= 0 && n < _width * _height);

        return *(_data + n);
    }

    /** Accesses an element of the field at a given point.
     * \param x The X coordinate of the value to access.
     * \param y The Y coordinate of the value to access.
     * \return A reference to the value at position \p x / \p y. */
    T& operator()(int x, int y)
    {
        assert(x >= 0 && x < _width);
        assert(y >= 0 && y < _height);

        return *(_data + x + y * _width);
    }

    /** Accesses an element of the field at a given point.
     * \param x The X coordinate of the value to access.
     * \param y The Y coordinate of the value to access.
     * \return The value at position \p x / \p y. */
    T operator()(int x, int y) const
    {
        assert(x >= 0 && x < _width);
        assert(y >= 0 && y < _height);

        return *(_data + x + y * _width);
    }
};

/** Checks if the used SDK provides the correct API version required by the user's code.
 * If the function fails, text will be written to the console window. In addition a messagebox can be shown when \p showMessagebox is true.
 * \ingroup SLMDisplayCPP
 * \param requiredApiVersion The API required by your code.
 * \param showMessagebox Defines if a messagebox will be shown, if there is an error.
 * \return Return true when the API version of the SDK is the same as the provided \p requiredApiVersion. */
extern bool heds_requires_version(int requiredApiVersion, bool showMessagebox = false);

/** Provides an error string in the encoding of the file system for a given error.
 * \ingroup SLMDisplayCPP
 * \see heds_errorcodes
 * \param error The error code to provide the string for.
 * \return An error string for the given error. Zero when the given error is invalid. */
extern const fschar* heds_error_string_fs(heds_errorcode error);

/** A helper class to manage your SLM SDK instance.
 * \ingroup SLMDisplayCPP */
class heds_instance
{
protected:

    /** The size of the SLM in pixels. */
    heds_slm_size_t _size;

    /** The size of the SLM in millimeter. */
    heds_slm_size_mm_t _size_mm;

    /** The pixelsize of the SLM in meters. */
    double _pixelsize;

    /** The refresh rate of the SLM in Hertz. */
    float _refreshrate;

public:

    /** The size of the SLM in pixels.
     * \return Returns zero when the instance has not been opened yet. */
    const heds_slm_size_t& size() const { return _size; }

    /** The size of the SLM in millimeter.
     * \return Returns zero when the instance has not been opened yet. */
    const heds_slm_size_mm_t& size_mm() const { return _size_mm; }

    /** The pixelsize of the SLM in meters.
     * \return Returns zero when the instance has not been opened yet. */
    double pixelsize() const { return _pixelsize; }

    /** The refresh rate of the SLM in Hertz.
     * \return Returns zero when the instance has not been opened yet. */
    float refreshrate() const { return _refreshrate; }

    /** Creates a new instance object. You have to call \ref open() to init this instance. */
    heds_instance() :
        _size({0, 0}),
        _size_mm({0.0, 0.0}),
        _pixelsize(0.0),
        _refreshrate(0.0f)
    {
    }

    /** Starts and opens the SLM Display SDK. Must be called before using most functions of SLM Display SDK.
     * \return HEDSERR_NoError when there is no error. */
    heds_errorcode open();

    /** Closes the SLM window. */
    void close();

    /** Closes the SLM window when this instance is destroyed. */
    ~heds_instance()
    {
        close();
    }

    /** Show or hide the SLM preview window. SLM must be initialized before opening the preview window.
     * \param show Pass true to open the preview window and false to close it.
     * \return HEDSERR_NoError when there is no error. */
    heds_errorcode utils_slmpreview_show(bool show = true);

    /** Sets the Zernike values for the SLM.
     * \param values An array of Zernike values. Refer to \ref heds_zernikevalues for more details. Must be smaller than \ref HEDSZER_COUNT. Pass empty to disable Zernike again.
     * \return HEDSERR_NoError when there is no error. */
    heds_errorcode slm_zernike(const std::vector<float> &values);
};

}

#endif // HOLOEYE_SLMDISPLAYSDK_SHARED_CPP_H

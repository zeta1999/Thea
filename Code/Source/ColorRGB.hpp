//============================================================================
//
// This file is part of the Thea toolkit.
//
// This software is distributed under the BSD license, as detailed in the
// accompanying LICENSE.txt file. Portions are derived from other works:
// their respective licenses and copyright information are reproduced in
// LICENSE.txt and/or in the relevant source files.
//
// Author: Siddhartha Chaudhuri
// First version: 2013
//
//============================================================================

/*
 ORIGINAL HEADER

 @file ColorRGB.h

 Color class

 @maintainer Morgan McGuire, http://graphics.cs.williams.edu
 @cite Portions based on Dave Eberly's Magic Software Library
      at <A HREF="http://www.magic-software.com">http://www.magic-software.com</A>

 @created 2001-06-02
 @edited  2009-04-28

 Copyright 2000-2009, Morgan McGuire.
 All rights reserved.
*/

#ifndef __Thea_ColorRGB_hpp__
#define __Thea_ColorRGB_hpp__

#include "Common.hpp"
#include "ColorL.hpp"
#include "Math.hpp"
#include "MatVec.hpp"

namespace Thea {

// Forward declarations
class ColorRGB8;
class ColorRGBA;

/**
 * A color with three floating-point channels: red, green and blue, each in [0, 1]. Derived from the G3D library:
 * http://g3d.sourceforge.net
 */
class THEA_API ColorRGB
{
  private:
    Real c[3];  ///< Three components: red, green and blue.

  public:
    /** Default constructor. Does not initialize fields. */
    ColorRGB() {}

    /** Construct from red, green and blue components. */
    ColorRGB(Real r_, Real g_, Real b_) { c[0] = r_; c[1] = g_; c[2] = b_; }

    /** Initialize all channels to the same value. */
    explicit ColorRGB(Real v) { c[0] = c[1] = c[2] = v; }

    /** Construct a color from a 3-vector. */
    explicit ColorRGB(Vector3 const & v) { c[0] = v[0]; c[1] = v[1]; c[2] = v[2]; }

    /** Construct a color from three components in an array. */
    explicit ColorRGB(Real const * v) { c[0] = v[0]; c[1] = v[1]; c[2] = v[2]; }

    /** Initialize from a 32-bit RGBA color. For conversion from a consistent source type. */
    ColorRGB(ColorRGBA const & other);

    /** Copy constructor. */
    ColorRGB(ColorRGB const & other) { c[0] = other.c[0]; c[1] = other.c[1]; c[2] = other.c[2]; }

    /** Construct from a color with byte channels, with automatic scaling from [0, 255] to [0, 1]. */
    ColorRGB(ColorRGB8 const & src);

    /** Initialize from an HTML-style color (e.g. 0xFF0000 == RED) */
    static ColorRGB fromARGB(uint32 argb);

    /** Random color with unit magnitude. */
    static ColorRGB random();

    /** Returns one of the color wheel colors (such as red, green, cyan). Does not include white, black, or gray. */
    static ColorRGB const & wheelRandom();

    /**
     * Generate colors according to the ANSI color set, mod 16.
     *
     * @see pastelMap
     */
    static ColorRGB ansiMap(int i);

    /**
     * Generate colors using a hash such that adjacent values are unlikely to have similar colors. Useful for rendering with
     * stable but arbitrary colors, e.g., when debugging a mesh algorithm.
     *
     * @see ansiMap
     */
    static ColorRGB pastelMap(int i);

    /** The value of the red channel. */
    Real r() const { return c[0]; }

    /** A reference to the red channel. */
    Real & r() { return c[0]; }

    /** The value of the green channel. */
    Real g() const { return c[1]; }

    /** A reference to the green channel. */
    Real & g() { return c[1]; }

    /** The value of the blue channel. */
    Real b() const { return c[2]; }

    /** A reference to the blue channel. */
    Real & b() { return c[2]; }

    /** Get the address of the array storing color channel values in RGB order. */
    Real const * data() const { return c; }

    /** Get the address of the array storing color channel values in RGB order. */
    Real * data() { return c; }

    /** Array-style channel access. */
    template <typename IntegerT> Real const & operator[](IntegerT channel) const
    {
      debugAssertM(channel >= 0 && channel <= 2, "ColorRGB: Channel must be 0, 1 or 2");
      return c[channel];
    }

    /** Array-style channel access. */
    template <typename IntegerT> Real & operator[](IntegerT channel)
    {
      debugAssertM(channel >= 0 && channel <= 2, "ColorRGB: Channel must be 0, 1 or 2");
      return c[channel];
    }

    /** Set all channels simultaneously. */
    void set(Real r_, Real g_, Real b_)
    {
      c[0] = r_;
      c[1] = g_;
      c[2] = b_;
    }

    /** Addition. */
    ColorRGB operator+(ColorRGB const & rhs) const
    {
      return ColorRGB(c[0] + rhs.c[0], c[1] + rhs.c[1], c[2] + rhs.c[2]);
    }

    /** Subtraction. */
    ColorRGB operator-(ColorRGB const & rhs) const
    {
      return ColorRGB(c[0] - rhs.c[0], c[1] - rhs.c[1], c[2] - rhs.c[2]);
    }

    /** Multiplication by a scalar. */
    ColorRGB operator*(Real s) const
    {
      return ColorRGB(c[0] * s, c[1] * s, c[2] * s);
    }

    /** Component-wise multiplication by another color. */
    ColorRGB operator*(ColorRGB const & rhs) const
    {
      return ColorRGB(c[0] * rhs.c[0], c[1] * rhs.c[1], c[2] * rhs.c[2]);
    }

    /** Division by a scalar. */
    ColorRGB operator/(Real s) const
    {
      return ColorRGB(c[0] / s, c[1] / s, c[2] / s);
    }

    /** Component-wise division by another color. */
    ColorRGB operator/(ColorRGB const & rhs) const
    {
      return ColorRGB(c[0] / rhs.c[0], c[1] / rhs.c[1], c[2] / rhs.c[2]);
    }

    /** Negation of the color. */
    ColorRGB operator-() const
    {
      return ColorRGB(-c[0], -c[1], -c[2]);
    }

    /** Add and assign. */
    ColorRGB & operator+=(ColorRGB const & rhs)
    {
      c[0] += rhs.c[0];
      c[1] += rhs.c[1];
      c[2] += rhs.c[2];
      return *this;
    }

    /** Subtract and assign. */
    ColorRGB & operator-=(ColorRGB const & rhs)
    {
      c[0] -= rhs.c[0];
      c[1] -= rhs.c[1];
      c[2] -= rhs.c[2];
      return *this;
    }

    /** Multiply component-wise and assign. */
    ColorRGB & operator*=(ColorRGB const & rhs)
    {
      c[0] *= rhs.c[0];
      c[1] *= rhs.c[1];
      c[2] *= rhs.c[2];
      return *this;
    }

    /** Divide component-wise and assign. */
    ColorRGB & operator/=(ColorRGB const & rhs)
    {
      c[0] /= rhs.c[0];
      c[1] /= rhs.c[1];
      c[2] /= rhs.c[2];
      return *this;
    }

    /** Multiply by a scalar and assign. */
    ColorRGB & operator*=(Real s)
    {
      c[0] *= s;
      c[1] *= s;
      c[2] *= s;
      return *this;
    }

    /** Divide by a scalar and assign. */
    ColorRGB & operator/=(Real s)
    {
      c[0] /= s;
      c[1] /= s;
      c[2] /= s;
      return *this;
    }

    /** Get the square of the magnitude of the color. */
    Real squaredNorm() const { return c[0] * c[0] + c[1] * c[1] + c[2] * c[2]; }

    /** Get the magnitude of the color. */
    Real norm() const { return std::sqrt(squaredNorm()); }

    /** Scale the color to unit magnitude. */
    void normalize()
    {
      *this = normalized();
    }

    /** Get a unit magnitude color by dividing by the magnitude. */
    ColorRGB normalized() const
    {
      Real len = norm();
      if (std::abs(len) < 32 * std::numeric_limits<Real>::min())
        return ColorRGB(0, 0, 0);
      else
        return *this / len;
    }

    /** Check if two colors are approximately equal. */
    bool fuzzyEq(ColorRGB const & other) const
    {
      return Math::fuzzyEq((*this - other).squaredNorm(), (Real)0);
    }

    /** Check if two colors are not approximately equal. */
    bool fuzzyNe(ColorRGB const & other) const
    {
      return Math::fuzzyNe((*this - other).squaredNorm(), (Real)0);
    }

    /** Raise the components to powers specified as another color. */
    ColorRGB pow(ColorRGB const & rhs) const
    {
      return ColorRGB(std::pow(c[0], rhs.c[0]), std::pow(c[1], rhs.c[1]), std::pow(c[2], rhs.c[2]));
    }

    /** Raise the components to a power. */
    ColorRGB pow(Real s) const
    {
      return ColorRGB(std::pow(c[0], s), std::pow(c[1], s), std::pow(c[2], s));
    }

    /** Get the per-component maximum of two colors. */
    ColorRGB max(ColorRGB const & rhs) const
    {
      return ColorRGB(std::max(c[0], rhs.c[0]), std::max(c[1], rhs.c[1]), std::max(c[2], rhs.c[2]));
    }

    /** Get the per-component minimum of two colors. */
    ColorRGB min(ColorRGB const & rhs) const
    {
      return ColorRGB(std::min(c[0], rhs.c[0]), std::min(c[1], rhs.c[1]), std::min(c[2], rhs.c[2]));
    }

    /** Get the largest component. */
    Real max() const
    {
      return std::max(std::max(c[0], c[1]), c[2]);
    }

    /** Get the smallest component. */
    Real min() const
    {
      return std::min(std::min(c[0], c[1]), c[2]);
    }

    /** Swap the red and blue channels. */
    ColorRGB bgr() const
    {
      return ColorRGB(c[2], c[1], c[0]);
    }

    /** Get a string representation of the color. */
    std::string toString() const;

    /**
     * Convert from HSV to RGB.
     *
     * @note toHSV(fromHSV(hsv)) may not be hsv, if it is at a grey point or black point. The components of hsv should lie in
     *   the unit interval.
     *
     * @see Alvy Ray Smith. "Color Gamut Transform Pairs". SIGGRAPH 1978
     */
    static ColorRGB fromHSV(Vector3 const & hsv);

    /**
     * Convert from RGB to HSV.
     *
     * @see fromHSV
     */
    Vector3 toHSV() const;

    /** Duplicates the Matlab jet colormap map [0, 1] --> (r, g, b) where blue is close to 0 and red is close to 1. */
    static ColorRGB jetColorMap(Real val);

    /**
     * Get a color with a specified hue, and maximum saturation and value.
     *
     * @param hue The target hue. Must be in [0, 1].
     */
    static ColorRGB rainbowColorMap(Real hue);

    // Named colors
    static ColorRGB const & red();     ///< Red color.
    static ColorRGB const & green();   ///< Green color.
    static ColorRGB const & blue();    ///< Blue color.
    static ColorRGB const & purple();  ///< Purple color.
    static ColorRGB const & cyan();    ///< Cyan color.
    static ColorRGB const & yellow();  ///< Yellow color.
    static ColorRGB const & brown();   ///< Brown color.
    static ColorRGB const & orange();  ///< Orange color.
    static ColorRGB const & black();   ///< Black color.
    static ColorRGB const & gray();    ///< Gray color.
    static ColorRGB const & white();   ///< White color.
    static ColorRGB const & zero();    ///< Color with all channels zero (black).

}; // class ColorRGB

/** Multiply by a scalar. */
inline ColorRGB
operator*(Real s, ColorRGB const & c)
{
  return c * s;
}

/** Multiply by a one-channel color. */
inline ColorRGB
operator*(ColorL & s, ColorRGB const & c)
{
  return c * s.value();
}

/** Multiply by a one-channel color. */
inline ColorRGB
operator*(ColorRGB const & c, ColorL & s)
{
  return c * s.value();
}

} // namespace Thea

#endif

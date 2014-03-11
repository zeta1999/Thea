//============================================================================
//
// This file is part of the Thea project.
//
// This software is covered by the following BSD license, except for portions
// derived from other works which are covered by their respective licenses.
// For full licensing information including reproduction of these external
// licenses, see the file LICENSE.txt provided in the documentation.
//
// Copyright (C) 2013, Siddhartha Chaudhuri/Princeton University
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holders nor the names of contributors
// to this software may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//============================================================================

#ifndef __Thea_Algorithms_BestFitSphere3_hpp__
#define __Thea_Algorithms_BestFitSphere3_hpp__

#include "../Common.hpp"
#include "../Array.hpp"
#include "../Ball3.hpp"

namespace Thea {
namespace Algorithms {

/** Approximate best-fit sphere. */
class THEA_API BestFitSphere3
{
  public:
    THEA_DEF_POINTER_TYPES(BestFitSphere3, shared_ptr, weak_ptr)

    /** Default constructor. */
    BestFitSphere3();

    /** Add a point to the set. */
    void addPoint(Vector3 const & point);

    /** Remove all data and (lazily) set the sphere to null. */
    void clear();

    /** Remove all cached data to free memory, but do <b>not</b> mark the sphere for recomputation. */
    void releaseMemoryWithoutUpdate();

    /** Get the radius of the sphere. */
    Real getRadius() const;

    /** Get the diameter of the sphere. */
    Real getDiameter() const;

    /** Get the center of the sphere. */
    Vector3 const & getCenter() const;

    /** Get the ball bounded by the sphere. */
    Ball3 const & getBall() const;

  private:
    /** Recompute the best-fit sphere. */
    void update() const;

    TheaArray<Vector3> points;
    Real eps;
    mutable Ball3 ball;
    mutable bool updated;

}; // class BestFitSphere3

} // namespace Thea
} // namespace Algorithms

#endif
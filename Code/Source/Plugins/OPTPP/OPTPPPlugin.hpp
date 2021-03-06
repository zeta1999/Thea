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
// First version: 2009
//
//============================================================================

#ifndef __Thea_Algorithms_OPTPPPlugin_hpp__
#define __Thea_Algorithms_OPTPPPlugin_hpp__

#include "../../Plugin.hpp"
#include "OPTPPCommon.hpp"

namespace Thea {
namespace Algorithms {

// Forward declaration
class OPTPPNumericalOptimizerFactory;

/** A OPT++-based plugin for solving nonlinear optimization problems. */
class THEA_OPTPP_DLL_LOCAL OPTPPPlugin : public Plugin
{
  public:
    /** Constructor. */
    OPTPPPlugin(FactoryRegistry * registry_);

    /** Destructor. */
    ~OPTPPPlugin();

    char const * getName() const;
    void install();
    void startup();
    void shutdown();
    void uninstall();

  private:
    FactoryRegistry * registry;
    OPTPPNumericalOptimizerFactory * factory;
    bool started;

}; // class OPTPPPlugin

} // namespace Algorithms
} // namespace Thea

#endif

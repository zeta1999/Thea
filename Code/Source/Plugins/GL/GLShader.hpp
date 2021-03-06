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

#ifndef __Thea_Graphics_GLShader_hpp__
#define __Thea_Graphics_GLShader_hpp__

#include "../../Graphics/Shader.hpp"
#include "../../Array.hpp"
#include "../../Map.hpp"
#include "GLCommon.hpp"
#include "GLTexture.hpp"
#include "GLHeaders.hpp"

namespace Thea {
namespace Graphics {
namespace GL {

// Forward declarations
class GLRenderSystem;

/** An OpenGL shader. */
class THEA_GL_DLL_LOCAL GLShader : public Shader
{
  public:
    /** Constructor. */
    GLShader(GLRenderSystem * render_system_, char const * name_);

    /** Destructor. */
    ~GLShader();

    /** Get the parent rendersystem. */
    GLRenderSystem * getRenderSystem() const { return render_system; }

    char const * getName() const { return name.c_str(); }

    int8 isComplete() const { return complete; }

    void attachModuleFromFile(ModuleType type, char const * path);
    void attachModuleFromString(ModuleType type, char const * source);

    int8 hasUniform(char const * uniform_name) const { return uniforms.find(uniform_name) != uniforms.end(); }

    void setUniform(char const * uniform_name, float32 value);
    void setUniform(char const * uniform_name, int32 value);
    void setUniform(char const * uniform_name, Vector2 const & value);
    void setUniform(char const * uniform_name, Vector3 const & value);
    void setUniform(char const * uniform_name, Vector4 const & value);
    void setUniform(char const * uniform_name, ColorL8 const & value);
    void setUniform(char const * uniform_name, ColorL const & value);
    void setUniform(char const * uniform_name, ColorRGB8 const & value);
    void setUniform(char const * uniform_name, ColorRGB const & value);
    void setUniform(char const * uniform_name, ColorRGBA8 const & value);
    void setUniform(char const * uniform_name, ColorRGBA const & value);
    void setUniform(char const * uniform_name, Matrix2 const & value);
    void setUniform(char const * uniform_name, Matrix3 const & value);
    void setUniform(char const * uniform_name, Matrix4 const & value);
    void setUniform(char const * uniform_name, Texture * value);

    void setUniform(char const * uniform_name, int64 num_values, float32 const * values);
    void setUniform(char const * uniform_name, int64 num_values, int32 const * values);
    void setUniform(char const * uniform_name, int64 num_values, Vector2 const * values);
    void setUniform(char const * uniform_name, int64 num_values, Vector3 const * values);
    void setUniform(char const * uniform_name, int64 num_values, Vector4 const * values);
    void setUniform(char const * uniform_name, int64 num_values, ColorL8 const * values);
    void setUniform(char const * uniform_name, int64 num_values, ColorL const * values);
    void setUniform(char const * uniform_name, int64 num_values, ColorRGB8 const * values);
    void setUniform(char const * uniform_name, int64 num_values, ColorRGB const * values);
    void setUniform(char const * uniform_name, int64 num_values, ColorRGBA8 const * values);
    void setUniform(char const * uniform_name, int64 num_values, ColorRGBA const * values);
    void setUniform(char const * uniform_name, int64 num_values, Matrix2 const * values);
    void setUniform(char const * uniform_name, int64 num_values, Matrix3 const * values);
    void setUniform(char const * uniform_name, int64 num_values, Matrix4 const * values);
    void setUniform(char const * uniform_name, int64 num_values, Texture * const * values);

    /** Link the various modules of the shader into a single program. */
    void link();

    /** Use the shader for rendering. */
    void use();

    /** Get the OpenGL ID of the shader. */
    GLhandleARB getGLID() const { return program_id; }

  private:
    /** A value for a uniform variable. */
    struct UniformValue
    {
      float64 f_val;
      int32 i_val;
      Array<float32> f_array;
      Array<int32> i_array;
      GLTexture * texture;
    };

    /** Data related to an uniform variable. */
    struct UniformData
    {
      GLenum type;
      GLint size;
      GLint location;
      int32 texunit;
      bool has_value;
      UniformValue value;
      bool requires_rebind;

      /** Constructor. */
      UniformData() : has_value(false), requires_rebind(false) {}

      /** Note that the value has been changed. */
      void valueChanged() { has_value = true; requires_rebind = true; }
    };

    /** A set of uniforms read from source code. */
    typedef Map<std::string, UniformData> Uniforms;

    /** Read the list of active uniforms in the shader object. */
    void readActiveUniforms();

    /** Bind the user-provided uniforms to the shader object. */
    void bindUniforms();

    /** Check if a build step (compile or link) succeeded, and throw a custom error if it did not. */
    void checkBuildStatus(GLhandleARB obj_id, GLenum status_field, std::string const & error_msg);

    GLRenderSystem * render_system;
    std::string name;
    bool complete;
    bool linked;
    bool has_vertex_module;
    bool has_fragment_module;
    GLhandleARB program_id;
    Uniforms uniforms;

}; // class GLShader

} // namespace GL
} // namespace Graphics
} // namespace Thea

#endif

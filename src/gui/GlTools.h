
#ifndef GlTools_HeaderFile
#define GlTools_HeaderFile

#include <V3d_View.hxx>
#include <OpenGl_Context.hxx>

namespace echocad
{

//! Auxiliary wrapper to avoid OpenGL macros collisions between Qt and OCCT headers.
class GlTools
{
public:
  //! Return GL context.
  static Handle(OpenGl_Context) GetGlContext (const Handle(V3d_View)& theView);
};

} // namespace echocad

#endif // GlTools_HeaderFile

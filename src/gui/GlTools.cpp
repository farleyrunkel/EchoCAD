
#include "GlTools.h"

#include <OpenGl_GraphicDriver.hxx>
#include <OpenGl_FrameBuffer.hxx>
#include <OpenGl_View.hxx>
#include <OpenGl_Window.hxx>

// ================================================================
// Function : GetGlContext
// Purpose  :
// ================================================================

namespace echocad
{

	Handle(OpenGl_Context) GlTools::GetGlContext(const Handle(V3d_View)& theView)
	{
		Handle(OpenGl_View) aGlView = Handle(OpenGl_View)::DownCast(theView->View());
		return aGlView->GlWindow()->GetGlContext();
	}

} // namespace echocad
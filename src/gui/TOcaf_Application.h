
#ifndef _TOcaf_Application_HeaderFile
#define _TOcaf_Application_HeaderFile

#include <TColStd_SequenceOfExtendedString.hxx>
#include <TDocStd_Application.hxx>

//! Sample OCAF application
class TOcaf_Application : public TDocStd_Application
{
	DEFINE_STANDARD_RTTI_INLINE(TOcaf_Application, TDocStd_Application)
public:

	Standard_EXPORT TOcaf_Application();
};

#endif

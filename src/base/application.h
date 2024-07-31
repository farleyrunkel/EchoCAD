
#ifndef ECHOCAD_APPLICATION_H
#define ECHOCAD_APPLICATION_H


#include "document.h"
#include <Standard_Handle.hxx>
#include <XCAFApp_Application.hxx>


namespace App {

// Provides management of Document objects
class Application : public XCAFApp_Application {

    DEFINE_STANDARD_RTTI_INLINE(Application, XCAFApp_Application)

public:
    Application() {};
	~Application() {};

};


} // namespace App


using ApplicationPtr = Handle(App::Application);

#endif // !ECHOCAD_APPLICATION_H

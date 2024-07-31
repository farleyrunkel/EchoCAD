
#ifndef ECHOCAD_GUIAPPLICATION_H
#define ECHOCAD_GUIAPPLICATION_H

#include <memory>

#include "application.h"

namespace echocad {

class GuiDocument;

class GuiApplication {
public:
    GuiApplication(const ApplicationPtr& app) {};
    ~GuiApplication() {};

    // Not copyable
    GuiApplication(const GuiApplication&) = delete;
    GuiApplication& operator=(const GuiApplication&) = delete;

    const ApplicationPtr& application() const { return myApplication; };

private:
    ApplicationPtr myApplication;

};

} // namespace echocad

#endif // ECHOCAD_GUIAPPLICATION_H
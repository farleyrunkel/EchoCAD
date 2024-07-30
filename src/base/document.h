#ifndef ECHOCAD_DOCUMENT_H
#define ECHOCAD_DOCUMENT_H

#include <TDocStd_Document.hxx>
#include "signal.h"

#include <string>
#include <string_view>

namespace echocad {

// Provides a data container, composed of labels and attributes
// It extends TDocStd_Document to provide an actualized model tree of its contents
// Entities are actually "root" data items
class Document : public TDocStd_Document {
public:

};

} // namespace echocad


#endif // !ECHOCAD_DOCUMENT_H

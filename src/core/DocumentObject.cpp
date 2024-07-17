#include "DocumentObject.h"

namespace App
{

    DocumentObject::DocumentObject()
    {
		// Constructor implementation
	}

    DocumentObject::DocumentObject(const std::string& typeName, const std::string& objectName)
        : typeName(typeName), objectName(objectName) {
        // Constructor implementation
    }

    DocumentObject::~DocumentObject() {
        // Destructor implementation
    }

    const std::string& DocumentObject::getTypeName() const {
        // Return the type name of the object
        return typeName;
    }

    const std::string& DocumentObject::getObjectName() const {
        // Return the object name
        return objectName;
    }

    void DocumentObject::setObjectName(const std::string& newName) {
        // Set a new name for the object
        objectName = newName;
    }
}

#include "Document.h"
#include <iostream>
#include "DocumentObject.h"

namespace App
{
    Document::Document() : saved(false) {
        // Constructor implementation
        std::cout << "Document created" << std::endl;
    }

    Document::~Document() {
        // Destructor implementation
        clearDocument();
        std::cout << "Document destroyed" << std::endl;
    }

    void Document::clearDocument() {
        // Clear the document content
        for (auto* obj : objects) {
            delete obj;
        }
        objects.clear();
        saved = false;
    }

    bool Document::save() {
        // Simulate saving the document
        std::cout << "Document saved" << std::endl;
        saved = true;
        return saved;
    }

    bool Document::saveAs(const char* file) {
        // Simulate saving the document with a new name
        name = file;
        std::cout << "Document saved as: " << file << std::endl;
        saved = true;
        return saved;
    }

    bool Document::isSaved() const {
        // Check if the document is saved
        return saved;
    }

    const char* Document::getName() const {
        // Return the document name
        return name.c_str();
    }

    DocumentObject* Document::addObject(const char* sType, const char* pObjectName) {
        // Simulate adding an object to the document
        // This is a placeholder implementation
        DocumentObject* newObj = new DocumentObject(); // Assuming DocumentObject has a default constructor
        objects.push_back(newObj);
        std::cout << "Added object of type: " << sType << " with name: " << (pObjectName ? pObjectName : "Unnamed") << std::endl;
        return newObj;
    }
}

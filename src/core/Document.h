#ifndef APP_DOCUMENT_H
#define APP_DOCUMENT_H

#include <string>
#include <vector>

namespace App
{
    class DocumentObject;

    /// The simplified document class
    class Document
    {
    public:
        Document();
        ~Document();

        // Basic document operations
        void clearDocument();
        bool save();
        bool saveAs(const char* file);
        bool isSaved() const;
        const char* getName() const;

        // Object handling
        DocumentObject* addObject(const char* sType, const char* pObjectName = nullptr);

    private:
        std::string name;
        bool saved;
        std::vector<DocumentObject*> objects;
    };
}

#endif // APP_DOCUMENT_H
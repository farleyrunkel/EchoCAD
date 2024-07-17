#ifndef APP_DOCUMENTOBJECT_H
#define APP_DOCUMENTOBJECT_H

#include <string>

namespace App
{
    /// The simplified DocumentObject class
    class DocumentObject
    {
    public:
        DocumentObject();
        DocumentObject(const std::string& typeName, const std::string& objectName = "");
        ~DocumentObject();

        // Basic operations
        const std::string& getTypeName() const;
        const std::string& getObjectName() const;
        void setObjectName(const std::string& newName);

    private:
        std::string typeName;
        std::string objectName;
    };
}

#endif // APP_DOCUMENTOBJECT_H

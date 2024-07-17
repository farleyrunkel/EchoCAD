#ifndef GUI_TREE_H
#define GUI_TREE_H

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <unordered_map> 
#include "Document.h"
#include "DocumentObject.h"

class DocumentObjectItem : public QTreeWidgetItem {
public:
    DocumentObjectItem(const QString& name) : QTreeWidgetItem() {
        setText(0, name);
    }
};

class DocumentItem : public QTreeWidgetItem {
public:
    DocumentItem(const QString& name) : QTreeWidgetItem() {
        setText(0, name); 
    }
};

class DocumentTreeWidget : public QTreeWidget {
public:
    DocumentTreeWidget():QTreeWidget(this) {

        setHeaderLabel(tr("Document structure")); 

        QVBoxLayout* layout = new QVBoxLayout(this); 

        setLayout(layout); 

        addDocument("Document example");
    }

    void addDocument(const QString& name) {
        DocumentItem* docItem = new DocumentItem(name);
        addTopLevelItem(docItem);

        addDocumentObject(docItem, "Document object example");
    }

    void addDocumentObject(DocumentItem* docItem, const QString& name) {
        DocumentObjectItem* objItem = new DocumentObjectItem(name);
        docItem->addChild(objItem); 
    }

private:

    std::unordered_map<const App::Document*, DocumentItem*> DocumentMap;

};

#endif // GUI_TREE_H
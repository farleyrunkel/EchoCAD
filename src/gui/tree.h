#ifndef ECHOCAD_TREE_H
#define ECHOCAD_TREE_H

#include <QStyledItemDelegate>
#include <QTreeWidget>
#include <TOcaf_Application.h>
#include <TDF_Label.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDF_Tool.hxx>
#include <TDF_ChildIterator.hxx>
#include <TCollection_AsciiString.hxx>
#include <stack>
#include <TDocStd_SequenceOfDocument.hxx>
#include <TDF_AttributeIterator.hxx>


class TreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    using QTreeWidget::QTreeWidget;

    void setApp(TOcaf_Application* app)
	{
		myApp = app;
	}

	void DumpChildren(const TDF_Label& aLabel)
	{
		TDF_ChildIterator it;
		TCollection_AsciiString es;
		for (it.Initialize(aLabel, Standard_True); it.More(); it.Next()) {
			TDF_Tool::Entry(it.Value(), es);
			std::cout << es.ToCString() << std::endl;
		}
	}

	void DumpChildrenRecursive(const TDF_Label& aLabel)
	{
		if (aLabel.IsNull())
			return;
		TDF_ChildIterator it;
		TCollection_AsciiString es;
		for (it.Initialize(aLabel, Standard_True); it.More(); it.Next()) {
			TDF_Tool::Entry(it.Value(), es);
			std::cout << es.ToCString() << std::endl;
		}
	}

	void updateTree(const Handle(TDocStd_Document)& theDoc) {
		clear();

		TDF_Label rootLabel = theDoc->Main();
		TCollection_AsciiString es;
		TDF_Tool::Entry(rootLabel, es);
	
		QTreeWidgetItem* rootItem = new QTreeWidgetItem(this);
		rootItem->setText(0, es.ToCString());

		// use stack to iterate over children and build tree item
		std::stack<std::pair<TDF_Label, QTreeWidgetItem*>> stack;
		stack.push(std::make_pair(rootLabel, rootItem));
		while (!stack.empty()) {
			auto [label, item] = stack.top();
			stack.pop();

			loadLabelAttributes(label, item);

			TDF_ChildIterator it;
			for (it.Initialize(label, Standard_True); it.More(); it.Next()) {
				TDF_Label childLabel = it.Value();
				TCollection_AsciiString es;
				TDF_Tool::Entry(childLabel, es);
				QTreeWidgetItem* childItem = new QTreeWidgetItem(item);
				childItem->setText(0, es.ToCString());
				stack.push(std::make_pair(childLabel, childItem));
			}
		}
	}

	void loadLabelAttributes(const TDF_Label& label, QTreeWidgetItem* treeItem)
{
		for (TDF_AttributeIterator it(label); it.More(); it.Next()) {
			const Handle_TDF_Attribute ptrAttr = it.Value();
			const Standard_GUID& attrId = ptrAttr->ID();
			QString text;
			QString value;
			if (attrId == TDataStd_Name::GetID()) {
				const auto& name = static_cast<const TDataStd_Name&>(*ptrAttr);
				text = "TDataStd_Name";
				value = QString::fromUtf16(name.Get().ToExtString(), name.Get().Length());;
			}
			else {
				std::stringstream sstream;
				ptrAttr->Dump(sstream);
				QString strDump = QString::fromStdString(sstream.str());

				int i = 0;
				while (i < strDump.size() && strDump.at(i).isSpace()) ++i;

				const int wordStart = i;
				while (i < strDump.size() && !strDump.at(i).isSpace()) ++i;

				const int wordEnd = i;
				while (i < strDump.size() && strDump.at(i).isSpace()) ++i;

				const int dataStart = i;
				text = wordStart < strDump.size() ? strDump.mid(wordStart, wordEnd - wordStart) : QStringLiteral("??");
				strDump = strDump.right(strDump.size() - dataStart);
				value = strDump.replace(QChar('\n'), QString("  "));
			}

			auto attrTreeItem = new QTreeWidgetItem;
			attrTreeItem->setText(0, text + ": " + value);
			attrTreeItem->setToolTip(0, text + ": " + value);

			treeItem->addChild(attrTreeItem);
		}
	}

	void addDocument(const Handle(TDocStd_Document)& theDoc)
	{
		myDoc = theDoc;
		updateTree(theDoc);
	}

	void setDoc(const Handle(TDocStd_Document)& doc)
	{
		myDoc = doc;
	}

private:
	Handle(TOcaf_Application) myApp;
	Handle(TDocStd_Document) myDoc;
	TDocStd_SequenceOfDocument myDocs;
};

#endif // !ECHOCAD_TREE_H

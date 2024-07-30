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

class TreeWidget : public QTreeWidget
{
    Q_OBJECT

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

	void updateTree() {
		clear();
		if (myApp.IsNull())
			return;

		TDF_Label rootLabel = myDoc->Main();
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

	void setDoc(Handle(TDocStd_Document) doc)
	{
		myDoc = doc;
		updateTree();
	}


private:
	Handle(TOcaf_Application) myApp;
	Handle(TDocStd_Document) myDoc;
};

#endif // !ECHOCAD_TREE_H


#ifndef _OcctQtViewer_HeaderFile
#define _OcctQtViewer_HeaderFile

#include <QOpenGLWidget>

#include <iostream>

#include <Standard_WarningsDisable.hxx>
#include <Standard_WarningsRestore.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_ViewController.hxx>
#include <V3d_View.hxx>
#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>
#include <TDocStd_Document.hxx>
#include <TDataStd_Real.hxx>
#include <TDataStd_Name.hxx>
#include <TFunction_Function.hxx>
#include <TFunction_Logbook.hxx>
#include <TFunction_Driver.hxx>
#include <TFunction_DriverTable.hxx>
#include <TDataStd_Integer.hxx>
#include <DNaming_BoxDriver.hxx>
#include <TNaming_NamedShape.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <TDF_TagSource.hxx>
#include <TDF_Label.hxx>
#include <TDF_ChildIterator.hxx>
#include <DNaming.hxx>
#include <ModelDefinitions.hxx>
#include <TPrsStd_AISViewer.hxx>

#include "ViewCube.h"
#include "Manipulator.h"
#include "TOcaf_Application.h"

class AIS_ViewCube;

namespace echocad {

class ModelEditor : public QOpenGLWidget, public AIS_ViewController
{
  Q_OBJECT
public:
    enum EditMode {
        Select = 0,
        Move,
        Rotate,
};

  //! Main constructor.
  ModelEditor (QWidget* theParent = nullptr);

  //! Destructor.
  virtual ~ModelEditor();

  //! Return Viewer.
  const Handle(V3d_Viewer)& Viewer() const { return myViewer; }

  //! Return View.
  const Handle(V3d_View)& View() const { return myView; }

  //! Return AIS context.
  const Handle(AIS_InteractiveContext)& Context() const { return myContext; }

  void Display(const TopoDS_Shape& shape) {myContext->Display(new AIS_Shape(shape), AIS_Shaded, 0, false);}

  //! Return OpenGL info.
  const QString& getGlInfo() const { return myGlInfo; }

  //! Minial widget size.
  virtual QSize minimumSizeHint() const override { return QSize(200, 200); }

  //! Default widget size.
  virtual QSize sizeHint()        const override { return QSize(720, 480); }

  void setEditMode(EditMode theMode) { myCurrentMode = theMode; }

  void showManipulator() { 

      if (myCurrentMode == Select) {
          auto aManip = new Manipulator();

          myContext->Display(aManip, true);
      }

}

  void open(const TCollection_ExtendedString& path);

  void test() {

      // Open a new command (for undo)
      myOcafDoc->NewCommand();

      // A data structure for our box:
      // the box itself is attached to the BoxLabel label (as his name and his function attribute)
      // its arguments (dimensions: width, length and height; and position: x, y, z)
      // are attached to the child labels of the box:
      // 0:1 Box Label ---> Name --->  Named shape ---> Function
      //     0:1:1 -- Width Label
      //     0:1:2 -- Length Label
      //     0:1:3 -- Height Label
      //     0:1:4 -- X Label
      //     0:1:5 -- Y Label
      //     0:1:6 -- Z Label

      // Create a new label in the data structure for the box
      TDF_Label aLabel = TDF_TagSource::NewChild(myOcafDoc->Main());

      Standard_Real aBoxWidth(30.0), aBoxLength(20.0), aBoxHeight(10.0);
      Standard_Real aBoxX(0.0), aBoxY(0.0), aBoxZ(0.0);
      Standard_CString aBoxName("OcafBox");
      // Create the data structure : Set the dimensions, position and name attributes
      TDataStd_Real::Set(aLabel.FindChild(1), aBoxWidth);
      TDataStd_Real::Set(aLabel.FindChild(2), aBoxLength);
      TDataStd_Real::Set(aLabel.FindChild(3), aBoxHeight);
      TDataStd_Real::Set(aLabel.FindChild(4), aBoxX);
      TDataStd_Real::Set(aLabel.FindChild(5), aBoxY);
      TDataStd_Real::Set(aLabel.FindChild(6), aBoxZ);
      TDataStd_Name::Set(aLabel, aBoxName); // Name

      // Instantiate a TFunction_Function attribute connected to the current box driver
      // and attach it to the data structure as an attribute of the Box Label
      Handle(TFunction_Function) myFunction = TFunction_Function::Set(aLabel, BOX_GUID);

      // Initialize and execute the box driver (look at the "Execute()" code)
      Handle(TFunction_Logbook) aLogBook = TFunction_Logbook::Set(aLabel);

      Handle(TFunction_Driver) myBoxDriver;
      // Find the TOcafFunction_BoxDriver in the TFunction_DriverTable using its GUID
      if (!TFunction_DriverTable::Get()->FindDriver(BOX_GUID, myBoxDriver))
      {
          std::cout << "Ocaf Box driver not found" << std::endl;
      }

      myBoxDriver->Init(aLabel);
      if (myBoxDriver->Execute(aLogBook))
      {
          std::cout << "Create Box function execute failed" << std::endl;
      }

      // Get the TPrsStd_AISPresentation of the new box TNaming_NamedShape
      Handle(TPrsStd_AISPresentation) anAisPresentation = TPrsStd_AISPresentation::Set(aLabel, TNaming_NamedShape::GetID());
      // Display it
      anAisPresentation->Display(true);
      // Attach an integer attribute to aLabel to memorize it's displayed
      TDataStd_Integer::Set(aLabel, 1);
      myContext->UpdateCurrentViewer();

      // Close the command (for undo)
      myOcafDoc->CommitCommand();

      std::cout << "Created a box with name: " << aBoxName << std::endl;
      std::cout << "base coord X: " << aBoxX << " Y: " << aBoxY << " Z: " << aBoxZ << std::endl;
      std::cout << "width: " << aBoxWidth << " length: " << aBoxLength << " height: " << aBoxHeight << std::endl;
  }

  void closeAllDocument() {     
      // Close all documents
      for (auto a = 0; a < myApp->NbDocuments(); a++) {
          Handle(TDocStd_Document) aDoc;
          myApp->GetDocument(a, aDoc);
		  myApp->Close(aDoc);
	  }
  }

public:

  //! Handle subview focus change.
  virtual void OnSubviewChanged(const Handle(AIS_InteractiveContext)&,
                                const Handle(V3d_View)&,
                                const Handle(V3d_View)& theNewView) override;

protected: // OpenGL events

  virtual void initializeGL() override;
  virtual void paintGL() override;
  virtual void resizeGL(int width, int height) override {
    if ( !myView.IsNull() )
	  myView->MustBeResized();
  }

protected: // user input events

  virtual void closeEvent       (QCloseEvent*  theEvent) override;
  virtual void keyPressEvent    (QKeyEvent*    theEvent) override;
  virtual void mousePressEvent  (QMouseEvent*  theEvent) override;
  virtual void mouseReleaseEvent(QMouseEvent*  theEvent) override;
  virtual void mouseMoveEvent   (QMouseEvent*  theEvent) override;
  virtual void wheelEvent       (QWheelEvent*  theEvent) override;

private:

  //! Dump OpenGL info.
  void dumpGlInfo (bool theIsBasic, bool theToPrint);

  //! Request widget paintGL() event.
  void updateView();

  //! Handle view redraw.
  virtual void handleViewRedraw (const Handle(AIS_InteractiveContext)& theCtx,
                                 const Handle(V3d_View)& theView) override;

private:
  Handle(TOcaf_Application)         myApp;
  Handle(TDocStd_Document)      myOcafDoc;
  Handle(V3d_Viewer)             myViewer;
  Handle(V3d_View)               myView;
  Handle(AIS_InteractiveContext) myContext;
  Handle(AIS_InteractiveObject) myViewSphere;

  Handle(V3d_View)               myFocusView;

  QString myGlInfo;
  bool myIsCoreProfile;
  
  EditMode myCurrentMode;
};


} // end namespace echocad


#endif // _OcctQtViewer_HeaderFile

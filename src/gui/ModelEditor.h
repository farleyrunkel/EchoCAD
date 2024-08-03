
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
        None = -1,
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

  void closeAllDocument() {     
      // Close all documents
      for (auto a = 0; a < myApp->NbDocuments(); a++) {
          Handle(TDocStd_Document) aDoc;
          myApp->GetDocument(a, aDoc);
		  myApp->Close(aDoc);
	  }
  }

signals:
   void documentOpened(const Handle(TDocStd_Document)& theDoc);

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

  void showManipulator(const gp_Ax2& thePosition) {
	  if (myCurrentMode == Select) {
          ;
		  myContext->Display(myManipulator, true);
	  }
  }


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

  Handle(Manipulator) myManipulator;

  QPoint m_lastPos;
};


} // end namespace echocad


#endif // _OcctQtViewer_HeaderFile

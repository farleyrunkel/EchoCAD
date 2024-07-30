#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QCalendarWidget>
#include <QScrollArea>
#include <QDesktopServices>
#include <QRegExp>
#include <QTextBrowser>
#include <QGraphicsDropShadowEffect>
#include <QToolButton>
#include <QFileDialog>

#include <BRepPrimAPI_MakeSphere.hxx>
#include <AIS_Shape.hxx>
#include <AIS_ViewCube.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakeWedge.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <TopoDS.hxx>
#include <TPrsStd_AISPresentation.hxx>

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/eval.h>

#include "ioverlaywidget.h"
#include "stylemanager.h"
#include "config.h"
#include "IJupyterWidget.h"

namespace py = pybind11;

using namespace echocad;

MainWindow::MainWindow(QWidget *theParent)
    : QMainWindow(theParent),
    mViewer(nullptr),
    mGptProcessor(nullptr),
    mSplitter(nullptr),
    mLineEdit(nullptr),
    mEditor(nullptr),
    mPythonInterpreter(nullptr),
    myTreeWidget(nullptr)
{
    setupMainUi(new QSplitter);
    setupMenuBar(new QMenuBar);

    setupPythonEditor(new QWidget);
    setupOcctViewer(new ModelEditor);

    setGPTProcessor(new GptProcessor);
    setPythonInterpreter(new PythonInterpreter);

    setConnects();

    setParameters();
}

void MainWindow::setParameters() {
    mPythonInterpreter->importDLL({ OCCT_DLL_DIR, QT_DLL_DIR, OPENGL_DLL_DIR, ECHOCAD_BIN_DIR });
    mPythonInterpreter->loadModule( PYECHOCAD_PYD_DIR );
    mPythonInterpreter->setSysVariable("viewer", py::cast(viewer()));

    mSplitter->widget(0)->setVisible(true);
}

void MainWindow::setPythonInterpreter(PythonInterpreter* thePython) {
    mPythonInterpreter = thePython;
}

void MainWindow::setGPTProcessor(GptProcessor*  theGpt) {
    mGptProcessor = theGpt;
}

void MainWindow::setConnects() {
    connect(mGptProcessor, &GptProcessor::predictionReady, this, &MainWindow::onPredictionReady);
    connect(mPythonInterpreter, &PythonInterpreter::logMessage, [this](const QString& message) {
		qDebug() << "Python log: " << message;
        mTextBrowser->append(message);
	});

    connect(mPythonInterpreter, &PythonInterpreter::executed, [this](const QString& result) {
        if (result.isEmpty()) {
            mEditor->addItem(new IJupyterItem(""));
            return;
        }
        auto item = new IJupyterItem(result);
        item->setCodeStyleSheets();
        mEditor->addItem(item);
        mEditor->addItem(new IJupyterItem(""));
	});
}

void MainWindow::setupMainUi(QSplitter* splitter) {
    resize(1260, 800);

    setCentralWidget(splitter);

    mSplitter = splitter;
    mSplitter->setHandleWidth(0);

    StyleManager::instance().loadStyleSheet("://styles/style.qss");
    // StyleManager::instance().enableBorders(true);
    StyleManager::instance().applyStyleSheet(this);
    StyleManager::instance().applyPalette(this);
}
void MainWindow::setupPythonEditor(QWidget* theEditor)
{
    mSplitter->addWidget(theEditor);
    auto mEditorWidget = theEditor;
    mEditorWidget->setObjectName("EditorWidget");

    auto aLayout = new QVBoxLayout(mEditorWidget);
    aLayout->setContentsMargins(0, 0, 0, 0);
    aLayout->setSpacing(2);

    {
        QWidget* aButtonsBox = new QWidget();

        aLayout->addWidget(aButtonsBox);
        QHBoxLayout* aButtonsLayout = new QHBoxLayout(aButtonsBox);
        {
            // add button
            mSplitterButtons[0] = new QPushButton("", aButtonsBox);

            mSplitterButtons[0]->setIcon(QIcon("://icons/sidebar-left.svg"));
            mSplitterButtons[0]->setObjectName("RoundedButton");
            aButtonsLayout->addWidget(mSplitterButtons[0]);

            // add button
            QObject::connect(mSplitterButtons[0], &QPushButton::clicked, [this]()
                {
                    mSplitter->widget(0)->setVisible(!mSplitter->widget(0)->isVisible());
                    mSplitterButtons[1]->setHidden(false);
                });
        }
        {
            // add stretch
            aButtonsLayout->addStretch();
        }
        {
            // add button
            QPushButton* aButton = new QPushButton("", aButtonsBox);
            aButton->setIcon(QIcon("://icons/play.svg"));
            aButton->setObjectName("RoundedButton");

            aButtonsLayout->addWidget(aButton);
            connect(aButton, &QPushButton::clicked, [this]()
                {
                    mPythonInterpreter->executeScript(mEditor->activeText());
                });
        }
        {
            // add button
            QPushButton* aButton = new QPushButton("", aButtonsBox);
            aButton->setIcon(QIcon("://icons/plus.svg"));
            aButton->setObjectName("RoundedButton");
            aButtonsLayout->addWidget(aButton);
            connect(aButton, &QPushButton::clicked, [this]()
                {
                    mEditor->addItem(new IJupyterItem());
                });
        }
        {
            // add button
            QPushButton* aButton = new QPushButton("", aButtonsBox);
            aButton->setIcon(QIcon("://icons/arrow-rotate-right.svg"));
            aButton->setObjectName("RoundedButton");

            aButtonsLayout->addWidget(aButton);
            connect(aButton, &QPushButton::clicked, [this]()
                {
                    mEditor->clearItems();
                });

        }
    }
    {
        // set a line
        QFrame* aLine = new QFrame();
        aLine->setFrameShape(QFrame::HLine);
        aLine->setFixedHeight(1);
        aLine->setFrameShadow(QFrame::Sunken);
        aLine->setStyleSheet("background-color: #30363d;");
        aLayout->addWidget(aLine);
    }
    {
        // set editor area
		mEditor = new IJupyterWidget(mEditorWidget);
        mEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        aLayout->addWidget(mEditor);
    }
}


void MainWindow::setupOcctViewer(ModelEditor* theViewer)
{
    mSplitter->addWidget(theViewer);

    mViewer = theViewer;
    mViewer->setObjectName("OcctViewer");

    // 3D Viewer and some controls on top of it
    QVBoxLayout* aLayout = new QVBoxLayout(mViewer);
    aLayout->setContentsMargins(0, 0, 0, 20);
    aLayout->setSpacing(0);
    aLayout->setAlignment(Qt::AlignTop);
    {
        QWidget* aButtonsBox = new QWidget();


        QHBoxLayout* aButtonsLayout = new QHBoxLayout(aButtonsBox);
        aButtonsLayout->setAlignment(Qt::AlignLeft);
        {
            // add button
            mSplitterButtons[1] = new QPushButton("", aButtonsBox);
            mSplitterButtons[1]->setIcon(QIcon("://icons/sidebar-left.svg"));
            // Set object name for rounded button
            mSplitterButtons[1]->setObjectName("RoundedButton");
            aButtonsLayout->addWidget(mSplitterButtons[1]);
            aLayout->addWidget(aButtonsBox);
            // add button
            QObject::connect(mSplitterButtons[1], &QPushButton::clicked, [this]()
                {
                    mSplitter->widget(0)->setVisible(!mSplitter->widget(0)->isVisible());
                    mSplitterButtons[1]->setHidden(true);
                });
            // connect mSpltterButton1 and widget 0 when width of widget 0 is zero set button show
            QObject::connect(mSplitter, &QSplitter::splitterMoved, this, [this] {
                if (0 == mSplitter->widget(0)->width()) {
                    mSplitterButtons[1]->setHidden(false);
                }
                ; });
        }
        {
            // add spacer
            QWidget* aSpacer = new QWidget();
            aSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            aSpacer->setFixedWidth(10);
            aButtonsLayout->addWidget(aSpacer);
        }
        {
            // add selete mode point
            QToolButton* aButton = new QToolButton(aButtonsBox);
            aButton->setIcon(QIcon("://icons/select_point.svg"));
            aButton->setObjectName("RoundedButton");
            aButton->setToolTip("Select Mode: Vertex");
            aButton->setAutoRaise(true);
            aButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
            aButton->setCheckable(true);
            aButtonsLayout->addWidget(aButton);
            connect(aButton, &QToolButton::clicked, this, [this, aButton]() {
                if (aButton->isChecked()) {
                    mViewer->Context()->Activate(AIS_Shape::SelectionMode(TopAbs_VERTEX));
                }
                else {
                    mViewer->Context()->Deactivate(AIS_Shape::SelectionMode(TopAbs_VERTEX));
                }
                ; });
        }
        {
			// add selete mode edge
            QToolButton* aButton = new QToolButton(aButtonsBox);
            aButton->setIcon(QIcon("://icons/select_edge.svg"));
            aButton->setObjectName("RoundedButton");
            aButton->setToolTip("Select Mode: Edge");
            aButton->setAutoRaise(true);
            aButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
            aButton->setCheckable(true);
            aButtonsLayout->addWidget(aButton);
            connect(aButton, &QToolButton::clicked, this, [this, aButton]() {
                if (aButton->isChecked()) {
                    mViewer->Context()->Activate(AIS_Shape::SelectionMode(TopAbs_EDGE));
                }
                else {
                    mViewer->Context()->Deactivate(AIS_Shape::SelectionMode(TopAbs_EDGE));
                }
                ; });
		}
		{
			// add selete mode face
            QToolButton* aButton = new QToolButton( aButtonsBox);
			aButton->setIcon(QIcon("://icons/select_face.svg"));
			aButton->setObjectName("RoundedButton");
            aButton->setToolTip("Select Mode: Face");
            aButton->setAutoRaise(true);
            aButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
            aButton->setCheckable(true);
			aButtonsLayout->addWidget(aButton);
            connect(aButton, &QToolButton::clicked, this, [this, aButton]() {       
                if (aButton->isChecked()) {
                    mViewer->Context()->Activate(AIS_Shape::SelectionMode(TopAbs_FACE));
                }
                else {
                    mViewer->Context()->Deactivate(AIS_Shape::SelectionMode(TopAbs_FACE));
                }
                ;});
		}
        {
            // add spacer
            QWidget* aSpacer = new QWidget();
            aSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            aSpacer->setFixedWidth(10);
            aButtonsLayout->addWidget(aSpacer);
        }
        {
            // add topods shape box button
            QPushButton* aButton = new QPushButton("", aButtonsBox);
            aButton->setIcon(QIcon("://icons/box.svg"));
            aButton->setObjectName("RoundedButton");
            aButtonsLayout->addWidget(aButton);
            connect(aButton, &QPushButton::clicked, [this]()
				{
                    auto box = BRepPrimAPI_MakeBox(100, 100, 100).Shape();
					mViewer->Context()->Display(new AIS_Shape(box), Standard_True, Standard_False, Standard_True, Standard_True);
                   // update and show
                    mViewer->View()->Update();             
				});
        }

        {
            // add topods shape sphere button
            QPushButton* aButton = new QPushButton("", aButtonsBox);
            aButton->setIcon(QIcon("://icons/sphere.svg"));
            aButton->setObjectName("RoundedButton");
            aButtonsLayout->addWidget(aButton);
            connect(aButton, &QPushButton::clicked, [this]()
                {
                    auto sphere = BRepPrimAPI_MakeSphere(100).Shape();
                    mViewer->Context()->Display(new AIS_Shape(sphere), Standard_True);
                    // update and show
                    mViewer->View()->Update();
                    ; });
        }
        {
            // add cylinder shape
            QPushButton* aButton = new QPushButton("", aButtonsBox);
            aButton->setIcon(QIcon("://icons/cylinder.svg"));
            aButton->setObjectName("RoundedButton");
            aButtonsLayout->addWidget(aButton);
            connect(aButton, &QPushButton::clicked, [this]()
                {
                    auto cylinder = BRepPrimAPI_MakeCylinder(50, 100).Shape();
                    mViewer->Context()->Display(new AIS_Shape(cylinder), Standard_True);
                    // update and show
                    mViewer->View()->Update();
                    ; });
        }
        {
            // cone
            QPushButton* aButton = new QPushButton("", aButtonsBox);
            aButton->setIcon(QIcon("://icons/cone.svg"));
            aButton->setObjectName("RoundedButton");
            aButtonsLayout->addWidget(aButton);
            connect(aButton, &QPushButton::clicked, [this]()
				{
					auto cone = BRepPrimAPI_MakeCone(50, 0, 100).Shape();
					mViewer->Context()->Display(new AIS_Shape(cone), Standard_True);
					// update and show
					mViewer->View()->Update();
					; });
        
        }
        {
            // torus
            QPushButton* aButton = new QPushButton("", aButtonsBox);
            aButton->setIcon(QIcon("://icons/torus.svg"));
            aButton->setObjectName("RoundedButton");
            aButtonsLayout->addWidget(aButton);
            connect(aButton, &QPushButton::clicked, [this]()
                {
                    auto torus = BRepPrimAPI_MakeTorus(50, 20).Shape();
                    mViewer->Context()->Display(new AIS_Shape(torus), Standard_True);
                    // update and show
                    mViewer->View()->Update();
                    ; });
        }
		{
			// wedge
			QPushButton* aButton = new QPushButton("", aButtonsBox);
			aButton->setIcon(QIcon("://icons/wedge.svg"));
			aButton->setObjectName("RoundedButton");
			aButtonsLayout->addWidget(aButton);
			connect(aButton, &QPushButton::clicked, [this]()
				{
					auto wedge = BRepPrimAPI_MakeWedge(100, 100, 100, 50).Shape();
                    mViewer->Context()->Display(new AIS_Shape(wedge), Standard_True);
					// update and show
					mViewer->View()->Update();
					; });
		}
        {
            // add spacer
            QWidget* aSpacer = new QWidget();
            aSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            aSpacer->setFixedWidth(10);
            aButtonsLayout->addWidget(aSpacer);
        }
        {
            // extrude button
            QPushButton* aButton = new QPushButton("", aButtonsBox);
            aButton->setIcon(QIcon("://icons/extrude.svg"));
            aButton->setObjectName("RoundedButton");
            aButtonsLayout->addWidget(aButton);
            connect(aButton, &QPushButton::clicked, [this]()
                {
                    //               for (mViewer->Context()->InitSelected(); mViewer->Context()->MoreSelected(); mViewer->Context()->NextSelected())
                                   //{     
                    //                   TopoDS_Shape anShape = mViewer->Context()->SelectedShape();
                    //                   if (anShape.IsNull())
                                   //	{
                                   //		QMessageBox::warning(this, "Warning", "Please select a shape first!");
                                   //		return;
                                   //	}
                    //                   gp_Trsf agp_Trsf =  gp_Trsf();
                    //                   agp_Trsf.SetTranslation(gp_Vec(0, 0, 100));
                    //                   anShape.Move(TopLoc_Location(agp_Trsf));

                    //                   auto newAisShaple = new AIS_Shape(anShape);
                    //                   mViewer->Context()->Display(newAisShaple, Standard_True);
                                   //};

                    mViewer->setEditMode(ModelEditor::EditMode::Select);

                    mViewer->showManipulator();
                });
        }
        {
            // add spacer
            QWidget* aSpacer = new QWidget();
            aSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            aButtonsLayout->addWidget(aSpacer);
        }
        {
            // add expand button
            QPushButton* aButton = new QPushButton("", aButtonsBox);
            aButton->setIcon(QIcon("://icons/expand.svg"));
            aButton->setObjectName("RoundedButton");
            aButtonsLayout->addWidget(aButton);
            connect(aButton, &QPushButton::clicked, [this]()
                {
                    if (mViewer->Context()->NbSelected() != 0) {
                        mViewer->Context()->FitSelected(mViewer->View());
                    }
                    else {
                        mViewer->View()->FitAll();
                        mViewer->View()->Redraw();
                    }
                });
        }
        {


        }
        {
            QSlider* aSlider = new QSlider(Qt::Horizontal);
            aSlider->setObjectName("backgroundSlider");

            aSlider->setValue(0);
            aSlider->setRange(0, 255);

            aSlider->setSingleStep(1);
            aSlider->setPageStep(15);
            aSlider->setTickInterval(15);
            aSlider->setTickPosition(QSlider::TicksRight);
            aSlider->setFixedWidth(200);

            // Set initial value
            Quantity_Color aColor1 = mViewer->View()->BackgroundColor();

            aButtonsLayout->addWidget(aSlider);

            connect(aSlider, &QSlider::valueChanged, [this, aSlider](int theValue)
                {
                    const float aVal = theValue / (float)aSlider->maximum();
                    const Quantity_Color aColor(aVal, aVal, aVal, Quantity_TOC_sRGB);
                    qDebug() << "Slider value:" << theValue;
                    qDebug() << "Color value:" << aVal;

                    for (const Handle(V3d_View)& aSubviewIter : mViewer->View()->Subviews())
                    {
                        aSubviewIter->SetBackgroundColor(aColor);
                        aSubviewIter->Invalidate();
                    }
                    //myViewer->View()->SetBackgroundColor (aColor);
                    mViewer->View()->SetBackgroundColor(aColor);
                    mViewer->View()->Invalidate();
                    mViewer->update();
                });

            aSlider->setValue(aColor1.Red() * aSlider->maximum());
        }
    }

    {
        // Set up the bottom area of the chats page
        QFrame* aFrame = new QFrame;
		aLayout->addWidget(aFrame);
        aFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        aFrame->setFixedWidth(240);
        aFrame->setStyleSheet("border: 1px; border-radius: 10px; background-color: rgba(40, 40, 40, 0.5);");

        auto aFrameLayout = new QVBoxLayout(aFrame);
        aFrameLayout->setContentsMargins(0, 0, 0, 0);
        aFrameLayout->setSpacing(0);

        auto aSplitter = new QSplitter(aFrame);
        aFrameLayout->addWidget(aSplitter);
        aSplitter->setOrientation(Qt::Vertical);
        aSplitter->setContentsMargins(0, 0, 0, 0);
        aSplitter->setChildrenCollapsible(false);
        {
            myTreeWidget = new TreeWidget(aFrame);
            myTreeWidget->setObjectName("TreeWidget");
            myTreeWidget->setHeaderHidden(true);
            aSplitter->addWidget(myTreeWidget);

            connect(mViewer, &ModelEditor::documentOpened, [this](const Handle(TDocStd_Document)& theDoc)
				{
					myTreeWidget->addDocument(theDoc);
				});
        }
        {
            auto aWidget = new QWidget(aFrame);
            auto aLayout = new QVBoxLayout(aWidget);
            aSplitter->addWidget(aWidget);

            aLayout->setContentsMargins(0, 0, 0, 0);
            // Add a text browser
            mTextBrowser = new QTextBrowser(aWidget);
            mTextBrowser->setObjectName("TextBrowser");
            mTextBrowser->setFrameShape(QFrame::NoFrame);
            mTextBrowser->setStyleSheet("background-color: transparent;");
            aLayout->addWidget(mTextBrowser);

            // Set up the bottom area of the chats page
            mLineEdit = new ILineEdit(this);
            mLineEdit->setPlaceholderText("Describe what you want to create ...");
            mLineEdit->setFixedHeight(40);
            mLineEdit->rightButton()->setIcon(QIcon("://icons/send.svg"));

            aLayout->addWidget(mLineEdit);
        	connect(mLineEdit->rightButton(), &QPushButton::clicked, mLineEdit, &ILineEdit::returnPressed);
            connect(mLineEdit, &ILineEdit::returnPressed, this, &MainWindow::onExecuteButtonClicked);
        }
    }
}

void MainWindow::setupMenuBar(QMenuBar* theMenuBar)
{
    // menu bar with Quit item
    QMenuBar* aMenuBar = theMenuBar;
    setMenuBar(aMenuBar);

    QMenu* aMenuFile = aMenuBar->addMenu("&File");
    {
        QAction* anActionSplit = new QAction(aMenuFile);
        anActionSplit->setText("Split Views");
        aMenuFile->addAction(anActionSplit);
        connect(anActionSplit, &QAction::triggered, [this]()
            {
                if (!mViewer->View()->Subviews().IsEmpty())
                {
                    // remove subviews
                    mViewer->View()->View()->SetSubviewComposer(false);
                    NCollection_Sequence<Handle(V3d_View)> aSubviews = mViewer->View()->Subviews();
                    for (const Handle(V3d_View)& aSubviewIter : aSubviews)
                    {
                        aSubviewIter->Remove();
                    }
                    mViewer->OnSubviewChanged(mViewer->Context(), nullptr, mViewer->View());
                }
                else
                {
                    // create two subviews splitting window horizontally
                    mViewer->View()->View()->SetSubviewComposer(true);

                    Handle(V3d_View) aSubView1 = new V3d_View(mViewer->Viewer());
                    aSubView1->SetImmediateUpdate(false);
                    aSubView1->SetWindow(mViewer->View(), Graphic3d_Vec2d(0.5, 1.0),
                        Aspect_TOTP_LEFT_UPPER, Graphic3d_Vec2d(0.0, 0.0));

                    Handle(V3d_View) aSubView2 = new V3d_View(mViewer->Viewer());
                    aSubView2->SetImmediateUpdate(false);
                    aSubView2->SetWindow(mViewer->View(), Graphic3d_Vec2d(0.5, 1.0),
                        Aspect_TOTP_LEFT_UPPER, Graphic3d_Vec2d(0.5, 0.0));

                    mViewer->OnSubviewChanged(mViewer->Context(), nullptr, aSubView1);
                }
                mViewer->View()->Invalidate();
                mViewer->update();
            });
    }
    {
        QAction* anActionQuit = new QAction(aMenuFile);
        anActionQuit->setText("Quit");
        aMenuFile->addAction(anActionQuit);
        connect(anActionQuit, &QAction::triggered, [this]()
            {
                close();
            });
    } 
    {
        QAction* anActionOpen = new QAction(aMenuFile);
        anActionOpen->setText("Open");
        aMenuFile->addAction(anActionOpen);
        connect(anActionOpen, &QAction::triggered, [this]()
            {
                QString aFileName = QFileDialog::getOpenFileName(this, "Open File", QString(), "All Files (*.*)");
                if (!aFileName.isEmpty())
                {
                    mViewer->open(aFileName.toStdWString().c_str());
                }
            });
    }
    {
        QAction* anActionClose = new QAction(aMenuFile);
        anActionClose->setText("Close");
        aMenuFile->addAction(anActionClose);
        connect(anActionClose, &QAction::triggered, [this]()
            {
                mViewer->closeAllDocument();
                mViewer->View()->Update();
            });
    }

    QMenu* aMenuAbout = aMenuBar->addMenu("About");
    {  
        {
            QAction* anActionGoto = new QAction(aMenuFile);
            anActionGoto->setText("Goto EchoCAD");
            aMenuAbout->addAction(anActionGoto);
            connect(anActionGoto, &QAction::triggered, [this]()
                {
                    // Goto the office website
                    QDesktopServices::openUrl(QUrl("https://github.com/farleyrunkel/EchoCAD"));
                });
        }
        {
            QAction* anActionAbout = new QAction(aMenuFile);
            anActionAbout->setText("About");
            aMenuAbout->addAction(anActionAbout);
            connect(anActionAbout, &QAction::triggered, [this]()
                {
                    QMessageBox::information(0, "About Sample", QString()
                        + "OCCT 3D Viewer sample embedded into Qt Widgets.\n\n"
                        + "Open CASCADE Technology v." OCC_VERSION_STRING_EXT "\n"
                        + "Qt v." QT_VERSION_STR "\n\n"
                        + "OpenGL info:\n"
                        + mViewer->getGlInfo());
                });
        }
    }
}


QWidget* MainWindow::createInputLine(QWidget* theParent) {

    QWidget* aWidget = new QWidget(theParent);
    mLineEdit = new ILineEdit(aWidget);
    auto aButton = new QPushButton("Execute", aWidget);

    auto aLayout = new QHBoxLayout(aWidget);

    aLayout->addWidget(mLineEdit);
    aLayout->addWidget(aButton);

    return aWidget;
}


void MainWindow::onExecuteButtonClicked() {
    QString aString = mLineEdit->text();
    mGptProcessor->processInput(aString);
}

void MainWindow::onPredictionReady(const QString &thePrediction) {
    qDebug() << "Prediction:" << thePrediction;
    // Extract Python code from the prediction
    QString aCode = extractPythonCode(thePrediction);

    // Execute the extracted Python code
    executePythonCode(aCode);                                                                                                                  

    // Update UI with the prediction
    mLineEdit->setText(thePrediction);
}


QString MainWindow::extractPythonCode(const QString& theText) {
    // Extract Python code from the GPT response
    // This is a simple example, you may need to handle different formats or multiple code blocks

    qDebug() << "Prediction:" << theText;

    QRegExp aRegex("```(python)?(.*)```");

    if (aRegex.indexIn(theText) != -1) {
        return aRegex.cap(2).trimmed();
    }
    return QString();
}

void MainWindow::executePythonCode(const QString& theCode) {
    try {
        // Convert QString to std::string for pybind11
        std::string aCode = theCode.toStdString();

        // Execute the Python code
        py::exec(aCode);
    }
    catch (const py::error_already_set& e) {
        qDebug() << "Python error: " << e.what();
    }
    catch (const std::exception& e) {
        qDebug() << "Standard exception: " << e.what();
    }
    catch (...) {
        qDebug() << "Unknown error occurred";
    }
}
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QCalendarWidget>
#include <QScrollArea>
#include <QDesktopServices>
#include <QRegExp>
#include <QTextBrowser>

#include <BRepPrimAPI_MakeSphere.hxx>
#include <AIS_Shape.hxx>

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/eval.h>

#include "ioverlaywidget.h"
#include "stylemanager.h"
#include "config.h"

namespace py = pybind11;

MainWindow::MainWindow(QWidget *theParent)
    : QMainWindow(theParent),
    mViewer(nullptr),
    mGptProcessor(nullptr),
    mSplitter(nullptr),
    mLineEdit(nullptr),
    mEditor(nullptr),
    mPythonInterpreter(nullptr)
{
    setupMainUi(new QSplitter);
    setupMenuBar(new QMenuBar);

    setupPythonEditor(new QWidget);
    setupOcctViewer(new IOcctWidget);

    setGPTProcessor(new GptProcessor);
    setPythonInterpreter(new PythonInterpreter);

    setConnects();

    emit mPythonInterpreter->dllNeeded({ OCCT_DLL_DIR, QT_DLL_DIR, OPENGL_DLL_DIR, ECHOCAD_BIN_DIR });
    emit mPythonInterpreter->moduleNeeded( PYECHOCAD_PYD_DIR );
    emit mSplitter->widget(0)->setVisible(false);
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
    aLayout->setSpacing(0);
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
            aButton->setIcon(QIcon("://icons/caret-right.svg"));
            aButton->setObjectName("RoundedButton");

            aButtonsLayout->addWidget(aButton);
            connect(aButton, &QPushButton::clicked, [this]()
                {
                    mPythonInterpreter->executeScript(mEditor->text());
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
                    mEditor->setText(mLineEdit->text());
                });

        }
    }
    {
        // add horizontal line with bottom margin shadow
        QFrame* aLine = new QFrame(mEditorWidget);
        aLine->setFrameShape(QFrame::NoFrame);
        aLine->setFixedHeight(1);
        aLine->setStyleSheet("background-color: #383838;");
        aLayout->addWidget(aLine);
    }
    {
        // set editor area
		mEditor = new QsciScintilla(mEditorWidget);
        mEditor->setObjectName("PythonEditor");

        mEditor->setFrameShape(QFrame::NoFrame);
		aLayout->addWidget(mEditor);

        QFont aFont;
        aFont.setFamily("Consolas"); // Set the font family to Consolas
        aFont.setPointSize(10);

        // Set the lexer to Python
        auto aLexer = new QsciLexerPython();
        aLexer->setDefaultFont(aFont);
        aLexer->setFont(aFont, 1); // comment
        aLexer->setFont(aFont, 3); // singlequotes
        aLexer->setFont(aFont, 4); // doublequotes
        aLexer->setFont(aFont, 6); // triplequotes
        aLexer->setColor(Qt::red, 1); // comment color
        aLexer->setColor(Qt::darkGreen, 5); // keyword color
        aLexer->setColor(Qt::darkBlue, 15); // decorator color
        // aLexer->setColor(Qt::white); // default color

        aLexer->setDefaultPaper(QColor(StyleManager::instance().colorPalette("DarkBackground")));

        mEditor->setLexer(aLexer);

        mEditor->setFont(aFont);
        mEditor->setMarginsFont(aFont);

        // Margin 0 is used for line numbers
        mEditor->setMarginType(0, QsciScintilla::NumberMargin);
        mEditor->setMarginWidth(0, "0000");
        mEditor->setMarginsBackgroundColor(QColor(StyleManager::instance().colorPalette("DarkBackground")));
        mEditor->setMarginsForegroundColor(Qt::white);

        mEditor->setFolding(QsciScintilla::NoFoldStyle, 2);

        // Brace matching
        mEditor->setBraceMatching(QsciScintilla::SloppyBraceMatch);

        // Current line visible with special background color
        mEditor->setCaretLineVisible(true);
        mEditor->setCaretLineBackgroundColor(QColor(StyleManager::instance().colorPalette("DarkBackground")));

        // set pointer color white
        mEditor->setCaretForegroundColor(Qt::white);

    }
}


void MainWindow::setupOcctViewer(IOcctWidget* theViewer)
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
            aSpacer->setFixedWidth(20);
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
					mViewer->View()->FitAll();
					mViewer->View()->Redraw();
				});
        }
        {
            // add spacer
            QWidget* aSpacer = new QWidget();
            aSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            aButtonsLayout->addWidget(aSpacer);
        }
        {
            QSlider* aSlider = new QSlider(Qt::Horizontal);
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
        aFrame->setStyleSheet("border: none; border-radius: 10px; background-color: rgba(40, 40, 40, 0.5);");

        auto aFrameLayout = new QVBoxLayout(aFrame);
        aFrameLayout->setContentsMargins(0, 0, 0, 0);
        aFrameLayout->setSpacing(0);
        {
            // Add a text browser
            mTextBrowser = new QTextBrowser(aFrame);
            mTextBrowser->setObjectName("TextBrowser");
            mTextBrowser->setOpenExternalLinks(true);
            mTextBrowser->setOpenLinks(true);
            mTextBrowser->setFrameShape(QFrame::NoFrame);
            mTextBrowser->setStyleSheet("background-color: transparent;");
            aFrameLayout->addWidget(mTextBrowser);
        }
        {
            // Set up the bottom area of the chats page
            mLineEdit = new ILineEdit(this);
            mLineEdit->setPlaceholderText("Describe what you want to create ...");
            mLineEdit->setFixedHeight(40);
            mLineEdit->rightButton()->setIcon(QIcon("://icons/send.svg"));

            aFrameLayout->addWidget(mLineEdit);
        	connect(mLineEdit->rightButton(), &QPushButton::clicked, mLineEdit, &ILineEdit::returnPressed);
            connect(mLineEdit, &ILineEdit::returnPressed, this, &MainWindow::onExecuteButtonClicked);
        }

    }
    {
        // Create a sphere shape
        TopoDS_Shape aSphere = BRepPrimAPI_MakeSphere(gp_Pnt(0, 0, 0), 100.0).Shape();
        Handle(AIS_Shape) aShape = new AIS_Shape(aSphere);

        // Display the sphere in the viewer
        mViewer->Context()->Display(aShape, AIS_Shaded, 0, false);
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
    QRegExp aRegex("```(python)?([\\s\\S]+?)```");
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
#include "MainWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QCalendarWidget>
#include <QScrollArea>
#include <QDesktopServices>

#include <BRepPrimAPI_MakeSphere.hxx>
#include <AIS_Shape.hxx>

#include "ioverlaywidget.h"
#include "stylemanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    mViewer(nullptr),
    gptProcessor(new GPTProcessor(this)),
    executeButton(nullptr)
{
    setupMainUi();

    setupMenuBar();
    setupOcctViewer();

    connect(gptProcessor, &GPTProcessor::predictionReady, this, &MainWindow::onPredictionReady);
}

void MainWindow::setupMainUi() {
    resize(1260, 800);

    mViewer = new IOcctWidget();
    setCentralWidget(mViewer);

    StyleManager::instance().loadStyleSheet("://resources/styles/style.qss");

    // StyleManager::instance().enableBorders(true);

    StyleManager::instance().applyStyleSheet(this);
    StyleManager::instance().applyPalette(this);

}

void MainWindow::setupOcctViewer()
{
    // 3D Viewer and some controls on top of it
    QVBoxLayout* aLayout = new QVBoxLayout(mViewer);
    aLayout->setDirection(QBoxLayout::BottomToTop);
    aLayout->setAlignment(Qt::AlignBottom);
	{
        // Set up the bottom area of the chats page
        mInput = new ILineEdit(this);
        mInput->setPlaceholderText("Describe what you want to create ...");
        mInput->setFixedHeight(40);
        mInput->rightButton()->setIcon(QIcon("://resources/icons/send.svg"));

		aLayout->addWidget(mInput);
		connect(mInput->rightButton(), &QPushButton::clicked, mInput, &ILineEdit::returnPressed);
        connect(mInput, &ILineEdit::returnPressed, this, &MainWindow::onExecuteButtonClicked);
	}
    {
        QWidget* aSliderBox = new QWidget();
        QHBoxLayout* aSliderLayout = new QHBoxLayout(aSliderBox);
        aSliderLayout->setAlignment(Qt::AlignRight);
        {
            QSlider* aSlider = new QSlider(Qt::Vertical);
            aSlider->setValue(0);
            aSlider->setRange(0, 255);
            aSlider->setSingleStep(1);
            aSlider->setPageStep(15);
            aSlider->setTickInterval(15);
            aSlider->setTickPosition(QSlider::TicksRight);

            // Set initial value
			Quantity_Color aColor1(Quantity_NOC_BLACK);
            Quantity_Color aColor2(Quantity_NOC_BLACK);
            mViewer->View()->GradientBackground().Colors(aColor1, aColor2);

            aSliderLayout->addWidget(aSlider);

            connect(aSlider, &QSlider::valueChanged, [this, aSlider](int theValue)
                {
                    const float aVal = theValue / (float)aSlider->maximum();
                    const Quantity_Color aColor(aVal, aVal, aVal, Quantity_TOC_sRGB);
                    qDebug() << "Slider value:" << theValue;
                    qDebug() << "Color value:" << aVal;

                    for (const Handle(V3d_View)& aSubviewIter : mViewer->View()->Subviews())
                    {
                        aSubviewIter->SetBgGradientColors(aColor, Quantity_NOC_BLACK, Aspect_GradientFillMethod_Elliptical);
                        aSubviewIter->Invalidate();
                    }
                    //myViewer->View()->SetBackgroundColor (aColor);
                    mViewer->View()->SetBgGradientColors(aColor, Quantity_NOC_BLACK, Aspect_GradientFillMethod_Elliptical);
                    mViewer->View()->Invalidate();
                    mViewer->update();
                });

            aSlider->setValue(aColor1.Red() * aSlider->maximum());
        }
        aLayout->addWidget(aSliderBox);
    }
    {
        // Create a sphere shape
        TopoDS_Shape aSphere = BRepPrimAPI_MakeSphere(gp_Pnt(0, 0, 0), 100.0).Shape();
        Handle(AIS_Shape) aShape = new AIS_Shape(aSphere);

        // Display the sphere in the viewer
        mViewer->Context()->Display(aShape, AIS_Shaded, 0, false);
    }
}

void MainWindow::setupMenuBar() 
{
    // menu bar with Quit item
    QMenuBar* aMenuBar = new QMenuBar();
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
    setMenuBar(aMenuBar);
}


QWidget* MainWindow::createInputLine(QWidget* parent) {

    QWidget* widget = new QWidget(parent);
    mInput = new ILineEdit(widget);
    executeButton = new QPushButton("Execute", widget);

    auto inputLayout = new QHBoxLayout(widget);

    inputLayout->addWidget(mInput);
    inputLayout->addWidget(executeButton);

    return widget;
}


void MainWindow::onExecuteButtonClicked() {
    QString userInput = mInput->text();
    gptProcessor->processInput(userInput);
}

void MainWindow::onPredictionReady(const QString &prediction) {
    qDebug() << "Prediction:" << prediction;
    // 这里可以处理预测结果，比如更新UI等
    mInput->setText(prediction);
}

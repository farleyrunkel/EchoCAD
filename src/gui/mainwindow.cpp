#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QCalendarWidget>
#include <QScrollArea>
#include "ioverlaywidget.h"
#include <BRepPrimAPI_MakeSphere.hxx>
#include <AIS_Shape.hxx>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    mViewer(nullptr),
    gptProcessor(new GPTProcessor(this)),
    executeButton(nullptr)
{
    setupMainUi();

    setupMenuBar();
    setupOcctViewer();
}

void MainWindow::setupMainUi() {
    resize(1260, 800);
    mViewer = new IOcctWidget();
    setCentralWidget(mViewer);
}

void MainWindow::setupOcctViewer()
{
    // 3D Viewer and some controls on top of it
    QVBoxLayout* aLayout = new QVBoxLayout(mViewer);
    aLayout->setDirection(QBoxLayout::BottomToTop);
    aLayout->setAlignment(Qt::AlignBottom);
	{
		QWidget* aInputLine = createInputLine(mViewer);
		aLayout->addWidget(aInputLine);
		connect(executeButton, &QPushButton::clicked, this, &MainWindow::onExecuteButtonClicked);
		connect(gptProcessor, &GPTProcessor::predictionReady, this, &MainWindow::onPredictionReady);
	}
    {
        QPushButton* aQuitBtn = new QPushButton("About");
        aLayout->addWidget(aQuitBtn);
        connect(aQuitBtn, &QPushButton::clicked, [this]()
            {
                QMessageBox::information(0, "About Sample", QString()
                    + "OCCT 3D Viewer sample embedded into Qt Widgets.\n\n"
                    + "Open CASCADE Technology v." OCC_VERSION_STRING_EXT "\n"
                    + "Qt v." QT_VERSION_STR "\n\n"
                    + "OpenGL info:\n"
                    + mViewer->getGlInfo());
            });
    }
    {
        QWidget* aSliderBox = new QWidget();
        QHBoxLayout* aSliderLayout = new QHBoxLayout(aSliderBox);
        {
            QLabel* aSliderLabel = new QLabel("Background");
            aSliderLabel->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: white; }");
            aSliderLabel->setGeometry(50, 50, 50, 50);
            aSliderLabel->adjustSize();
            aSliderLayout->addWidget(aSliderLabel);
        }
        {
            QSlider* aSlider = new QSlider(Qt::Horizontal);
            aSlider->setRange(0, 255);
            aSlider->setSingleStep(1);
            aSlider->setPageStep(15);
            aSlider->setTickInterval(15);
            aSlider->setTickPosition(QSlider::TicksRight);
            aSlider->setValue(0);
            aSliderLayout->addWidget(aSlider);
            connect(aSlider, &QSlider::valueChanged, [this](int theValue)
                {
                    const float aVal = theValue / 255.0f;
                    const Quantity_Color aColor(aVal, aVal, aVal, Quantity_TOC_sRGB);

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
    QMenu* aMenuWindow = aMenuBar->addMenu("&File");
    {
        QAction* anActionSplit = new QAction(aMenuWindow);
        anActionSplit->setText("Split Views");
        aMenuWindow->addAction(anActionSplit);
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
        QAction* anActionQuit = new QAction(aMenuWindow);
        anActionQuit->setText("Quit");
        aMenuWindow->addAction(anActionQuit);
        connect(anActionQuit, &QAction::triggered, [this]()
            {
                close();
            });
    }
    setMenuBar(aMenuBar);
}


QWidget* MainWindow::createInputLine(QWidget* parent) {

    QWidget* widget = new QWidget(parent);
    input = new QLineEdit(widget);
    executeButton = new QPushButton("Execute", widget);

    auto inputLayout = new QHBoxLayout(widget);

    inputLayout->addWidget(input);
    inputLayout->addWidget(executeButton);

    return widget;
}


void MainWindow::onExecuteButtonClicked() {
    QString userInput = input->text();
    gptProcessor->processInput(userInput);
}

void MainWindow::onPredictionReady(const QString &prediction) {
    qDebug() << "Prediction:" << prediction;
    // 这里可以处理预测结果，比如更新UI等
    input->setText(prediction);
}

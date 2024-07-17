#include "IJupyterItem.h"

IJupyterItem::IJupyterItem(QWidget* parent) : QWidget(parent) {

    setMainUi();
    setFixedHeight(30);
    setStyleSheets();

    // clicked

}

void IJupyterItem::setMainUi() {

    myLineEdit = new QLineEdit(this);

    aLayout = new QHBoxLayout(this);

    setLayout(aLayout);

    aLayout->setContentsMargins(2, 2, 2, 2);
    myColorLabel = new QLabel("", this);

    myColorLabel->setFixedWidth(6);

    myTagLabel = new QLabel(" [ ]:", this);

    aLayout->addWidget(myColorLabel);
    aLayout->addWidget(myTagLabel);
    aLayout->addWidget(myLineEdit);


    // setFixedHeight(40);

}

void IJupyterItem::setStyleSheets() {

    this->setStyleSheet(R"(
        IJupyterWidget {
        background-color: #1e1e1e;
        border: 1px;
    }
         IJupyterWidget:pressed {
	    border: 1px solid #238635;
}
    )");

    myColorLabel->setStyleSheet(R"(
			QLabel {
				background-color: #238635;
       border-radius: 2px;
			})");
    myTagLabel->setStyleSheet(R"(
			QLabel {
				color: #238635;
			})");

    myLineEdit->setStyleSheet(R"(
        QLineEdit {
            border: 1px solid #30363d;
            border-radius: 5px;
            background-color: #0d1117;  
            height: 20px;
            color: white;
            font-family: Consolas;
    }
        QLineEdit:focus {
            border: 1px solid #238635;
        }
    )");

}

void IJupyterItem::setPythonLexer() {

    mEditor->setObjectName("PythonEditor");

    mEditor->setFrameShape(QFrame::NoFrame);

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

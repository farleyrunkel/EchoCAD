#include "IJupyterItem.h"

IJupyterItem::IJupyterItem(QWidget* parent) : QWidget(parent)
{

    setMainUi();
    setFixedHeight(30);
    setStyleSheets();

    // clicked

}

void IJupyterItem::setMainUi() {

    setObjectName("myIJupyterItem");

    aLayout = new QHBoxLayout(this);

    setLayout(aLayout);

    aLayout->setContentsMargins(4, 4, 4, 4);
    aLayout->setSpacing(2);
    myColorLabel = new QLabel("", this);

    myColorLabel->setFixedWidth(6);

    myTagLabel = new QLabel(" [ ]:", this);
    myTagLabel->setFixedWidth(26);

    myLineEdit = new ILineEdit;

    aLayout->addWidget(myColorLabel);
    aLayout->addWidget(myTagLabel);
    aLayout->addWidget(myLineEdit);

    aLayout->setAlignment(Qt::AlignLeft);
    aLayout->setDirection(QBoxLayout::LeftToRight);

    connect(myLineEdit, &ILineEdit::focused, [this](bool isFocused) {
        this->setActiveStyleSheets();
        emit itemClicked(this);
      });

}

void IJupyterItem::setActiveStyleSheets() {
    myColorLabel->setStyleSheet(R"(
			QLabel {
				background-color: #238635;
            border-radius: 2px;
			})");
    myTagLabel->setStyleSheet(R"(
			QLabel {
				color: #238635;
			})");
}

void IJupyterItem::setStyleSheets() {
    this->setStyleSheet(R"(
        IJupyterItem {
            border: 1px hidden #30363d;
    }
    )");

    myColorLabel->setStyleSheet(R"(
			QLabel {
				background-color: transparent;
            border-radius: 2px;
			})");
    myTagLabel->setStyleSheet(R"(
			QLabel {
				color: #FFFFF0;
			})");
    myLineEdit->setStyleSheet(R"(
        ILineEdit {
            border: 1px solid #30363d;
            border-radius: 5px;
            height: 20px;
            color: white;
            font-family: Consolas;
			font-size: 10pt;
    }
        ILineEdit:focus {
            border: 1px solid #238635;
			}
)");

}

inline void IJupyterItem::mousePressEvent(QMouseEvent* event) {
    // if point is in this widget
      emit itemClicked(this);
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

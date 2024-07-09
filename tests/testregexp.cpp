#include <QTest>


/**
 * @brief The TestRegExp class is a unit test class for testing regular expression functionality.
 *
 * This class inherits from QObject and contains test cases for various regular expression operations.
 */

class TestRegExp : public QObject {
    Q_OBJECT
private slots:
    void toUpper();
    void toRegularExpression();
    void toRegularExpression2();
};

void TestRegExp::toRegularExpression() {
    QString str = "background-color: transparent; /*@PrimaryColor*/";

    QRegularExpression regex(R"(\s([^\s;]+)\s*;\s*/\*\s*@(\w+)\s*\*/)");
    QRegularExpressionMatchIterator it = regex.globalMatch(str);
    QRegularExpressionMatch match = it.next();

    QCOMPARE(match.hasMatch(), true);
    QCOMPARE(match.captured(1), "transparent");
    QCOMPARE(match.captured(2), "PrimaryColor");
}
void TestRegExp::toRegularExpression2() {
    QString str = "border: 1px solid transparent; /*@DarkBorder*/";

    QRegularExpression regex(R"(\s([^\s;]+)\s*;\s*/\*\s*@(\w+)\s*\*/)");
    QRegularExpressionMatchIterator it = regex.globalMatch(str);
    QRegularExpressionMatch match = it.next();

    QCOMPARE(match.hasMatch(), true);
    QCOMPARE(match.captured(1), "transparent");
    QCOMPARE(match.captured(2), "DarkBorder");
}

void TestRegExp::toUpper() {
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}

QTEST_MAIN(TestRegExp)

#include "testregexp.moc"

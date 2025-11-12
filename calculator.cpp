#include "calculator.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>

Calculator::Calculator(QWidget *parent)
    : QWidget(parent),
    display(nullptr),
    leftOperand(0.0),
    waitingForOperand(true)
{
    createUI();
    setWindowTitle("Calculator");
}

Calculator::~Calculator() {}

void Calculator::createUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    display = new QLineEdit;
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setText("0");
    QFont df = display->font();
    df.setPointSize(20);
    display->setFont(df);
    display->setMinimumHeight(60);
    mainLayout->addWidget(display);

    QGridLayout *gridLayout = new QGridLayout;

    // 第一排：退格, /, *, -
    QPushButton *backButton = createButton("←", SLOT(backspaceClicked()));
    QPushButton *divButton  = createButton("/", SLOT(operatorClicked()));
    QPushButton *mulButton  = createButton("*", SLOT(operatorClicked()));
    QPushButton *subButton  = createButton("-", SLOT(operatorClicked()));

    gridLayout->addWidget(backButton, 0, 0);
    gridLayout->addWidget(divButton,  0, 1);
    gridLayout->addWidget(mulButton,  0, 2);
    gridLayout->addWidget(subButton,  0, 3);

    // 數字 7 8 9
    QPushButton *b7 = createButton("7", SLOT(digitClicked()));
    QPushButton *b8 = createButton("8", SLOT(digitClicked()));
    QPushButton *b9 = createButton("9", SLOT(digitClicked()));
    gridLayout->addWidget(b7, 1, 0);
    gridLayout->addWidget(b8, 1, 1);
    gridLayout->addWidget(b9, 1, 2);

    // 數字 4 5 6
    QPushButton *b4 = createButton("4", SLOT(digitClicked()));
    QPushButton *b5 = createButton("5", SLOT(digitClicked()));
    QPushButton *b6 = createButton("6", SLOT(digitClicked()));
    gridLayout->addWidget(b4, 2, 0);
    gridLayout->addWidget(b5, 2, 1);
    gridLayout->addWidget(b6, 2, 2);

    // 數字 1 2 3
    QPushButton *b1 = createButton("1", SLOT(digitClicked()));
    QPushButton *b2 = createButton("2", SLOT(digitClicked()));
    QPushButton *b3 = createButton("3", SLOT(digitClicked()));
    gridLayout->addWidget(b1, 3, 0);
    gridLayout->addWidget(b2, 3, 1);
    gridLayout->addWidget(b3, 3, 2);

    // 0 (跨兩格) 與 .
    QPushButton *b0    = createButton("0", SLOT(digitClicked()));
    QPushButton *point = createButton(".", SLOT(pointClicked()));
    gridLayout->addWidget(b0,    4, 0, 1, 2);
    gridLayout->addWidget(point, 4, 2);


    QPushButton *addButton = createButton("+", SLOT(operatorClicked()));
    gridLayout->addWidget(addButton,   1, 3, 2, 1);


    QPushButton *equalButton = createButton("=", SLOT(equalClicked()));
    gridLayout->addWidget(equalButton, 3, 3, 2, 1);



    gridLayout->setHorizontalSpacing(8);
    gridLayout->setVerticalSpacing(8);

    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);
}

QPushButton* Calculator::createButton(const QString &text, const char *member)
{
    QPushButton *button = new QPushButton(text);
    button->setMinimumSize(60, 60);
    QFont font = button->font();
    font.setPointSize(18);
    button->setFont(font);

    // 樣式設定
    if ((text >= "0" && text <= "9") || text == ".") {
        // 數字鍵：黑底白字
        button->setStyleSheet(
            "QPushButton { background-color: #000000; color: #FFFFFF; border: 1px solid #333333; border-radius: 6px; }"
            "QPushButton:pressed { background-color: #222222; }"
            );
    } else if (text == "←") {
        button->setStyleSheet(
            "QPushButton { background-color: #FFB74D; color: #000; border: 1px solid #F57C00; border-radius: 6px; }"
            "QPushButton:pressed { background-color: #F57C00; }"
            );
    } else if (text == "=") {
        button->setStyleSheet(
            "QPushButton { background:#4CAF50; color:#FFF; border:1px solid #2E7D32; "
            "border-radius:10px; font-size:65px; padding:20px 10px; }"
            "QPushButton:pressed { background:#2E7D32; }"
            );
    } else if (text == "+") {
        button->setStyleSheet(
            "QPushButton { background:#2196F3; color:#FFF; border:1px solid #1565C0; "
            "border-radius:10px; font-size:65px; padding:20px 10px; }"
            "QPushButton:pressed { background:#1565C0; }"
            );
    } else {
        // 其他運算子：藍系
        button->setStyleSheet(
            "QPushButton { background-color: #64B5F6; color: #000000; border: 1px solid #1E88E5; border-radius: 6px; }"
            "QPushButton:pressed { background-color: #1E88E5; color: #FFFFFF; }"
            );
    }

    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void Calculator::digitClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    QString digitValue = clickedButton->text();

    if (display->text() == "0" || waitingForOperand)
        display->setText(digitValue);
    else
        display->setText(display->text() + digitValue);

    waitingForOperand = false;
}

void Calculator::pointClicked()
{
    if (waitingForOperand) {
        display->setText("0.");
        waitingForOperand = false;
        return;
    }
    if (!display->text().contains('.'))
        display->setText(display->text() + ".");
}

void Calculator::operatorClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    QString op = clickedButton->text();

    double value = display->text().toDouble();
    if (!currentOperator.isEmpty()) {
        // 連續運算處理
        double right = value;
        double result = 0.0;
        if (currentOperator == "+") result = leftOperand + right;
        else if (currentOperator == "-") result = leftOperand - right;
        else if (currentOperator == "*") result = leftOperand * right;
        else if (currentOperator == "/") {
            if (right == 0.0) { display->setText("Error"); currentOperator.clear(); waitingForOperand = true; return; }
            result = leftOperand / right;
        }
        display->setText(QString::number(result));
        leftOperand = result;
    } else {
        leftOperand = value;
    }

    currentOperator = op;
    waitingForOperand = true;
}

void Calculator::equalClicked()
{
    if (currentOperator.isEmpty())
        return;

    double right = display->text().toDouble();
    double result = 0.0;

    if (currentOperator == "+") result = leftOperand + right;
    else if (currentOperator == "-") result = leftOperand - right;
    else if (currentOperator == "*") result = leftOperand * right;
    else if (currentOperator == "/") {
        if (right == 0.0) { display->setText("Error"); currentOperator.clear(); waitingForOperand = true; return; }
        result = leftOperand / right;
    }

    display->setText(QString::number(result));
    currentOperator.clear();
    waitingForOperand = true;
}

void Calculator::clearClicked()
{
    display->setText("0");
    currentOperator.clear();
    leftOperand = 0.0;
    waitingForOperand = true;
}

void Calculator::backspaceClicked()
{
    if (waitingForOperand) {
        // 若剛運算完不處理
        return;
    }
    QString txt = display->text();
    if (txt.length() <= 1) {
        display->setText("0");
        waitingForOperand = true;
    } else {
        txt.chop(1);
        display->setText(txt);
    }
}

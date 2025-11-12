#include "calculator.h"
#include <QVBoxLayout>
#include <QFont>

Calculator::Calculator(QWidget *parent)
    : QWidget(parent), leftOperand(0.0), waitingForOperand(true)
{
    createUI();
    setWindowTitle(tr("Calculator"));
    setFixedSize(300, 400);
}

Calculator::~Calculator()
{
}

void Calculator::createUI()
{
    // Create display
    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);
    QFont font = display->font();
    font.setPointSize(20);
    display->setFont(font);
    display->setStyleSheet("QLineEdit { background-color: white; color: black; border: 2px solid gray; padding: 5px; }");

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(display);

    // Create button grid layout
    QGridLayout *gridLayout = new QGridLayout;
    
    // Create number buttons (0-9)
    for (int i = 1; i <= 9; i++) {
        QPushButton *button = createButton(QString::number(i), SLOT(digitClicked()));
        int row = (9 - i) / 3 + 1;
        int col = (i - 1) % 3;
        gridLayout->addWidget(button, row, col);
    }
    
    // Create 0 button
    QPushButton *zeroButton = createButton("0", SLOT(digitClicked()));
    gridLayout->addWidget(zeroButton, 4, 0, 1, 2);
    
    // Create decimal point button
    QPushButton *pointButton = createButton(".", SLOT(pointClicked()));
    gridLayout->addWidget(pointButton, 4, 2);
    
    // Create operator buttons
    QPushButton *divButton = createButton("/", SLOT(operatorClicked()));
    QPushButton *mulButton = createButton("*", SLOT(operatorClicked()));
    QPushButton *subButton = createButton("-", SLOT(operatorClicked()));
    QPushButton *addButton = createButton("+", SLOT(operatorClicked()));
    
    gridLayout->addWidget(divButton, 0, 3);
    gridLayout->addWidget(mulButton, 1, 3);
    gridLayout->addWidget(subButton, 2, 3);
    gridLayout->addWidget(addButton, 3, 3);
    
    // Create clear and equal buttons
    QPushButton *clearButton = createButton("C", SLOT(clearClicked()));
    QPushButton *equalButton = createButton("=", SLOT(equalClicked()));
    
    gridLayout->addWidget(clearButton, 0, 0, 1, 3);
    gridLayout->addWidget(equalButton, 4, 3);
    
    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);
}

QPushButton* Calculator::createButton(const QString &text, const char *member)
{
    QPushButton *button = new QPushButton(text);
    button->setMinimumSize(60, 60);
    QFont font = button->font();
    font.setPointSize(16);
    button->setFont(font);
    
    // Style buttons
    if ((text >= "0" && text <= "9") || text == ".") {
        button->setStyleSheet("QPushButton { background-color: #f0f0f0; border: 1px solid #ccc; border-radius: 5px; }"
                             "QPushButton:pressed { background-color: #d0d0d0; }");
    } else if (text == "C") {
        button->setStyleSheet("QPushButton { background-color: #ff6b6b; color: white; border: 1px solid #ff5252; border-radius: 5px; }"
                             "QPushButton:pressed { background-color: #ff5252; }");
    } else if (text == "=") {
        button->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: 1px solid #45a049; border-radius: 5px; }"
                             "QPushButton:pressed { background-color: #45a049; }");
    } else {
        button->setStyleSheet("QPushButton { background-color: #2196F3; color: white; border: 1px solid #1976D2; border-radius: 5px; }"
                             "QPushButton:pressed { background-color: #1976D2; }");
    }
    
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void Calculator::digitClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    QString digitValue = clickedButton->text();
    
    if (waitingForOperand) {
        display->setText(digitValue);
        waitingForOperand = false;
    } else {
        if (display->text() == "0") {
            display->setText(digitValue);
        } else {
            display->setText(display->text() + digitValue);
        }
    }
}

void Calculator::pointClicked()
{
    if (waitingForOperand) {
        display->setText("0.");
        waitingForOperand = false;
    } else if (!display->text().contains(".")) {
        display->setText(display->text() + ".");
    }
}

void Calculator::operatorClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();
    
    if (!currentOperator.isEmpty()) {
        // Perform the pending operation
        double result = 0.0;
        if (currentOperator == "+") {
            result = leftOperand + operand;
        } else if (currentOperator == "-") {
            result = leftOperand - operand;
        } else if (currentOperator == "*") {
            result = leftOperand * operand;
        } else if (currentOperator == "/") {
            if (operand != 0.0) {
                result = leftOperand / operand;
            } else {
                display->setText("Error");
                leftOperand = 0.0;
                currentOperator.clear();
                waitingForOperand = true;
                return;
            }
        }
        display->setText(QString::number(result));
        leftOperand = result;
    } else {
        leftOperand = operand;
    }
    
    currentOperator = clickedOperator;
    waitingForOperand = true;
}

void Calculator::equalClicked()
{
    if (currentOperator.isEmpty()) {
        return;
    }
    
    double operand = display->text().toDouble();
    double result = 0.0;
    
    if (currentOperator == "+") {
        result = leftOperand + operand;
    } else if (currentOperator == "-") {
        result = leftOperand - operand;
    } else if (currentOperator == "*") {
        result = leftOperand * operand;
    } else if (currentOperator == "/") {
        if (operand != 0.0) {
            result = leftOperand / operand;
        } else {
            display->setText("Error");
            leftOperand = 0.0;
            currentOperator.clear();
            waitingForOperand = true;
            return;
        }
    }
    
    display->setText(QString::number(result));
    leftOperand = 0.0;
    currentOperator.clear();
    waitingForOperand = true;
}

void Calculator::clearClicked()
{
    display->setText("0");
    leftOperand = 0.0;
    currentOperator.clear();
    waitingForOperand = true;
}

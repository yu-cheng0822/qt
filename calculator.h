#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QString>

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void digitClicked();
    void operatorClicked();
    void equalClicked();
    void clearClicked();
    void pointClicked();
    void backspaceClicked();   // 新增：退格

private:
    void createUI();
    QPushButton* createButton(const QString &text, const char *member);

    QLineEdit *display;
    QString currentOperator;
    double leftOperand;
    bool waitingForOperand;
};

#endif // CALCULATOR_H

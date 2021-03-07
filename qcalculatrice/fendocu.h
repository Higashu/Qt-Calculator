#ifndef FENDOCU_H
#define FENDOCU_H

#include <QDialog>
#include <QHBoxLayout>
#include <QTextEdit>

class FenDocu : public QDialog
{

    Q_OBJECT
    QHBoxLayout *layout;
    QTextEdit *documentation;

public:
    FenDocu(QWidget *parent = 0);
};

#endif // FENDOCU_H

#ifndef BOUTTON_H
#define BOUTTON_H

#include <QToolButton>

class boutton : public QToolButton
{
    Q_OBJECT

public:
    boutton(const QString &texte, QWidget *parent = 0);
    QSize sizeIndic() const;
};

#endif // BOUTTON_H

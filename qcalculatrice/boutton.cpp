#include "boutton.h"

boutton::boutton(const QString &texte, QWidget *parent)
    : QToolButton(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setFont(QFont("Arial", 10, 50, false));
    setText(texte);
}

QSize boutton::sizeIndic() const
{
    QSize size = QToolButton::sizeHint();
    size.rheight() += 20;
    size.rwidth() = qMax(size.width(), size.height());
    return size;
}

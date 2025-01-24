#ifndef ELATIMEEDIT_H
#define ELATIMEEDIT_H

#include <QTimeEdit>

#include "stdafx.h"

class ElaTimeEditPrivate;
class ELA_EXPORT ElaTimeEdit : public QTimeEdit
{
    Q_OBJECT
    Q_Q_CREATE(ElaTimeEdit)
public:
    explicit ElaTimeEdit(QWidget* parent = nullptr);
    ~ElaTimeEdit();
    void stepBy(int steps);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
    void focusInEvent(QFocusEvent *event) override;
};

#endif // ELATIMEEDIT_H

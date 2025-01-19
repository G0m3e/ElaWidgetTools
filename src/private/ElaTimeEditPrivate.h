#ifndef ELATIMEEDITPRIVATE_H
#define ELATIMEEDITPRIVATE_H

#include <QObject>

#include "stdafx.h"
class ElaTimeEdit;
class ElaMenu;
class ElaTimeEditPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTimeEdit)
public:
    explicit ElaTimeEditPrivate(QObject* parent = nullptr);
    ~ElaTimeEditPrivate();

private:
    ElaMenu* _createStandardContextMenu();
};

#endif // ELATIMEEDITPRIVATE_H

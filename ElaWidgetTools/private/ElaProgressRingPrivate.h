#ifndef ELAPROGRESSRINGPRIVATE_H
#define ELAPROGRESSRINGPRIVATE_H

#include <QObject>

#include "stdafx.h"

class ElaProgressRingStyle;
class QPropertyAnimation;
class ElaProgressRing;
class ElaProgressRingPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaProgressRing)
public:
    explicit ElaProgressRingPrivate(QObject* parent = nullptr);
    ~ElaProgressRingPrivate();

private:
    bool _isBusyAnimation{false};
    ElaProgressRingStyle* _style{nullptr};
    QPropertyAnimation* _busyAnimation{nullptr};
};

#endif // ELAPROGRESSRINGPRIVATE_H

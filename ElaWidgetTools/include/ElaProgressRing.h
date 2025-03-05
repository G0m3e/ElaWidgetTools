#ifndef ELAPROGRESSRING_H
#define ELAPROGRESSRING_H

#include <QProgressBar>

#include "stdafx.h"

class ElaProgressRingPrivate;
class ELA_EXPORT ElaProgressRing : public QProgressBar
{
    Q_OBJECT
    Q_Q_CREATE(ElaProgressRing)
public:
    explicit ElaProgressRing(QWidget* parent = nullptr);
    ~ElaProgressRing();
    void setMinimum(int minimum);
    void setMaximum(int maximum);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
};

#endif // ELAPROGRESSRING_H

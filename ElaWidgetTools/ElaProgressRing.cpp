#include "ElaProgressRing.h"

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QStyleOption>

#include "ElaProgressRingPrivate.h"
#include "ElaProgressRingStyle.h"
ElaProgressRing::ElaProgressRing(QWidget* parent)
    : QProgressBar(parent), d_ptr(new ElaProgressRingPrivate())
{
    Q_D(ElaProgressRing);
    setObjectName("ElaProgressRing");
    setStyleSheet("#ElaProgressRing{background-color:transparent;}");
    d->q_ptr = this;
    d->_style = new ElaProgressRingStyle(style());
    setStyle(d->_style);
    setMinimumHeight(64);
    d->_busyStartAnimation = new QPropertyAnimation(d->_style, "startAngle");
    connect(d->_busyStartAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    d->_busyStartAnimation->setDuration(2000);
    d->_busyStartAnimation->setKeyValueAt(0.0, 0);
    d->_busyStartAnimation->setKeyValueAt(0.25, 270);
    d->_busyStartAnimation->setKeyValueAt(0.33, 360);
    d->_busyStartAnimation->setKeyValueAt(0.67, 720);
    d->_busyStartAnimation->setKeyValueAt(1.0, 1080);
    d->_busyStartAnimation->setLoopCount(-1);

    d->_busySweepAnimation = new QPropertyAnimation(d->_style, "sweepAngle");
    d->_busySweepAnimation->setDuration(2000);
    d->_busySweepAnimation->setKeyValueAt(0.0, 0);
    d->_busySweepAnimation->setKeyValueAt(0.25, 90);
    d->_busySweepAnimation->setKeyValueAt(0.33, 180);
    d->_busySweepAnimation->setKeyValueAt(0.67, 90);
    d->_busySweepAnimation->setKeyValueAt(1.0, 0);
    d->_busySweepAnimation->setLoopCount(-1);

    d->_busyAnimationGroup = new QParallelAnimationGroup(d->_style);
    d->_busyAnimationGroup->addAnimation(d->_busyStartAnimation);
    d->_busyAnimationGroup->addAnimation(d->_busySweepAnimation);
}

ElaProgressRing::~ElaProgressRing()
{
}

void ElaProgressRing::setMinimum(int minimum)
{
    Q_D(ElaProgressRing);
    if (d->_isBusyAnimation && !(maximum() == 0 && minimum == 0))
    {
        d->_isBusyAnimation = false;
        d->_busyAnimationGroup->stop();
    }
    QProgressBar::setMinimum(minimum);
}

void ElaProgressRing::setMaximum(int maximum)
{
    Q_D(ElaProgressRing);
    if (d->_isBusyAnimation && !(minimum() == 0 && maximum == 0))
    {
        d->_isBusyAnimation = false;
        d->_busyAnimationGroup->stop();
    }
    QProgressBar::setMaximum(maximum);
}

void ElaProgressRing::paintEvent(QPaintEvent* event)
{
    Q_D(ElaProgressRing);
    if (!d->_isBusyAnimation && minimum() == 0 && maximum() == 0)
    {
        QStyleOptionProgressBar option;
        option.initFrom(this);
        d->_isBusyAnimation = true;
        d->_busyAnimationGroup->start();
    }
    QProgressBar::paintEvent(event);
}

void ElaProgressRing::resizeEvent(QResizeEvent* event)
{
    Q_D(ElaProgressRing);
    // if (orientation() == Qt::Horizontal)
    // {
    //     d->_busyStartAnimation->setEndValue(this->width());
    // }
    // else
    // {
    //     d->_busyStartAnimation->setEndValue(this->height());
    // }
    QProgressBar::resizeEvent(event);
}

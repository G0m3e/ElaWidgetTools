#include "ElaTeachingTip.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTimer>
#include <ElaTheme.h>
#include <ElaIconButton.h>
#include <ElaText.h>
#include <QEvent>
#include <QMouseEvent>

ElaTeachingTip::ElaTeachingTip(QWidget *parent, QWidget *target, ArrowPosition position)
    : QWidget(parent)
    , m_pTarget(target)
    , m_oArrowType(position)
    , m_pLayout(new QHBoxLayout(this))
    , m_pEffect(new ScaleEffect(this))
{
    setFixedSize(150, 50);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_pLabel = new ElaText("TeachingTip", 14, this);

    m_pCloseBtn = new ElaIconButton(ElaIconType::Xmark, 17, 30, 30, this);
    m_pCloseBtn->setBorderRadius(5);
    connect(m_pCloseBtn, &ElaIconButton::clicked, this, &ElaTeachingTip::hideTip);

    m_pLayout->addWidget(m_pLabel);
    m_pLayout->addWidget(m_pCloseBtn, 0, Qt::AlignTop | Qt::AlignRight);

    int l = 4, t = 4, r = 4, b = 4;
    switch (m_oArrowType)
    {
    case Top:    b += m_nArrowSize; break;
    case Bottom: t += m_nArrowSize; break;
    case Left:   r += m_nArrowSize; break;
    case Right:  l += m_nArrowSize; break;
    }
    m_pLayout->setContentsMargins(l, t, r, b);

    m_oThemeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        m_oThemeMode = themeMode;
    });
    QTimer::singleShot(0, this, [=](){
        getBubblePath();
        m_pEffect->setScale(0.0);
        m_pEffect->setScaleOrigin(m_oArrowPos);
        setGraphicsEffect(m_pEffect);
    });
    m_pShowAnim = new QPropertyAnimation(m_pEffect, "scale", this);
    m_pShowAnim->setDuration(250);
    m_pShowAnim->setStartValue(0.0);
    m_pShowAnim->setEndValue(1.0);
    m_pShowAnim->setEasingCurve(QEasingCurve::OutExpo);

    m_pHideAnim = new QPropertyAnimation(m_pEffect, "scale", this);
    m_pHideAnim->setDuration(250);
    m_pHideAnim->setStartValue(1.0);
    m_pHideAnim->setEndValue(0.0);
    m_pHideAnim->setEasingCurve(QEasingCurve::InExpo);
    connect(m_pHideAnim, &QPropertyAnimation::finished, this, &ElaTeachingTip::hide);
}

void ElaTeachingTip::setText(const QString &text)
{
    m_pLabel->setText(text);
    adjustSize();
}

void ElaTeachingTip::setArrowPosition(ArrowPosition position)
{
    m_oArrowType = position;
    update();
}

void ElaTeachingTip::setCentralWidget(QWidget *centralWidget)
{
    m_pLayout->takeAt(0);
    m_pLayout->insertWidget(0, centralWidget);
    delete m_pLabel;
}

void ElaTeachingTip::setCloseButtonVisible(bool bVisible)
{
    m_pCloseBtn->setVisible(bVisible);
}

void ElaTeachingTip::showTip()
{
    if (m_pShowAnim->state() == QAbstractAnimation::Running ||
        m_pHideAnim->state() == QAbstractAnimation::Running)
    {
        return;
    }
    updatePosition();
    show();
    m_pShowAnim->start();
}

void ElaTeachingTip::hideTip()
{
    if (m_pShowAnim->state() == QAbstractAnimation::Running ||
        m_pHideAnim->state() == QAbstractAnimation::Running)
    {
        return;
    }
    m_pHideAnim->start();
}

QRect getRectByCenter(const QPoint& center, int width, int height)
{
    int x = center.x() - width / 2;
    int y = center.y() - height / 2;
    return QRect(x, y, width, height);
}

QFlags<OverflowDirection> checkOverflow(const QRect& a, const QRect& b)
{
    QFlags<OverflowDirection> result;

    if (b.left() < a.left())
        result |= OverflowDirection::Left;
    if (b.right() > a.right())
        result |= OverflowDirection::Right;
    if (b.top() < a.top())
        result |= OverflowDirection::Top;
    if (b.bottom() > a.bottom())
        result |= OverflowDirection::Bottom;

    return result;
}


void ElaTeachingTip::updatePosition()
{
    if (!m_pTarget || !parentWidget())
        return;

    QPoint targetCenterGlobal;
    switch (m_oArrowType)
    {
    case Top:
    case Bottom:
        targetCenterGlobal = m_pTarget->mapToGlobal(QPoint(m_pTarget->width() / 2, 0));
        break;
    case Left:
    case Right:
        targetCenterGlobal = m_pTarget->mapToGlobal(QPoint(0, m_pTarget->height() / 2));
        break;
    }

    QPoint targetCenterLocal = parentWidget()->mapFromGlobal(targetCenterGlobal);

    int x = 0;
    int y = 0;

    switch (m_oArrowType)
    {
    case Top:
        x = targetCenterLocal.x() - width() / 2;
        y = targetCenterLocal.y() - height();
        break;
    case Bottom:
        x = targetCenterLocal.x() - width() / 2;
        y = targetCenterLocal.y() + m_pTarget->height();
        break;
    case Left:
        x = targetCenterLocal.x() - width();
        y = targetCenterLocal.y() - height() / 2;
        break;
    case Right:
        x = targetCenterLocal.x() + m_pTarget->width();
        y = targetCenterLocal.y() - height() / 2;
        break;
    }
    move(parentWidget()->mapToGlobal(QPoint(x, y)));
}

void ElaTeachingTip::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.save();
    painter.setBrush(ElaThemeColor(m_oThemeMode, PopupBase));
    QPainterPath path = getBubblePath();
    QPen pen(ElaThemeColor(m_oThemeMode, PopupBorder));
    pen.setWidthF(1.0);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawPath(path);
    painter.restore();
}

bool ElaTeachingTip::event(QEvent *event)
{
    if (event->type() == QEvent::WindowDeactivate || event->type() == QEvent::MouseButtonDblClick ||
        (event->type() == QEvent::MouseButtonPress && !geometry().toRectF().contains(static_cast<QMouseEvent*>(event)->globalPosition())))
    {
        hideTip();
        return true;
    }
    return QWidget::event(event);
}

QPainterPath ElaTeachingTip::getBubblePath()
{
    const int radius = m_nRadius;
    QRect rect = this->rect();

    switch (m_oArrowType)
    {
    case Bottom: rect.adjust(0, m_nArrowSize, 0, 0); break;
    case Top:    rect.adjust(0, 0, 0, -m_nArrowSize); break;
    case Right:  rect.adjust(m_nArrowSize, 0, 0, 0); break;
    case Left:   rect.adjust(0, 0, -m_nArrowSize, 0); break;
    }

    QPainterPath path;
    int x = rect.x();
    int y = rect.y();
    int w = rect.width();
    int h = rect.height();

    auto arcToCorner = [&](int rx, int ry, int startAngle) {
        path.arcTo(QRectF(rx, ry, radius * 2, radius * 2), startAngle, -90);
    };

    switch (m_oArrowType)
    {
    case Bottom:
    {
        int cx = rect.center().x();
        int ay = y - m_nArrowSize;
        m_oArrowPos = QPoint(cx, ay);
        path.moveTo(x + radius, y);
        path.lineTo(cx - m_nArrowSize, y);
        path.lineTo(cx, ay);
        path.lineTo(cx + m_nArrowSize, y);
        path.lineTo(x + w - radius, y);
        arcToCorner(x + w - 2 * radius, y, 90);            // top-right
        path.lineTo(x + w, y + h - radius);
        arcToCorner(x + w - 2 * radius, y + h - 2 * radius, 0); // bottom-right
        path.lineTo(x + radius, y + h);
        arcToCorner(x, y + h - 2 * radius, 270);           // bottom-left
        path.lineTo(x, y + radius);
        arcToCorner(x, y, 180);                            // top-left
        break;
    }
    case Top:
    {
        int cx = rect.center().x();
        int ay = y + h + m_nArrowSize;
        m_oArrowPos = QPoint(cx, ay);
        path.moveTo(x + radius, y);
        path.lineTo(x + w - radius, y);
        arcToCorner(x + w - 2 * radius, y, 90);            // top-right
        path.lineTo(x + w, y + h - radius);
        arcToCorner(x + w - 2 * radius, y + h - 2 * radius, 0); // bottom-right

        path.lineTo(cx + m_nArrowSize, y + h);
        path.lineTo(cx, ay);
        path.lineTo(cx - m_nArrowSize, y + h);

        path.lineTo(x + radius, y + h);
        arcToCorner(x, y + h - 2 * radius, 270);           // bottom-left
        path.lineTo(x, y + radius);
        arcToCorner(x, y, 180);                            // top-left
        break;
    }
    case Right:
    {
        int cy = rect.center().y();
        int ax = x - m_nArrowSize;
        m_oArrowPos = QPoint(ax, cy);
        path.moveTo(x, y + radius);
        path.lineTo(x, cy - m_nArrowSize);
        path.lineTo(ax, cy);
        path.lineTo(x, cy + m_nArrowSize);

        path.lineTo(x, y + h - radius);
        arcToCorner(x, y + h - 2 * radius, 270);           // bottom-left
        path.lineTo(x + w - radius, y + h);
        arcToCorner(x + w - 2 * radius, y + h - 2 * radius, 0); // bottom-right
        path.lineTo(x + w, y + radius);
        arcToCorner(x + w - 2 * radius, y, 90);            // top-right
        path.lineTo(x + radius, y);
        arcToCorner(x, y, 180);                            // top-left
        break;
    }
    case Left:
    {
        int cy = rect.center().y();
        int ax = x + w + m_nArrowSize;
        m_oArrowPos = QPoint(ax, cy);
        path.moveTo(x + radius, y);
        path.lineTo(x + w - radius, y);
        arcToCorner(x + w - 2 * radius, y, 90);            // top-right
        path.lineTo(x + w, cy - m_nArrowSize);

        path.lineTo(ax, cy);
        path.lineTo(x + w, cy + m_nArrowSize);

        path.lineTo(x + w, y + h - radius);
        arcToCorner(x + w - 2 * radius, y + h - 2 * radius, 0); // bottom-right
        path.lineTo(x + radius, y + h);
        arcToCorner(x, y + h - 2 * radius, 270);           // bottom-left
        path.lineTo(x, y + radius);
        arcToCorner(x, y, 180);                            // top-left
        break;
    }
    }

    path.closeSubpath();
    return path;
}

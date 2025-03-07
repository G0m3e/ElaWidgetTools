#include "ElaProgressRingStyle.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "ElaTheme.h"
ElaProgressRingStyle::ElaProgressRingStyle(QStyle* style)
{
    setProperty("startAngle", 0);
    setProperty("sweepAngle", 0);
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaProgressRingStyle::~ElaProgressRingStyle()
{
}

void ElaProgressRingStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ProgressBarGroove:
    {
        //背景轨道
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        int size = qMin(option->rect.width(), option->rect.height());
        painter->setPen(QPen(ElaThemeColor(_themeMode, BasicChute), size*0.08, Qt::SolidLine, Qt::RoundCap));
        painter->setBrush(ElaThemeColor(_themeMode, BasicChute));
        // qDebug() << option->rect;
        QPoint center = option->rect.center();
        int margin = 5;
        int radius = size / 2;
        // QRectF arcRect(center.x() - radius + margin, center.y() - radius + margin, (radius - margin) * 2, (radius - margin) * 2);
        QRectF arcRect = QRectF(option->rect).adjusted(5, 5, -5, -5);
        painter->drawArc(arcRect, 0, 360 * 16);
        painter->restore();
        return;
    }
    case QStyle::CE_ProgressBarContents:
    {
        //滑块
        const QStyleOptionProgressBar* popt = qstyleoption_cast<const QStyleOptionProgressBar*>(option);
        if (!popt)
        {
            break;
        }
        QRect contentRect = popt->rect;
        int size = qMin(contentRect.width(), contentRect.height());
        QPoint center = contentRect.center();
        int margin = 5;
        int radius = size / 2;
        // QRectF arcRect(center.x() - radius + margin, center.y() - radius + margin, (radius - margin) * 2, (radius - margin) * 2);
        QRectF arcRect = QRectF(option->rect).adjusted(5, 5, -5, -5);
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        painter->setPen(QPen(ElaThemeColor(_themeMode, PrimaryNormal), size*0.08, Qt::SolidLine, Qt::RoundCap));
        painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
        const bool inverted = popt->invertedAppearance;
        bool reverse = popt->direction == Qt::RightToLeft;
        if (inverted)
        {
            reverse = !reverse;
        }
        if (popt->minimum == 0 && popt->maximum == 0)
        {
            //忙碌动画
            int startAngle = this->property("startAngle").toInt();
            if (startAngle < 0)
            {
                startAngle = 0;
            }
            int sweepAngle = this->property("sweepAngle").toInt();
            if(reverse)
            {
                painter->drawArc(arcRect, startAngle * 16, -sweepAngle * 16);
            }
            else
            {
                painter->drawArc(arcRect, -startAngle * 16, sweepAngle * 16);
            }
        }
        else
        {
            qreal ratio = popt->progress / (qreal)(popt->maximum - popt->minimum);
            int angleSpan = int(360 * ratio * 16);
            if(reverse)
            {
                painter->drawArc(arcRect,
                                 90 * 16, angleSpan);
            }
            else
            {
                painter->drawArc(arcRect,
                                 90 * 16, -angleSpan);
            }
        }
        painter->restore();
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}


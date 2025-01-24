#include "ElaTimeEditStyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionSpinBox>

#include "ElaTheme.h"
ElaTimeEditStyle::ElaTimeEditStyle(QStyle* style)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaTimeEditStyle::~ElaTimeEditStyle()
{
}

void ElaTimeEditStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    switch (control)
    {
    case QStyle::CC_SpinBox:
    {
        const QStyleOptionSpinBox* sopt = qstyleoption_cast<const QStyleOptionSpinBox*>(option);
        if (!sopt)
        {
            break;
        }
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        //背景
        painter->setPen(ElaThemeColor(_themeMode, BasicBorder));
        painter->setBrush(ElaThemeColor(_themeMode, BasicBase));
        painter->drawRoundedRect(sopt->rect, 4, 4);
        //添加按钮
        QRect addLineRect = subControlRect(control, sopt, SC_ScrollBarAddLine, widget);
        if (sopt->activeSubControls == SC_ScrollBarAddLine)
        {
            if (sopt->state & QStyle::State_Sunken && sopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(ElaThemeColor(_themeMode, BasicPressAlpha));
            }
            else
            {
                if (sopt->state & QStyle::State_MouseOver)
                {
                    painter->setBrush(ElaThemeColor(_themeMode, BasicHoverAlpha));
                }
                else
                {
                    painter->setBrush(ElaThemeColor(_themeMode, BasicBaseDeep));
                }
            }
        }
        else
        {
            painter->setBrush(ElaThemeColor(_themeMode, BasicBaseDeep));
        }
        const int arcRadius = 4;
        QPainterPath addLinePath;
        addLinePath.moveTo(addLineRect.x() + arcRadius, addLineRect.y());
        addLinePath.lineTo(addLineRect.right() - arcRadius, addLineRect.y());
        addLinePath.arcTo(QRectF(addLineRect.right() - arcRadius * 2, addLineRect.y(), arcRadius * 2, arcRadius * 2), 90, -90);
        addLinePath.lineTo(addLineRect.right(), addLineRect.bottom() - arcRadius);
        addLinePath.arcTo(QRectF(addLineRect.right() - arcRadius * 2, addLineRect.bottom() - arcRadius * 2, arcRadius * 2, arcRadius * 2), 0, -90);
        addLinePath.lineTo(addLineRect.x() + arcRadius, addLineRect.bottom());
        addLinePath.arcTo(QRectF(addLineRect.x(), addLineRect.bottom() - arcRadius * 2, arcRadius * 2, arcRadius * 2), 270, -90);
        addLinePath.lineTo(addLineRect.x(), addLineRect.y() + arcRadius);
        addLinePath.arcTo(QRectF(addLineRect.x(), addLineRect.y(), arcRadius * 2, arcRadius * 2), 180, -90);
        addLinePath.closeSubpath();
        painter->drawPath(addLinePath);

        //减少按钮
        QRect subLineRect = subControlRect(control, sopt, SC_ScrollBarSubLine, widget);
        if (sopt->activeSubControls == SC_ScrollBarSubLine)
        {
            if (sopt->state & QStyle::State_Sunken && sopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(ElaThemeColor(_themeMode, BasicPressAlpha));
            }
            else
            {
                if (sopt->state & QStyle::State_MouseOver)
                {
                    painter->setBrush(ElaThemeColor(_themeMode, BasicHoverAlpha));
                }
                else
                {
                    painter->setBrush(ElaThemeColor(_themeMode, BasicBaseDeep));
                }
            }
        }
        else
        {
            painter->setBrush(ElaThemeColor(_themeMode, BasicBaseDeep));
        }
        QPainterPath subLinePath;
        subLinePath.moveTo(subLineRect.x() + arcRadius, subLineRect.y());
        subLinePath.lineTo(subLineRect.right() - arcRadius, subLineRect.y());
        subLinePath.arcTo(QRectF(subLineRect.right() - arcRadius * 2, subLineRect.y(), arcRadius * 2, arcRadius * 2), 90, -90);
        subLinePath.lineTo(subLineRect.right(), subLineRect.bottom() - arcRadius);
        subLinePath.arcTo(QRectF(subLineRect.right() - arcRadius * 2, subLineRect.bottom() - arcRadius * 2, arcRadius * 2, arcRadius * 2), 0, -90);
        subLinePath.lineTo(subLineRect.x() + arcRadius, subLineRect.bottom());
        subLinePath.arcTo(QRectF(subLineRect.x(), subLineRect.bottom() - arcRadius * 2, arcRadius * 2, arcRadius * 2), 270, -90);
        subLinePath.lineTo(subLineRect.x(), subLineRect.y() + arcRadius);
        subLinePath.arcTo(QRectF(subLineRect.x(), subLineRect.y(), arcRadius * 2, arcRadius * 2), 180, -90);
        subLinePath.closeSubpath();
        painter->drawPath(subLinePath);
        //底边线
        if (sopt->state & QStyle::State_HasFocus)
        {
            painter->setPen(QPen(ElaThemeColor(_themeMode, PrimaryNormal), 2));
            painter->drawLine(subLineRect.right() + 1, 2*subLineRect.y() + subLineRect.height() - 2, addLineRect.left() - 1, 2*subLineRect.y() + subLineRect.height() - 2);
        }
        else
        {
            painter->setPen(ElaThemeColor(_themeMode, BasicHemline));
            painter->drawLine(subLineRect.right() + 1, 2*subLineRect.y() + subLineRect.height() - 1, addLineRect.left() - 1, 2*subLineRect.y() + subLineRect.height() - 1);
        }

        //添加图标
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(15);
        painter->setFont(iconFont);
        painter->setPen(ElaThemeColor(_themeMode, BasicText));
        painter->drawText(addLineRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::Plus));
        //减小图标
        painter->drawText(subLineRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::Minus));
        painter->restore();
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

QRect ElaTimeEditStyle::subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const
{
    QRect rect = QProxyStyle::subControlRect(cc, opt, sc, widget);
    switch (cc)
    {
    case CC_SpinBox:
    {
        switch (sc)
        {
        case SC_ScrollBarAddLine:
        {
            //增加按钮
            QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget);
            return QRect(spinBoxRect.width() - spinBoxRect.height() + 5, 5, spinBoxRect.height() - 10, spinBoxRect.height() - 10);
        }
        case SC_ScrollBarSubLine:
        {
            //减少按钮
            QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget);
            return QRect(5, 5, spinBoxRect.height() - 10, spinBoxRect.height() - 10);
        }
        case SC_SpinBoxEditField:
        {
            QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget);
            return QRect(spinBoxRect.height(), 0, spinBoxRect.width() - 2 * spinBoxRect.height(), spinBoxRect.height());
        }
        default:
        {
            break;
        }
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return rect;
}

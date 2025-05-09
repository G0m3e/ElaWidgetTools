#pragma once

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <ElaTheme.h>

class QHBoxLayout;
class ElaIconButton;
class QPropertyAnimation;
class ElaText;
class ScaleEffect : public QGraphicsEffect
{
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale)

public:
    explicit ScaleEffect(QObject *parent = nullptr)
        : QGraphicsEffect(parent), m_scale(1.0) {}

    void setScale(qreal scale)
    {
        m_scale = scale;
        update();
    }

    qreal scale() const { return m_scale; }

protected:
    void draw(QPainter *painter) override
    {
        if (!source()) return;

        QPoint offset;
        QPixmap pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset);

        painter->save();

        // 缩放中心设置为尖尖位置
        QPointF center = m_scaleOrigin.isNull() ? pixmap.rect().center() : m_scaleOrigin;

        painter->translate(center);
        painter->scale(m_scale, m_scale);
        painter->translate(-center);

        painter->drawPixmap(offset, pixmap);

        painter->restore();
    }

public:
    void setScaleOrigin(const QPointF &origin) { m_scaleOrigin = origin; }

private:
    qreal m_scale;
    QPointF m_scaleOrigin;
};

enum class OverflowDirection {
    None = 0,
    Left = 1,
    Right = 2,
    Top = 4,
    Bottom = 8
};

class ElaTeachingTip : public QWidget
{
    Q_OBJECT
public:
    enum ArrowPosition {
        Top,
        Bottom,
        Left,
        Right
    };

    explicit ElaTeachingTip(QWidget *parent = nullptr, QWidget *target = nullptr, ArrowPosition position = Bottom);
    void setText(const QString &text);
    void setArrowPosition(ArrowPosition position);
    void setCentralWidget(QWidget* centralWidget);
    void setCloseButtonVisible(bool bVisible);
    void showTip();
    void hideTip();

private:
    void updatePosition();
    QPoint getCenter(ArrowPosition oArrowType);
    QPainterPath getBubblePath();

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;

private:
    QWidget* m_pTarget;
    ArrowPosition m_oArrowType;
    QPoint m_oArrowPos;
    ElaText* m_pLabel;
    QPushButton* m_closeBtn;
    int m_nArrowSize = 5;
    int m_nRadius = 8;
    QColor m_textColor = Qt::white;
    ElaThemeType::ThemeMode m_oThemeMode;
    QHBoxLayout* m_pLayout{nullptr};
    ElaIconButton* m_pCloseBtn{nullptr};
    ScaleEffect* m_pEffect{nullptr};
    QPropertyAnimation* m_pShowAnim{nullptr};
    QPropertyAnimation* m_pHideAnim{nullptr};
};

#ifndef ELAPROGRESSRINGSTYLE_H
#define ELAPROGRESSRINGSTYLE_H

#include <QProxyStyle>

#include "Def.h"
class ElaProgressRingStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaProgressRingStyle(QStyle* style = nullptr);
    ~ElaProgressRingStyle();
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAPROGRESSRINGSTYLE_H

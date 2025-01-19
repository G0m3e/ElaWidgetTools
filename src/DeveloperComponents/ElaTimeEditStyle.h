#ifndef ELATIMEEDITSTYLE_H
#define ELATIMEEDITSTYLE_H

#include <QProxyStyle>

#include "Def.h"
class ElaTimeEditStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaTimeEditStyle(QStyle* style = nullptr);
    ~ElaTimeEditStyle();
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELATIMEEDITSTYLE_H

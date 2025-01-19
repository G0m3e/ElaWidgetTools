#include "ElaTimeEdit.h"

#include <QContextMenuEvent>
#include <QLineEdit>
#include <QPainter>

#include "DeveloperComponents/ElaTimeEditStyle.h"
#include "ElaMenu.h"
#include "ElaTimeEditPrivate.h"
#include "ElaTheme.h"
ElaTimeEdit::ElaTimeEdit(QWidget* parent)
    : QTimeEdit(parent), d_ptr(new ElaTimeEditPrivate())
{
    Q_D(ElaTimeEdit);
    d->q_ptr = this;
    setFixedSize(120, 30);
    setObjectName("ElaTimeEdit");
    setStyle(new ElaTimeEditStyle(style()));
    lineEdit()->setAlignment(Qt::AlignCenter);
    lineEdit()->setStyleSheet("background-color:transparent");
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::transparent);
        palette.setColor(QPalette::Text, ElaThemeColor(themeMode, BasicText));
        lineEdit()->setPalette(palette);
    });
}

ElaTimeEdit::~ElaTimeEdit()
{
}

void ElaTimeEdit::contextMenuEvent(QContextMenuEvent *event)
{
    Q_D(ElaTimeEdit);
    ElaMenu* menu = d->_createStandardContextMenu();
    if (!menu)
    {
        return;
    }
    menu->addSeparator();
    const uint se = stepEnabled();
    QAction* up = menu->addElaIconAction(ElaIconType::Plus, tr("增加"));
    up->setEnabled(se & StepUpEnabled);
    QAction* down = menu->addElaIconAction(ElaIconType::Minus, tr("减少"));
    down->setEnabled(se & StepDownEnabled);
    menu->addSeparator();

    const QAbstractSpinBox* that = this;
    const QPoint pos = (event->reason() == QContextMenuEvent::Mouse)
                           ? event->globalPos()
                           : mapToGlobal(QPoint(event->pos().x(), 0)) + QPoint(width() / 2, height() / 2);
    const QAction* action = menu->exec(pos);
    delete menu;
    if (that && action)
    {
        if (action == up)
        {
            stepBy(1);
        }
        else if (action == down)
        {
            stepBy(-1);
        }
    }
    event->accept();
}

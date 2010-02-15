#include <QMenu>
#include "Menu.h"
#include <QtDebug>

Menu::Menu(QWidget *parent, ControlHelper *helper)
    : QObject(parent),
    m_helper(helper)
{
    parent->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(parent, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(menuRequested(const QPoint&)));
}

void Menu::menuRequested(const QPoint& pos)
{
    QAction *defaults = new QAction("Default value", NULL);
    QAction *midiLearn = new QAction("MIDI Learn", NULL);
    QAction *debugPrint = new QAction("(Debug print)", NULL);

    QList<QAction*> actions;
    actions << defaults << midiLearn << debugPrint;

    QWidget *widget = qobject_cast<QWidget*>(parent());
    if (!widget) return;

    QAction *response = QMenu::exec(actions, widget->mapToGlobal(pos));
    if (midiLearn == response) {
        m_helper->MIDILearn();
    } else if (defaults == response) {
        m_helper->defaults();
    } else if (debugPrint == response) {
        m_helper->debugPrint();
    }
}

#include "Menu.moc"

// vim: sw=4 sts=4 et tw=100

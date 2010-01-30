#ifndef MENU_H
#define MENU_H

#include "ControlHelper.h"
#include <QObject>

/** 
 * @brief A menu, used for all widgets that are connected to controls in the gui. Automatically
 * assigned through the ControlHelper class.
 */
class Menu : public QObject
{
    Q_OBJECT
    public:
        Menu(QWidget *parent, ControlHelper* helper);

    private slots:
        void menuRequested(const QPoint& pos);

    private:
        ControlHelper *m_helper;

};

#endif // MENU_H

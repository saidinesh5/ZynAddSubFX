#ifndef MENU_H
#define MENU_H

#include "ControlHelper.h"
#include <QObject>

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

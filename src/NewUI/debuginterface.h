#ifndef DEBUGINTERFACE_H
#define DEBUGINTERFACE_H

#include <QDialog>
#include "../Controls/Node.h"

class DebugInterface : public QDialog, public NodeUser
{
    Q_OBJECT

    public:
        DebugInterface(QWidget *parent = NULL, class Master *master = NULL);
        void handleEvent(Event &event);

    signals:
        void newEvent(QString info);

    private slots:
        void receiveEvent(QString info);

    private:
        class Master *master;
        class QTextEdit *text;
        class Tree *tree;

};

#endif // _DEBUGINTERFACE_H

#ifndef DEBUGINTERFACE_H
#define DEBUGINTERFACE_H

#include <QDialog>

class DebugInterface : public QDialog
{
    Q_OBJECT

    public:
        DebugInterface(QWidget *parent = NULL, class Master *master = NULL);

    private:
        class Master *master;

};

#endif // _DEBUGINTERFACE_H

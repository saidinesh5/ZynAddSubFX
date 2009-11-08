#include <QDialog>
#include "../Misc/Master.h"
#include "ui_NewAddNote.h"

#ifndef _ADDNOTEUI_H_
#define _ADDNOTEUI_H_

class AddnoteUI:public QDialog, Ui::AddnoteUI
{
    Q_OBJECT

    public:
        AddnoteUI(QString id);

    private:
        QString id;
};


#endif /* include _ADDNOTEUI_H_ */


#include <QDialog>
#include "../Misc/Master.h"
#include "ui_addnoteui.h"

#ifndef _ADDNOTEUI_H_
#define _ADDNOTEUI_H_

class AddNoteUi : public QDialog, Ui::addnoteui
{
    Q_OBJECT

public:
    AddNoteUi(QString id);

private slots:
    //void simpleRefresh();

private:
    QString id;
};


#endif /* include _ADDNOTEUI_H_ */

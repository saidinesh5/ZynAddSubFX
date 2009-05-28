#include <QDialog>
#include "../Misc/Master.h"
#include "ui_addnoteui.h"

#ifndef _MASTERUI_H_
#define _MASTERUI_H_

class AddNoteUi : public QDialog, Ui::addnoteui
{
	Q_OBJECT

	public:
		AddNoteUi(Master *master_,int *exitprogram_);

	private slots:
		void simpleRefresh();

		int *Pexitprogram;
		Master *master;
};


#endif /* include _MASTERUI_H_ */

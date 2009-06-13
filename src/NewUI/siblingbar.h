#ifndef _SIBLINGBAR_H_
#define _SIBLINGBAR_H_

#include <QTabBar>
#include <QSet>

class SiblingBar : public QTabBar
{
	Q_OBJECT

	public:
		SiblingBar(QWidget *parent=NULL);
		void addControlWidgets(QWidget *widget);
		void setChildrenContainer(class ControlContainer* container);

	private:
		QSet<class ControlHelper*> foundControls;

};

#endif /* ifndef _SIBLINGBAR_H_ */

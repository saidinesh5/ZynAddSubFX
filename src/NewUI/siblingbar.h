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
		void mouseReleaseEvent(class QMouseEvent *event);

	private slots:
		void updateFoundControls();

	private:
		QSet<class ControlHelper*> m_foundControls;
		class ControlContainer* m_childContainer;

};

#endif /* ifndef _SIBLINGBAR_H_ */

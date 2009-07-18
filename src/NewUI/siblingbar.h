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
    void setNode(class Node* container);
    void mouseReleaseEvent(class QMouseEvent *event);
    QString getCurrentChild();

private slots:
    void updateFoundControls();

private:
    QSet<class ControlHelper*> m_foundControls;
    class Node* m_childContainer;

};

#endif /* ifndef _SIBLINGBAR_H_ */

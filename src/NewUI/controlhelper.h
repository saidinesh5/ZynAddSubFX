#ifndef _CONTROLHELPER_H_
#define _CONTROLHELPER_H_

#include "../Controls/Control.h"
#include "../Controls/ControlUser.h"
#include <QObject>
#include <QMutex>

class ControlHelper : public QObject, public NodeUser
{
    Q_OBJECT
public:
    ControlHelper(QObject *parent);
    bool eventFilter ( QObject * watched, QEvent * event );
    QString getControlId();
    void setControl(QString absoluteId);
    QString relativeControlId() const;

    void requestValue();

    //reimplemented from nodeuser
    void handleEvent(Event *event);

public slots:
    void setValue(char value);
    void setValue(int value);

signals:
    void valueChanged(int value);

private:
    GenControl *m_control;
    QString m_relativeControlId;
    int expectedValueEvents;
    QMutex expectedEventMutex;
};

#endif /* ifndef _CONTROLHELPER_H_ */

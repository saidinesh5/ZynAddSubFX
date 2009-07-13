#ifndef _CONTROLHELPER_H_
#define _CONTROLHELPER_H_

#include "../Controls/Control.h"
#include "../Controls/ControlUser.h"
#include <QObject>

class ControlHelper : public QObject, public ControlUser
{
    Q_OBJECT
public:
    ControlHelper(QObject *parent);
    //void controlChanged(Control* control);
    bool eventFilter ( QObject * watched, QEvent * event );
    QString getControlId();
    void setControl(QString absoluteId);
    QString relativeControlId() const;

public slots:
    void setValue(char value);
    void setValue(int value);

signals:
    void valueChanged(int value);

private:
    GenControl *m_control;
    QString m_relativeControlId;
};

#endif /* ifndef _CONTROLHELPER_H_ */

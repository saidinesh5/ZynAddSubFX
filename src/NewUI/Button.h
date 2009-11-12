#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>

class Button : public QPushButton
{
    Q_OBJECT
    public:
        Button(QWidget *parent = NULL);
};

#endif // BUTTON_H

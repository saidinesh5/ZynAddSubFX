#ifndef PROPERTYWATCH_H
#define PROPERTYWATCH_H

#include <QObject>

class PropertyWatch : public QObject
{
    public:
        PropertyWatch(QObject *parent);

        /**
         * @brief The Qt event handler, reimplementedfor watching the parent widget for property
         * changes that would change the current control etc.
         */
        bool eventFilter(QObject *watched, QEvent *event);
};

#endif // PROPERTYWATCH_H

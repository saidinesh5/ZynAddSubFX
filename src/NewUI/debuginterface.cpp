#include "debuginterface.h"
#include <QTextEdit>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QScrollBar>
#include "../Misc/Master.h"

class Tree : public QTreeWidget
{
    public:
        NodeUser *user;
        
        Tree(QWidget *parent, NodeUser *user)
            : QTreeWidget(parent),
            user(user)
        {
            setColumnCount(1);

            for (NodeIterator i = Node::getRoot()->getChildren().begin();
                    i != Node::getRoot()->getChildren().end();
                    ++i)
            {
                QTreeWidgetItem *item = new QTreeWidgetItem(this);
                item->setText(0, QString::fromStdString((*i)->getId()));
                addChildren(*i, item);
                (*i)->addRedirection(user);
            }

        }

        void addChildren(Node *parent, QTreeWidgetItem *parentItem)
        {

            for (NodeIterator i = parent->getChildren().begin();
                    i != parent->getChildren().end();
                    ++i)
            {
                QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
                item->setText(0, QString::fromStdString((*i)->getId()));
                addChildren(*i, item);
                (*i)->addRedirection(user);
            }

        }


};


DebugInterface::DebugInterface(QWidget *parent, Master *master)
    : QDialog(parent),
    master(master)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    tree = new Tree(this, this);

    layout->addWidget(tree);

    text = new QTextEdit(this);

    layout->addWidget(text);

    connect(this, SIGNAL(newEvent(QString)),
            this, SLOT(receiveEvent(QString)));

}

void DebugInterface::handleEvent(Event &event)
{
    //this is the place to make it print actual useful info about the event
    QString info;

    if (event.type() == Event::ChangeEvent) {
        info += "(ChangeEvent) ";
        info += "val:" + QString::number(static_cast<ChangeEvent&>(event).getVal());
    } else if (event.type() == Event::NewValueEvent) {
        info += "(NewValueEvent) ";
    } else {
        info += "(Unknown event type: " + int(event.type()) + QString(")");
    }
    emit newEvent(info);
}

void DebugInterface::receiveEvent(QString info)
{
    text->append(info);
    text->verticalScrollBar()->setValue(text->verticalScrollBar()->maximum());
}


#include "debuginterface.moc"
// vim: sw=4 sts=4 et tw=100

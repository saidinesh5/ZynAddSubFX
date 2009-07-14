#include "debuginterface.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include "../Misc/Master.h"

class Tree : public QTreeWidget
{
    public:
        Master *master;
        
        Tree(QWidget *parent, Master *master)
            : QTreeWidget(parent),
            master(master)
        {
            setColumnCount(1);

            for (NodeIterator i = Node::getRoot()->getChildren().begin();
                    i != Node::getRoot()->getChildren().end();
                    ++i)
            {
                QTreeWidgetItem *item = new QTreeWidgetItem(this);
                item->setText(0, QString::fromStdString((*i)->getId()));
                addChildren(*i, item);
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
            }

        }


};


DebugInterface::DebugInterface(QWidget *parent, Master *master)
    : QDialog(parent),
    master(master)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    QTreeWidget *tree = new Tree(this, master);

    layout->addWidget(tree);
}


#include "debuginterface.moc"
// vim: sw=4 sts=4 et tw=100

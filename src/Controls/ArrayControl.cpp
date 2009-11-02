#include "ArrayControl.h"
#include <string.h>
#include <stdio.h>
#include "EventClasses.h"

using std::string;

ArrayControl::ArrayControl(Node *parent, std::string id, int bufsize)
    : GenControl(parent, id),
    m_bufsize(bufsize)
{
    m_front = new REALTYPE[bufsize];
    m_back = new REALTYPE[bufsize];
}

ArrayControl::~ArrayControl()
{
    delete m_front;
    delete m_back;
}

void ArrayControl::readArray(REALTYPE *buffer, int *size)
{
    memcpy(buffer, m_front, sizeof(REALTYPE)*m_bufsize);
    *size = m_bufsize;
}

void ArrayControl::swapBuffers()
{
    REALTYPE *tmp = m_front;
    m_front = m_back;
    m_back = tmp;
}

void ArrayControl::writeArray(REALTYPE* array, int size)
{
    if (size > m_bufsize) {
        printf("ERROR: writeArray to too small buffer!!!\n");
        return;
    }
    memcpy(m_back, array, sizeof(REALTYPE)*size);
    swapBuffers();

    forward(new NewValueEvent(this));
}

char ArrayControl::getCharValue() const
{

}

void ArrayControl::setValue(char val)
{

}

string ArrayControl::getString() const
{

}

void ArrayControl::defaults()
{

}

// vim: sw=4 sts=4 et tw=100

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

    memset(m_front, 0, sizeof(REALTYPE)*bufsize);
    memset(m_back, 0, sizeof(REALTYPE)*bufsize);

}

ArrayControl::~ArrayControl()
{
    delete [] m_front;
    delete [] m_back;
}

void ArrayControl::readArray(REALTYPE *buffer, int *size)
{
    lock();

    if (*size < m_bufsize) {
        std::cout << "Arraycontrol error: the read buffer is too small\n";
        unlock();
        return;
    }

    memcpy(buffer, m_front, sizeof(REALTYPE)*m_bufsize);
    *size = m_bufsize;
    unlock();
}

void ArrayControl::swapBuffers()
{
    REALTYPE *tmp = m_front;
    m_front = m_back;
    m_back = tmp;
}

string ArrayControl::getString() const
{

}

void ArrayControl::defaults()
{

}

void ArrayControl::finishWrite()
{
    lock();
    swapBuffers();
    unlock();

    forward(new NewValueEvent(this));
}

REALTYPE* ArrayControl::writeBuffer()
{
    return m_back;
}

int ArrayControl::size()
{
    return m_bufsize;
}

// vim: sw=4 sts=4 et tw=100

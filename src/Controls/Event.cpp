#include "Event.h"
#include <iostream>

int Event::ref = 0;

Event::Event(enum ev_type ntype)
    : internalType(ntype),
    isOwned(false)
{
    ref++;

    if (ref > 50) //random number. refcount is usually not this high.
        std::cout << "Event::ref == " << ref << ", possible event memleak?" << std::endl;
}

Event::~Event()
{
    ref--;
}

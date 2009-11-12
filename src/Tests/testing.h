#ifndef TESTS_TESTING_H
#define TESTS_TESTING_H

//be nice and include me only once!

int dummy_rand()
{
    return 123456;
}

int (*rand_func)() = dummy_rand;

#endif // TESTS_TESTING_H

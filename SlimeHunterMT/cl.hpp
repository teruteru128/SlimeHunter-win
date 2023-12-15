#pragma once

#include <iostream>

#define checkError(label, openclFunction)          \
    if (cl_int err = (openclFunction))      \
    {                                       \
        std::cerr << "error ["  label "]: " << err << std::endl;        \
        return openclFunction;\
    }

int clsample();
int clmain(std::atomic_uint64_t *seed);

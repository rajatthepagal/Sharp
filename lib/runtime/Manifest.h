//
// Created by BraxtonN on 2/17/2017.
//

#ifndef SHARP_MANIFEST_H
#define SHARP_MANIFEST_H

#include "../../stdimports.h"
#include "oo/string.h"

class Method;

/**
 * Application info
 */
struct Manifest {

    nString executable;
    nString application;
    nString version;
    bool debug;
    int64_t entry;
    size_t methods, classes;
    int fvers;
    int target;
    int64_t isize;
    size_t strings;
    int64_t baseaddr;
    Method* main;
};


#endif //SHARP_MANIFEST_H

//
// Created by bknun on 2/18/2017.
//

#ifndef SHARP_ARRAY_H
#define SHARP_ARRAY_H

#include "../../../stdimports.h"

class gc_object;

class ArrayObject {
public:
    ArrayObject()
    :
            len(0),
            arry(NULL)
    {
    }

    ArrayObject(int64_t max, int type);

    int64_t len;
    gc_object* arry;

    void free();
};


#endif //SHARP_ARRAY_H

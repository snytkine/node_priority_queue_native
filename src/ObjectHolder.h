//
// Created by Snytkine, Dmitri (CORP) on 4/25/17.
//

#ifndef HEAP_LIB_OBJECTHOLDER_H
#define HEAP_LIB_OBJECTHOLDER_H


#include <node.h>
#include <v8.h>
#import "bootstrap.h"


class MyObjectHolder {

public:

    MyObjectHolder(double p, v8::Isolate *isolate, v8::Local<v8::Object> lo);

    ~MyObjectHolder();

    CopyablePersistentObject cpo;
    double priority;

};


#endif //HEAP_LIB_OBJECTHOLDER_H

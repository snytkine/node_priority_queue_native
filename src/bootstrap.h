//
// Created by Snytkine, Dmitri (CORP) on 4/23/17.
//

#ifndef HEAP_LIB_BOOTSTRAP_H
#define HEAP_LIB_BOOTSTRAP_H


#include <v8.h>
#include <iostream>


#define DEBUG 0

#if DEBUG
#define LOGD(a) {std::cerr << "[" << __FILE__ << "][" << __FUNCTION__ << "][Line " << __LINE__ << "] " << a << std::endl;}
#define LOGD2(a, b) {std::cerr << "[" << __FILE__ << "][" << __FUNCTION__ << "][Line " << __LINE__ << "] " << a << b << std::endl;}
#else
#define LOGD(...)
#define LOGD2(...)
#endif


typedef v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>> CopyablePersistentObject;
//typedef v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> CopyablePersistentFunction;


#endif //HEAP_LIB_BOOTSTRAP_H

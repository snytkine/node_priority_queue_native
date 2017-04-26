//
// Created by Snytkine, Dmitri (CORP) on 4/25/17.
//
#import "bootstrap.h"
#include "ObjectHolder.h"


QObjectHolder::QObjectHolder(double p, v8::Isolate *isolate, v8::Local<v8::Object> lo) : priority(p) {

    LOGD("ObjectHolder Constructor Called")
    cpo.Reset(isolate, lo);

}

QObjectHolder::~QObjectHolder() {
    LOGD("QObjectHolder destructor called")
}



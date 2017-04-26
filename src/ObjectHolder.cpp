//
// Created by Snytkine, Dmitri (CORP) on 4/25/17.
//
#import "bootstrap.h"
#include "ObjectHolder.h"


QObjectHolder::QObjectHolder(double p, v8::Isolate *isolate, v8::Local<v8::Object> lo) : priority(p) {

    LOGD("ObjectHolder Constructor Called")

    cpo.Reset(isolate, lo);
    //isolate_ = isolate;

}

QObjectHolder::~QObjectHolder() {
    LOGD2("QObjectHolder destructor called for priority=,", priority)
}

QObjectHolder::QObjectHolder(const QObjectHolder &qoh2) {
    LOGD2("QObjectHolder COPY Constructor called for priority=", qoh2.priority);

    cpo = qoh2.cpo; // CopyablePersistent has assignment operator which calls its Copy()

    //cpo.Reset(qoh2.isolate_, qoh2.cpo);
    //isolate_ = qoh2.isolate_;
    priority = qoh2.priority;

}



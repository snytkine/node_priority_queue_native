//
// Created by Snytkine, Dmitri (CORP) on 4/25/17.
//

#include "ObjectHolder.h"


QObjectHolder::QObjectHolder(double p, v8::Isolate *isolate, v8::Local<v8::Value> lo, LocalType t) : priority(p), T_(t) {

    LOGD("ObjectHolder Constructor Called")

    cpo.Reset(isolate, lo);
    isolate_ = isolate;

}

QObjectHolder::~QObjectHolder() {
    LOGD2("QObjectHolder destructor called for priority=", priority)
    cpo.Reset();
}

// copy constructor
QObjectHolder::QObjectHolder(const QObjectHolder &qoh2) {
    LOGD2("QObjectHolder COPY Constructor called for priority=", qoh2.priority);

    //CopyablePersistentObject cpox(qoh2.cpo);
    cpo.Reset(qoh2.isolate_, std::move(qoh2.cpo)); // CopyablePersistent has assignment operator which calls its Copy()

    std::move(qoh2);

    priority = qoh2.priority;

}

// move constructor
QObjectHolder::QObjectHolder(QObjectHolder &&other) {
    LOGD2("QObjectHolder Move Constructor called for priority=", other.priority);

    //cpo = other.cpo.Pass(); // CopyablePersistent has assignment operator which calls its Copy()

    cpo.Reset(other.isolate_, std::move(other.cpo));
    //cpo.Reset(qoh2.isolate_, qoh2.cpo);
    isolate_ = other.isolate_;
    priority = other.priority;

    other.isolate_ = nullptr;
    // Let's not change other's priority, its just an int, and what if other is used again somehow?
}

// move assignment
QObjectHolder &QObjectHolder::operator=(QObjectHolder &&other) {
    LOGD("QObjectHolder Move assignment operator called for priority");
    if (this != &other) {
        cpo = other.cpo.Pass();
        priority = other.priority;
        other.isolate_ = nullptr;
        // Do not mess with other's priority, it's only an int, nothing gained by changing its value
        LOGD2("Moved QObjectHolder with priorit=", priority);
    }
    return *this;
}



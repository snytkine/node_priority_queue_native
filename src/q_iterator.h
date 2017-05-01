//
// Created by Snytkine, Dmitri (CORP) on 4/29/17.
//

#ifndef HEAP_LIB_Q_ITERATOR_H
#define HEAP_LIB_Q_ITERATOR_H


#include <node.h>
#include <node_object_wrap.h>
#include <iostream>

#include <vector>
#include <algorithm>
#include <queue>
#include <memory>

#include "bootstrap.h"
#include "ObjectHolder.h"
#include "mypq.h"


using namespace v8;

class QIter : public node::ObjectWrap {

public:

    static v8::Persistent<v8::Function> ictor;

    static void Init(v8::Local<v8::Object> exports);


    void setQ(std::shared_ptr<HolderQ> hq) {
        hq_ = hq;
    }

private:
    explicit QIter() {};
    explicit QIter(std::shared_ptr<HolderQ> hq): hq_(hq) {};

    static void New(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Next(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Return(const v8::FunctionCallbackInfo<v8::Value> &args);

    std::shared_ptr<HolderQ> hq_;
};


#endif //HEAP_LIB_Q_ITERATOR_H

//
// Created by Snytkine, Dmitri (CORP) on 4/26/17.
//

#ifndef HEAP_LIB_MYPQ_H
#define HEAP_LIB_MYPQ_H

#include <node.h>
#include <node_object_wrap.h>
#include <iostream>

#include <vector>
#include <algorithm>
#include <queue>
#include <memory>

#include "bootstrap.h"
#include "ObjectHolder.h"


using namespace v8;


class PriorityQ : public node::ObjectWrap {

public:
    static void Init(v8::Local<v8::Object> exports);

private:
    explicit PriorityQ();

    explicit PriorityQ(Isolate *isolate, Local<Function> cmp);

    ~PriorityQ() {
        LOGD("~~ NODE_PQ DESTRUCTOR CALLED ~~")
    }

    static void New(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Next(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Return(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Push(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Top(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Pop(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Size(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void GetIterator(const v8::FunctionCallbackInfo<v8::Value> &args);


    //std::pair<LocalType, Local<Value>> top_(Isolate* isolate, std::shared_ptr<HolderQ> hq);

    static v8::Persistent<v8::Function> constructor;
    bool hasComparator_ = false;

    std::shared_ptr<HolderQ> hq_;

};

#endif //HEAP_LIB_MYPQ_H

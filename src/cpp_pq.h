//
// Created by Snytkine, Dmitri (CORP) on 4/23/17.
//

#ifndef HEAP_LIB_CPP_PQ_H
#define HEAP_LIB_CPP_PQ_H


#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>
#include <v8-util.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>
#include "bootstrap.h"
#include "ObjectHolder.h"

//using namespace v8;

//typedef std::function<bool(ObjectHolder, ObjectHolder)> compareHolder;

//typedef std::priority_queue<ObjectHolder, std::vector<ObjectHolder>, compareHolder> HolderQ;

class MyPQ : public node::ObjectWrap {

public:
    static void Init(v8::Local<v8::Object> exports);

    ~MyPQ();


protected:

    //compareHolder fCompare;
    //std::shared_ptr<HolderQ> hq;

    explicit MyPQ(v8::Isolate *isolate);

    static void New(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Push(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Pop(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Top(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Size(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Empty(const v8::FunctionCallbackInfo<v8::Value> &args);

    static v8::Persistent<v8::Function> constructor;
};


#endif //HEAP_LIB_CPP_PQ_H

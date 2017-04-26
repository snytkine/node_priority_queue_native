//
// Created by Snytkine, Dmitri (CORP) on 4/26/17.
//

#ifndef HEAP_LIB_MYPQ_H
#define HEAP_LIB_MYPQ_H

#include <node.h>
#include <node_object_wrap.h>
#include <iostream>
#include <cmath>


#include <vector>
#include <algorithm>
#include <queue>
#include <memory>

#include "bootstrap.h"
#include "ObjectHolder.h"

using namespace v8;


typedef std::function<bool (QObjectHolder, QObjectHolder)> compareHolder;

typedef std::priority_queue<QObjectHolder, std::vector<QObjectHolder>, compareHolder> HolderQ;

class MyPQ : public node::ObjectWrap {
public:
    static void Init(v8::Local<v8::Object> exports);

private:
    explicit MyPQ(double a = 0, double b = 0, double c = 0);
    ~MyPQ() {
    }

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void Push(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void Pop(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void Size(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void At(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Roots(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void GetCoeff(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void SetCoeff(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

    static v8::Persistent<v8::Function> constructor;
    std::shared_ptr<HolderQ> hq;

    double a_;
    double b_;
    double c_;
};

#endif //HEAP_LIB_MYPQ_H

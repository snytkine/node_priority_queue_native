//
// Created by Snytkine, Dmitri (CORP) on 4/26/17.
//

#ifndef HEAP_LIB_MYPQ_H
#define HEAP_LIB_MYPQ_H

#include <node.h>
#include <node_object_wrap.h>
#include <iostream>
#include <cmath>
#include "bootstrap.h"

using namespace v8;


class MyPQ : public node::ObjectWrap {
public:
    static void Init(v8::Local<v8::Object> exports);

private:
    explicit MyPQ(double a = 0, double b = 0, double c = 0)
            : a_(a), b_(b), c_(c) {}
    ~MyPQ() {}

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void At(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Roots(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void GetCoeff(Local<String> property, const PropertyCallbackInfo<Value>& info);
    static void SetCoeff(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

    static v8::Persistent<v8::Function> constructor;
    double a_;
    double b_;
    double c_;
};

#endif //HEAP_LIB_MYPQ_H

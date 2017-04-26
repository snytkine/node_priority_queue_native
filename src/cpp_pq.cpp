//
// Created by Snytkine, Dmitri (CORP) on 4/23/17.
//

#include "cpp_pq.h"
#include <v8.h>
#include <iostream>


using namespace v8;


using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;

Persistent<Function> MyPQ::constructor;

MyPQ::MyPQ(Isolate *isolate) {

    /*auto compare = [&](const ObjectHolder &lhs, const ObjectHolder &rhs) -> bool {
        return lhs.priority < rhs.priority;
    };

    hq = std::make_shared<HolderQ>(compare);*/
}


void MyPQ::Init(Local<Object> exports) {
    Isolate *isolate = exports->GetIsolate();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "PriorityQueueNative"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    //NODE_SET_PROTOTYPE_METHOD(tpl, "top", Top);
    //NODE_SET_PROTOTYPE_METHOD(tpl, "pop", Pop);
    //NODE_SET_PROTOTYPE_METHOD(tpl, "push", Push);
    NODE_SET_PROTOTYPE_METHOD(tpl, "size", Size);
    //NODE_SET_PROTOTYPE_METHOD(tpl, "empty", Empty);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "PriorityQueueNative"), tpl->GetFunction());
}


void MyPQ::New(const FunctionCallbackInfo<Value> &args) {

    Isolate *isolate = args.GetIsolate();

    if (args.IsConstructCall()) {

        if (args[0]->IsObject()) {
            LOGD("Constructor called with Object");
        }

        MyPQ *obj = new MyPQ(isolate);


        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        isolate->ThrowException(
                String::NewFromUtf8(isolate, "Must use new keyword to create new instance of object"));
        return;

    }
}


void MyPQ::Push(const FunctionCallbackInfo<Value> &args) {

    Isolate *isolate = args.GetIsolate();
    MyPQ *obj = node::ObjectWrap::Unwrap<MyPQ>(args.Holder());

   /* Local<Object> loc = args[0]->ToObject();
    Local<Number> num = args[1]->ToNumber(isolate);
    double d = num->NumberValue();

    obj->hq->emplace(d, isolate, loc);*/

}


void MyPQ::Size(const FunctionCallbackInfo<Value> &args) {

    /*Isolate *isolate = args.GetIsolate();
    MyPQ *obj = node::ObjectWrap::Unwrap<MyPQ>(args.Holder());

    size_t d = obj->hq->size();
    args.GetReturnValue().Set(Number::New(isolate, d));*/

}


void MyPQ::Empty(const FunctionCallbackInfo<Value> &args) {

    /*Isolate *isolate = args.GetIsolate();
    MyPQ *obj = node::ObjectWrap::Unwrap<MyPQ>(args.Holder());

    size_t d = obj->hq->size();
    bool e = d > 0;
    args.GetReturnValue().Set(Boolean::New(isolate, e));*/

}


void MyPQ::Top(const FunctionCallbackInfo<Value> &args) {

    /*Isolate *isolate = args.GetIsolate();
    MyPQ *obj = node::ObjectWrap::Unwrap<MyPQ>(args.Holder());

    size_t d = obj->hq->size();

    if (d > 0) {

        ObjectHolder oh = obj->hq->top();
        CopyablePersistentObject cpo = oh.cpo;

        Local<Object> lo = cpo.Get(isolate);

        args.GetReturnValue().Set(lo);
    }*/

}


void MyPQ::Pop(const FunctionCallbackInfo<Value> &args) {

    /*Isolate *isolate = args.GetIsolate();
    MyPQ *obj = node::ObjectWrap::Unwrap<MyPQ>(args.Holder());

    size_t d = obj->hq->size();

    if (d > 0) {

        ObjectHolder oh = obj->hq->top();
        CopyablePersistentObject cpo = oh.cpo;

        Local<Object> lo = cpo.Get(isolate);

        obj->hq->pop();
        args.GetReturnValue().Set(lo);
    }*/

}


NODE_MODULE(mypq, MyPQ::Init)




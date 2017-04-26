#include <node.h>
#include <node_object_wrap.h>
#include <iostream>
#include <cmath>
#include <memory>
#include <cstdio>
#include "bootstrap.h"
#include "ObjectHolder.h"
#include "mypq.h"


using namespace v8;


Persistent<Function> MyPQ::constructor;

void MyPQ::At(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    double x = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    MyPQ *poly = Unwrap<MyPQ>(args.Holder());

    double results = x * x * poly->a_ + x * poly->b_ + poly->c_;

    args.GetReturnValue().Set(Number::New(isolate, results));
}

void MyPQ::Roots(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    MyPQ *poly = Unwrap<MyPQ>(args.Holder());

    Local<Array> roots = Array::New(isolate);
    double desc = poly->b_ * poly->b_ - (4 * poly->a_ * poly->c_);
    if (desc >= 0) {
        double r = (-poly->b_ + sqrt(desc)) / (2 * poly->a_);
        roots->Set(0, Number::New(isolate, r));
        if (desc > 0) {
            r = (-poly->b_ - sqrt(desc)) / (2 * poly->a_);
            roots->Set(1, Number::New(isolate, r));
        }
    }
    args.GetReturnValue().Set(roots);
}


void MyPQ::Push(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    MyPQ *obj = Unwrap<MyPQ>(args.Holder());

    Local<Object> lo = args[0]->ToObject(isolate);
    Local<Number> ln = args[1]->ToNumber(isolate);
    double d = ln->NumberValue();
    LOGD2("~ Adding to hq with priority=", d);

    //QObjectHolder qoh = QObjectHolder(d, isolate, lo);

    //LOGD("qoh created");


    obj->hq->emplace(d, isolate, lo);
    LOGD("Item emplaced");
}


void MyPQ::Pop(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    MyPQ *obj = Unwrap<MyPQ>(args.Holder());

    if (obj->hq->size() > 0) {
        LOGD("Inside Pop :: Have items in queue")
        CopyablePersistentObject cpo = obj->hq->top().cpo;
        Local<Object> lo = cpo.Get(isolate);
        LOGD("Before hq->pop()")
        obj->hq->pop();
        args.GetReturnValue().Set(lo);
    } else {
        LOGD("NO ITEMS IN QUEUE")
    }

}


void MyPQ::Size(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    MyPQ *obj = Unwrap<MyPQ>(args.Holder());


    LOGD("Looking for size")
    size_t sz = obj->hq->size();
    //Local<Object> lo = cpo.Get(isolate);
    args.GetReturnValue().Set(Number::New(isolate, sz));


}


void MyPQ::GetCoeff(Local<String> property, const PropertyCallbackInfo<Value> &info) {
    Isolate *isolate = info.GetIsolate();
    MyPQ *obj = Unwrap<MyPQ>(info.This());

    v8::String::Utf8Value s(property);
    std::string str(*s);

    if (str == "a") {
        info.GetReturnValue().Set(Number::New(isolate, obj->a_));
    } else if (str == "b") {
        info.GetReturnValue().Set(Number::New(isolate, obj->b_));
    } else if (str == "c") {
        info.GetReturnValue().Set(Number::New(isolate, obj->c_));
    }

}

void MyPQ::SetCoeff(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void> &info) {
    MyPQ *obj = Unwrap<MyPQ>(info.This());

    v8::String::Utf8Value s(property);
    std::string str(*s);

    if (str == "a") {
        obj->a_ = value->NumberValue();
    } else if (str == "b") {
        obj->b_ = value->NumberValue();
    } else if (str == "c") {
        obj->c_ = value->NumberValue();
    }
}


void MyPQ::Init(v8::Local<v8::Object> exports) {

    //
    Isolate *isolate = exports->GetIsolate();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "PriorityQueueNative"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "at", At);
    NODE_SET_PROTOTYPE_METHOD(tpl, "roots", Roots);


    NODE_SET_PROTOTYPE_METHOD(tpl, "push", Push);
    NODE_SET_PROTOTYPE_METHOD(tpl, "pop", Pop);
    NODE_SET_PROTOTYPE_METHOD(tpl, "size", Size);

    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "a"), GetCoeff, SetCoeff);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "b"), GetCoeff, SetCoeff);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "c"), GetCoeff, SetCoeff);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "PriorityQueueNative"),
                 tpl->GetFunction());


    //
}

void MyPQ::New(const v8::FunctionCallbackInfo<v8::Value> &args) {

    //
    Isolate *isolate = args.GetIsolate();

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new Polynomial(...)`
        double a = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        double b = args[1]->IsUndefined() ? 0 : args[1]->NumberValue();
        double c = args[2]->IsUndefined() ? 0 : args[2]->NumberValue();
        MyPQ *obj = new MyPQ(a, b, c);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {

        // Invoked as plain function `Polynomial(...)`, turn into construct call.
        const int argc = 3;
        Local<Value> argv[argc] = {args[0], args[1], args[2]};
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        args.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }

}

MyPQ::MyPQ(double a, double b, double c)
        : a_(a), b_(b), c_(c) {

    auto compare = [&](const QObjectHolder &lhs, const QObjectHolder &rhs) -> bool {
        return lhs.priority < rhs.priority;
    };

    //std::shared_ptr sp;

    hq = std::make_shared<HolderQ>(compare);
}

void InitAll(Local<Object> exports) {
    MyPQ::Init(exports);
}

NODE_MODULE(mypq, InitAll)




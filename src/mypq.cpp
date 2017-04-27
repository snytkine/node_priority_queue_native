#include <node.h>
#include <node_object_wrap.h>
#include <iostream>
#include "bootstrap.h"
#include "ObjectHolder.h"
#include "mypq.h"


using namespace v8;


Persistent<Function> MyPQ::constructor;

void MyPQ::Push(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    MyPQ *obj = Unwrap<MyPQ>(args.Holder());

    Local<Object> lo = args[0]->ToObject(isolate);
    Local<Number> ln = args[1]->ToNumber(isolate);
    double d = ln->NumberValue();
    LOGD2("~ Adding to hq with priority=", d);

    //CopyablePersistentObject up;
    //up.Reset(isolate, lo);
    //QObjectHolder qoh = QObjectHolder(d, isolate, lo);

    //LOGD("qoh created");
    //std::make_shared<QObjectHolder>(d, isolate, lo);

    obj->hq->push(std::make_shared<QObjectHolder>(d, isolate, lo));
    LOGD("Item emplaced");
}


void MyPQ::Pop(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    MyPQ *obj = Unwrap<MyPQ>(args.Holder());

    if (obj->hq->size() > 0) {
        LOGD("Inside Pop :: Have items in queue")

        Local<Object> lo = obj->hq->top()->cpo.Get(isolate);
        LOGD("Before hq->pop()")

        obj->hq->pop();
        // call .Reset on cpo because we not going to need it anymore
        // because it was just gone from the queue!
        args.GetReturnValue().Set(lo);
    } else {
        LOGD("NO ITEMS IN QUEUE")
    }

}



void MyPQ::Top(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    MyPQ *obj = Unwrap<MyPQ>(args.Holder());

    if (obj->hq->size() > 0) {
        LOGD("Inside Top :: Have items in queue")

        Local<Object> lo = obj->hq->top()->cpo.Get(isolate);
        args.GetReturnValue().Set(lo);
    } else {
        LOGD("NO ITEMS IN QUEUE. WILL RETURN UNDEFINED TO TOP")
    }

}


void MyPQ::Size(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    MyPQ *obj = Unwrap<MyPQ>(args.Holder());


    LOGD("Looking for size")
    size_t sz = obj->hq->size();

    args.GetReturnValue().Set(Number::New(isolate, sz));

}


void MyPQ::Init(v8::Local<v8::Object> exports) {

    //
    Isolate *isolate = exports->GetIsolate();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "PriorityQueueNative"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);


    NODE_SET_PROTOTYPE_METHOD(tpl, "push", Push);
    NODE_SET_PROTOTYPE_METHOD(tpl, "top", Top);
    NODE_SET_PROTOTYPE_METHOD(tpl, "pop", Pop);
    NODE_SET_PROTOTYPE_METHOD(tpl, "size", Size);;

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "PriorityQueueNative"),
                 tpl->GetFunction());


    //
}

void MyPQ::New(const v8::FunctionCallbackInfo<v8::Value> &args) {

    //
    Isolate *isolate = args.GetIsolate();

    if (args.IsConstructCall()) {
        // Invoked as constructor
        MyPQ *obj = new MyPQ();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {

        // Invoked as plain function, turn into construct call.
        const int argc = 3;
        Local<Value> argv[argc] = {args[0], args[1], args[2]};
        Local<Function> cons = Local<Function>::New(isolate, constructor);

        MaybeLocal<Object> o = cons->NewInstance(isolate->GetCurrentContext(), argc, argv);
        args.GetReturnValue().Set(o.ToLocalChecked());
    }

}

MyPQ::MyPQ() {

    auto compare = [&](const std::shared_ptr<QObjectHolder> &lhs, const std::shared_ptr<QObjectHolder> &rhs) -> bool {
        return lhs->priority < rhs->priority;
    };

    //std::shared_ptr sp;

    hq = std::make_shared<HolderQ>(compare);
}

void InitAll(Local<Object> exports) {
    MyPQ::Init(exports);
}

NODE_MODULE(mypq, InitAll)




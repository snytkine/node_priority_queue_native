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
    double d = 0;
    Local<Object> lo = args[0]->ToObject(isolate);
    // Check for second arg requirements
    // its required ONLY if there is no comparator
    if (!obj->hasComparator()) {
        if (args.Length() < 2) {
            isolate->ThrowException(v8::Exception::TypeError(
                    String::NewFromUtf8(isolate,
                                        "PriorityQueue instantiated without comparator function. Must pass a priority number as second argument to this function")));

            return;
        } else if (!args[1]->IsNumber()) {
            isolate->ThrowException(v8::Exception::TypeError(
                    String::NewFromUtf8(isolate, "Second parameter must be a number")));

        }

        Local<Number> ln = args[1]->ToNumber(isolate);
        d = ln->NumberValue();
        LOGD2("~ Adding to hq with priority=", d);
    }

    obj->hq->push(std::make_shared<QObjectHolder>(d, isolate, lo));
    LOGD("Item pushed to queue");
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

}

void MyPQ::New(const v8::FunctionCallbackInfo<v8::Value> &args) {

    Isolate *isolate = args.GetIsolate();

    if (args.IsConstructCall()) {
        MyPQ *obj;
        // Invoked as constructor
        if (args[0]->IsFunction()) {
            Local<Function> cmp = Local<Function>::Cast(args[0]);
            obj = new MyPQ(isolate, cmp);
        } else {
            obj = new MyPQ();
        }

        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {

        // Invoked as plain function, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = {args[0]};
        Local<Function> cons = Local<Function>::New(isolate, constructor);

        MaybeLocal<Object> o = cons->NewInstance(isolate->GetCurrentContext(), argc, argv);
        args.GetReturnValue().Set(o.ToLocalChecked());
    }

}

MyPQ::MyPQ() {

    auto compare = [&](const std::shared_ptr<QObjectHolder> &lhs, const std::shared_ptr<QObjectHolder> &rhs) -> bool {

        //LOGD("CALLING NATIVE COMPATATOR")
        return lhs->priority < rhs->priority;
        //LOGD2("Result of comparotor call is ~~~~~~~~~ ", ret)
        //LOGD2("LHS=", lhs->priority)
        //LOGD2("RHS=", rhs->priority)

        //return ret;
    };

    hq = std::make_shared<HolderQ>(compare);
}


MyPQ::MyPQ(Isolate *isolate, Local<Function> cmp) {

    LOGD("Setting up comparotor")
    if (cmp.IsEmpty()) {
        LOGD("cmp is EMPTY!");
    } else {
        LOGD("CMP WAS NOT EMPTY")
    }

    hasComparator_ = true;

    CopyablePersistentObject pcmp;
    pcmp.Reset(isolate, cmp);

    auto ptrCmp = std::make_shared<CopyablePersistentObject>(std::move(pcmp));

    auto compare = [ptrCmp, isolate](const std::shared_ptr<QObjectHolder> &lhs,
                                     const std::shared_ptr<QObjectHolder> &rhs) -> bool {
        //return lhs->priority < rhs->priority;
        LOGD("CALLING JS COMPARATOR !")
        Local<Object> g = ptrCmp->Get(isolate);
        if (g.IsEmpty()) {
            LOGD("comparotor was empty inside persistent")
        } else {
            LOGD("cmp was NOT empty in persistent")
        }
        Local<Function> fn = Local<Function>::Cast(g);
        LOGD("CP AFTER CAST")
        Handle<Value> argv[2];
        Local<Object> o1 = lhs->cpo.Get(isolate);
        argv[0] = o1;
        LOGD("AFTER ARG0")
        Local<Object> o2 = rhs->cpo.Get(isolate);
        argv[1] = o2;
        LOGD("ADTER ARG2")
        MaybeLocal<Value> res = fn->Call(Null(isolate), 2, argv);
        if (res.IsEmpty()) {
            LOGD("!!!!!!!!!!!!!! BAD FUNCTION CALL. No RESULT !!!!!!!!")
            return false;
        } else {
            return res.ToLocalChecked()->BooleanValue();
            //LOGD2("Result of comparotor call is ~~~~~~~~~ ", ret);

            //return ret;
        }

    };

    hq = std::make_shared<HolderQ>(compare);
}

void InitAll(Local<Object> exports) {
    MyPQ::Init(exports);
}

NODE_MODULE(mypq, InitAll)




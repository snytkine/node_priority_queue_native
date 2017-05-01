#include <node.h>
#include <node_object_wrap.h>
#include <iostream>
#include "bootstrap.h"
#include "ObjectHolder.h"
#include "mypq.h"

using namespace v8;

Persistent<Function> PriorityQ::constructor;

void PriorityQ::Push(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    PriorityQ *obj = Unwrap<PriorityQ>(args.Holder());
    double d = 0;
    Local<Object> lo = args[0]->ToObject(isolate);
    // Check for second arg requirements
    // its required ONLY if there is no comparator
    if (!obj->hasComparator()) {
        /*if (args.Length() < 2) {
            isolate->ThrowException(v8::Exception::TypeError(
                    String::NewFromUtf8(isolate,
                                        "PriorityQueue instantiated without comparator function. Must pass a priority number as second argument to this function")));

            return;
        } else if (!args[1]->IsNumber()) {
            isolate->ThrowException(v8::Exception::TypeError(
                    String::NewFromUtf8(isolate, "Second parameter must be a number")));

        }*/

        Local<Number> ln = args[1]->ToNumber(isolate);
        d = ln->NumberValue();
        LOGD2("~ Adding to hq_ with priority=", d);
    }

    obj->hq_->push(std::make_shared<QObjectHolder>(d, isolate, lo));
    LOGD("Item pushed to queue");
}


void PriorityQ::Pop(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    PriorityQ *obj = Unwrap<PriorityQ>(args.Holder());

    // Very important to check size first
    // if queue is empty then calling top and pop
    // will result in segmentation fault
    // wrapping this inside native try/catch will not help
    if (obj->hq_->size() > 0) {
        LOGD("Inside Pop :: Have items in queue")
        // if we have intermediate copy obj->hq_->top()
        // and then use it to Get(isolate) then destructor may be called on that intermediate UniquePersistent
        // because it's not moved, its copied, and then it will reset the actual stored object in v8
        // if we want to use intermediate object then we must use move assignment instead of copy assignment

        Local<Object> lo = obj->hq_->top()->cpo.Get(isolate);
        LOGD("Before hq_->pop()")

        obj->hq_->pop();
        // call .Reset on cpo because we not going to need it anymore
        // because it was just gone from the queue!
        // Except that in case of UniquePersistent there is already a destructor that calls Reset and it will be called
        // when element is removed from que. To be 100% safe we can check if cpo.IsEmpty()
        // but that would only work with intermediate objects.

        args.GetReturnValue().Set(lo);

    } else {
        LOGD("NO ITEMS IN QUEUE")

    }

}


void PriorityQ::Top(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    PriorityQ *obj = Unwrap<PriorityQ>(args.Holder());

    if (obj->hq_->size() > 0) {
        LOGD("Inside Top :: Have items in queue")

        Local<Object> lo = obj->hq_->top()->cpo.Get(isolate);
        args.GetReturnValue().Set(lo);
    } else {
        LOGD("NO ITEMS IN QUEUE. WILL RETURN UNDEFINED TO TOP")
    }

}


void PriorityQ::Size(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    PriorityQ *obj = Unwrap<PriorityQ>(args.Holder());


    LOGD("Looking for size")
    size_t sz = obj->hq_->size();

    args.GetReturnValue().Set(Number::New(isolate, sz));

}


void PriorityQ::Init(v8::Local<v8::Object> exports) {

    Isolate *isolate = exports->GetIsolate();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "PriorityQueueNative"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);


    NODE_SET_PROTOTYPE_METHOD(tpl, "push", Push);
    NODE_SET_PROTOTYPE_METHOD(tpl, "top", Top);
    NODE_SET_PROTOTYPE_METHOD(tpl, "pop", Pop);
    NODE_SET_PROTOTYPE_METHOD(tpl, "size", Size);

    NODE_SET_ITERATOR_METHOD(tpl, GetIterator);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "PriorityQueueNative"),
                 tpl->GetFunction());

}

void PriorityQ::New(const v8::FunctionCallbackInfo<v8::Value> &args) {

    Isolate *isolate = args.GetIsolate();

    if (args.IsConstructCall()) {
        PriorityQ *obj;
        // Invoked as constructor
        if (args[0]->IsFunction()) {
            Local<Function> cmp = Local<Function>::Cast(args[0]);
            obj = new PriorityQ(isolate, cmp);
        } else {
            obj = new PriorityQ();
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

PriorityQ::PriorityQ() {

    auto compare = [&](const std::shared_ptr<QObjectHolder> &lhs, const std::shared_ptr<QObjectHolder> &rhs) -> bool {

        return lhs->priority < rhs->priority;
    };

    hq_ = std::make_shared<HolderQ>(compare);
}


PriorityQ::PriorityQ(Isolate *isolate, Local<Function> cmp) {
    HandleScope handle_scope(isolate);
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
        }

    };

    hq_ = std::make_shared<HolderQ>(compare);
}


void PriorityQ::GetIterator(const v8::FunctionCallbackInfo<v8::Value> &args) {

    Isolate *isolate = args.GetIsolate();

    Local<Value> argv[1] = {args.Holder()};

    Local<Function> cons = Local<Function>::New(isolate, QIter::ictor);
    MaybeLocal<Object> maybeit = cons->NewInstance(isolate->GetCurrentContext(), 1, argv);
    if(!maybeit.IsEmpty()){
        args.GetReturnValue().Set(maybeit.ToLocalChecked());
    }

}

void InitAll(Local<Object> exports) {
    PriorityQ::Init(exports);
    QIter::Init(exports);
}

NODE_MODULE(mypq, InitAll)




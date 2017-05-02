#include <node.h>
#include <node_object_wrap.h>
#include <iostream>
#include "bootstrap.h"
#include "ObjectHolder.h"
#include "priorityqueue_native.h"

using namespace v8;

Persistent<Function> PriorityQ::constructor;


PriorityQ::PriorityQ() {

    auto compare = [](const std::shared_ptr<QObjectHolder> &lhs, const std::shared_ptr<QObjectHolder> &rhs) -> bool {

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

        Local<Value> o1 = lhs->cpo.Get(isolate);
        argv[0] = o1;
        LOGD("AFTER ARG0")

        Local<Value> o2 = rhs->cpo.Get(isolate);
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


void PriorityQ::Init(v8::Local<v8::Object> exports) {

    Isolate *isolate = exports->GetIsolate();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "priority_queue_native"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(tpl, "push", Push);
    NODE_SET_PROTOTYPE_METHOD(tpl, "top", Top);
    NODE_SET_PROTOTYPE_METHOD(tpl, "pop", Pop);
    NODE_SET_PROTOTYPE_METHOD(tpl, "size", Size);
    NODE_SET_PROTOTYPE_METHOD(tpl, "next", Next);

    NODE_SET_ITERATOR_METHOD(tpl, GetIterator);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "PriorityQueue"),
                 tpl->GetFunction());

    exports->Set(String::NewFromUtf8(isolate, "PriorityQueueCompare"),
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


void PriorityQ::Push(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    PriorityQ *obj = Unwrap<PriorityQ>(args.Holder());
    double d = 0;
    LocalType t;
    Local<Value> lo;
    if (args[0]->IsNumber()) {
        t = LocalType::NUMBER;
        lo = args[0]->ToNumber(isolate);
    } else if (args[0]->IsString()) {
        t = LocalType::STRING;
        lo = args[0]->ToString(isolate);
    } else if (args[0]->IsBoolean()) {
        t = LocalType::BOOLEAN;
        lo = args[0]->ToBoolean(isolate);
    } else {
        t = LocalType::OBJECT;
        lo = args[0]->ToObject(isolate);
    }


    // Check for second arg requirements
    // its required ONLY if there is no comparator
    if (!obj->hasComparator_) {
        Local<Number> ln = args[1]->ToNumber(isolate);
        d = ln->NumberValue();
        LOGD2("~ Adding to hq_ with priority=", d);
    }

    obj->hq_->push(std::make_shared<QObjectHolder>(d, isolate, lo, t));
    LOGD("Item pushed to queue");
}


Local<Value> PriorityQ::Top_(const v8::FunctionCallbackInfo<v8::Value> &args) {

    Isolate *isolate = args.GetIsolate();
    EscapableHandleScope my_handle_scope(isolate);
    PriorityQ *obj = Unwrap<PriorityQ>(args.Holder());
    //Local<Value> ret;

    if (obj->hq_->size() > 0) {
        LOGD("Inside Top :: Have items in queue")

        Local<Value> lo = obj->hq_->top()->cpo.Get(isolate);
        LocalType t = obj->hq_->top()->T_;


        switch (t) {
            case LocalType::NUMBER: LOGD("RETURNING AS NUMBER")
                //args.GetReturnValue().Set(lo->ToNumber(isolate));
                return my_handle_scope.Escape(lo->ToNumber(isolate));
                break;

            case LocalType::STRING: LOGD("RETURNING AS STRING")
                //args.GetReturnValue().Set(lo->ToString(isolate));
                return my_handle_scope.Escape(lo->ToString(isolate));
                break;

            case LocalType::BOOLEAN: LOGD("RETURNING AS BOOLEAN")
                //args.GetReturnValue().Set(lo->ToBoolean(isolate));
                return my_handle_scope.Escape(lo->ToBoolean(isolate));
                break;

            default: LOGD("RETURNING AS OBJECT")
                //args.GetReturnValue().Set(lo->ToObject(isolate));
                return my_handle_scope.Escape(lo->ToObject(isolate));

        }

    } else {
        LOGD("NO ITEMS IN QUEUE. WILL RETURN UNDEFINED TO TOP")
        return my_handle_scope.Escape(Undefined(isolate));
    }

}


void PriorityQ::Top(const v8::FunctionCallbackInfo<v8::Value> &args) {

    args.GetReturnValue().Set(Top_(args));

}


void PriorityQ::Pop(const v8::FunctionCallbackInfo<v8::Value> &args) {
    LOGD("ENTERED POP");
    PriorityQ *obj = Unwrap<PriorityQ>(args.Holder());

    // Very important to check size first
    // if queue is empty then calling top and pop
    // will result in segmentation fault
    // wrapping this inside native try/catch will not help
    args.GetReturnValue().Set(Top_(args));
    if (obj->hq_->size() > 0) {

        obj->hq_->pop();
    }

}


void PriorityQ::Size(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    PriorityQ *obj = Unwrap<PriorityQ>(args.Holder());


    LOGD("Looking for size")
    size_t sz = obj->hq_->size();

    args.GetReturnValue().Set(Number::New(isolate, sz));

}


void PriorityQ::GetIterator(const v8::FunctionCallbackInfo<v8::Value> &args) {

    args.GetReturnValue().Set(args.This());
}


void PriorityQ::Next(const FunctionCallbackInfo<Value> &args) {

    Isolate *isolate = args.GetIsolate();
    PriorityQ *obj = Unwrap<PriorityQ>(args.Holder());

    Local<Object> retObj = Object::New(isolate);

    Local<Value> lv;

    if (obj->hq_->size() > 0) {
        LOGD("Inside Pop :: Have items in queue")
        lv = Top_(args);
        obj->hq_->pop();
        // now we must create local object with properties that iterator
        // is expected to return
        //
        retObj->Set(String::NewFromUtf8(isolate, ITER_VALUE), lv);
        retObj->Set(String::NewFromUtf8(isolate, ITER_DONE), Boolean::New(isolate, false));


    } else {
        LOGD("ITERATOR::NEXT NO ITEMS IN QUEUE")
        retObj->Set(String::NewFromUtf8(isolate, ITER_VALUE), Undefined(isolate));
        retObj->Set(String::NewFromUtf8(isolate, ITER_DONE), Boolean::New(isolate, true));
    }

    args.GetReturnValue().Set(retObj);
}


// If implementing optional return method of iterator
// its very important that it returns IteratorResult from it, otherwise
// some v8 versions will crash.
void PriorityQ::Return(const v8::FunctionCallbackInfo<v8::Value> &args) {
    LOGD("^^^^^^^^^^^^^ ITER::Return called ^^^^^^^^^^^^^");
    Isolate *isolate = args.GetIsolate();
    Local<Object> retObj = Object::New(isolate);
    retObj->Set(String::NewFromUtf8(isolate, ITER_DONE), Boolean::New(isolate, true));
    args.GetReturnValue().Set(retObj);
}


void InitAll(Local<Object> exports) {
    PriorityQ::Init(exports);
}

NODE_MODULE(mypq, InitAll)




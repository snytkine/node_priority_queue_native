//
// Created by Snytkine, Dmitri (CORP) on 4/29/17.
//

#include "q_iterator.h"


Persistent<Function> QIter::ictor;

void QIter::Next(const FunctionCallbackInfo<Value> &args) {

    Isolate *isolate = args.GetIsolate();
    QIter *obj = Unwrap<QIter>(args.Holder());

    Local<Object> retObj = Object::New(isolate);


    if (obj->hq_->size() > 0) {
        LOGD("Inside Pop :: Have items in queue")


        Local<Object> lo = obj->hq_->top()->cpo.Get(isolate);
        LOGD("Before hq_->pop()")

        obj->hq_->pop();
        // now we must create local object with properties that iterator
        // is expected to return
        //
        retObj->Set(String::NewFromUtf8(isolate, ITER_VALUE), lo);
        retObj->Set(String::NewFromUtf8(isolate, ITER_DONE), Boolean::New(isolate, false));


    } else {
        LOGD("ITERATOR::NEXT NO ITEMS IN QUEUE")
        retObj->Set(String::NewFromUtf8(isolate, ITER_VALUE), Undefined(isolate));
        retObj->Set(String::NewFromUtf8(isolate, ITER_DONE), Boolean::New(isolate, true));
    }

    args.GetReturnValue().Set(retObj);
}

void QIter::Init(v8::Local<v8::Object> exports) {
    Isolate *isolate = exports->GetIsolate();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "@@iterator"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);


    NODE_SET_PROTOTYPE_METHOD(tpl, "next", Next);
    NODE_SET_PROTOTYPE_METHOD(tpl, "return", Return);

    ictor.Reset(isolate, tpl->GetFunction());
    // We don't add this constructor to exports because this class is not
    // should not be available as a stand-alone module class
}

void QIter::New(const v8::FunctionCallbackInfo<v8::Value> &args) {
    Isolate *isolate = args.GetIsolate();
    LOGD("Entered QIter JS CONSTRUCTOR")
    if (args.IsConstructCall()) {
        LOGD("QIter JS CALLED WITH NEW")
        QIter *obj;
        // Invoked as constructor
        if (args[0]->IsObject()) {
            Local<Object> pq = args[0]->ToObject(isolate);
            MyPQ *lpq = Unwrap<MyPQ>(pq);

            obj = new QIter(lpq->getQ());
            obj->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        } else {
            LOGD("ITER object not passed in JS Constructor call")
        }
    } else {

        LOGD("ITER NOT CONSTRUCTOR CALL")
    }
}

void QIter::Return(const v8::FunctionCallbackInfo<v8::Value> &args) {
    LOGD("^^^^^^^^^^^^^ ITER::Return called ^^^^^^^^^^^^^");
    Isolate *isolate = args.GetIsolate();
    Local<Object> retObj = Object::New(isolate);
    retObj->Set(String::NewFromUtf8(isolate, ITER_DONE), Boolean::New(isolate, true));
    args.GetReturnValue().Set(retObj);
}

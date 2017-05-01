//
// Created by Snytkine, Dmitri (CORP) on 4/23/17.
//

#ifndef HEAP_LIB_BOOTSTRAP_H
#define HEAP_LIB_BOOTSTRAP_H


#include <v8.h>
#include <iostream>



inline void NODE_SET_ITERATOR_METHOD(v8::Local<v8::FunctionTemplate> recv,
                                      v8::FunctionCallback callback) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Signature> s = v8::Signature::New(isolate, recv);
    v8::Local<v8::FunctionTemplate> t =
            v8::FunctionTemplate::New(isolate, callback, v8::Local<v8::Value>(), s);
    v8::Local<v8::String> fn_name = v8::String::NewFromUtf8(isolate, "@@iterator");
    t->SetClassName(fn_name);
    recv->PrototypeTemplate()->Set(v8::Symbol::GetIterator(isolate), t);
}

#define DEBUG 0

static const char ITER_NEXT[] = "next";
static const char ITER_VALUE[] = "value";
static const char ITER_DONE[] = "done";

#if DEBUG
#define LOGD(a) {std::cerr << "[" << __FILE__ << "][" << __FUNCTION__ << "][Line " << __LINE__ << "] " << a << std::endl;}
#define LOGD2(a, b) {std::cerr << "[" << __FILE__ << "][" << __FUNCTION__ << "][Line " << __LINE__ << "] " << a << b << std::endl;}
#else
#define LOGD(...)
#define LOGD2(...)
#endif


typedef v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>> CopyablePersistent;

typedef v8::UniquePersistent<v8::Object> CopyablePersistentObject;
//typedef v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> CopyablePersistentFunction;


#endif //HEAP_LIB_BOOTSTRAP_H

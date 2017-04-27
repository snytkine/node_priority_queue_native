//
// Created by Snytkine, Dmitri (CORP) on 4/26/17.
//

#ifndef HEAP_LIB_MYPQ_H
#define HEAP_LIB_MYPQ_H

#include <node.h>
#include <node_object_wrap.h>
#include <iostream>

#include <vector>
#include <algorithm>
#include <queue>
#include <memory>

#include "bootstrap.h"
#include "ObjectHolder.h"

using namespace v8;


typedef std::function<bool(std::shared_ptr<QObjectHolder>, std::shared_ptr<QObjectHolder>)> compareHolder;

typedef std::priority_queue<std::shared_ptr<QObjectHolder>, std::vector<std::shared_ptr<QObjectHolder>>, compareHolder> HolderQ;

class MyPQ : public node::ObjectWrap {
public:
    static void Init(v8::Local<v8::Object> exports);
    bool hasComparator(){
        return hasComparator_;
    }

private:
    explicit MyPQ();
    explicit MyPQ(Isolate* isolate, Local<Function> cmp);

    ~MyPQ() {
        LOGD("~~ MYPQ DESTRUCTOR CALLED ~~")
    }

    static void New(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Push(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Top(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Pop(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Size(const v8::FunctionCallbackInfo<v8::Value> &args);


    static v8::Persistent<v8::Function> constructor;
    std::shared_ptr<HolderQ> hq;
    bool hasComparator_ = false;
    //v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>> comparator_;
};

#endif //HEAP_LIB_MYPQ_H

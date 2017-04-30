//
// Created by Snytkine, Dmitri (CORP) on 4/25/17.
//

#ifndef HEAP_LIB_OBJECTHOLDER_H
#define HEAP_LIB_OBJECTHOLDER_H


#include <node.h>
#include <iostream>
#include <queue>
#include <vector>
#include <v8.h>
#include "bootstrap.h"


class QObjectHolder {

public:

    QObjectHolder(double p, v8::Isolate *isolate, v8::Local<v8::Object> lo);
    //QObjectHolder(double p, CopyablePersistentObject o);

    QObjectHolder(const QObjectHolder &qoh2);
    QObjectHolder(QObjectHolder&& other);
    QObjectHolder& operator=(QObjectHolder&& other);

    ~QObjectHolder();

    CopyablePersistentObject cpo;
    double priority;
    v8::Isolate *isolate_;

};


typedef std::function<bool(std::shared_ptr<QObjectHolder>, std::shared_ptr<QObjectHolder>)> compareHolder;
typedef std::priority_queue<std::shared_ptr<QObjectHolder>, std::vector<std::shared_ptr<QObjectHolder>>, compareHolder> HolderQ;

#endif //HEAP_LIB_OBJECTHOLDER_H

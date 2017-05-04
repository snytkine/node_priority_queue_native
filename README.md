# priorityqueue_native is a node.js module implementing priority_queue container backed by the C++ std::priority_queue

#### Introduction
A priority queue is a data storage container where you can add items and get items back one at a time.

The order of items will be from highest priority to lowest. That of cause is the main feature of priority queue - regardless of insertion order of items the highest priority item is always at the top of the queue. After you get the highest priority item using pop() method the highest priority from remaining items is at the top of queue again, and so on until the queue is empty.

[More about priority queue on Wikipedia](https://en.wikipedia.org/wiki/Priority_queue)

When there are no items left in queue calling the size() will return 0 and calling top() or pop() will return undefined

Object created by this module also Iterable

Many programming languages have a implementation of priority queue as part of the language, but Javascript and node.js do not have one. There are several node.js modules written in JavaScript that provide functionality of Priority Queue but they all implement it differently. The C++ standard library had a priority queue implementation for a long time (over 20 years now).
[C++ std::priority_queue](https://en.wikipedia.org/wiki/Priority_queue)

This module just uses the C++ priority_queue library and exposes the Javascript object with methods and implementation similar to native C++ library. In addition this module exposes an Iterated via the Symbol.iterator() method which makes this object Iterable, so it can be used as source for spread operator, as source for Array.from or just iterated over in a for..of loop

#
### Installation
```javascript
npm install priorityqueue_native@latest
```
#
### Dependencies
- Minimum version of Node is 5.11.0
- This moduel does not depend on any other node.js modules; however this is a native module, meaning it it written in C++ and must be compiled by node.js
As such, it expects the node-gyp to be installed (*** node-gyp comes with npm by default so usually you don't have to worry about installing node-gyp ***, unless you want to upgrade it, in which case [Read This](https://github.com/nodejs/node-gyp/wiki/Updating-npm's-bundled-node-gyp) ) and you must also have a c++ compiler on the machine. To intall compiler in Windows you need to install Visual Studio. To install C++ compiler on MAC OS you need to install [Xcode](https://developer.apple.com/xcode/)
On Linux you need to have c++ compiler that supports C++ 11
- To use the "for..of" iterator feature Node 6.0 or higher is required. It may work with lower versions with --harmony flag but we cannot guarantee it.


### Usage
- There are 2 ways to use priority queue:
1) With comparator function
2) using number value for item's priority

#### 1. With priority value:
Create new PriorityQueue object, then use 2 parameters for push method
#### push(item, priority)
where priority is a number, can be integer or floating point

```javascript
const pq = require('priorityqueue_native');
let todos = new pq.PriorityQueue();

    todos.push({"task": "Wash Dishes", "category":"house"}, 2)
    todos.push({"task": "Math homework", "category":"school"}, 4)
    todos.push({"task": "History paper", "category":"school"}, 3)
    todos.push({"task": "Walk the dog", "category":"other"}, 5)
    todos.push({"task": "Rotate tires", "category":"car"}, 1)

```
Now you have queue with 5 items and can use following methods to get items out of the queue:

### todos.top();   // get value of first item but do not remove it from queue
### todos.pop();   // removes first item from queue and returns it
### todos.size();  // get number of items in queue

#### PriorityQueue Object is Iterable

Easiest way to iterate over items in queue is to use for..of loop

```javascript

for(let item of todos){
  console.log(JSON.stringify(item);
}

```
Once the for..of loop is finished running the queue is empty and running for..of loop on the same queue will not produce any more results.

You can easily convert the priority queue to Array using Array.from method (ES6 feature)
```javascript
let todoArray = Array.from(todos);
```
Now you have an array and you can iterate over it as many times as you want, you can call .map, .filter, .forEach and any othe array methods.

The reason that Array.from(todos) works is because this PriorityQueue object is iterable
The PriorityQueue is itself an iterator, but it also iterable, meaning that it has Symbol.iterator() method that just returns reference to itself

####  2. With comparator function
Create PriorityQueue passing comparator function to constructor. Then use push() method with a single parameter - the object (or string or number) that can be compared to another one of the same type of items using comparator function.

In this example we pass todo objects that have "priority" field and comparator function that uses this "priority" property for comparing items.

Comparator function must implement "less than" logic using first and second item as parameters.

```javascript
    const pq = require('priorityqueue_native');
    let todos = new pq.PriorityQueue(function (lhs, rhs) {
        return lhs.priority < rhs.priority
    });

    todos.push({"task": "Wash Dishes", "category":"house", "priority": 2});
    todos.push({"task": "Math homework", "category":"school", "priority": 4});
    todos.push({"task": "History paper", "category":"school", "priority": 3});
    todos.push({"task": "Walk the dog", "category":"other", "priority": 5});
    todos.push({"task": "Rotate tires", "category":"car", "priority": 1});


```
You can use this queue the same way as in first example to get items or iterate over items.


### TypeScript friendly!
This module comes with index.d.ts type definition and is ready to be used in typescript project.

In Typescript version this PriorityQueue is a generic class and actually there are 2 different classes - one for each implementation of the queue, with and without comparator function.

Example of Typescript usage:

```typescript
import {PriorityQueue, PriorityQueueCompare} from 'priorityqueue_native'

interface item {
    task: string
    category: string
    priority: number
}

// Option 1 - no comparator function, passing priority to push() method
let todos: PriorityQueue<item> = new PriorityQueue<item>();

todos.push({"task": "Wash Dishes", "category":"house", "priority": 2}, 2);
todos.push({"task": "Math homework", "category":"school", "priority": 4}, 4);
todos.push({"task": "History paper", "category":"school", "priority": 3}, 3);
todos.push({"task": "Walk the dog", "category":"other", "priority": 5}, 5);
todos.push({"task": "Rotate tires", "category":"car", "priority": 1}, 1);


// Option 2. With Comparator function:

let todos2: PriorityQueueCompare<item> = new PriorityQueueCompare<item>((left: item, right: item) => left.priority < right.priority);

todos2.push({"task": "Wash Dishes", "category":"house", "priority": 2});
todos2.push({"task": "Math homework", "category":"school", "priority": 4});
todos2.push({"task": "History paper", "category":"school", "priority": 3});
todos2.push({"task": "Walk the dog", "category":"other", "priority": 5});
todos2.push({"task": "Rotate tires", "category":"car", "priority": 1});
```

#
## Important Performance Considerations

Using the first example, passing the priority value as second parameter to queue is faster that using comparator function. The reason for this is that the priority queue will store that number as native c++ value and comparing items will be super-fast.

Using comparator function is slower because the module must convert this function into persistent object and then every time it has to compare items it must convert that function back to native JS object, then convert 2 items from format stored in c++ into native JS object and then run the JS function inside C++ code. It's not too bad, it's still fast, as fast as any other priority queue node.js module that is written in JavaScript

Performance will always be better if you can generate priority value when you adding item to queue and pass it as second parameter.

#### Performance metric
We have dome a very simple performance test, adding 100 objects to PriorityQueue using method 1 (no comparator function) and then converting the queue to array using Array.from(queue), which basically runs the .pop() 100 times, moving items from queue into the array

These 2 steps - adding 100 items and converting to Array takes between 0.25 to 0.5 millisecond on a Macbook Pro



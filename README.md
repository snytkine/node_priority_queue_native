# priorityqueue_native is a priority queue backed by the C++ std::priority_queue

##### This module uses ES6 Iterable interface (for..of) loop and requires at least version 6 of Node.js or older version using --harmony option when starting older node.js
####
###
#### Introduction
A priority queue is a data storage container where you can add items and get items back one at a time.

The order of items will be from highest priority to lowest. That of cause is the main feature of priority queue - regardless of insertion order of items the highest priority item is always at the top of the queue. After you get the highest priority item using pop() method the highest priority from remaining items is at the top of queue again, and so on until the queue is empty.

When there are no items left in queue calling the size() will return 0 and calling top() or pop() will return undefined

Object created by this module also Iterable


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
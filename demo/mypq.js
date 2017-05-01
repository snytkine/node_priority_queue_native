"use strict";
var priorityqueue_native_1 = require("priorityqueue_native");
var gh = new priorityqueue_native_1.PriorityQueue();
var gh2 = new priorityqueue_native_1.PriorityQueueCompare(function (left, right) { return left.priority < right.priority; });
gh.push({ val: 3, priority: 4 }, 4);
gh2.push({ val: 5, priority: 4 });
console.log("in 2 size: ", gh2.size());

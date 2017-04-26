const pq = require('../src/build/Release/mypq');

console.log("BEFORE CONSTRUCTOR");
let mypq = new pq.PriorityQueueNative(1,3,2);
console.log("AFTER CONSTRUCTOR");

mypq.push({"my": 1}, 1);
console.log("PUSHED ONE");
mypq.push({"y": 6 }, 6);
console.log("PUSHED 6");


mypq.push({"my" : 3}, 3);
console.log("PUSHED 3");

console.log("size: ", mypq.size());

let res = mypq.pop();
console.log("POPPED1: ", JSON.stringify(res));



let res2 = mypq.pop();
console.log("POPPED2: ", JSON.stringify(res2));

console.log("SIZE NOW: ", mypq.size())
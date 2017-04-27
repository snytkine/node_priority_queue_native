const pq = require('../src/build/Release/mypq');
var microtime = require('microtime')



//console.log("BEFORE CONSTRUCTOR");
let mypq = new pq.PriorityQueueNative(1,3,2);
//console.log("AFTER CONSTRUCTOR");
let start = microtime.now();
mypq.push({"my": 1}, 1);
mypq.push({"my": 6 }, 6);
mypq.push({"my" : 3}, 3);
mypq.push({"my" : 22}, 22);
mypq.push({"my" : 12}, 12);
mypq.push({"my" : 212}, 212);
mypq.push({"my" : 32}, 32);
mypq.push({"my" : 332}, 332);
mypq.push({"my" : 442}, 442);
mypq.push({"my" : 444.2}, 444.2);


mypq.push({"my": 11}, 11);
mypq.push({"my": 16 }, 16);
mypq.push({"my" : 13}, 13);
mypq.push({"my" : 122}, 122);
mypq.push({"my" : 112}, 112);
mypq.push({"my" : 1212}, 1212);
mypq.push({"my" : 132}, 132);
mypq.push({"my" : 1332}, 1332);
mypq.push({"my" : 1442}, 1442);
mypq.push({"my" : 1444.2}, 1444.2);
//console.log("PUSHED 2");

//console.log("size: ", mypq.size());

//let res = mypq.pop();
//console.log("POPPED1: ", JSON.stringify(res));



//let res2 = mypq.pop();

//let len = mypq.size();
//console.log("First one: ", mypq.top())
let sum = 0
while(mypq.size() > 0){
    //console.log("POPPED:",mypq.pop());
    var g = mypq.pop();
    sum += g.my;
}

let end = microtime.now();
let total = end - start;
console.log("FINISHED IN ", total);
console.log("SUM=", sum);


let extraOne = mypq.pop();

console.log("EXTRAONE: ", extraOne)
//console.log("POPPED2: ", JSON.stringify(res2));

//console.log("SIZE NOW: ", mypq.size())

//console.log("Another pop: ", mypq.pop())

//console.log("Another pop: ", mypq.pop())
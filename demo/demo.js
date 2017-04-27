const pq = require('../src/build/Release/mypq');
var microtime = require('microtime')


//console.log("BEFORE CONSTRUCTOR");
let mypq = new pq.PriorityQueueNative();
//console.log("AFTER CONSTRUCTOR");

/*mypq.push({"my": 1}, 1);
mypq.push({"my": 6}, 6);
mypq.push({"my": 3}, 3);
mypq.push({"my": 22}, 22);
mypq.push({"my": 12}, 12);
mypq.push({"my": 212}, 212);
mypq.push({"my": 32}, 32);
mypq.push({"my": 332}, 332);
mypq.push({"my": 442}, 442);
mypq.push({"my": 444.2}, 444.2);
*/

let makeObj = function (num) {
    return {
        "val": num
    }
}

let nums = [
    71, 112, 113, 0.01, 447.3, 812.9, 309, 2001, 21.412, 256.21, 88.8872, 320, 523, 4299, 0.553, 0.991, 9.123, 4.3, 139, 91232, 9, 2, 7, 4, 8, 14, 56, 343, 66, 762.44, 762.55, 88, 89, 3, 6, 98, 99, 1, 102.1, 0.99, 999, 12.5, 44, 424, 488, 0.77, 2334, 55.3, 172, 4.11
];
//console.log("PUSHED 2");
//console.log("Total objects: ", nums.length);
//console.log("size: ", mypq.size());

let objcts = nums.map(n => makeObj(n));
let start = microtime.now();
objcts.forEach( o => mypq.push(o));

//let res = mypq.pop();
//console.log("POPPED1: ", JSON.stringify(res));


//let res2 = mypq.pop();

//let len = mypq.size();
//console.log("First one: ", mypq.top())
let sum = 0
while (mypq.size() > 0) {
    //console.log("POPPED:",mypq.pop());
    var g = mypq.pop();
    sum += g.val;
}

let end = microtime.now();
let total = end - start;
console.log("FINISHED PUSH/POP ", objcts.length, "total time: ", total);
console.log("SUM=", sum);


let extraOne = mypq.pop();

console.log("EXTRAONE: ", extraOne)
//console.log("POPPED2: ", JSON.stringify(res2));

//console.log("SIZE NOW: ", mypq.size())

//console.log("Another pop: ", mypq.pop())

//console.log("Another pop: ", mypq.pop())
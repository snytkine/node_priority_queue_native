const pq = require('../src/build/Release/mypq');
const objcts = require('./data')
var microtime = require('microtime')


//console.log("BEFORE CONSTRUCTOR");

let mypq = new pq.PriorityQueueNative(function (lhs, rhs) {
    return lhs.val < rhs.val
});

mypq = new pq.PriorityQueueNative();
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


let start = microtime.now();

let sum = 0.0;
let looped = 0;
let inner = 0;

for (let i = 0; i < 1; i += 1) {

    objcts.forEach(o => mypq.push(o, o.val));

    while (mypq.size() > 0) {
        //console.log("POPPED:",mypq.pop());
        let g = mypq.pop();
        //console.log(g.val)
        sum += g.val;
        inner += 1;
    }

    looped += 1;
}

let end = microtime.now();
let total = end - start;
console.log("FINISHED PUSH/POP ", objcts.length, "total time: ", total, " Looped: ", looped, "inner: ", inner);
console.log("SUM=", sum);


let extraOne = mypq.pop();

console.log("EXTRAONE: ", extraOne);

let myRes = objcts.sort( (l, r) => r.val > l.val ).reduce((acc, o) => acc += o.val, 0.0);
console.log("myRes=", myRes)
//console.log("POPPED2: ", JSON.stringify(res2));

//console.log("SIZE NOW: ", mypq.size())

//console.log("Another pop: ", mypq.pop())

//console.log("Another pop: ", mypq.pop())
const pq = require('../src/build/Release/mypq');
const makeObjects = require('./data')
var microtime = require('microtime')


//console.log("Objects:", JSON.stringify(makeObjects(50)));
//return;
//console.log("BEFORE CONSTRUCTOR");

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

    let mypq = new pq.PriorityQueueNative(function (lhs, rhs) {
        //console.log("Called comparator with lhs:", lhs.val, " rhs: ", rhs.val)
        return lhs.val < rhs.val
    });

    let objcts = makeObjects(50);
    //mypq = new pq.PriorityQueueNative();
    objcts.forEach(o => mypq.push(o));

    let it = mypq.iter();
    let done = false;

    let g = it.next();
    console.log("=====  G:", JSON.stringify(g));

    g = it.next();
    console.log("======= G:", JSON.stringify(g));

    while(!done){
        let n = it.next();
        done = n.done;
        console.log("N: ", JSON.stringify(n.value));
    }

    /*while (mypq.size() > 0) {
        //console.log("POPPED:",mypq.pop());
        let g = mypq.pop();
        //console.log(g.val)
        sum += g.val;
        inner += 1;
    }*/

    looped += 1;
}

let end = microtime.now();
let total = end - start;
console.log("FINISHED PUSH/POP objects per loop:", 50, "total time: ", total, " Looped: ", looped, "inner: ", inner);
console.log("SUM=", sum);


//let extraOne = mypq.pop();

//console.log("EXTRAONE: ", extraOne);

//let myRes = objcts.sort( (l, r) => r.val > l.val ).reduce((acc, o) => acc += o.val, 0.0);
//console.log("myRes=", myRes)
//console.log("POPPED2: ", JSON.stringify(res2));

//console.log("SIZE NOW: ", mypq.size())

//console.log("Another pop: ", mypq.pop())

//console.log("Another pop: ", mypq.pop())
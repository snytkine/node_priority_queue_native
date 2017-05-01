/**
 * Created by snytkind on 4/26/17.
 */

const FastPriorityQueue = require('fastpriorityqueue');
const makeObjects = require('./data');
var microtime = require('microtime');


let start = microtime.now();

let sum = 0;
let looped = 0;
let inner = 0;

for (let i = 0; i < 1; i += 1) {


    var x = new FastPriorityQueue(function (lhs, rhs) {
        return lhs.val > rhs.val
    })
    let objcts = makeObjects(50);
    objcts.forEach(o => x.add(o));


    while (!x.isEmpty()) {

        let g = x.poll();
        //console.log(g.val);
        sum += g.val;
        inner += 1;
    }

    looped += 1;
}


let end = microtime.now();
let total = end - start;
console.log("FINISHED PUSH/POP objects per loop: ", 50, "total time: ", total, " Looped: ", looped, "inner: ", inner);
console.log("SUM=", sum);


//let extraOne = x.poll();

//console.log("EXTRAONE: ", extraOne)



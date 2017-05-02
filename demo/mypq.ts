import {PriorityQueue, PriorityQueueCompare} from 'priorityqueue_native'

interface item {
    val: number
    priority: number
}



let gh: PriorityQueue<item> = new PriorityQueue<item>();

let gh2: PriorityQueueCompare<any> = new PriorityQueueCompare<any>((left: string, right: string) => left < right);

gh2.push(true)
gh2.push("CAT");
gh2.push("DOG");
gh2.push(function(){
    console.log("I WAS ADDED AS A FUNCTION");
    return "FUNCTION CALLED!";
})


console.log("in 2 size: ", gh2.size());
console.log("TOP: ", gh2.top())
console.log("TOP AGAIN: ", gh2.top());
console.log("size after TOP: ", gh2.size());

//let aBC = Array.from(gh2);
let it = gh2[Symbol.iterator]();

console.log("NEXT: ", it.next());
console.log("NEXT2: ", it.next());
console.log("NEXT3: ", it.next());
console.log("C: ", gh2.pop());
console.log("C", gh2.pop())


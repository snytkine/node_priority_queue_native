import {PriorityQueue, PriorityQueueCompare} from 'priorityqueue_native'

interface item {
    task: string
    category: string
    priority: number
}

let todos: PriorityQueue<item> = new PriorityQueue<item>();

todos.push({"task": "Wash Dishes", "category":"house", "priority": 2}, 2);
todos.push({"task": "Math homework", "category":"school", "priority": 4}, 4);
todos.push({"task": "History paper", "category":"school", "priority": 3}, 3);
todos.push({"task": "Walk the dog", "category":"other", "priority": 5}, 5);
todos.push({"task": "Rotate tires", "category":"car", "priority": 1}, 1);


let todos2: PriorityQueueCompare<item> = new PriorityQueueCompare<item>((left: item, right: item) => left.priority < right.priority);

todos2.push({"task": "Wash Dishes", "category":"house", "priority": 2});
todos2.push({"task": "Math homework", "category":"school", "priority": 4});
todos2.push({"task": "History paper", "category":"school", "priority": 3});
todos2.push({"task": "Walk the dog", "category":"other", "priority": 5});
todos2.push({"task": "Rotate tires", "category":"car", "priority": 1});

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


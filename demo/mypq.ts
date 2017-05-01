import {PriorityQueue, PriorityQueueCompare} from 'priorityqueue_native'

interface item {
    val: number
    priority: number
}
let gh: PriorityQueue<item> = new PriorityQueue<item>();

let gh2: PriorityQueueCompare<item> = new PriorityQueueCompare<item>((left: item, right: item) => left.priority < right.priority);

gh.push({val: 3, priority: 4}, 4);

gh2.push({val: 5, priority: 4});
console.log("in 2 size: ", gh2.size())



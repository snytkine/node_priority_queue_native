declare interface QIteratorResult<T> {
    value: T
    done: boolean
}


declare interface QIterator<T> {
    next(value?: any): QIteratorResult<T>;
    return(value?: any): QIteratorResult<T>;
}


interface QIterable<T> {
    [Symbol.iterator]: () => QIterator<T>
}


export declare class PriorityQueue<T> {

    push: (item: T, priority: number) => undefined
    top: () => T | undefined
    pop: () => T | undefined
    size: () => number
    next: (value?: any) => QIteratorResult<T>
    constructor()
    [Symbol.iterator]: () => QIterator<T>

}


export declare class PriorityQueueCompare<T> {

    push: (item: T) => undefined
    top: () => T | undefined
    pop: () => T | undefined
    size: () => number
    next: (value?: any) => QIteratorResult<T>
    constructor(fn: (left: T, right: T) => boolean )
    [Symbol.iterator]: () => QIterator<T>

}
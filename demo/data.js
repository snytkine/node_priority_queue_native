/**
 * Created by snytkind on 4/27/17.
 */

let makeObj = function (num) {
    return {
        "val": Math.floor(Math.random() * 10000)
    }
}

//71, 112, 113, 0.01, 447.3, 812.9,
let nums = [
    139, 1233, 9, 2, 7, 4, 8, 14, 56, 343.01, 66.02, 0.01, 447.3, 812.9, 762.44, 762.55, 21.412, 256.21, 88.8872, 0.553, 0.991, 9.123, 4.3, 88, 89, 3, 6, 98, 99, 1, 102.1, 0.99, 999, 12.5, 44, 424, 488, 0.77, 2334, 55.3, 172, 4.11
];

//309, 2001,  320, 523,

//let objcts = nums.map(n => makeObj(n));

let makeObjects = (num) => {
    return Array.from({length: num}, () => makeObj());
}

module.exports = makeObjects;

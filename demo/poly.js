/**
 * Created by snytkind on 4/25/17.
 */
const addon = require('../src/build/Release/polynomial');

var poly = new addon.Polynomial(1, 3, 2);

console.log(poly.at(4));
console.log(poly.roots());
poly.c = 0;
console.log(poly.at(4));

console.log(poly);
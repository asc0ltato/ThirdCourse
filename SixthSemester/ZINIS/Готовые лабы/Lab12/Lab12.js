const {
    calculateGCD, calculateGCD3, findPrimes,
    sieveOfEratosthenes, canonForm, isConcatenatedNumberPrime
} = require('./mylib');

const num1 = 50;
const num2 = 30;
const gcdTwo = calculateGCD(num1, num2);
console.log(`НОД чисел ${num1} и ${num2} равен ${gcdTwo}`);

console.log('-------------------------------------------------------------');

const num3 = 93;
const gcdThree = calculateGCD3(num1, num2, num3);
console.log(`НОД чисел ${num1}, ${num2} и ${num3} равен ${gcdThree}`);

console.log('-------------------------------------------------------------');

const n1 = 531;
const {primes: primes1, count: count1} = findPrimes(2, n1);
const expCount1 = n1 / Math.log(n1);

console.log(`Простые числа от 2 до ${n1}: ${primes1.join(", ")}`);
console.log(`Количество простых чисел: ${count1}`);
console.log(`Ожидаемое количество простых чисел: ${expCount1}`);

if (count1 > expCount1) {
    console.log(`Количество простых чисел превышает ожидаемое количество простых чисел в ${(count1 / expCount1).toFixed(1)}`);
} else {
    console.log(`Ожидаемое количество простых чисел превышает количество простых чисел в ${(expCount1 / count1).toFixed(1)}`);
}

console.log('-------------------------------------------------------------');

const m2 = 499, n2 = 531;
const {primes: primes2, count: count2} = sieveOfEratosthenes(m2, n2);
const expCount2 = n2 / Math.log(n2) - m2 / Math.log(m2);
console.log(`Простые числа от ${m2} до ${n2}: ${primes2.join(", ")}`);
console.log(`Количество простых чисел от 499 до 531: ${count2}`);
console.log(`Ожидаемое количество простых чисел: ${expCount2}`);

if (count2 > expCount2) {
    console.log(`Количество простых чисел превышает ожидаемое количество простых чисел в ${(count2 / expCount2).toFixed(1)}`);
} else {
    console.log(`Ожидаемое количество простых чисел превышает количество простых чисел в ${(expCount2 / count2).toFixed(1)}`);
}

console.log('-------------------------------------------------------------');

const numCanon = canonForm(num1);
const mCanon = canonForm(m2);
const nCanon = canonForm(n2);
console.log(`Каноническая форма записи числа m: ${numCanon}`);
console.log(`Каноническая форма записи числа m: ${mCanon}`);
console.log(`Каноническая форма записи числа n: ${nCanon}`);

console.log('-------------------------------------------------------------');

console.log(isConcatenatedNumberPrime(m2, n2));

console.log('-------------------------------------------------------------');

const gcd = calculateGCD(m2, n2);
console.log(`НОД чисел ${m2} и ${n2} равен ${gcd}`);
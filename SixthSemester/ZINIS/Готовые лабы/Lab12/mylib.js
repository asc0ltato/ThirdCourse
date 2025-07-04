const calculateGCD = (a, b) =>
    b === 0 ? a : calculateGCD(b, a % b);

const calculateGCD3 = (a, b, c) =>
    calculateGCD(calculateGCD(a, b), c);

function Prime(num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 === 0 || num % 3 === 0) return false;

    for (let i = 5; i <= Math.sqrt(num); i++) {
        if (num % i === 0) {
            return false;
        }
    }
    return true;
}

function findPrimes(n, m) {
    const primes = [];

    for (let i = n; i <= m; i++) {
        if (Prime(i)) {
            primes.push(i);
        }
    }

    return {
        primes,
        count: primes.length
    };
}

function sieveOfEratosthenes(n1, m1) {
    const num = Array(m1 + 1).fill(true);
    num[0] = num[1] = false;

    // Решето Эратосфена
    for (let i = 2; i <= Math.sqrt(m1); i++) {
        if (num[i]) {
            for (let j = i * i; j <= m1; j += i) {
                num[j] = false;
            }
        }
    }

    const primes = [];
    for (let i = Math.max(n1, 2); i <= m1; i++) {
        if (num[i]) {
            primes.push(i);
        }
    }

    return {
        primes,
        count: primes.length
    };
}

const primeFactors = (num) => {
    const factors = [];
    let divisor = 2;
    while (num > 1) {
        while (num % divisor === 0) {
            factors.push(divisor);
            num /= divisor;
        }
        divisor++;
    }
    return factors;
};

const canonForm = (num) => {
    const factors = primeFactors(num);
    const factorCounts = {};

    for (const factor of factors) {
        factorCounts[factor] = (factorCounts[factor] || 0) + 1;
    }

    const formattedFactors = Object.entries(factorCounts).map(([factor, count]) => {
        return count > 1 ? `${factor}^${count}` : factor;
    });

    return `${num} = ${formattedFactors.join(' * ')}`;
};

const isPrime = (num) => {
    if (num < 2) return { isPrime: false, divisor: null };
    for (let i = 2; i <= Math.sqrt(num); i++) {
        if (num % i === 0) {
            return { isPrime: false, divisor: i };
        }
    }
    return { isPrime: true, divisor: null };
};

const isConcatenatedNumberPrime = (m, n) => {
    const nLength = n.toString().length;
    const concatenatedNumber = m * Math.pow(10, nLength) + n;
    const result = isPrime(concatenatedNumber);
    if (result.isPrime) {
        return `Число, состоящее из конкатенации цифр ${m} и ${n}, является простым`;
    } else {
        return `Число, состоящее из конкатенации цифр ${m} и ${n}, не является простым и делится на ${result.divisor}`;
    }
};

module.exports = {
    calculateGCD, calculateGCD3, findPrimes,
    sieveOfEratosthenes, canonForm, isConcatenatedNumberPrime
};
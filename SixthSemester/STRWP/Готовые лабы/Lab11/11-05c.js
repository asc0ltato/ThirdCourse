const rpcWSC = require('rpc-websockets').Client;
let ws = new rpcWSC('ws://localhost:4000/');
const async = require('async');

ws.on('open', () => {
    async.waterfall([
        (cb) => {
            ws.call('square', [3])
                .then(result => cb(null, result))
                .catch(err => cb(err));
        },

        (square3Result, cb) => {
            ws.call('square', [5, 4])
                .then(result => cb(null, square3Result, result))
                .catch(err => cb(err));
        },

        (square3Result, square54Result, cb) => {
            ws.call('mul', [3, 5, 7, 9, 11, 13])
                .then(result => cb(null, square3Result, square54Result, result))
                .catch(err => cb(err));
        },

        (square3Result, square54Result, mulResult, cb) => {
            ws.call('sum', [square3Result, square54Result, mulResult])
                .then(result => cb(null, result))
                .catch(err => cb(err));
        },

        (sumResult, cb) => {
            ws.login({ login: 'qwerty', password: '123' })
                .then(() => ws.call('fib', [7]))
                .then(fib7Result => {
                    const lastFib = Array.isArray(fib7Result) && fib7Result.length > 0
                        ? fib7Result[fib7Result.length - 1]
                        : 0;
                    cb(null, sumResult, lastFib);
                })
                .catch(err => cb(err));
        },

        (sumResult, fib7Result, cb) => {
            ws.call('mul', [2, 4, 6])
                .then(mul246Result => cb(null, sumResult, fib7Result, mul246Result))
                .catch(err => cb(err));
        },

        (sumResult, fib7Result, mul246Result, cb) => {
            let finalResult = sumResult + fib7Result * mul246Result;
            cb(null, finalResult);
        }
    ], (error, result) => {
        if (error)
            console.error(error);
        else
            console.log(result);
        ws.close();
    });
});
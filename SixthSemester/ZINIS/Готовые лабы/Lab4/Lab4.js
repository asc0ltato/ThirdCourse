const fs = require('fs');
const mylib = require('./mylib');

function main() {
    const inputData = fs.readFileSync('Lab4.txt', 'ascii');
    const binaryMessage = mylib.toBinary(inputData);
    const k = binaryMessage.length;
    const r = Math.floor(Math.log2(k) + 1);
    const n = r + k;

    const hammingMatrix = mylib.getHammingMatrix(k, r);
    const Xr = mylib.calcRedundantBits(hammingMatrix, binaryMessage, k, r);

    console.log(inputData);
    console.log(`Xk: ${binaryMessage}`);
    console.log("k = ", k);
    console.log("r = ", r);
    console.log("n = ", n);
    console.log("Проверочная матрица Хэмминга:");
    hammingMatrix.forEach(row => console.log(mylib.getBitsStr(row)));
    console.log(`Избыточные биты Xr: ${mylib.getBitsStr(Xr)}`);

    for (let numOfErrors = 0; numOfErrors < 3; numOfErrors++) {
        const binaryWithError = mylib.genErr(binaryMessage, numOfErrors);
        console.log(`Сообщение с ${numOfErrors} ошибками: ${binaryWithError}`);

        const XrWithError = mylib.calcRedundantBits(hammingMatrix, binaryWithError, k, r);
        console.log(`Избыточные биты Xr с ${numOfErrors} ошибками: ${mylib.getBitsStr(XrWithError)}`);

        const S = mylib.calcSyndrome(Xr, XrWithError, r);
        console.log(`Синдром: ${mylib.getBitsStr(S)}`);

        if (S.some(bit => bit !== 0)) {
            const En = mylib.genErrVector(S, hammingMatrix);
            console.log(`Вектор ошибок: ${mylib.getBitsStr(En)}`);

            const correctedBinary = mylib.correctErr(binaryWithError, En);
            console.log(`Исправленное сообщение: ${correctedBinary}`);

            const correctedText = mylib.fromBinary(correctedBinary);
            console.log(`Исправленное сообщение в ascii: ${correctedText}`);
        } else {
            console.log('Нет ошибок для исправления');
        }
    }
}

main();
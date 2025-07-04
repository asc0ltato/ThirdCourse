const {
    makeCanonicalG, makeCheckMatrix, encode,
    getSyndrome, findErrorPosition, xorStr,
    splitMessage, interleave, deinterleave,
    addRandomGroupErrors
} = require('./mylib');

const main = () => {
    const g = '101111';
    const r = g.length - 1;
    const k = 6;
    const n = k + r;

    const originalMessage = '101001111111101111111001110000110001111010100111111101111111001110000110001111010100111111';
    console.log('\nИнформационное сообщение:', originalMessage);

    const infoWords = splitMessage(originalMessage, k);
    console.log('\nСтруктура информационного потока на входе кодера:');
    infoWords.forEach(word => { console.log(`${word}`);});

    const encodedWords = infoWords.map(word => encode(word, g, k, n));
    console.log('\nПоследовательность символов сообщения на выходе кодера (кодовые слова):');
    console.log(encodedWords.join(''));

    console.log('\n');

    console.log('\nСтруктура информационного потока на выходе кодера (матрица перемежения):');
    encodedWords.forEach(word => { console.log(`${word}`);});

    const interleaveMatrix = encodedWords.map(word => word.split(''));
    const interleaved = interleave(interleaveMatrix);
    console.log('\nЗакодированное сообщение после перемежения', interleaved);

    const errorGroupSizes = [4, 5, 7];
    const trialsPerGroup = 30;
    console.log('\nСообщение с ошибками:', errorGroupSizes);

    errorGroupSizes.forEach(groupLen => {
        let successCount = 0;

        for (let t = 0; t < trialsPerGroup; t++) {
            const errored = addRandomGroupErrors(interleaved, groupLen);
            console.log('\nПередаваемое сообщение, содержащее группу ошибок', errored)

            const depMatrix = deinterleave(errored, encodedWords.length, n);
            console.log('\nМатрица деперемежения:');
            depMatrix.forEach(word => { console.log(`${word}`);});

            console.log('\nОшибки разнесены по всему сообщению:');
            console.log(depMatrix.join(''));

            const H = makeCheckMatrix(makeCanonicalG(k, n, g), k, n);
            const correctedWords = depMatrix.map(code => {
                const syndrome = getSyndrome(code, g);
                const errVec = findErrorPosition(syndrome, H, n);
                const corrected = xorStr(code, errVec);
                return corrected.slice(0, k);
            });

            const recoveredMessage = correctedWords.join('');
            console.log('\nИнформационный поток на выходе декодера циклического кода:');
            console.log(recoveredMessage);

            const resultWords = splitMessage(recoveredMessage, k);
            console.log('\nСтруктура информационного потока на выходе декодера:');
            resultWords.forEach(word => { console.log(`${word}`);});

            if (recoveredMessage === originalMessage) {
                console.log('\nИсходное информационное сообщение получено верно.');
                successCount++;
            }
            console.log('\n----------------------------------------------------------\n');
        }

        const result = (successCount/trialsPerGroup) * 100;
        console.log(`\nОшибка длины ${groupLen}: успешно восстановлено ${result} %`);
    });
};

main();
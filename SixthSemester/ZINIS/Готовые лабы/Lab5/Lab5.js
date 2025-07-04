const {
    generateBinary, checkParityBits,
    introduceError, findError, createZMatrix
} = require('./mylib');

const main = () => {
    const size = 40;
    const inputWord = generateBinary(size);
    console.log('Исходное слово:', inputWord.join(' '));

    const k1 = [5, 4, 5, 2];
    const k2 = [8, 10, 4, 10];
    const parityGroups = [[2, 3], [2, 3], [2, 3, 4, 5], [2, 3, 4, 5]];

    for (let i = 0; i < k1.length; i++) {
        console.log(`\n--------------------------Матрица ${i + 1}--------------------------`);
        let matrix = [];

        if (parityGroups[i].includes(4) || parityGroups[i].includes(5)) {
            const firstHalf = inputWord.slice(0, Math.floor(inputWord.length / 2));
            const secondHalf = inputWord.slice(Math.floor(inputWord.length / 2));

            const matrix1 = [];
            const matrix2 = [];

            for (let j = 0; j < k1[i]; j++) {
                matrix1.push(firstHalf.slice(j * k2[i], (j + 1) * k2[i]));
            }

            for (let j = 0; j < k1[i]; j++) {
                matrix2.push(secondHalf.slice(j * k2[i], (j + 1) * k2[i]));
            }

            matrix = matrix1.concat(matrix2);

            console.log(`Матрица для k1 = ${k1[i]} и k2 = ${k2[i]} (Первая половина):`);
            matrix1.forEach(row => console.log(row.join(' ')));

            console.log(`Матрица для k1 = ${k1[i]} и k2 = ${k2[i]} (Вторая половина):`);
            matrix2.forEach(row => console.log(row.join(' ')));

            console.log('\nКоличество групп паритетов =', parityGroups[i].join(', '));

            const parityBits = checkParityBits(matrix, parityGroups[i], k1[i], k2[i]);
            console.log('Паритетные биты:', parityBits.join(' '));

            const encodedWord = [...inputWord, ...parityBits];
            console.log('Закодированное слово:', encodedWord.join(' '));

            const errorCount = 2;
            const receivedMatrix = introduceError(matrix, k1[i], k2[i], errorCount);
            console.log('\nПолученная матрица с ошибкой:');
            receivedMatrix.forEach(row => console.log(row.join(' ')));

            const receivedParityBits = checkParityBits(receivedMatrix, parityGroups[i], k1[i], k2[i]);
            console.log('Паритетные биты полученного слова:', receivedParityBits.join(' '));

            if (parityBits.every((val, index) => val === receivedParityBits[index])) {
                console.log('Паритетные биты совпадают, ошибок не обнаружено');
            } else {
                const correctedMatrix = findError(receivedParityBits, parityBits, k1[i], k2[i], receivedMatrix);
                console.log('\nИсправленная матрица:');
                correctedMatrix.forEach(row => console.log(row.join(' ')));
            }

            const zMatrix = createZMatrix(matrix1, matrix2);
            console.log('\nZ-матрица:');
            zMatrix.forEach(row => console.log(row.join(' ')));

            const testWords = 100;
            for (let errorCount = 1; errorCount <= 2; errorCount++) {
                let successfulCorrections = 0;

                for (let j = 0; j < testWords; j++) {
                    const corruptedMatrix = introduceError(matrix, k1[i], k2[i], errorCount);
                    const received = checkParityBits(corruptedMatrix, parityGroups[i], k1[i], k2[i]);
                    const corrected = findError(received, parityBits, k1[i], k2[i], corruptedMatrix);
                    if (inputWord.join('') === corrected.flat().join('')) {
                        successfulCorrections++;
                    }
                }

                const successRate = Math.round(successfulCorrections / testWords);
                console.log(`Способность к исправлению для ${errorCount} ошибки: ${successRate}`);
            }
        } else {
            for (let j = 0; j < k1[i]; j++) {
                matrix.push(inputWord.slice(j * k2[i], (j + 1) * k2[i]));
            }

            console.log(`Матрица для k1 = ${k1[i]} и k2 = ${k2[i]}:`);
            matrix.forEach(row => console.log(row.join(' ')));

            console.log('\nКоличество групп паритетов =', parityGroups[i].join(', '));

            const parityBits = checkParityBits(matrix, parityGroups[i], k1[i], k2[i]);
            console.log('Паритетные биты:', parityBits.join(' '));

            const encodedWord = [...inputWord, ...parityBits];
            console.log('Закодированное слово:', encodedWord.join(' '));

            const errorCount = 1;
            const receivedMatrix = introduceError(matrix, k1[i], k2[i], errorCount);
            console.log('\nПолученная матрица с ошибкой:');
            receivedMatrix.forEach(row => console.log(row.join(' ')));

            const receivedParityBits = checkParityBits(receivedMatrix, parityGroups[i], k1[i], k2[i]);
            console.log('\nПаритетные биты полученного слова:', receivedParityBits.join(' '));

            if (parityBits.every((val, index) => val === receivedParityBits[index])) {
                console.log('Паритетные биты совпадают, ошибок не обнаружено');
            } else {
                const correctedMatrix = findError(receivedParityBits, parityBits, k1[i], k2[i], receivedMatrix);
                console.log('\nИсправленная матрица:');
                correctedMatrix.forEach(row => console.log(row.join(' ')));
            }

            const testWords = 100;
            for (let errorCount = 1; errorCount <= 2; errorCount++) {
                let successfulCorrections = 0;

                for (let j = 0; j < testWords; j++) {
                    const corruptedMatrix = introduceError(matrix, k1[i], k2[i], errorCount);
                    const received = checkParityBits(corruptedMatrix, parityGroups[i], k1[i], k2[i]);
                    const corrected = findError(received, parityBits, k1[i], k2[i], corruptedMatrix);
                    if (inputWord.join('') === corrected.flat().join('')) {
                        successfulCorrections++;
                    }
                }

                const successRate = Math.round(successfulCorrections / testWords);
                console.log(`Способность к исправлению для ${errorCount} ошибки: ${successRate}`);
            }
        }
    }
};

main();
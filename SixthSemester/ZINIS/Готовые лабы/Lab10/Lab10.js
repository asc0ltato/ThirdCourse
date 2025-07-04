const fs = require('fs');
const readline = require('readline');
const { performance } = require('perf_hooks');
const {
    addSymbolsToBuffer,
    checkBufferSize,
    findMatchingSequence
} = require('./mylib');

async function main() {
    const filePath = 'text.txt';

    if (!fs.existsSync(filePath)) {
        fs.writeFileSync(filePath, '');
        console.log(`Файл ${filePath} был создан.`);
    }

    const inputText = fs.readFileSync(filePath, 'utf-8');
    console.log('Исходное сообщение: ' + inputText + '\n');

    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout
    });

    const promptUser = (query) => new Promise(resolve => rl.question(query, resolve));

    let dictionarySize = 0;
    let slidingWindowSize = 0;

    while (dictionarySize === 0) {
        dictionarySize = parseInt(await promptUser("\nРазмер словаря (n1): "));
    }

    while (slidingWindowSize === 0) {
        slidingWindowSize = parseInt(await promptUser("\nРазмер скользящего окна (n2): "));
    }

    rl.close();

    let dictionaryBuffer = '0'.repeat(dictionarySize);
    let slidingWindow = inputText.substring(0, slidingWindowSize);
    let remainingText = inputText.substring(slidingWindowSize);

    console.log("\nСжатие\n");

    let compressedOutput = '';
    let nextSymbol;
    let matchStartIndex = 0;
    let matchLength = 0;

    const compressionStartTime = performance.now();

    while (slidingWindow.length > 0) {
        ({ matchIndex: matchStartIndex, matchLength, nextChar: nextSymbol } = findMatchingSequence(dictionaryBuffer, slidingWindow));

        ({ buffer: dictionaryBuffer, slidingWindow } = addSymbolsToBuffer(matchLength + 1, dictionaryBuffer, slidingWindow));
        dictionaryBuffer = checkBufferSize(dictionarySize, dictionaryBuffer);

        ({ buffer: slidingWindow, slidingWindow: remainingText } = addSymbolsToBuffer(matchLength + 1, slidingWindow, remainingText));
        slidingWindow = checkBufferSize(slidingWindowSize, slidingWindow);

        console.log(`${dictionaryBuffer}\t|\t${slidingWindow}\t|\t${matchStartIndex}${matchLength}${nextSymbol}\n`);

        compressedOutput += `${matchStartIndex}${matchLength}${nextSymbol}`;
    }

    const compressionEndTime = performance.now();

    console.log("\nСжатое сообщение: " + compressedOutput);
    console.log("\nРаспаковка\n");

    const decompressionStartTime = performance.now();

    let decompressedText = '';
    let tempString = '';
    dictionaryBuffer = '0'.repeat(dictionarySize);

    for (let i = 0; i < compressedOutput.length / 3; i++) {
        matchStartIndex = parseInt(compressedOutput[3 * i]);
        matchLength = parseInt(compressedOutput[3 * i + 1]);
        nextSymbol = compressedOutput[3 * i + 2];

        if (matchLength === 0 && matchStartIndex === 0) {
            decompressedText += nextSymbol;
            dictionaryBuffer += nextSymbol;
        } else {
            tempString = dictionaryBuffer.substring(matchStartIndex - 1, matchStartIndex - 1 + matchLength) + nextSymbol;
            decompressedText += tempString;
            dictionaryBuffer += tempString;
        }

        dictionaryBuffer = checkBufferSize(dictionarySize, dictionaryBuffer);

        console.log(`${matchStartIndex}${matchLength}${nextSymbol}\t\t|\t${dictionaryBuffer}\t\t|\t${decompressedText}\n`);
    }

    const decompressionEndTime = performance.now();

    console.log(`Время кодирования: ${(compressionEndTime - compressionStartTime).toFixed(4)} мс`);
    console.log(`Время декодирования: ${(decompressionEndTime - decompressionStartTime).toFixed(4)} мс`);

    console.log("\nИсходное сообщение: " + inputText.length + " символов");
    console.log("Сжатое сообщение: " + compressedOutput.length + " символов");

    const compressionEfficiency = (inputText.length / compressedOutput.length) * 100;
    console.log(`\nЭффективность сжатия составляет ${compressionEfficiency}%`);
}

main();
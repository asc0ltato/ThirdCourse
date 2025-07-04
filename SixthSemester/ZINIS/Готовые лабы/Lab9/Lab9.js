const fs = require('fs');
const {
    displayTable, getProbabilityAndFrequency, getSortedArray,
    getSummaryProbabilities, shannonFano, getEncodedMessage,
    getDecodedMessage, encodingToBytes, huffmanEncode, huffmanDecode
} = require('./mylib.js');

function main() {
    const input1 = "Жук Светлана";
    const input2 = fs.readFileSync('bulgarian.txt', 'utf8').trim();

    console.log("Сообщение 1 (ФИ): " + input1);
    processShennonInput(input1);

    console.log("Сообщение 2 (bulgarian.txt): " + input2);
    processShennonInput(input2);

    console.log("Сообщение 1 (ФИ): " + input1);
    processHuffmanInput(input1);

    console.log("Сообщение 2 (bulgarian.txt): " + input2);
    processHuffmanInput(input2);
}

function processShennonInput(input) {
    let { chars, frequency, probability } = getProbabilityAndFrequency(input);

    console.log("Таблица частот и вероятностей:\n");
    getProbabilityAndFrequency(input, chars, frequency, probability);
    printFrequencyTable(chars, frequency, probability);

    getSortedArray(chars, probability);
    getSortedArray(chars, frequency);
    printFrequencyTable(chars, frequency, probability);

    const sumOfProbabilities = getSummaryProbabilities(probability);
    console.log("Сумма вероятностей: " + sumOfProbabilities.toFixed(0) + "\n");

    console.log("Метод сжатия Шеннона-Фано:");
    const letterBitsArray = Array(chars.length).fill("");
    shannonFano(0, chars.length - 1, probability, letterBitsArray);

    printBitTable(chars, letterBitsArray);

    const encodedMessage = getEncodedMessage(input, chars, letterBitsArray);
    const decodedMessage = getDecodedMessage(encodedMessage, chars, letterBitsArray);

    console.log("Закодированное сообщение: " + encodedMessage);
    console.log("Декодированное сообщение: " + decodedMessage + "\n");

    printCompressionEfficiency(input, encodedMessage);
}

function printFrequencyTable(chars, frequency, probability) {
    const headers = ["Символ", "Частота", "Вероятность"];
    const rows = chars.map((char, i) => [char, frequency[i], probability[i].toFixed(3)]);
    displayTable(headers, rows);
}

function processHuffmanInput(input) {
    const { codeTable, frequency } = huffmanEncode(input);

    console.log("Таблица частот и вероятностей:\n");
    printFrequencyTableFromCode(frequency, input);

    console.log("Метод сжатия Хаффмана:");
    printHuffmanCodeTable(codeTable);

    const encodedMessage = getEncodedMessage(input, Object.keys(codeTable), Object.values(codeTable));
    const decodedMessage = huffmanDecode(encodedMessage, codeTable);

    console.log("Закодированное сообщение: " + encodedMessage);
    console.log("Декодированное сообщение: " + decodedMessage + "\n");

    printCompressionEfficiency(input, encodedMessage);
}

function printFrequencyTableFromCode(frequency, input) {
    console.table(
        Object.entries(frequency).map(([char, count]) => ({
            "Символ": char,
            "Частота": count,
            "Вероятность": (count / input.length).toFixed(3)
        }))
    );
}

function printHuffmanCodeTable(codeTable) {
    console.table(
        Object.entries(codeTable).map(([char, bits]) => ({
            "Символ": char,
            "Биты": bits
        }))
    );
}

function printBitTable(chars, letterBitsArray) {
    const bitTableHeaders = ["Символы", "Биты"];
    const bitTableRows = chars.map((char, i) => [char, letterBitsArray[i]]);
    displayTable(bitTableHeaders, bitTableRows);
}

function printCompressionEfficiency(input, encodedMessage) {
    console.log("В ASCII:");
    const asciiEncoded = encodingToBytes(input);
    console.log("Закодированное сообщение: " + asciiEncoded + "\n");
    console.log("Сообщение: " + input + "\n");

    console.log("Символов после сжатия: " + encodedMessage.length);
    console.log("Символов в ASCII: " + asciiEncoded.length);

    const times = (asciiEncoded.length / encodedMessage.length).toFixed(3);
    console.log("\nЭффективность сжатия:");
    console.log("Сжали в " + times + " раз по сравнению с кодом ASCII\n");
}

main();
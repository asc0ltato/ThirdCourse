function displayTable(headers, rows) {
    const tableData = rows.map(row => {
        const obj = {};
        headers.forEach((h, i) => {
            obj[h] = row[i];
        });
        return obj;
    });
    console.table(tableData);
}1

function getProbabilityAndFrequency(inputMessage) {
    const chars = [];
    const frequency = [];
    const probability = [];

    [...new Set(inputMessage)].forEach(ch => {
        const count = inputMessage.split(ch).length - 1;
        chars.push(ch);
        frequency.push(count);
        probability.push(count / inputMessage.length);
    });

    return { chars, frequency, probability };
}

function getSortedArray(lettersArray, probabilitiesArray) {
    const combined = lettersArray.map((char, i) => ({ char, probability: probabilitiesArray[i] }));
    combined.sort((a, b) => b.probability - a.probability);

    combined.forEach((item, i) => {
        lettersArray[i] = item.char;
        probabilitiesArray[i] = item.probability;
    });
}

function getSummaryProbabilities(probabilitiesArray) {
    return probabilitiesArray.reduce((acc, prob) => acc + prob, 0);
}

function toSplitSequences(left, right, probabilitiesArray) {
    const total = probabilitiesArray.slice(left, right + 1).reduce((acc, prob) => acc + prob, 0);
    const halfTotal = total / 2;
    let acc = 0;
    let i;
    for (i = left; i <= right; i++) {
        acc += probabilitiesArray[i];
        if (acc >= halfTotal) break;
    }
    return i;
}

function shannonFano(left, right, probabilitiesArray, letterBitsArray) {
    if (left >= right) return;
    const splitPoint = toSplitSequences(left, right, probabilitiesArray);
    for (let i = left; i <= splitPoint; i++) letterBitsArray[i] += "0";
    for (let i = splitPoint + 1; i <= right; i++) letterBitsArray[i] += "1";
    shannonFano(left, splitPoint, probabilitiesArray, letterBitsArray);
    shannonFano(splitPoint + 1, right, probabilitiesArray, letterBitsArray);
}

function getEncodedMessage(input, charsArray, charBitsArray) {
    return input.split('').map(ch => charBitsArray[charsArray.indexOf(ch)]).join('');
}

function getDecodedMessage(encodedMessage, charsArray, charBitsArray) {
    let decodedMessage = '';
    let currentBits = '';
    for (const bit of encodedMessage) {
        currentBits += bit;
        const index = charBitsArray.indexOf(currentBits);
        if (index !== -1) {
            decodedMessage += charsArray[index];
            currentBits = '';
        }
    }
    return decodedMessage;
}

function encodingToBytes(message) {
    const encoder = new TextEncoder();
    const bytes = encoder.encode(message);
    return Array.from(bytes).map(b => b.toString(2).padStart(8, '0')).join('');
}

class HuffmanNode {
    constructor(symbol, probability, left = null, right = null) {
        this.symbol = symbol;
        this.probability = probability;
        this.left = left;
        this.right = right;
    }
}

function traverseHuffmanTree(node, code, codeTable) {
    if (!node.left && !node.right) {
        codeTable[node.symbol] = code;
    } else {
        if (node.left) traverseHuffmanTree(node.left, code + "0", codeTable);
        if (node.right) traverseHuffmanTree(node.right, code + "1", codeTable);
    }
}

function huffmanEncode(input) {
    const frequency = {};
    for (const ch of input) {
        frequency[ch] = (frequency[ch] || 0) + 1;
    }

    const priorityQueue = Object.entries(frequency).map(([symbol, count]) =>
        new HuffmanNode(symbol, count / input.length)
    );

    while (priorityQueue.length > 1) {
        priorityQueue.sort((a, b) => a.probability - b.probability);
        const left = priorityQueue.shift();
        const right = priorityQueue.shift();
        const newNode = new HuffmanNode(null, left.probability + right.probability, left, right);
        priorityQueue.push(newNode);
    }

    const root = priorityQueue[0];
    const codeTable = {};
    traverseHuffmanTree(root, "", codeTable);

    return { codeTable, frequency };
}

function huffmanDecode(encodedMessage, codeTable) {
    const reverseCodeTable = Object.fromEntries(
        Object.entries(codeTable).map(([k, v]) => [v, k])
    );
    let decodedMessage = '';
    let currentBits = '';
    for (const bit of encodedMessage) {
        currentBits += bit;
        if (reverseCodeTable[currentBits]) {
            decodedMessage += reverseCodeTable[currentBits];
            currentBits = '';
        }
    }
    return decodedMessage;
}

module.exports = {
    displayTable, getProbabilityAndFrequency, getSortedArray,
    getSummaryProbabilities, shannonFano, getEncodedMessage,
    getDecodedMessage, encodingToBytes, huffmanEncode, huffmanDecode
};
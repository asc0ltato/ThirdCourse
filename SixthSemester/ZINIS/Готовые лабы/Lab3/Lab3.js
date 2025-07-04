const fs = require('fs');
const readline = require('readline');

const Alphabets = {
    Italian: "аbcdеfghijklmnоpqrstuvwxyz",
    Base64: "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/",
    Binary: "01"
};

const calculateShannonEntropy = (text) => {
    const freq = {};
    for (let char of text) {
        if (!freq[char]) freq[char] = 0;
        freq[char]++;
    }
    const totalChars = text.length;
    let entropy = 0;
    for (let char in freq) {
        const p = freq[char] / totalChars;
        entropy -= p * Math.log2(p);
    }
    return entropy.toFixed(3);
};

const calculateHartleyEntropy = (alphabet) => {
    const result = Math.log2(alphabet.length);
    return result.toFixed(3);
};

const calculateRedundancy = (shannonEntropy, hartleyEntropy) => {
    const result = ((hartleyEntropy - shannonEntropy) / hartleyEntropy) * 100;
    return result.toFixed(3);
};

const encodeDocumentToBase64 = (filename) => {
    const data = fs.readFileSync(filename, 'utf-8').toLowerCase();
    return Buffer.from(data).toString('base64');
};

const toBinary = (str) => {
    return str.split('').map(char => {
        return char.charCodeAt(0).toString(2).padStart(8, '0');
    }).join('');
};

const xorBinaryStrings = (binaryA, binaryB) => {
    const length = Math.max(binaryA.length, binaryB.length);
    binaryA = binaryA.padStart(length, '0');
    binaryB = binaryB.padStart(length, '0');

    let result = '';
    for (let i = 0; i < length; i++) {
        result += (binaryA[i] === binaryB[i] ? '0' : '1');
    }

    return result;
};

const binaryToString = (binary) => {
    let str = '';
    for (let i = 0; i < binary.length; i += 8) {
        const byte = binary.slice(i, i + 8);
        str += String.fromCharCode(parseInt(byte, 2));
    }
    return str;
};

const xorBinaryOperation = (a, b) => {
    const binaryA = toBinary(a);
    const binaryB = toBinary(b);

    const xorAB = xorBinaryStrings(binaryA, binaryB);
    const xorABB = xorBinaryStrings(xorAB, binaryB);

    return { xorAB, xorABB, binaryA, binaryB };
};

const toBase64 = (str) => {
    return Buffer.from(str, 'utf-8').toString('base64');
};

const xorBase64Operation = (a, b) => {
    const base64A = toBase64(a);
    const base64B = toBase64(b);

    const binaryA = toBinary(base64A);
    const binaryB = toBinary(base64B);

    const xorAB = xorBinaryStrings(binaryA, binaryB);
    const xorABB = xorBinaryStrings(xorAB, binaryB);

    const xorBase64 = binaryToString(xorAB);
    const xorBase64_XORb = binaryToString(xorABB);

    return { xorBase64, xorBase64_XORb, originalA: a, originalB: b };
};

const menu = async () => {
    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout
    });

    const a = "Zhuk";
    const b = "Sveltana";
    let menuFlag = true;

    while (menuFlag) {
        console.log("\nВыберите задание:");
        console.log("1. Документ в base64");
        console.log("2. Распределение частотных свойств алфавитов по документам");
        console.log("3. XOR в кодах ASCII");
        console.log("4. XOR в кодах base64");
        console.log("5. Выход");

        const chose = await new Promise((resolve) => rl.question('\nВаш выбор: ', resolve));

        if (chose === '1') {
            const output = encodeDocumentToBase64("italian.txt");
            fs.writeFileSync('italianBase64.txt', output, 'utf-8');
            console.log(output.match(/.{1,64}/g).join('\n'));
        } else if (chose === '2') {
            const bDoc = encodeDocumentToBase64("italian.txt");
            const textItalian = fs.readFileSync("italian.txt", "utf-8").toLowerCase();

            const entropyShannonItalian = calculateShannonEntropy(textItalian);
            console.log("Энтропия Шеннона документа italian.txt: ", entropyShannonItalian);
            const entropyHartleyItalian = calculateHartleyEntropy(Alphabets.Italian);
            console.log("Энтропия Хартли документа italian.txt: ", entropyHartleyItalian);
            console.log("Избыточность документа italian.txt", calculateRedundancy(entropyShannonItalian, entropyHartleyItalian));

            const entropyShannonB = calculateShannonEntropy(bDoc);
            console.log("Энтропия Шеннона документа italianBase64.txt: ", entropyShannonB);
            const entropyHartleyB = calculateHartleyEntropy(Alphabets.Base64);
            console.log("Энтропия Хартли документа italianBase64.txt: ", entropyHartleyB);
            console.log("Избыточность документа italianBase64.txt", calculateRedundancy(entropyShannonB, entropyHartleyB));
        } else if (chose === '3') {
            const { xorAB, xorABB, binaryA, binaryB } = xorBinaryOperation(a, b);

            const maxLength = Math.max(binaryA.length, binaryB.length);
            console.log(`a: ${a}\nb: ${b}`);
            console.log(`a (ASCII в двоичном формате): ${binaryA.padStart(maxLength, '0')}`);
            console.log(`b (ASCII в двоичном формате): ${binaryB.padStart(maxLength, '0')}`);
            console.log(`Результат a XOR b (ASCII в двоичном формате): ${xorAB.padStart(maxLength, '0')}`);
            console.log(`Результат a XOR b XOR b (ASCII в двоичном формате): ${xorABB.padStart(maxLength, '0')}`);
            const xorABBStr = binaryToString(xorABB);
            console.log(`Результат a XOR b XOR b (строка): ${xorABBStr}`);
        } else if (chose === '4') {
            const { xorBase64, xorBase64_XORb, originalA, originalB } = xorBase64Operation(a, b);

            console.log(`a: ${originalA}\nb: ${originalB}`);
            console.log(`a (Base64): ${toBase64(originalA)}`);
            console.log(`b (Base64): ${toBase64(originalB)}`);
            console.log(`Результат a XOR b XOR b в Base64: ${xorBase64_XORb}`);
        } else if (chose === '5') {
            menuFlag = false;
        }
    }

    rl.close();
};

menu();
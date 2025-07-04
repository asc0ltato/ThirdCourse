const fs = require('fs');
const xlsx = require('xlsx');

const Alphabets = {
    Italian: "аbcdеfghijklmnоpqrstuvwxyz",
    Bulgarian: "абвгдежзийклмнопрстуфхцчшщъьюя",
    Binary: "01"
};

function readFile(filePath) {
    try {
        return fs.readFileSync(filePath, 'utf8').toLowerCase();
    } catch (error) {
        console.error(`Ошибка при чтении файла ${filePath}: ${error.message}`);
        return null;
    }
}

function calculateFrequencies(text, alphabet) {
    const frequency = new Map();
    let totalChars = 0;

    for (const char of alphabet) {
        frequency.set(char, 0);
    }

    for (const char of text) {
        if (alphabet.includes(char)) {
            frequency.set(char, frequency.get(char) + 1);
            totalChars++;
        }
    }

    return { frequency, totalChars };
}

function calculateProbabilities(frequency, totalChars) {
    const probabilities = new Map();
    for (const [char, count] of frequency.entries()) {
        probabilities.set(char, count / totalChars);
    }
    return probabilities;
}

function calculateEntropy(probabilities) {
    let entropy = 0;
    for (const p of probabilities.values()) {
        if (p > 0) {
            entropy -= p * Math.log2(p);
        }
    }
    return entropy;
}

function calculateConditionalEntropy(p) {
    const q = 1 - p;
    return - p * Math.log2(p) - q * Math.log2(q);
}

function calculateEffectiveEntropy(entropy, p, isBinary) {
    if (p === 0.5) return 0;
    if (isBinary && p === 1) return entropy;
    else if (!isBinary && p === 1) return 0;
    return entropy - calculateConditionalEntropy(p);
}

function calculateInformation(entropy, messageLength) {
    return entropy * messageLength;
}

function createExcelFile(frequency, alphabet, fileName) {
    if (alphabet === Alphabets.Binary) return;
    const filePath = `${fileName}.xlsx`;

    if (fs.existsSync(filePath)) {
        console.log(`Файл ${filePath} уже существует.`);
        return;
    }

    const wb = xlsx.utils.book_new();
    const data = [['Символ', 'Частота']];

    for (const [char, count] of frequency.entries()) {
        if (count > 0) {
            data.push([char, count]);
        }
    }

    const ws = xlsx.utils.aoa_to_sheet(data);
    xlsx.utils.book_append_sheet(wb, ws, "Частоты");

    xlsx.writeFile(wb, filePath);
    console.log(`Файл Excel с частотами сохранен как ${filePath}`);
}

function processFile(filePath, alphabet, fullName, fileName, isBinary = false) {
    const text = readFile(filePath);
    if (!text) return;

    const { frequency, totalChars } = calculateFrequencies(text, alphabet);
    const probabilities = calculateProbabilities(frequency, totalChars);
    const entropy = calculateEntropy(probabilities);

    console.log("-----------------------------------------------------------------------------");
    console.log(`Файл: ${filePath}`);
    console.log(`Общее количество символов: ${totalChars}`);
    console.log(`Энтропия: ${entropy.toFixed(3)} бит`);

    // console.log("Частоты символов:");
    // for (const [char, count] of frequency.entries()) {
    //     if (count > 0) {
    //         console.log(`'${char}': ${count}`);
    //     }
    // }

    // console.log("\nВероятности символов:");
    // for (const [char, prob] of probabilities.entries()) {
    //     if (prob > 0) {
    //         console.log(`'${char}': ${prob.toFixed(3)}`);
    //     }
    // }

    console.log(`Количество информации в ФИО: ${calculateInformation(entropy, fullName.length).toFixed(3)} бит`);

    [0.1, 0.5, 1].forEach(errorProb => {
        const effectiveEntropy = calculateEffectiveEntropy(entropy, errorProb, isBinary);
        console.log(`Эффективная энтропия при p = ${errorProb}: ${effectiveEntropy.toFixed(3)} бит`);
        console.log(`Информация с ошибкой ${errorProb}: ${calculateInformation(effectiveEntropy, fullName.length).toFixed(3)} бит`);
    });

    createExcelFile(frequency, alphabet, fileName, fullName);
}

const fullNameLat = "ZhukSvetlanaSergeevna";
const fullNameCir = "БръмбарСветланаСергеевна";
const fullNameASCII = "1000001011010001000011100001110101000010000110000110010100001101011000100001010000111011100001100001000011110110000110000100001000011000011010110001000000100001100111000011010110000110101100001100101000011110110000110000";
processFile('italian.txt', Alphabets.Italian, fullNameLat, 'italian');
processFile('bulgarian.txt', Alphabets.Bulgarian, fullNameCir, 'bulgarian');
processFile('binary.txt', Alphabets.Binary, fullNameASCII, 'binary', true);
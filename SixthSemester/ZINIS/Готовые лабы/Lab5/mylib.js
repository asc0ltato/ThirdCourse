const generateBinary = (n) => {
    return Array.from({ length: n }, () => Math.round(Math.random()));
};

const mod2 = (x) => x % 2;

const checkParityBits = (matrix, groups, k1, k2) => {
    let parityBits = [];

    for (let i = 0; i < 2; i++) {
        if (i === 0) {
            const parity = matrix.reduce((acc, row) => acc.map((sum, j) => sum + row[j]), Array(k2).fill(0));
            parityBits = parityBits.concat(parity.map(mod2));
        }
        else {
            const parity = matrix.map(row => row.reduce((acc, val) => acc + val, 0));
            parityBits = parityBits.concat(parity.map(mod2));
        }
    }

    if (groups.includes(3)) {
        for (let i = 0; i < k2; i++) {
            const diagonal = [];
            for (let j = 0; j < k1; j++) {
                if (i + j < k2) diagonal.push(matrix[j][i + j]);
            }

            const parity = diagonal.reduce((acc, val) => acc + val, 0);
            parityBits.push(mod2(parity));
        }
    }

    if (groups.includes(4)) {
        for (let i = 0; i < k2; i++) {
            const diagonal = [];
            for (let j = 0; j < k1; j++) {
                if (k1 - 1 - j >= 0 && i + j < k2) {
                    diagonal.push(matrix[k1 - 1 - j][i + j]);
                }
            }
            const parity = diagonal.reduce((acc, val) => acc + val, 0);
            parityBits.push(mod2(parity));
        }
    }

    if (groups.includes(5)) {
        for (let i = 0; i < k2 - 4; i++) {
            const group = [];
            for (let j = 0; j < k1; j++) {
                group.push(matrix[j][i], matrix[j][i + 1], matrix[j][i + 2], matrix[j][i + 3], matrix[j][i + 4]);
            }
            const parity = group.reduce((acc, val) => acc + val, 0);
            parityBits.push(mod2(parity));
        }
    }

    return parityBits;
};

const introduceError = (matrix, k1, k2, errorCount = 1) => {
    const clonedMatrix = matrix.map(row => [...row]);
    for (let n = 0; n < errorCount; n++) {
        const i = Math.floor(Math.random() * k1);
        const j = Math.floor(Math.random() * k2);
        clonedMatrix[i][j] = 1 - clonedMatrix[i][j];
    }
    return clonedMatrix;
};

const findError = (receivedParity, expectedParity, k1, k2, matrix) => {
    const correctedMatrix = matrix.map(row => [...row]);
    const incorrectColumns = [];
    const incorrectRows = [];

    for (let n = 0; n < k2; n++) {
        if (receivedParity[n] !== expectedParity[n]) {
            incorrectColumns.push(n);
        }
    }

    for (let n = k2; n < k1 + k2; n++) {
        if (receivedParity[n] !== expectedParity[n]) {
            incorrectRows.push(n - k2);
        }
    }

    if (incorrectRows.length === 1 && incorrectColumns.length === 1) {
        correctedMatrix[incorrectRows[0]][incorrectColumns[0]] = 1 - correctedMatrix[incorrectRows[0]][incorrectColumns[0]];
    }
    else if (incorrectRows.length > 1 || incorrectColumns.length > 1) {
        for (let i = 0; i < incorrectRows.length; i++) {
            for (let j = 0; j < incorrectColumns.length; j++) {
                correctedMatrix[incorrectRows[i]][incorrectColumns[j]] = 1 - correctedMatrix[incorrectRows[i]][incorrectColumns[j]];
            }
        }
    }

    return correctedMatrix;
};

const calculateParitySums = (matrix) => {
    const rowParitySums = matrix.map(row => row.reduce((sum, val) => sum + val, 0) % 2);
    const columnParitySums = matrix[0].map((_, colIdx) => matrix.reduce((sum, row) => sum + row[colIdx], 0) % 2);
    return { rowParitySums, columnParitySums };
};

const createZMatrix = (firstHalfMatrix, secondHalfMatrix) => {
    const numRows = firstHalfMatrix.length;
    const numCols = firstHalfMatrix[0].length;

    let zMatrix = Array.from({ length: numRows }, () => Array(numCols).fill(0));

    const { rowParitySums: rowSums1, columnParitySums: colSums1 } = calculateParitySums(firstHalfMatrix);
    const { rowParitySums: rowSums2, columnParitySums: colSums2 } = calculateParitySums(secondHalfMatrix);

    const colXORResult = rowSums1.map((sum, index) => sum ^ rowSums2[index]);
    const rowXORResult = colSums1.map((sum, index) => sum ^ colSums2[index]);

    zMatrix[numRows - 1] = rowXORResult;

    for (let i = 0; i < numRows; i++) {
        zMatrix[i][numCols - 1] = colXORResult[i];
    }

    zMatrix[numRows - 1][numCols - 1] = rowXORResult[rowXORResult.length - 1] ^ colXORResult[colXORResult.length - 1];
    return zMatrix;
};

module.exports = {
    generateBinary, checkParityBits,
    introduceError, findError, createZMatrix
};
function showMatrix(matrix) {
    matrix.forEach(row => {
        console.log(row);
    });
    console.log();
}

function createMatrix(input) {
    const matrix = [];
    for (let i = 0; i < input.length; i++) {
        matrix.push(input);
        input = input.substring(1) + input[0];
    }
    return matrix;
}

function copyAndSort(arr) {
    return [...arr].sort();
}

function extractColumnEnd(grid) {
    return grid.map(row => row.at(-1)).join('');
}

function findOriginalIndex(original, array) {
    return array.indexOf(original);
}

function prependCharacters(chars, matrix) {
    return matrix.map((row, i) => chars[i] + row);
}

function restoreMatrixFromEncoded(encodedStr) {
    let matrix = Array(encodedStr.length).fill('');

    for (let i = 0; i < encodedStr.length; i++) {
        matrix = prependCharacters(encodedStr, matrix);
        showMatrix(matrix);
        matrix.sort();
    }

    return matrix;
}

module.exports = {
    showMatrix, copyAndSort, createMatrix,
    extractColumnEnd, findOriginalIndex,
    restoreMatrixFromEncoded
};
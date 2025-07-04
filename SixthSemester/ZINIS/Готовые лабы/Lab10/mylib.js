const addSymbolsToBuffer = (numberOfSymbols, buffer, slidingWindow) => {
    const symbolsToAdd = Math.min(numberOfSymbols, slidingWindow.length);
    if (symbolsToAdd > 0) {
        buffer += slidingWindow.slice(0, symbolsToAdd);
        slidingWindow = slidingWindow.slice(symbolsToAdd);
    }
    return { buffer, slidingWindow };
};

const checkBufferSize = (maxSize, buffer) => {
    return buffer.length > maxSize ? buffer.slice(-maxSize) : buffer;
};

const findMatchingSequence = (buffer, slidingWindow) => {
    let matchLength = 0;
    let nextChar = slidingWindow[0] || ' ';
    let matchIndex = 0;

    for (let i = 1; i <= slidingWindow.length; i++) {
        const sequence = slidingWindow.slice(0, i);
        const index = buffer.indexOf(sequence);
        if (index !== -1) {
            matchLength = i;
            matchIndex = index + 1;
            nextChar = slidingWindow[i] || ' ';
        } else {
            break;
        }
    }

    return { matchIndex, matchLength, nextChar };
};

module.exports = {
    addSymbolsToBuffer,
    checkBufferSize,
    findMatchingSequence
};
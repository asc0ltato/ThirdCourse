module.exports = {
    toBinary: (text) => {
        return text
            .split('')
            .map(char => char.charCodeAt(0).toString(2).padStart(8, '0'))
            .join('');
    },

    fromBinary: (binary) => {
        return binary
            .match(/.{8}/g)
            .map(byte => String.fromCharCode(parseInt(byte, 2)))
            .join('');
    },

    getHammingMatrix: (k, r) => {
        const matrix = [];

        for (let i = 0; i < r; i++) {
            matrix[i] = new Array(k + r).fill(0);
        }

        for (let j = 0; j < k; j++) {
            const pos = j + 1;
            for (let i = 0; i < r; i++) {
                matrix[i][j] = (pos & (1 << i)) ? 1 : 0;
            }
        }

        for (let i = 0; i < r; i++) {
            matrix[i][k + i] = 1;
        }

        return matrix;
    },

    calcRedundantBits: (hammingMatrix, binaryMessage, k, r) => {
        const Xr = new Array(r).fill(0);
        for (let i = 0; i < r; i++) {
            for (let j = 0; j < k; j++) {
                Xr[i] ^= parseInt(binaryMessage[j]) & hammingMatrix[i][j];
            }
        }
        return Xr;
    },

    getBitsStr: (bits) => bits.join(''),

    genErr: (binaryMessage, numOfErrors) => {
        let binaryWithError = binaryMessage.split('');
        for (let i = 0; i < numOfErrors; i++) {
            const pos = Math.floor(Math.random() * binaryWithError.length);
            binaryWithError[pos] = binaryWithError[pos] === '0' ? '1' : '0';
        }
        return binaryWithError.join('');
    },

    calcSyndrome: (Xr, XrWithError, r) => {
        const S = [];
        for (let i = 0; i < r; i++) {
            S[i] = Xr[i] ^ XrWithError[i];
        }
        return S;
    },

    genErrVector: (S, hammingMatrix) => {
        const En = new Array(hammingMatrix[0].length).fill(0);
        const syndromeValue = parseInt(S.join(''), 2); // синдром в 10 представлении
        if (syndromeValue > 0 && syndromeValue <= hammingMatrix[0].length) {
            En[syndromeValue - 1] = 1;
        }
        return En;
    },

    correctErr: (binaryWithError, En) => {
        const corrected = binaryWithError.split('');
        for (let i = 0; i < En.length; i++) {
            if (En[i] === 1) {
                corrected[i] = corrected[i] === '0' ? '1' : '0';
            }
        }
        return corrected.join('');
    }
};
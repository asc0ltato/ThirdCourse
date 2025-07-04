const {
    showMatrix, copyAndSort, createMatrix,
    extractColumnEnd, findOriginalIndex,
    restoreMatrixFromEncoded
} = require('./mylib');
const { performance } = require('perf_hooks');

const main = () => {
    const inputSample = "пес";
    const binaryString  = Array.from(inputSample)
        .map(ch => ch.charCodeAt(0).toString(2))
        .join("");

    console.log(`Информационное сообщение в ASCII (пес): ${binaryString}\n`);

    const samples = [
        "света",
        "жук",
        "песнетворчество",
        binaryString
    ];

    samples.forEach(M => {
        console.log("\t\t\t\tСообщение: " + M);
        console.log("\t\t\t\tКодирование\n");
        const startEncode = performance.now();

        const w1 = createMatrix(M);
        console.log("Матрица W1 (сдвиг строк)");
        showMatrix(w1);

        const w2 = copyAndSort(w1);
        console.log("Матрица W2 (отсортированные строки в алфавитном порядке)");
        showMatrix(w2);

        const MklastColumn = extractColumnEnd(w2);
        const zPosition = findOriginalIndex(M, w2);
        const encodedOutput = MklastColumn + zPosition;

        const codedPart = encodedOutput.substring(0, M.length);

        console.log("Закодированное сообщение: " + codedPart);
        console.log(`Номер строки, содержащей корректное сообщение (z): ${zPosition + 1}\n`);

        const endEncode = performance.now();
        const encodeTime = endEncode - startEncode;

        console.log("\t\t\t\tДекодирование\n");
        const startDecode = performance.now();

        const restored  = restoreMatrixFromEncoded(codedPart);
        console.log("Воссоздаваемая матрица W2 (отсортированные строки в алфавитном порядке)");
        showMatrix(restored);

        const decoded = restored[zPosition];
        console.log("Декодированное сообщение: " + decoded);

        const endDecode  = performance.now();
        const decodeTime = endDecode - startDecode;

        console.log(decoded === M ? "Декодирование завершилось успешно!"
            : "Возникла ошибка");

        console.log("\nВремя кодирования: " + encodeTime.toFixed(4) + " мс");
        console.log("Время декодирования: " + decodeTime.toFixed(4) + " мс\n");
    });
}

main(); 
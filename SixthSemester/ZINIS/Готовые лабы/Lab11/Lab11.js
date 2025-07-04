const {
    InfoString, Compressor,
    calculateCompressionEfficiency,
    checkCompressionOverflow,
    printCompressionSteps
} = require('./mylib');

function main(word) {
    const wordLength = word.length;
    const compressor = new Compressor();

    compressor.build(word);

    console.log(`Вероятности для слова "${word}":`);
    for (const node of compressor.nodes) {
        console.log(`p(${node.symbol}) = ${(node.high - node.low).toFixed(5)}`);
    }

    compressor.build(word);

    console.log("\nИнтервальная шкала:");
    compressor.printScale();

    const compressed = compressor.compress(word);
    console.log("\nСжатые данные:");
    printCompressionSteps();
    // console.log(InfoString.sb.join('\n'));
    // console.log(`\nРезультат: ${compressed}`);

    const decompressed = compressor.decompress(compressed, wordLength);
    console.log("\nРасжатые данные:");
    console.log(InfoString.sb.join('\n'));
    console.log(`\nРезультат: ${decompressed}`);

    checkCompressionOverflow(compressed);
    calculateCompressionEfficiency(compressed, word);
}

main('песнетворчество');
main('песнетворчествоэлектрифицированный');
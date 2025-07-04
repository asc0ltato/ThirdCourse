const util = require('util');
const ee = require('events');

const db_data = [
    {
        id: 1,
        name: 'Глухова Дашка-какашка',
        bday: '2004-11-28'
    },
    {
        id: 2,
        name: 'Рудяк Валера',
        bday: '2004-04-30'
    },
    {
        id: 3,
        name: 'Лемешевский Владик',
        bday: '2004-04-25'
    },
    {
        id: 4,
        name: 'Турчин Никитосик-эминем',
        bday: '2005-07-27'
    }
];

function DB() {
    this.select = () => db_data;
    this.insert = (row) => {
        row.id = db_data.length ? db_data[db_data.length - 1].id + 1 : 1;
        db_data.push(row);
    };
    this.update = (row) => {
        let index = db_data.findIndex(el => el.id === row.id);
        if (index !== -1) {
            db_data[index] = row;
        }
    };
    this.delete = (id) => {
        let index = db_data.findIndex(el => el.id === id);
        if (index !== -1) {
            return db_data.splice(index, 1)[0];
        }
        return null;
    }
}

util.inherits(DB, ee.EventEmitter);

exports.DB = DB;
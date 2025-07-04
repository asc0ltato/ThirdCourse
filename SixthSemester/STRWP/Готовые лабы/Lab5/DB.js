const EventEmitter = require('events');

class DB extends EventEmitter {
    constructor() {
        super();
        this.db_data = [
            { id: 1, name: 'Глухова Дашка-какашка', bday: '2004-11-28' },
            { id: 2, name: 'Рудяк Валера', bday: '2004-04-30' },
            { id: 3, name: 'Лемешевский Владик', bday: '2004-04-25' },
            { id: 4, name: 'Турчин Никита-эминем', bday: '2005-07-27' }
        ];
    }

    select() {
        return this.db_data;
    }

    insert(row) {
        row.id = this.db_data.length ? this.db_data[this.db_data.length - 1].id + 1 : 1;
        this.db_data.push(row);
    }

    update(row) {
        let index = this.db_data.findIndex(el => el.id === row.id);
        if (index !== -1) {
            this.db_data[index] = row;
        }
    }

    delete(id) {
        let index = this.db_data.findIndex(el => el.id === id);
        if (index !== -1) {
            return this.db_data.splice(index, 1)[0];
        }
        return null;
    }

    commit() {
        this.emit('COMMIT');
    }
}

module.exports = { DB };
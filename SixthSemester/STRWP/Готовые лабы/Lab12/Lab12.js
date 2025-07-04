import {Sequelize, DataTypes} from 'sequelize';
import http from 'http';
import fs from "fs";

const sequelize = new Sequelize('ZSS', 'sa', '102005', {
  host: 'localhost',
  dialect: 'mssql',
  dialectOptions: {
    encrypt: false,
    trustServerCertificate: true
  }
});

const Faculty = sequelize.define('Faculty', {
    FACULTY: {
      type: DataTypes.STRING(10),
      allowNull: false,
      primaryKey: true
    },
    FACULTY_NAME: {
      type: DataTypes.STRING(50),
      allowNull: false,
    }
  }, {
    tableName: 'FACULTY',
    timestamps: false,
  }
)

const Pulpit = sequelize.define('Pulpit', {
    PULPIT: {
      type: DataTypes.STRING(20),
      allowNull: false,
      primaryKey: true
    },
    PULPIT_NAME: {
      type: DataTypes.STRING(100),
      allowNull: false,
    },
    FACULTY: {
      type: DataTypes.STRING(10),
      allowNull: false,
      references: {
        model: Faculty,
        key: 'FACULTY'
      }
    },
  },
  {
    tableName: 'Pulpit',
    timestamps: false,
  }
);

const Subject = sequelize.define('Subject', {
    SUBJECT: {
      type: DataTypes.STRING(10),
      allowNull: false,
      primaryKey: true
    },
    SUBJECT_NAME: {
      type: DataTypes.STRING(100),
      allowNull: false,
    },
    PULPIT: {
      type: DataTypes.STRING(20),
      allowNull: false,
      references: {
        model: Pulpit,
        key: 'PULPIT'
      }
    },
  },
  {
    tableName: 'Subject',
    timestamps: false,
  }
);

const AuditoriumType = sequelize.define('AuditoriumType', {
    AUDITORIUM_TYPE: {
      type: DataTypes.STRING(10),
      allowNull: false,
      primaryKey: true
    },
    AUDITORIUM_TYPENAME: {
      type: DataTypes.STRING(30),
      allowNull: false,
    },
  },
  {
    tableName: 'Auditorium_Type',
    timestamps: false,
  }
);

const Auditorium = sequelize.define('Auditorium', {
    AUDITORIUM: {
      type: DataTypes.STRING(20),
      allowNull: false,
      primaryKey: true
    },
    AUDITORIUM_NAME: {
      type: DataTypes.STRING(50),
      allowNull: false,
    },
    AUDITORIUM_CAPACITY: {
      type: DataTypes.INTEGER,
      allowNull: false,
    },
    AUDITORIUM_TYPE: {
      type: DataTypes.STRING(50),
      allowNull: false,
      references: {
        model: AuditoriumType,
        key: 'AUDITORIUM_TYPE'
      }
    }
  },
  {
    tableName: 'Auditorium',
    timestamps: false,
  }
);

Faculty.hasMany(Pulpit, { foreignKey: 'FACULTY' });
Pulpit.belongsTo(Faculty, { foreignKey: 'FACULTY' });

Pulpit.hasMany(Subject, { foreignKey: 'PULPIT' });
Subject.belongsTo(Pulpit, { foreignKey: 'PULPIT' });

AuditoriumType.hasMany(Auditorium, { foreignKey: 'AUDITORIUM_TYPE' });
Auditorium.belongsTo(AuditoriumType, { foreignKey: 'AUDITORIUM_TYPE' });

const server = http.createServer(async (req, res) => {
  if (req.method === "GET") {
    if (req.url === '/') {
      res.setHeader('Content-Type', 'text/html');
      res.end(fs.readFileSync(process.cwd() + '/index.html'));
      return;
    }
    try {
      res.setHeader('Content-Type', 'application/json');
      switch (req.url) {
        case '/api/faculties':
          res.end(JSON.stringify(await Faculty.findAll()));
          break;
        case '/api/pulpits':
          res.end(JSON.stringify(await Pulpit.findAll()));
          break;
        case '/api/subjects':
          res.end(JSON.stringify(await Subject.findAll()));
          break;
        case '/api/auditoriumstypes':
          res.end(JSON.stringify(await AuditoriumType.findAll()));
          break;
        case '/api/auditoriums':
          res.end(JSON.stringify(await Auditorium.findAll()));
          break;
        default:
          res.statusCode = 404;
          res.end(JSON.stringify({ error: 'Not Found' }));
      }
    } catch (error) {
      res.statusCode = 500;
      res.setHeader('Content-Type', 'application/json');
      res.end(JSON.stringify({ error: 'Internal Server Error', details: error.message }));
    }
  }

  if (req.method === 'POST') {
    let body = '';

    req.on('data', chunk => {
      body += chunk.toString();
    });

    req.on('end', async () => {
      body = JSON.parse(body);
      try {
        let result;
        switch (req.url) {
          case '/api/faculties':
            if (!body.FACULTY || !body.FACULTY_NAME) throw { errors:  'Тело должно иметь FACULTY и FACULTY_NAME' }
            result = await Faculty.create(body);
            break;
          case '/api/pulpits':
            if (!body.PULPIT || !body.PULPIT_NAME || !body.FACULTY) throw { errors: 'Тело должно иметь PULPIT, PULPIT_NAME и FACULTY' }
            result = await Pulpit.create(body);
            break;
          case '/api/subjects':
            if (!body.SUBJECT || !body.SUBJECT_NAME || !body.PULPIT) throw { errors: 'Тело должно иметь SUBJECT, SUBJECT_NAME и PULPIT' }
            result = await Subject.create(body);
            break;
          case '/api/auditoriumstypes':
            if (!body.AUDITORIUM_TYPE || !body.AUDITORIUM_TYPENAME) throw { errors: 'Тело должно иметь AUDITORIUM_TYPE и AUDITORIUM_TYPENAME' }
            result = await AuditoriumType.create(body);
            break;
          case '/api/auditoriums':
            if (!body.AUDITORIUM || !body.AUDITORIUM_NAME || !body.AUDITORIUM_CAPACITY || !body.AUDITORIUM_TYPE) throw { errors: 'Тело должно иметь AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY и AUDITORIUM_TYPE' }
            result = await Auditorium.create(body);
            break;
          default:
            res.statusCode = 404;
            res.end();
            break;
        }
        if (result) {
          res.statusCode = 200;
          res.setHeader('Content-Type', 'application/json');
          res.end(JSON.stringify(result));
        }
      } catch (error) {
        res.statusCode = 500;
        res.setHeader('Content-Type', 'application/json');
        res.end(JSON.stringify({ error: error, details: error.message }));
      }
    });
  }

  if (req.method === 'PUT') {
    let body = '';

    req.on('data', chunk => {
      body += chunk.toString();
    });

    req.on('end', async () => {
      body = JSON.parse(body);
      try {
        let result;
        let count;
        switch (req.url) {
          case '/api/faculties':
            if (!body.FACULTY || !body.FACULTY_NAME) throw { errors:  'Тело должно иметь FACULTY и FACULTY_NAME' }
            result = await Faculty.findByPk(body.FACULTY);
            count = await Faculty.update({FACULTY_NAME: body.FACULTY_NAME}, {where: {FACULTY: body.FACULTY}});
            if (count[0] === 0) throw { errors: 'Факультет не найден'};
            break;
          case '/api/pulpits':
            if (!body.PULPIT || !body.PULPIT_NAME || !body.FACULTY) throw { errors: 'Тело должно иметь PULPIT, PULPIT_NAME и FACULTY' }
            result = await Pulpit.findByPk(body.PULPIT);
            count = await Pulpit.update({PULPIT_NAME: body.PULPIT_NAME, FACULTY: body.FACULTY}, {where: {PULPIT: body.PULPIT}});
            if (count[0] === 0) throw { errors: 'Кафедра не найдена'};
            break;
          case '/api/subjects':
            if (!body.SUBJECT || !body.SUBJECT_NAME || !body.PULPIT) throw { errors: 'Тело должно иметь SUBJECT, SUBJECT_NAME и PULPIT' }
            result = await Subject.findByPk(body.SUBJECT);
            count = await Subject.update({SUBJECT_NAME: body.SUBJECT_NAME, PULPIT: body.PULPIT}, {where: {SUBJECT: body.SUBJECT}});
            if (count[0] === 0) throw { errors: 'Предмет не найден'};
            break;
          case '/api/auditoriumstypes':
            if (!body.AUDITORIUM_TYPE || !body.AUDITORIUM_TYPENAME) throw { errors: 'Тело должно иметь AUDITORIUM_TYPE и AUDITORIUM_TYPENAME' }
            result = await AuditoriumType.findByPk(body.AUDITORIUM_TYPE);
            count = await AuditoriumType.update({AUDITORIUM_TYPENAME: body.AUDITORIUM_TYPENAME}, {where: {AUDITORIUM_TYPE: body.AUDITORIUM_TYPE}})
            if (count[0] === 0) throw { errors: 'Тип аудитории не найден'}
            break;
          case '/api/auditoriums':
            if (!body.AUDITORIUM || !body.AUDITORIUM_NAME || !body.AUDITORIUM_CAPACITY || !body.AUDITORIUM_TYPE) throw { errors: 'Тело должно иметь AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY и AUDITORIUM_TYPE' }
            result = await Auditorium.findByPk(body.AUDITORIUM);
            count = await Auditorium.update({ AUDITORIUM_NAME: body.AUDITORIUM_NAME, AUDITORIUM_CAPACITY: body.AUDITORIUM_CAPACITY, AUDITORIUM_TYPE: body.AUDITORIUM_TYPE }, { where: { AUDITORIUM: body.AUDITORIUM } });
            if (count[0] === 0) throw { errors: 'Аудитория не найдена'}
            break;
          default:
            res.statusCode = 404;
            res.end();
        }
        if (result) {
          res.statusCode = 200;
          res.setHeader('Content-Type', 'application/json');
          res.end(JSON.stringify(result));
        }
      } catch (error) {
        res.statusCode = 500;
        res.setHeader('Content-Type', 'application/json');
        res.end(JSON.stringify({ error: error.errors, details: error.message }));
      }
    });
  }

  if (req.method === 'DELETE') {
    let param = decodeURIComponent(req.url.split('/')[3]);
    try {
      let result;
      if (req.url.startsWith('/api/faculties/')) {
        result = await Faculty.findByPk(param);
        const count = await Faculty.destroy({
          where: { FACULTY: param }
        });
        if (count === 0) {
          throw { errors: 'Факультет не найден'}
        }
      } else if (req.url.startsWith('/api/pulpits/')) {
        result = await Pulpit.findByPk(param);
        const count = await Pulpit.destroy({
          where: { PULPIT: param }
        });
        if (count === 0) {
          throw { errors: 'Кафедра не найдена'}
        }
      } else if (req.url.startsWith('/api/subjects/')) {
        result = await Subject.findByPk(param);
        const count = await Subject.destroy(({
          where: { SUBJECT: param }
        }));
        if (count === 0) {
          throw { errors: 'Предмет не найден'}
        }
      } else if (req.url.startsWith('/api/auditoriumstypes/')) {
        result = await AuditoriumType.findByPk(param);
        const count = await AuditoriumType.destroy(({
          where: { AUDITORIUM_TYPE: param }
        }));
        if (count === 0) {
          throw { errors: 'Тип аудитории не найден'}
        }
      } else if (req.url.startsWith('/api/auditoriums/')) {
        result = await Auditorium.findByPk(param);
        const count = await Auditorium.destroy({
          where: {AUDITORIUM: param }
        });
        if (count === 0) {
          throw { errors: 'Аудитория не найдена'};
        }
      } else {
        res.statusCode = 404;
        res.setHeader('Content-Type', 'application/json');
        res.end(JSON.stringify({ error: 'Not Found' }));
        return;
      }

      res.statusCode = 200;
      res.setHeader('Content-Type', 'application/json');
      res.end(JSON.stringify(result));
    } catch (error) {
      res.statusCode = 500;
      res.setHeader('Content-Type', 'application/json');
      res.end(JSON.stringify({ error: error.errors, details: error.message }));
    }
  }
});

server.listen(3000, () => {
  console.log(`Server running at http://localhost:3000/`);
});

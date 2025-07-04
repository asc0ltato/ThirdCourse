create database ZSS;
use ZSS;

drop table PROGRESS;
drop table STUDENT;
drop table [GROUP];
drop table SUBJECT;
drop table TEACHER;
drop table PULPIT;
drop table PROFESSION;
drop table FACULTY;
drop table AUDITORIUM;
drop table AUDITORIUM_TYPE;

CREATE TABLE FACULTY (
                         FACULTY NVARCHAR(10) NOT NULL PRIMARY KEY,
                         FACULTY_NAME NVARCHAR(50) DEFAULT '???'
);

CREATE TABLE PROFESSION (
                            PROFESSION NVARCHAR(20) NOT NULL PRIMARY KEY,
                            FACULTY NVARCHAR(10) NOT NULL,
                            PROFESSION_NAME NVARCHAR(100) NULL,
                            QUALIFICATION NVARCHAR(50) NULL,
                            CONSTRAINT FK_PROFESSION_FACULTY FOREIGN KEY (FACULTY) REFERENCES FACULTY(FACULTY)
);

CREATE TABLE PULPIT (
                        PULPIT NVARCHAR(10) NOT NULL PRIMARY KEY,
                        PULPIT_NAME NVARCHAR(100) NULL,
                        FACULTY NVARCHAR(10) NOT NULL,
                        CONSTRAINT FK_PULPIT_FACULTY FOREIGN KEY (FACULTY) REFERENCES FACULTY(FACULTY)
);

CREATE TABLE TEACHER (
                         TEACHER NVARCHAR(10) NOT NULL PRIMARY KEY,
                         TEACHER_NAME NVARCHAR(100) NULL,
                         GENDER NVARCHAR(1) CHECK (GENDER IN ('м', 'ж')),
                         PULPIT NVARCHAR(10) NOT NULL,
                         CONSTRAINT FK_TEACHER_PULPIT FOREIGN KEY (PULPIT) REFERENCES PULPIT(PULPIT)
);

CREATE TABLE SUBJECT (
                         SUBJECT NVARCHAR(10) NOT NULL PRIMARY KEY,
                         SUBJECT_NAME NVARCHAR(100) NULL UNIQUE,
                         PULPIT NVARCHAR(10) NOT NULL,
                         CONSTRAINT FK_SUBJECT_PULPIT FOREIGN KEY (PULPIT) REFERENCES PULPIT(PULPIT)
);

CREATE TABLE AUDITORIUM_TYPE (
                                 AUDITORIUM_TYPE NVARCHAR(10) NOT NULL PRIMARY KEY,
                                 AUDITORIUM_TYPENAME NVARCHAR(30) NULL
);

CREATE TABLE AUDITORIUM (
                            AUDITORIUM NVARCHAR(20) NOT NULL PRIMARY KEY,
                            AUDITORIUM_TYPE NVARCHAR(10) NOT NULL,
                            AUDITORIUM_CAPACITY INT DEFAULT 1 CHECK (AUDITORIUM_CAPACITY BETWEEN 1 AND 300),
                            AUDITORIUM_NAME NVARCHAR(50) NULL,
                            CONSTRAINT FK_AUDITORIUM_AUDITORIUM_TYPE FOREIGN KEY (AUDITORIUM_TYPE) REFERENCES AUDITORIUM_TYPE(AUDITORIUM_TYPE)
);

CREATE TABLE [GROUP] (
                         IDGROUP INT NOT NULL PRIMARY KEY,
                         FACULTY NVARCHAR(10) NOT NULL,
                         PROFESSION NVARCHAR(20) NOT NULL,
                         YEAR_FIRST SMALLINT CHECK (YEAR_FIRST < YEAR(GETDATE()) + 2),
                         COURSE AS (YEAR(GETDATE()) - YEAR_FIRST), -- Убрано PERSISTED
                         CONSTRAINT FK_GROUP_FACULTY FOREIGN KEY (FACULTY) REFERENCES FACULTY(FACULTY),
                         CONSTRAINT FK_GROUP_PROFESSION FOREIGN KEY (PROFESSION) REFERENCES PROFESSION(PROFESSION)
);

CREATE TABLE STUDENT (
                         IDSTUDENT INT IDENTITY(1000,1) NOT NULL PRIMARY KEY,
                         IDGROUP INT NOT NULL,
                         NAME NVARCHAR(100),
                         BDAY DATE,
                         STAMP TIMESTAMP,
                         INFO XML DEFAULT NULL,
                         FOTO VARBINARY(MAX) DEFAULT NULL,
                         CONSTRAINT FK_STUDENT_GROUP FOREIGN KEY (IDGROUP) REFERENCES [GROUP](IDGROUP)
);

CREATE TABLE PROGRESS (
                          SUBJECT NVARCHAR(10) NOT NULL,
                          IDSTUDENT INT NOT NULL,
                          PDATE DATE,
                          NOTE INT CHECK (NOTE BETWEEN 1 AND 10),
                          CONSTRAINT FK_PROGRESS_SUBJECT FOREIGN KEY (SUBJECT) REFERENCES SUBJECT(SUBJECT),
                          CONSTRAINT FK_PROGRESS_STUDENT FOREIGN KEY (IDSTUDENT) REFERENCES STUDENT(IDSTUDENT)
);

--------------------------------------
select * from FACULTY WHERE FACULTY = 'test';
UPDATE FACULTY SET FACULTY_NAME = 'test_name2' WHERE FACULTY = 'test';
INSERT INTO FACULTY (FACULTY, FACULTY_NAME) VALUES
                                                (N'ТТЛП', N'Технологии и техника лесной промышленности'),
                                                (N'ТОВ', N'Технологии органических веществ'),
                                                (N'ХТиТ', N'Химические технологии и техника'),
                                                (N'ИЭФ', N'Инженерно-экономический'),
                                                (N'ЛХФ', N'Лесохозяйственный'),
                                                (N'ИДиП', N'Издательское дело и полиграфия'),
                                                (N'ИТ', N'Информационных технологий');
select * from PROFESSION;
INSERT INTO PROFESSION (PROFESSION, FACULTY, PROFESSION_NAME, QUALIFICATION) VALUES
                                                                                 (N'1-36 06 01', N'ИДиП', N'Полиграфическое оборудование и системы обработки информации', N'инженер-электромеханик'),
                                                                                 (N'1-36 07 01', N'ХТиТ', N'Машины и аппараты химических производств и предприятий строительных материалов', N'инженер-механик'),
                                                                                 (N'1-40 01 02', N'ИТ', N'Информационные системы и технологии', N'инженер-программист-системотехник'),
                                                                                 (N'1-46 01 01', N'ТТЛП', N'Лесоинженерное дело', N'инженер-технолог'),
                                                                                 (N'1-47 01 01', N'ИДиП', N'Издательское дело', N'редактор-технолог'),
                                                                                 (N'1-48 01 02', N'ТОВ', N'Химическая технология органических веществ, материалов и изделий', N'инженер-химик-технолог'),
                                                                                 (N'1-48 01 05', N'ТОВ', N'Химическая технология переработки древесины', N'инженер-химик-технолог'),
                                                                                 (N'1-54 01 03', N'ТОВ', N'Физико-химические методы и приборы контроля качества продукции', N'инженер по сертификации'),
                                                                                 (N'1-75 01 01', N'ЛХФ', N'Лесное хозяйство', N'инженер лесного хозяйства'),
                                                                                 (N'1-75 02 01', N'ЛХФ', N'Садово-парковое строительство', N'инженер садово-паркового строительства'),
                                                                                 (N'1-89 02 02', N'ЛХФ', N'Туризм и природопользование', N'специалист в сфере туризма');

select * from pulpit;
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY) VALUES
                                                      (N'РИТ', N'Редакционно-издательских технологий', N'ИДиП'),
                                                      (N'СБУАиА', N'Статистики, бухгалтерского учета, анализа и аудита', N'ИЭФ'),
                                                      (N'ТДП', N'Технологий деревообрабатывающих производств', N'ТТЛП'),
                                                      (N'ТиДИД', N'Технологии и дизайна изделий из древесины', N'ТТЛП'),
                                                      (N'ТиП', N'Туризма и природопользования', N'ЛХФ'),
                                                      (N'ТЛ', N'Транспорта леса', N'ТТЛП'),
                                                      (N'ТНВиОХТ', N'Технологии неорганических веществ и общей химической технологии', N'ХТиТ'),
                                                      (N'ТНХСиППМ', N'Технологии нефтехимического синтеза и переработки полимерных материалов', N'ТОВ'),
                                                      (N'ХПД', N'Химической переработки древесины', N'ТОВ'),
                                                      (N'ХТЭПиМЭЕ', N'Химии, технологии электрохимических производств и материалов электронной техники', N'ХТиТ'),
                                                      (N'ЭТиМ', N'Экономической теории и маркетинга', N'ИЭФ');

INSERT INTO TEACHER (TEACHER, TEACHER_NAME, GENDER, PULPIT) VALUES
                                                                (N'НСКВ', N'Носков Михаил Трофимович', NULL, N'ТЛ'),
                                                                (N'ПРКП', N'Прокопенко Николай Иванович', NULL, N'ТНХСиППМ'),
                                                                (N'МРЗВ', N'Морозова Елена Степановна', NULL, N'РИТ'),
                                                                (N'РВКС', N'Ровкас Андрей Петрович', NULL, N'ТДП'),
                                                                (N'РЖКВ', N'Рыжиков Леонид Николаевич', NULL, N'ТДП'),
                                                                (N'РМНВ', N'Романов Дмитрий Михайлович', NULL, N'РИТ'),
                                                                (N'СМЛВ', N'Смелов Владимир Владиславович', NULL, N'РИТ'),
                                                                (N'КРЛВ', N'Крылов Павел Павлович', NULL, N'РИТ'),
                                                                (N'ЧРН', N'Чернова Анна Викторовна', NULL, N'ХПД'),
                                                                (N'МХВ', N'Мохов Михаил Сергеевич', NULL, N'РИТ');


INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT) VALUES
                                                        (N'ПЗ', N'Представление знаний в компьютерных системах', N'РИТ'),
                                                        (N'ПИС', N'Проектирование информационных систем', N'РИТ'),
                                                        (N'ПСП', N'Программирование сетевых приложений', N'РИТ'),
                                                        (N'ПЭХ', N'Прикладная электрохимия', N'ХТЭПиМЭЕ'),
                                                        (N'СУБД', N'Системы управления базами данных', N'РИТ'),
                                                        (N'ТиОЛ', N'Технология и оборудование лесозаготовок', N'ТЛ'),
                                                        (N'ТРИ', N'Технология резиновых изделий', N'ТНХСиППМ'),
                                                        (N'ЭП', N'Экономика природопользования', N'ЭТиМ'),
                                                        (N'ЭТ', N'Экономическая теория', N'ЭТиМ');

INSERT INTO AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME) VALUES
                                                                       (N'ЛБ-X', N'Химическая лаборатория'),
                                                                       (N'ЛБ-К', N'Компьютерный класс'),
                                                                       (N'ЛБ-СК', N'Спец. компьютерный класс'),
                                                                       (N'ЛК', N'Лекционная'),
                                                                       (N'ЛК-К', N'Лекционная с уст. проектором');

INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY, AUDITORIUM_NAME) VALUES
                                                                                               (N'301-1', N'ЛБ-К', 15, N'301-1'),
                                                                                               (N'304-4', N'ЛБ-К', 90, N'304-4'),
                                                                                               (N'313-1', N'ЛК-К', 60, N'313-1'),
                                                                                               (N'314-4', N'ЛК', 90, N'314-4'),
                                                                                               (N'320-4', N'ЛК', 90, N'320-4'),
                                                                                               (N'324-1', N'ЛК-К', 50, N'324-1'),
                                                                                               (N'413-1', N'ЛБ-К', 15, N'413-1'),
                                                                                               (N'423-1', N'ЛБ-К', 90, N'423-1');
INSERT INTO [GROUP] (IDGROUP, FACULTY, PROFESSION, YEAR_FIRST) VALUES
                                                                   (22, N'ЛХФ', N'1-75 02 01', 2011),
                                                                   (23, N'ЛХФ', N'1-89 02 02', 2012),
                                                                   (24, N'ЛХФ', N'1-89 02 02', 2011),
                                                                   (25, N'ТТЛП', N'1-46 01 01', 2013),
                                                                   (26, N'ТТЛП', N'1-46 01 01', 2012),
                                                                   (27, N'ТТЛП', N'1-46 01 01', 2012),
                                                                   (28, N'ИЭФ', N'1-36 06 01', 2013),
                                                                   (29, N'ИЭФ', N'1-36 06 01', 2012),
                                                                   (30, N'ИЭФ', N'1-36 06 01', 2010),
                                                                   (31, N'ИЭФ', N'1-36 07 01', 2013),
                                                                   (32, N'ИЭФ', N'1-36 07 01', 2012);
SET IDENTITY_INSERT STUDENT ON;
INSERT INTO STUDENT (IDSTUDENT, IDGROUP, NAME, BDAY, STAMP, INFO, FOTO) VALUES
                                                                            (1000, 22, N'Пугач Михаил Трофимович', N'1996-01-12', NULL, NULL, NULL),
                                                                            (1001, 23, N'Авдеев Николай Иванович', N'1996-07-19', NULL, NULL, NULL),
                                                                            (1002, 24, N'Белова Елена Степановна', N'1996-05-22', NULL, NULL, NULL),
                                                                            (1003, 25, N'Вилков Андрей Петрович', N'1996-12-08', NULL, NULL, NULL),
                                                                            (1004, 26, N'Грушин Леонид Николаевич', N'1995-11-11', NULL, NULL, NULL),
                                                                            (1005, 27, N'Дунаев Дмитрий Михайлович', N'1996-08-24', NULL, NULL, NULL),
                                                                            (1006, 28, N'Клуни Иван Владиславович', N'1996-09-15', NULL, NULL, NULL),
                                                                            (1007, 29, N'Крылов Олег Павлович', N'1996-10-16', NULL, NULL, NULL);
INSERT INTO PROGRESS (SUBJECT, IDSTUDENT, PDATE, NOTE) VALUES
                                                           (N'ПЗ', 1000, N'2014-01-12', 4),
                                                           (N'ПЗ', 1001, N'2014-01-19', 5),
                                                           (N'ПЗ', 1002, N'2014-01-08', 9),
                                                           (N'ПЭХ', 1003, N'2014-01-11', 8),
                                                           (N'ПЭХ', 1004, N'2014-01-15', 4),
                                                           (N'СУБД', 1005, N'2014-01-16', 7),
                                                           (N'СУБД', 1006, N'2014-01-27', 6);


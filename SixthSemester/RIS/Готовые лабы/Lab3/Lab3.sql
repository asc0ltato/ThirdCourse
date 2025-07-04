CREATE PLUGGABLE DATABASE "PDBORCL" ADMIN USER ZSS_admin IDENTIFIED BY 102005
  FILE_NAME_CONVERT=(
    '/opt/oracle/oradata/SIGMASID/pdbseed/system01.dbf',   
    '/opt/oracle/oradata/SIGMASID/PDBORCL/system01.dbf',
    '/opt/oracle/oradata/SIGMASID/pdbseed/undotbs01.dbf', 
    '/opt/oracle/oradata/SIGMASID/PDBORCL/undotbs01.dbf',
    '/opt/oracle/oradata/SIGMASID/pdbseed/temp012025-02-14_23-30-43-753-PM.dbf',
    '/opt/oracle/oradata/SIGMASID/PDBORCL/temp012025-02-14_23-30-43-753-PM.dbf',
    '/opt/oracle/oradata/SIGMASID/pdbseed/sysaux01.dbf',
    '/opt/oracle/oradata/SIGMASID/PDBORCL/sysaux01.dbf'
  ) 
  STORAGE UNLIMITED TEMPFILE REUSE;

SELECT NAME, OPEN_MODE FROM V$PDBS;

ALTER PLUGGABLE DATABASE PDBORCL CLOSE IMMEDIATE;
DROP PLUGGABLE DATABASE PDBORCL INCLUDING DATAFILES;

ALTER PLUGGABLE DATABASE PDBORCL OPEN;
--------------------------------------------------------------------------------
--1)Создание пользователей на каждом сервере
--От SYS_PDBORCL
CREATE USER ZSS IDENTIFIED BY 102005;
GRANT CONNECT, CREATE SESSION, CREATE TABLE, CREATE DATABASE LINK TO ZSS;
ALTER USER ZSS QUOTA UNLIMITED ON SYSTEM;

select * from all_users;

SELECT tablespace_name FROM dba_tablespaces;

--2)Установка DBLINK между серверами
CREATE DATABASE LINK ZSS_LVO
CONNECT TO LVO IDENTIFIED BY "1234"
USING 'ZSS_LVO';

--SHUTDOWN IMMEDIATE;
--STARTUP;

DROP DATABASE LINK ZSS_LVO;
--3)Создание таблиц
CREATE TABLE ZSS (
   id NUMBER PRIMARY KEY,
   name VARCHAR2(255) NOT NULL
);

DROP TABLE ZSS;

SELECT * FROM ZSS;
SELECT * FROM LVO2@ZSS_LVO;
--4)Разработка SQL-скрипта для распределённых транзакций
--INSERT/INSERT
BEGIN
   INSERT INTO ZSS (id, name) VALUES (2, 'Lera');
   COMMIT;

   INSERT INTO LVO2@ZSS_LVO (id, name) VALUES (3, 'Lera');
   COMMIT;
END;

--INSERT/UPDATE
BEGIN
   INSERT INTO ZSS (id, name) VALUES (3, 'Nikitka');
   COMMIT;

   UPDATE LVO2@ZSS_LVO SET name = 'Nikitka' WHERE id = 1;
   COMMIT;
END;

--UPDATE/INSERT
BEGIN
   UPDATE ZSS SET name = 'Vlad' WHERE id = 1337;
   COMMIT;

   INSERT INTO LVO2@ZSS_LVO (id, name) VALUES (4, 'Vlad');
   COMMIT;
END;

--5)Смоделировать ошибку нарушения целостности
--Транзакция с нарушением ограничения целостности
BEGIN
   INSERT INTO ZSS (id, name) VALUES (1, 'Item 1');
   COMMIT;

   INSERT INTO LVO2@ZSS_LVO (id, name) VALUES (1, 'Invalid Item');
   COMMIT;
END;

--6)Смоделировать блокировку ресурса
BEGIN
   DELETE FROM LVO2@ZSS_LVO WHERE id = 1;
END;
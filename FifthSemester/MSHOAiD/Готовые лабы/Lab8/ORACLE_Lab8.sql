GRANT CREATE TYPE TO RL_ZSSCORE;

--Объектные типы данных для таблицы Cars
CREATE OR REPLACE TYPE Car_Type AS OBJECT (
    Car_ID NUMBER,
    Status_ID NUMBER,
    Brand VARCHAR2(255),
    Model VARCHAR2(255),
    YearOf NUMBER,
    License_Plate VARCHAR2(20),
    Rental_Price NUMBER(10, 2),
    Location VARCHAR2(255),
    --Дополнительный конструктор
    CONSTRUCTOR FUNCTION Car_Type (
        Car_ID NUMBER,
        Status_ID NUMBER,
        Brand VARCHAR2,
        Model VARCHAR2,
        YearOf NUMBER,
        License_Plate VARCHAR2,
        Rental_Price NUMBER,
        Location VARCHAR2
    ) RETURN SELF AS RESULT,
    --Метод экземпляра функцию
    MEMBER FUNCTION Get_Full_Name RETURN VARCHAR2,
    --Метод экземпляра процедуру
    MEMBER PROCEDURE Update_Rental_Price(New_Price NUMBER)
);

--Тело объекта
CREATE OR REPLACE TYPE BODY Car_Type AS
    CONSTRUCTOR FUNCTION Car_Type (
        Car_ID NUMBER,
        Status_ID NUMBER,
        Brand VARCHAR2,
        Model VARCHAR2,
        YearOf NUMBER,
        License_Plate VARCHAR2,
        Rental_Price NUMBER,
        Location VARCHAR2
    ) RETURN SELF AS RESULT IS
    BEGIN
        SELF.Car_ID := Car_ID;
        SELF.Status_ID := Status_ID;
        SELF.Brand := Brand;
        SELF.Model := Model;
        SELF.YearOf := YearOf;
        SELF.License_Plate := License_Plate;
        SELF.Rental_Price := Rental_Price;
        SELF.Location := Location;
        RETURN;
    END;
    
    MEMBER FUNCTION Get_Full_Name RETURN VARCHAR2 IS
    BEGIN
        RETURN SELF.Brand || ' ' || SELF.Model || ' (' || SELF.YearOf || ')';
    END;
    
    MEMBER PROCEDURE Update_Rental_Price(New_Price NUMBER) IS
    BEGIN
        SELF.Rental_Price := New_Price;
    END;
END;

--Объектный тип для таблицы Users
CREATE OR REPLACE TYPE User_Type AS OBJECT (
    User_ID NUMBER,
    Role_ID NUMBER,
    First_Name VARCHAR2(50),
    Last_Name VARCHAR2(50),
    Address VARCHAR2(255),
    Phone_Number VARCHAR2(15),
    Email VARCHAR2(255),
    Drivers_License VARCHAR2(20),
    --Метод сравнения типа MAP
    MAP MEMBER FUNCTION Compare_By_Name RETURN VARCHAR2
);

-- Тело объекта
CREATE OR REPLACE TYPE BODY User_Type AS
    MAP MEMBER FUNCTION Compare_By_Name RETURN VARCHAR2 IS
    BEGIN
        RETURN SELF.Last_Name || ', ' || SELF.First_Name;
    END;
END;

--Создание объектных таблиц и заполнение их из реляционной таблицы Cars
CREATE TABLE Cars_Object_Table OF Car_Type;

INSERT INTO Cars_Object_Table
SELECT Car_Type(
Car_ID, Status_ID, Brand, Model, YearOf, License_Plate, Rental_Price, Location
)
FROM Cars;

SELECT * FROM Cars_Object_Table;

--Создание объектных таблиц и заполнение их из реляционной таблицы Users
CREATE TABLE Users_Object_Table OF User_Type;

INSERT INTO Users_Object_Table
SELECT User_Type(
User_ID, Role_ID, First_Name, Last_Name, Address, Phone_Number, Email, Drivers_License
)
FROM Users;

SELECT * FROM Users_Object_Table;

--Создание объектного представления 
CREATE OR REPLACE VIEW Cars_Object_View AS
SELECT c.Car_ID, c.Brand, c.Model, c.YearOf, c.Rental_Price
FROM Cars_Object_Table c;

SELECT * FROM Cars_Object_View;

--Индексы
CREATE INDEX idx_cars_object_license_plate ON Cars_Object_Table (License_Plate);
CREATE INDEX idx_users_object_email ON Users_Object_Table (Email);
CREATE INDEX idx_full_name ON Cars_Object_Table (Brand || Model || YearOf);

--5 задание. Тест
SELECT c.Get_Full_Name() FROM Cars_Object_Table c;


DECLARE

    car Car_Type;
BEGIN

SELECT VALUE(c) INTO car
FROM Cars_Object_Table c
WHERE c.Car_ID = 1;


IF car IS NOT NULL THEN

    car.Update_Rental_Price(200.00);


    DBMS_OUTPUT.PUT_LINE('Updated Price: ' || car.Rental_Price);
ELSE
    DBMS_OUTPUT.PUT_LINE('Car with ID 1 not found');
END IF;
END;


SELECT u.Compare_By_Name() FROM Users_Object_Table u;

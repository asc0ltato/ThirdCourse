--.open C:\Users\super\Desktop\sqlite-tools-win-x64-3470000\Rent.db
CREATE TABLE Status (
    Status_ID INTEGER PRIMARY KEY,
    Status_Name TEXT
);

CREATE TABLE Role (
    Role_ID INTEGER PRIMARY KEY,
    Role_Name TEXT
);

CREATE TABLE Cars (
    Car_ID INTEGER PRIMARY KEY,
    Status_ID INTEGER,
    Brand TEXT,
    Model TEXT,
    YearOf INTEGER,
    License_Plate TEXT,
    Rental_Price REAL,
    Location TEXT,
    FOREIGN KEY (Status_ID) REFERENCES Status(Status_ID)
);

CREATE TABLE Users (
    User_ID INTEGER PRIMARY KEY,
    Role_ID INTEGER,
    First_Name TEXT,
    Last_Name TEXT,
    Address TEXT,
    Phone_Number TEXT,
    Email TEXT UNIQUE,
    Drivers_License TEXT,
    FOREIGN KEY (Role_ID) REFERENCES Role(Role_ID)
);

CREATE TABLE Payments (
    Payment_ID INTEGER PRIMARY KEY,
    Payment_Amount REAL,
    Payment_Date DATE,
    Payment_Method TEXT
);

CREATE TABLE Orders (
    Order_ID INTEGER PRIMARY KEY,
    User_ID INTEGER,
    Car_ID INTEGER,
    Payment_ID INTEGER,
    Start_DateTime DATE,
    End_DateTime DATE,
    FOREIGN KEY (User_ID) REFERENCES Users(User_ID),
    FOREIGN KEY (Car_ID) REFERENCES Cars(Car_ID),
    FOREIGN KEY (Payment_ID) REFERENCES Payments(Payment_ID)
);

INSERT INTO Status (Status_ID, Status_Name) VALUES 
(1, 'Доступен'),
(2, 'Арендован'),
(3, 'В ремонте');

INSERT INTO Role (Role_ID, Role_Name) VALUES 
(1, 'Guest'),
(2, 'User'),
(3, 'Manager'),
(4, 'Admin');

INSERT INTO Cars (Car_ID, Status_ID, Brand, Model, YearOf, License_Plate, Rental_Price, Location) VALUES 
(1, 1, 'Toyota', 'Camry', 2020, 'А123ВС77', 120.50, 'г.Минск, ул. Белорусская, д.19'),
(2, 2, 'Hyundai', 'Solaris', 2019, 'В456АС77', 100.99, 'г.Минск, ул. Казинца, д.23'),
(3, 3, 'BMW', 'X5', 2021, 'С789ДА77', 130.99, 'г.Минск, ул. Свердлова, д.11'),
(4, 1, 'Audi', 'A6', 2015, 'Е987МК77', 80.00, 'г.Минск, ул. Петра Глебки, д.9'),
(5, 2, 'Mercedes', 'C-Class', 2022, 'Р654ОМ77', 150.00, 'г.Минск, ул. Белорусская, д.19');

INSERT INTO Users (User_ID, Role_ID, First_Name, Last_Name, Address, Phone_Number, Email, Drivers_License) VALUES 
(1, 4, 'Иван', 'Иванов', 'г.Минск, ул. Независимость, д.20', '+37560000001', 'ivanov@mail.ru', NULL),
(2, 2, 'Ольга', 'Петрова', 'г.Минск, ул. Партизанская, д.15', '+37560000002', 'petrova@mail.ru', '7800222333'),
(3, 3, 'Алексей', 'Сидоров', 'г.Минск, ул. Киселева, д.13', '+37560000003', 'sidorov@mail.ru', NULL),
(4, 3, 'Мария', 'Васильева', 'г.Минск, ул. Жудра, д.50', '+37560000004', 'vasilieva@mail.ru', NULL),
(5, 2, 'Дмитрий', 'Козлов', 'г.Минск, ул. Кижеватова, д.26', '+37560000005', 'kozlov@mail.ru', '7400555666');

INSERT INTO Payments (Payment_ID, Payment_Amount, Payment_Date, Payment_Method) VALUES 
(1, 28920.00, '2024-09-10', 'Карта'),
(2, 24237.60, '2024-09-15', 'Наличка'),
(3, 31437.60, '2024-09-20', 'Наличка'),
(4, 19200.00, '2024-09-25', 'Карта'),
(5, 36000.00, '2024-09-30', 'Карта');

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime) VALUES 
(1, 2, 1, 2, '2024-09-01', '2024-09-10'),
(2, 5, 2, 3, '2024-09-05', '2024-09-15'),
(3, 4, 3, 2, '2024-09-10', '2024-09-20'),
(4, 2, 4, 3, '2024-09-15', '2024-09-25'),
(5, 5, 5, 1, '2024-09-20', '2024-09-30');

----------------------------------------------------------------------------------------
BEGIN TRANSACTION;
INSERT INTO Payments (Payment_ID, Payment_Amount, Payment_Date, Payment_Method) VALUES (6, 15000.00, '2024-10-01', 'Карта');
INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime) VALUES (6, 3, 4, 6, '2024-10-01', '2024-10-15');
COMMIT;
----------------------------------------------------------------------------------------
BEGIN TRANSACTION;
UPDATE Cars SET Status_ID = 2 WHERE Car_ID = 4;
UPDATE Orders SET Car_ID = 5 WHERE Order_ID = 6;
COMMIT;
----------------------------------------------------------------------------------------
BEGIN TRANSACTION;
DELETE FROM Users WHERE User_ID = 5;
COMMIT;
----------------------------------------------------------------------------------------
	
SELECT * FROM Cars;
SELECT * FROM Users;
SELECT * FROM Orders;
SELECT * FROM Payments;


CREATE VIEW OrderDetails AS
SELECT 
    O.Order_ID, 
    U.First_Name, 
    U.Last_Name, 
    C.Brand, 
    C.Model,
    C.License_Plate,
    O.Start_DateTime, 
    O.End_DateTime, 
    P.Payment_Amount, 
    P.Payment_Method
FROM Orders O
JOIN Users U ON O.User_ID = U.User_ID
JOIN Cars C ON O.Car_ID = C.Car_ID
JOIN Payments P ON O.Payment_ID = P.Payment_ID;

SELECT * FROM OrderDetails;

CREATE INDEX idx_Users_LastName_FirstName ON Users(Last_Name, First_Name);
CREATE INDEX idx_Users_DriversLicence ON Users(Drivers_License);
CREATE INDEX idx_Cars_Brand_Model ON Cars(Brand, Model);
CREATE INDEX idx_Cars_License_Plate ON Cars(License_Plate);

CREATE TRIGGER trg_CheckOrderInsert
BEFORE INSERT ON Orders
FOR EACH ROW
BEGIN
    SELECT CASE
        WHEN NEW.End_DateTime < NEW.Start_DateTime THEN
            RAISE(ABORT, 'Дата окончания аренды не может быть раньше даты начала.')
    END;
END;

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime) VALUES (6, 5, 1, 1, '2024-09-20', '2024-09-10');



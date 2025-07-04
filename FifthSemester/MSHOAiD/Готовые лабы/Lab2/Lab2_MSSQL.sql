create database Rent;
use Rent;
drop database Rent;
drop table Status;
drop table Role;
drop table Cars;
drop table Users;
drop table Payments;
drop table Orders;

CREATE TABLE Status (
    Status_ID INT PRIMARY KEY,
    Status_Name VARCHAR(50)
);

CREATE TABLE Role (
    Role_ID INT PRIMARY KEY,
    Role_Name VARCHAR(50)
);

CREATE TABLE Cars (
    Car_ID INT PRIMARY KEY,
	Status_ID INT,
	Brand VARCHAR(255),
    Model VARCHAR(255),
    YearOf INT,
	License_Plate VARCHAR(20),
    Rental_Price DECIMAL(10, 2),
    Location VARCHAR(255),
    FOREIGN KEY (Status_id) REFERENCES Status(Status_ID)
);

CREATE TABLE Users (
    User_ID INT PRIMARY KEY,
	Role_ID INT,
    First_Name VARCHAR(50),
    Last_Name VARCHAR(50),
    Address VARCHAR(255),
    Phone_Number VARCHAR(15),
    Email VARCHAR(255) UNIQUE,
    Drivers_License VARCHAR(20)
	FOREIGN KEY (Role_ID) REFERENCES Role(Role_ID)
);

CREATE TABLE Payments (
    Payment_ID INT PRIMARY KEY,
    Payment_Amount DECIMAL(10, 2),
    Payment_Date DATE,
	Payment_Method VARCHAR(50)
);

CREATE TABLE Orders (
    Order_ID INT PRIMARY KEY,
	User_ID INT,
	Car_ID INT,
    Payment_ID INT,
    Start_DateTime DATE,
    End_DateTime DATE,
    FOREIGN KEY (User_ID) REFERENCES Users(User_ID),
    FOREIGN KEY (Car_ID) REFERENCES Cars(Car_ID),
    FOREIGN KEY (Payment_ID) REFERENCES Payments(Payment_ID)
);

select * from Status;
select * from Role;
select * from Cars;
select * from Users;
select * from Orders;
select * from Payments;

CREATE SEQUENCE seq_User_ID
START WITH 1
INCREMENT BY 1
NO CACHE;  
 
CREATE SEQUENCE seq_Payment_ID
START WITH 1
INCREMENT BY 1
NO CACHE; 

CREATE SEQUENCE seq_Order_ID
START WITH 1
INCREMENT BY 1
NO CACHE; 

CREATE SEQUENCE seq_Car_ID
START WITH 1
INCREMENT BY 1
NO CACHE; 

INSERT INTO Status (Status_ID, Status_Name)
VALUES 
(1, 'Доступен'),
(2, 'Арендован'),
(3, 'В ремонте');

INSERT INTO Role (Role_ID, Role_Name)
VALUES 
(1, 'Guest'),
(2, 'User'),
(3, 'Manager'),
(4, 'Admin');

INSERT INTO Cars (Car_ID, Status_ID, Brand, Model, YearOf, License_Plate, Rental_Price, Location)
VALUES 
(NEXT VALUE FOR seq_Car_ID, 1, 'Toyota', 'Camry', 2020, 'А123ВС77', 120.50, 'г. Минск, ул. Белорусская, д.19'),
(NEXT VALUE FOR seq_Car_ID, 2, 'Hyundai', 'Solaris', 2019, 'В456АС77', 100.99, 'г. Минск, ул. Казинца, д.23'),
(NEXT VALUE FOR seq_Car_ID, 3, 'BMW', 'X5', 2021, 'С789ДА77', 130.99, 'г. Минск, ул. Свердлова, д.11'),
(NEXT VALUE FOR seq_Car_ID, 1, 'Audi', 'A6', 2015, 'Е987МК77', 80.00, 'г. Минск, ул. Петра Глебки, д.9'),
(NEXT VALUE FOR seq_Car_ID, 2, 'Mercedes', 'C-Class', 2022, 'Р654ОМ77', 150.00, 'г. Минск, ул. Белорусская, д.19');

INSERT INTO Users (User_ID, Role_ID, First_Name, Last_Name, Address, Phone_number, Email, Drivers_license)
VALUES 
(NEXT VALUE FOR seq_User_ID, 4, 'Иван', 'Иванов', 'г.Минск, ул. Независимость, д.20', '+37560000001', 'ivanov@mail.ru', NULL),
(NEXT VALUE FOR seq_User_ID, 2, 'Ольга', 'Петрова', 'г.Минск, ул. Партизанская, д.15', '+37560000002', 'petrova@mail.ru', '7800222333'),
(NEXT VALUE FOR seq_User_ID, 3, 'Алексей', 'Сидоров', 'г.Минск, ул. Киселева, д.13', '+37560000003', 'sidorov@mail.ru', NULL),
(NEXT VALUE FOR seq_User_ID, 3, 'Мария', 'Васильева', 'г.Минск, ул. Жудра, д.50', '+37560000004', 'vasilieva@mail.ru', NULL),
(NEXT VALUE FOR seq_User_ID, 2, 'Дмитрий', 'Козлов', 'г.Минск, ул. Кижеватова, д.26', '+37560000005', 'kozlov@mail.ru', '7400555666');

INSERT INTO Payments (Payment_ID, Payment_Amount, Payment_Date, Payment_Method)
VALUES 
(NEXT VALUE FOR seq_Payment_ID, 28920.00, '2024-09-10', 'Карта'),
(NEXT VALUE FOR seq_Payment_ID, 24237.60, '2024-09-15', 'Наличные'),
(NEXT VALUE FOR seq_Payment_ID, 31437.60, '2024-09-20', 'Наличные'),
(NEXT VALUE FOR seq_Payment_ID, 19200.00, '2024-09-25', 'Карта'),
(NEXT VALUE FOR seq_Payment_ID, 36000.00, '2024-09-30', 'Карта');

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES 
(NEXT VALUE FOR seq_Order_ID, 2, 1, 2, '2024-09-01', '2024-09-10'),
(NEXT VALUE FOR seq_Order_ID, 5, 2, 3, '2024-09-05', '2024-09-15'),
(NEXT VALUE FOR seq_Order_ID, 4, 3, 2, '2024-09-10', '2024-09-20'),
(NEXT VALUE FOR seq_Order_ID, 2, 4, 3, '2024-09-15', '2024-09-25'),
(NEXT VALUE FOR seq_Order_ID, 5, 5, 1, '2024-09-20', '2024-09-30');

-- Представление для доступных автомобилей
CREATE VIEW AvailableCars AS
SELECT 
    Car_ID, 
    Brand, 
    Model, 
    YearOf, 
    License_Plate, 
    Rental_Price, 
    Location 
FROM Cars 
WHERE Status_ID = 1;

SELECT * FROM AvailableCars;
DROP VIEW AvailableCars;
-- Представление для всех заказов с деталями пользователя и автомобиля
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
DROP VIEW OrderDetails;

CREATE INDEX idx_Users_LastName_FirstName ON Users(Last_Name, First_Name);
CREATE INDEX idx_Users_DriversLicence ON Users(Drivers_License);
CREATE INDEX idx_Cars_Brand_Model ON Cars(Brand, Model);
CREATE INDEX idx_Cars_License_Plate ON Cars(License_Plate);

-- Функция для расчета общей суммы арендных платежей за период
CREATE FUNCTION GetRentalIncomeForRole(@RoleName VARCHAR(50), @StartDate DATE, @EndDate DATE)
RETURNS DECIMAL(10, 2)
AS
BEGIN
    DECLARE @TotalIncome DECIMAL(10, 2);
    SELECT @TotalIncome = SUM(P.Payment_Amount)
    FROM Orders O
    JOIN Payments P ON O.Payment_ID = P.Payment_ID
    JOIN Users U ON O.User_ID = U.User_ID
    JOIN Role R ON U.Role_ID = R.Role_ID
    WHERE R.Role_Name = @RoleName 
    AND O.Start_DateTime >= @StartDate
    AND O.End_DateTime <= @EndDate;
    RETURN @TotalIncome; 
END;
DECLARE @RoleName VARCHAR(50) = 'User'; 
DECLARE @StartDate DATE = '2024-09-01'; 
DECLARE @EndDate DATE = '2024-09-30'; 
SELECT dbo.GetRentalIncomeForRole(@RoleName, @StartDate, @EndDate) AS TotalRentalIncome;
-- Процедура для создания нового заказа
CREATE PROCEDURE InsertOrder
    @UserID INT,
    @CarID INT,
    @PaymentAmount DECIMAL(10, 2),
    @PaymentDate DATETIME,
    @PaymentMethod VARCHAR(50),
    @StartDateTime DATETIME,
    @EndDateTime DATETIME
AS
BEGIN
    IF EXISTS (
        SELECT 1 
        FROM Orders 
        WHERE Car_ID = @CarID 
        AND (Start_DateTime < @EndDateTime AND End_DateTime > @StartDateTime)
    )
    BEGIN
        RAISERROR('Автомобиль недоступен на указанные даты', 16, 1);
        RETURN;
    END;
    DECLARE @PaymentID INT;
    
    INSERT INTO Payments (Payment_ID, Payment_Amount, Payment_Date, Payment_Method)
    VALUES (NEXT VALUE FOR seq_Payment_ID, @PaymentAmount, @PaymentDate, @PaymentMethod);
    
    SET @PaymentID = SCOPE_IDENTITY();
    INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
    VALUES (NEXT VALUE FOR seq_Order_ID, @UserID, @CarID, @PaymentID, @StartDateTime, @EndDateTime);
END;

DROP procedure InsertOrder;

select * from Payments;
select * from Orders;
select * from Cars;

EXEC InsertOrder 
    @UserID = 5, 
    @CarID = 4, 
    @PaymentAmount = 130.00, 
    @PaymentDate = '2024-10-10', 
    @PaymentMethod = 'Карта', 
    @StartDateTime = '2024-10-01', 
    @EndDateTime = '2024-10-05';

-- Триггер для автоматического обновления статуса автомобиля при создании нового заказа
CREATE TRIGGER trg_AutoUpdateCarStatus
ON Orders
AFTER INSERT
AS
BEGIN
    UPDATE Cars
    SET Status_ID = 2
    WHERE Car_ID IN (SELECT Car_ID FROM inserted);
END;

INSERT INTO Payments (Payment_ID, Payment_Amount, Payment_Date, Payment_Method)
VALUES (NEXT VALUE FOR seq_Payment_ID, 150.00, '2024-10-01', 'Карта');

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES (NEXT VALUE FOR seq_Order_ID, 5, 1, 6, '2024-10-01', '2024-10-05');

SELECT Car_ID, Status_ID FROM Cars WHERE Car_ID = 1;

-- Триггер для проверки на дублирующийся платеж
CREATE TRIGGER trg_CheckDuplicatePayment
ON Payments
INSTEAD OF INSERT
AS
BEGIN
    IF EXISTS (
        SELECT 1
        FROM Payments p
        JOIN inserted i
        ON p.Payment_Amount = i.Payment_Amount
        AND p.Payment_Date = i.Payment_Date
        AND p.Payment_Method = i.Payment_Method
    )
    BEGIN
        RAISERROR('Платеж с такими же данными уже существует', 16, 1);
        ROLLBACK TRANSACTION;
    END
    ELSE
    BEGIN
        INSERT INTO Payments (Payment_Amount, Payment_Date, Payment_Method)
        SELECT Payment_Amount, Payment_Date, Payment_Method
        FROM inserted;
    END
END;

INSERT INTO Payments (Payment_ID, Payment_Amount, Payment_Date, Payment_Method)
VALUES 
(NEXT VALUE FOR seq_Payment_ID, 130.00, '2024-05-10', 'Карта');

-- Триггер для проверки дат в таблице Orders
CREATE TRIGGER trg_CheckOrderDates
ON Orders
FOR INSERT, UPDATE
AS
BEGIN
    IF EXISTS (SELECT 1 FROM inserted WHERE End_DateTime < Start_DateTime)
    BEGIN
        RAISERROR('Дата окончания аренды не может быть раньше даты начала.', 16, 1);
        ROLLBACK TRANSACTION;
    END
END;

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES (NEXT VALUE FOR seq_Order_ID, 5, 1, 1, '2024-09-20', '2024-09-10');
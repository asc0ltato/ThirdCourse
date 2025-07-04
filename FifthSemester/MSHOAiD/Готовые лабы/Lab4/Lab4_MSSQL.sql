use Rent;

-- 1.Вычисление итогов предоставленных услуг помесячно, за квартал, за полгода, за год.
WITH ServiceTotals AS (
    SELECT 
        DATEPART(YEAR, o.Start_DateTime) AS ServiceYear,
        DATEPART(MONTH, o.Start_DateTime) AS ServiceMonth,
        DATEPART(QUARTER, o.Start_DateTime) AS ServiceQuarter,
        CASE 
            WHEN DATEPART(MONTH, o.Start_DateTime) <= 6 THEN 'Первые полгода'
            ELSE 'Вторые полгода'
        END AS ServiceHalfYear,
        p.Payment_Amount
    FROM Orders o
    JOIN Payments p ON o.Payment_ID = p.Payment_ID
)
SELECT
    ServiceYear,
    ServiceMonth,
    ServiceQuarter,
    ServiceHalfYear,
    SUM(Payment_Amount) AS MonthlyTotal,
    SUM(SUM(Payment_Amount)) OVER (PARTITION BY ServiceYear) AS YearlyTotal,
    SUM(SUM(Payment_Amount)) OVER (PARTITION BY ServiceYear, ServiceHalfYear) AS HalfYearlyTotal,
    SUM(SUM(Payment_Amount)) OVER (PARTITION BY ServiceYear, ServiceQuarter) AS QuarterlyTotal
FROM ServiceTotals
GROUP BY ServiceYear, ServiceMonth, ServiceQuarter, ServiceHalfYear
ORDER BY ServiceYear, ServiceMonth;

INSERT INTO Cars (Car_ID, Status_ID, Brand, Model, YearOf, License_Plate, Rental_Price, Location)
VALUES 
(NEXT VALUE FOR seq_Car_ID, 1, 'Ford', 'Focus', 2018, 'А321ВС77', 90.50, 'г.Минск, ул. Янки Купалы, д.5'),
(NEXT VALUE FOR seq_Car_ID, 1, 'Nissan', 'Qashqai', 2021, 'В654АС77', 110.00, 'г.Минск, ул. Ленина, д.30'),
(NEXT VALUE FOR seq_Car_ID, 2, 'Volkswagen', 'Passat', 2019, 'С987ДА77', 100.50, 'г.Минск, ул. Калинина, д.18'),
(NEXT VALUE FOR seq_Car_ID, 1, 'Kia', 'Sportage', 2020, 'Д456МК77', 105.00, 'г.Минск, ул. Богдановича, д.27'),
(NEXT VALUE FOR seq_Car_ID, 1, 'Chevrolet', 'Tahoe', 2017, 'Е654ОМ77', 140.00, 'г.Минск, ул. Гикало, д.9');

INSERT INTO Users (User_ID, Role_ID, First_Name, Last_Name, Address, Phone_number, Email, Drivers_license)
VALUES 
(NEXT VALUE FOR seq_User_ID, 2, 'Сергей', 'Морозов', 'г.Минск, ул. Победителей, д.10', '+37560000006', 'morozov@mail.ru', '8901234567'),
(NEXT VALUE FOR seq_User_ID, 2, 'Екатерина', 'Зайцева', 'г.Минск, ул. Сухая, д.20', '+37560000007', 'zaitseva@mail.ru', NULL),
(NEXT VALUE FOR seq_User_ID, 2, 'Николай', 'Федоров', 'г.Минск, ул. Якуба Коласа, д.15', '+37560000008', 'fedorov@mail.ru', '8000555666'),
(NEXT VALUE FOR seq_User_ID, 2, 'Анна', 'Соколова', 'г.Минск, ул. Машерова, д.13', '+37560000009', 'sokolova@mail.ru', '7300111222'),
(NEXT VALUE FOR seq_User_ID, 3, 'Василий', 'Ильин', 'г.Минск, ул. Щорса, д.7', '+37560000010', 'ilin@mail.ru', NULL);

INSERT INTO Payments (Payment_ID, Payment_Amount, Payment_Date, Payment_Method)
VALUES 
(NEXT VALUE FOR seq_Payment_ID, 15200.00, '2024-10-01', 'Карта'),
(NEXT VALUE FOR seq_Payment_ID, 12375.50, '2024-10-05', 'Наличка'),
(NEXT VALUE FOR seq_Payment_ID, 8900.00, '2024-10-10', 'Карта'),
(NEXT VALUE FOR seq_Payment_ID, 17500.00, '2024-10-15', 'Карта'),
(NEXT VALUE FOR seq_Payment_ID, 11350.00, '2024-10-20', 'Наличка'),
(NEXT VALUE FOR seq_Payment_ID, 24700.00, '2024-10-25', 'Карта'),
(NEXT VALUE FOR seq_Payment_ID, 18900.00, '2024-10-30', 'Карта');

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES 
(NEXT VALUE FOR seq_Order_ID, 6, 6, 6, '2024-10-01', '2024-10-05'),
(NEXT VALUE FOR seq_Order_ID, 7, 7, 7, '2024-10-06', '2024-10-10'),
(NEXT VALUE FOR seq_Order_ID, 8, 8, 8, '2024-10-11', '2024-10-15'),
(NEXT VALUE FOR seq_Order_ID, 9, 9, 9, '2024-10-16', '2024-10-20'),
(NEXT VALUE FOR seq_Order_ID, 10, 10, 10, '2024-10-21', '2024-10-25'),
(NEXT VALUE FOR seq_Order_ID, 5, 5, 4, '2024-10-01', '2024-10-15'),
(NEXT VALUE FOR seq_Order_ID, 3, 2, 5, '2024-09-25', '2024-10-05'),
(NEXT VALUE FOR seq_Order_ID, 4, 3, 7, '2024-10-10', '2024-10-30');

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES 
(NEXT VALUE FOR seq_Order_ID, 2, 1, 6, '2024-01-10', '2024-01-20'),
(NEXT VALUE FOR seq_Order_ID, 3, 2, 7, '2024-02-01', '2024-02-15'),
(NEXT VALUE FOR seq_Order_ID, 4, 3, 8, '2024-03-05', '2024-03-10'),
(NEXT VALUE FOR seq_Order_ID, 4, 4, 9, '2024-03-15', '2024-03-25');

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES 
(NEXT VALUE FOR seq_Order_ID, 5, 5, 10, '2024-04-01', '2024-04-10'),
(NEXT VALUE FOR seq_Order_ID, 6, 6, 11, '2024-05-05', '2024-05-20'),
(NEXT VALUE FOR seq_Order_ID, 7, 7, 6, '2024-06-01', '2024-06-15'),
(NEXT VALUE FOR seq_Order_ID, 8, 8, 7, '2024-06-20', '2024-06-30');

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES 
(NEXT VALUE FOR seq_Order_ID, 9, 9, 8, '2024-07-01', '2024-07-10'),
(NEXT VALUE FOR seq_Order_ID, 10, 10, 9, '2024-08-05', '2024-08-15'),
(NEXT VALUE FOR seq_Order_ID, 2, 1, 10, '2024-09-10', '2024-09-20'),
(NEXT VALUE FOR seq_Order_ID, 3, 2, 11, '2024-09-25', '2024-09-30');

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES 
(NEXT VALUE FOR seq_Order_ID, 4, 3, 6, '2024-10-05', '2024-10-15'),
(NEXT VALUE FOR seq_Order_ID, 5, 4, 7, '2024-11-01', '2024-11-10'),
(NEXT VALUE FOR seq_Order_ID, 6, 5, 8, '2024-11-01', '2024-12-20'),
(NEXT VALUE FOR seq_Order_ID, 7, 6, 9, '2024-11-01', '2024-11-11');

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES 
(NEXT VALUE FOR seq_Order_ID, 8, 7, 10, '2023-10-01', '2023-10-15'),
(NEXT VALUE FOR seq_Order_ID, 9, 8, 11, '2023-11-05', '2023-11-18'),
(NEXT VALUE FOR seq_Order_ID, 10, 9, 6, '2023-11-01', '2023-11-15');

select * from Payments;
select * from Orders;
select * from Users;
select * from Cars;

--2 Вычисление итогов предоставленных услуг за период:
--	объем услуг;
--	сравнение их с общим объемом услуг (в %);
--	сравнение с наибольшим объемом услуг (в %).
WITH ServiceVolumes AS (
    SELECT 
        o.Car_ID,
        DATEPART(YEAR, o.Start_DateTime) AS ServiceYear,
        DATEPART(MONTH, o.Start_DateTime) AS ServiceMonth,
        SUM(p.Payment_Amount) AS TotalServiceVolume
    FROM Orders o
    JOIN Payments p ON o.Payment_ID = p.Payment_ID
    WHERE o.Car_ID = 2
    GROUP BY 
        o.Car_ID,
        DATEPART(YEAR, o.Start_DateTime),
        DATEPART(MONTH, o.Start_DateTime)
)
SELECT 
    sv.ServiceYear,
    sv.ServiceMonth,
    sv.TotalServiceVolume,
    (sv.TotalServiceVolume / SUM(sv.TotalServiceVolume) OVER () * 100) AS PercentageOfTotal,
    (sv.TotalServiceVolume / MAX(sv.TotalServiceVolume) OVER () * 100) AS PercentageOfMax
FROM ServiceVolumes sv
ORDER BY sv.ServiceYear, sv.ServiceMonth;

--3. Продемонстрируйте применение функции ранжирования ROW_NUMBER() для разбиения результатов 
--запроса на страницы (по 20 строк на каждую страницу).
DECLARE @PageNumber INT;
SET @PageNumber = 1;

WITH RankedOrders AS (
    SELECT 
        ROW_NUMBER() OVER (ORDER BY Order_ID) AS RowNum,
        Order_ID,
        Car_ID,
        Start_DateTime,
        End_DateTime
    FROM Orders
)
SELECT 
    Order_ID,
    Car_ID,
    Start_DateTime,
    End_DateTime
FROM RankedOrders
WHERE RowNum > (20 * (@PageNumber - 1)) AND RowNum <= (20 * @PageNumber);

--4.Продемонстрируйте применение функции ранжирования ROW_NUMBER() для удаления дубликатов.
WITH RankedOrders AS (
SELECT 
    ROW_NUMBER() OVER (PARTITION BY Car_ID, Start_DateTime ORDER BY Order_ID) AS RowNum,
    Order_ID,
    Car_ID,
    Start_DateTime
FROM Orders
)
DELETE FROM RankedOrders
WHERE RowNum > 1;

------------------------------------------------------------------------------------------
WITH RankedOrders AS (
SELECT 
    ROW_NUMBER() OVER (PARTITION BY Car_ID, Start_DateTime ORDER BY Order_ID) AS RowNum,
    Order_ID,
    Car_ID,
    Start_DateTime
FROM Orders
)
SELECT * 
FROM RankedOrders
WHERE RowNum > 1;

--5.Вернуть для каждого вида клиентов суммы за аренду последних 6 месяцев помесячно.
WITH LastSixMonthsRentals AS (
    SELECT 
        r.Role_Name AS UserRole,             
        DATEPART(YEAR, o.Start_DateTime) AS RentYear,  
        DATEPART(MONTH, o.Start_DateTime) AS RentMonth,  
        SUM(p.Payment_Amount) AS MonthlyTotal          
    FROM Orders o
    JOIN Payments p ON o.Payment_ID = p.Payment_ID
    JOIN Users u ON o.User_ID = u.User_ID   
    JOIN Role r ON u.Role_ID = r.Role_ID 
    WHERE o.Start_DateTime >= DATEADD(MONTH, -6, GETDATE())  
    GROUP BY r.Role_Name, DATEPART(YEAR, o.Start_DateTime), DATEPART(MONTH, o.Start_DateTime)
)
SELECT 
    UserRole,
    RentYear,
    RentMonth,
    MonthlyTotal
FROM LastSixMonthsRentals
ORDER BY UserRole, RentYear, RentMonth;

--6.Какая услуга была предоставлена наибольшее число раз для определенного вида?
--Вернуть для всех клиентов.
WITH ServiceRankings AS (
    SELECT 
        r.Role_Name,                    
        o.Car_ID,                          
        COUNT(*) AS ServiceCount,         
        ROW_NUMBER() OVER (
            PARTITION BY r.Role_Name     
            ORDER BY COUNT(*) DESC       
        ) AS RowNum
    FROM Orders o
    JOIN Users u ON o.User_ID = u.User_ID        
    JOIN Role r ON u.Role_ID = r.Role_ID      
    GROUP BY r.Role_Name, o.Car_ID
)
SELECT 
    Role_Name,
    Car_ID,                              
    ServiceCount
FROM ServiceRankings
WHERE RowNum = 1                         
ORDER BY Role_Name;
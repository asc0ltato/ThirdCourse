SELECT *
FROM (
        SELECT
        u.User_ID,
        u.First_Name || ' ' || u.Last_Name AS Full_Name,
        c.Car_ID,
        c.Brand,
        c.Model,
        c.Rental_Price,
        EXTRACT(MONTH FROM o.Start_DateTime) AS Rent_Month,
        EXTRACT(YEAR FROM o.Start_DateTime) AS Rent_Year
    FROM
        Orders o
        JOIN Users u ON o.User_ID = u.User_ID
        JOIN Cars c ON o.Car_ID = c.Car_ID
)
MODEL
    PARTITION BY (User_ID, Full_Name, Car_ID, Brand, Model)
    DIMENSION BY (Rent_Year, Rent_Month)
    MEASURES (Rental_Price AS Price, 0 AS Next_Year_Price)
    RULES (
    Next_Year_Price[ANY, ANY] = Price[CV(Rent_Year), CV(Rent_Month)] * 1.1
);


SELECT *
FROM (
    SELECT
        o.Car_ID,
        COUNT(o.Order_ID) AS Orders_Count,
        TRUNC(o.Start_DateTime, 'MM') AS Rent_Period
    FROM Orders o
    GROUP BY o.Car_ID, TRUNC(o.Start_DateTime, 'MM')
)
MATCH_RECOGNIZE (
    PARTITION BY Car_ID
    ORDER BY Rent_Period
    MEASURES
        FIRST(A.Rent_Period) AS First_Rent_Period,
        FIRST(A.Orders_Count) AS First_Order_Count,
        FIRST(B.Rent_Period) AS Second_Rent_Period,
        FIRST(B.Orders_Count) AS Second_Order_Count,
        FIRST(C.Rent_Period) AS Three_Rent_Period,
        FIRST(C.Orders_Count) AS Three_Order_Count
    PATTERN (A B C)
    DEFINE
        A AS A.Orders_Count > PREV(A.Orders_Count),
        B AS B.Orders_Count < PREV(B.Orders_Count),
        C AS C.Orders_Count > PREV(C.Orders_Count)
);

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES (seq_Order_ID.NEXTVAL, 5, 3, 2, TO_DATE('2024-09-01', 'YYYY-MM-DD'), TO_DATE('2024-09-10', 'YYYY-MM-DD'));

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES (seq_Order_ID.NEXTVAL, 5, 3, 2, TO_DATE('2024-11-10', 'YYYY-MM-DD'), TO_DATE('2024-11-20', 'YYYY-MM-DD'));

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES (seq_Order_ID.NEXTVAL, 5, 3, 2, TO_DATE('2024-10-10', 'YYYY-MM-DD'), TO_DATE('2024-10-20', 'YYYY-MM-DD'));

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES (seq_Order_ID.NEXTVAL, 5, 3, 2, TO_DATE('2024-12-01', 'YYYY-MM-DD'), TO_DATE('2024-12-01', 'YYYY-MM-DD'));

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES (seq_Order_ID.NEXTVAL, 5, 3, 2, TO_DATE('2024-08-01', 'YYYY-MM-DD'), TO_DATE('2024-08-10', 'YYYY-MM-DD'));

SELECT * FROM ORDERS ;

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES (seq_Order_ID.NEXTVAL, 3, 1, 4, TO_DATE('2024-10-01', 'YYYY-MM-DD'), TO_DATE('2024-10-10', 'YYYY-MM-DD'));

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES (seq_Order_ID.NEXTVAL, 4, 1, 5, TO_DATE('2024-10-15', 'YYYY-MM-DD'), TO_DATE('2024-10-25', 'YYYY-MM-DD'));

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES (seq_Order_ID.NEXTVAL, 1, 2, 3, TO_DATE('2024-10-01', 'YYYY-MM-DD'), TO_DATE('2024-10-10', 'YYYY-MM-DD'));

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES (seq_Order_ID.NEXTVAL, 2, 2, 4, TO_DATE('2024-10-15', 'YYYY-MM-DD'), TO_DATE('2024-10-25', 'YYYY-MM-DD'));

INSERT INTO Orders (Order_ID, User_ID, Car_ID, Payment_ID, Start_DateTime, End_DateTime)
VALUES (seq_Order_ID.NEXTVAL, 3, 3, 5, TO_DATE('2024-11-01', 'YYYY-MM-DD'), TO_DATE('2024-11-10', 'YYYY-MM-DD'));
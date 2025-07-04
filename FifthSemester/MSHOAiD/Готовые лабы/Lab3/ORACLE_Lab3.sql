ALTER TABLE Cars
ADD Parent_ID NUMBER;

ALTER TABLE Cars
DROP COLUMN Parent_ID;

UPDATE Cars
SET Parent_ID = 1
WHERE Car_ID > 1 AND Car_ID < 5;

SELECT * FROM Cars;

CREATE OR REPLACE PROCEDURE DisplaySubordinates(
    NodeId NUMBER
)
IS
BEGIN
    FOR rec IN (
        SELECT
            Car_ID,
            Parent_ID,
            Status_ID,
            Brand,
            Model,
            YearOf,
            License_Plate,
            Rental_Price,
            Location
        FROM Cars
        START WITH Car_ID = NodeId
        CONNECT BY PRIOR Car_ID = Parent_ID
)
    LOOP
        DBMS_OUTPUT.PUT_LINE('Car ID: ' || rec.Car_ID ||
            ', Parent_ID: ' || rec.Parent_ID ||
            ', Status_ID: ' || rec.Status_ID ||
            ', Brand: ' || rec.Brand ||
            ', Model: ' || rec.Model ||
            ', Year: ' || rec.YearOf ||
            ', License Plate: ' || rec.License_Plate ||
            ', Rental Price: ' || rec.Rental_Price ||
            ', Location: ' || rec.Location);
    END LOOP;
END DisplaySubordinates;
/
EXEC DisplaySubordinates(1);

DROP PROCEDURE DisplaySubordinates;
--------------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE AddSubordinate(
    ParentNodeId IN NUMBER,
    NewBrand IN VARCHAR2,
    NewModel IN VARCHAR2,
    NewYearOf IN NUMBER,
    NewLicensePlate IN VARCHAR2,
    NewRentalPrice IN NUMBER,
    NewLocation IN VARCHAR2
)
AS
    NewCarId NUMBER;
BEGIN
    SELECT seq_Car_ID.NEXTVAL INTO NewCarId FROM dual;
    
    INSERT INTO Cars (Car_ID, Status_ID, Brand, Model, YearOf, License_Plate, Rental_Price, Location, Parent_ID)
    VALUES (NewCarId, 1, NewBrand, NewModel, NewYearOf, NewLicensePlate, NewRentalPrice, NewLocation, ParentNodeId);
END AddSubordinate;
/
EXEC AddSubordinate(2, 'BMW1', 'Almera', 2018, 'F111AA79', 90, 'г.Минск, ул. Примерная, д.2');
--------------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE MoveSubtree(
    SourceNodeId IN NUMBER,
    DestinationNodeId IN NUMBER
)
IS
BEGIN
    UPDATE Cars
    SET Parent_ID = DestinationNodeId
    WHERE Car_ID = SourceNodeId
    OR Car_ID IN (
        SELECT Car_ID
        FROM Cars
        START WITH Car_ID = SourceNodeId
        CONNECT BY PRIOR Car_ID = Parent_ID
    );
END MoveSubtree;

EXEC MoveSubtree(2, 1);
EXEC DisplaySubordinates(1);
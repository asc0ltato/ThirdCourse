use Rent;

--1.Для базы данных в СУБД SQL Server добавить для одной из таблиц столбец данных иерархического типа. 
ALTER TABLE Cars
ADD Hierarchy_ID HIERARCHYID;

ALTER TABLE Cars
DROP COLUMN Hierarchy_ID;

SELECT * FROM Cars;
--2.Создать процедуру, которая отобразит все подчиненные узлы с указанием уровня иерархии 
--(параметр – значение узла).
DECLARE @RootNode HIERARCHYID;
SET @RootNode = HIERARCHYID::GetRoot();
UPDATE Cars
SET Hierarchy_ID = @RootNode.GetDescendant(NULL, NULL)
WHERE Brand = 'Toyota'; 

DECLARE @ToyotaNode HIERARCHYID;
SET @ToyotaNode = (SELECT TOP 1 Hierarchy_ID FROM Cars WHERE Brand = 'Toyota');
UPDATE Cars
SET Hierarchy_ID = @ToyotaNode.GetDescendant(NULL, NULL)
WHERE Brand = 'Hyundai';

DECLARE @ToyotaNode HIERARCHYID;
SET @ToyotaNode = (SELECT TOP 1 Hierarchy_ID FROM Cars WHERE Brand = 'Toyota');
UPDATE Cars
SET Hierarchy_ID = HIERARCHYID::Parse('/1/2/')
WHERE Brand = 'BMW';

CREATE PROCEDURE GetNodes
    @Node HIERARCHYID
AS
BEGIN
    SELECT 
		Hierarchy_ID.GetLevel() AS Hierarchy_Level,
		Hierarchy_ID.ToString() AS Hierarchy_Path,
        Car_ID,
        Brand,
        Model
    FROM 
        Cars
    WHERE 
        Hierarchy_ID.IsDescendantOf(@Node) = 1;
END;

DECLARE @CarsRoot HIERARCHYID = HIERARCHYID::GetRoot();
EXEC GetNodes @CarsRoot; 

DROP PROCEDURE GetNodes;
--3.Создать процедуру, которая добавит подчиненный узел (параметр – значение узла).
CREATE PROCEDURE AddNode
    @ParentNode HIERARCHYID,
    @Car_ID INT,
	@Status_ID INT,
    @Brand VARCHAR(255),
    @Model VARCHAR(255),
    @YearOf INT,
    @License_Plate VARCHAR(20),
    @Rental_Price DECIMAL(10, 2),
    @Location VARCHAR(255)
AS
BEGIN
    DECLARE @NewHierarchyID HIERARCHYID;

    SET @NewHierarchyID = @ParentNode.GetDescendant(NULL, NULL);

    INSERT INTO Cars (Car_ID, Status_ID, Brand, Model, YearOf, License_Plate, Rental_Price, Location, Hierarchy_ID)
    VALUES (@Car_ID, @Status_ID, @Brand, @Model, @YearOf, @License_Plate, @Rental_Price, @Location, @NewHierarchyID);
END;

DECLARE @HyundaiNode HIERARCHYID;
SET @HyundaiNode = (SELECT TOP 1 Hierarchy_ID FROM Cars WHERE Brand = 'Hyundai');
DECLARE @NextCarID INT;
SET @NextCarID = NEXT VALUE FOR seq_User_ID;
EXEC AddNode 
    @ParentNode = @HyundaiNode,
	@Status_ID = 1,
    @Car_ID = @NextCarID,
    @Brand = 'Nissan',
    @Model = 'Almera',
    @YearOf = 2018,
    @License_Plate = 'F111AA77',
    @Rental_Price = 90.00,
    @Location = 'г.Минск, ул. Примерная, д.2';

DECLARE @CarsRoot HIERARCHYID = HIERARCHYID::GetRoot();
EXEC GetNodes @CarsRoot; 

DROP PROCEDURE AddNode;
DELETE FROM Cars WHERE Brand = 'Nissan';
--4.Создать процедуру, которая переместит всю подчиненную ветку (первый параметр – значение 
--верхнего перемещаемого узла, второй параметр – значение узла, в который происходит перемещение).
CREATE PROCEDURE MoveSubordinateBranch
    @SourceNode HIERARCHYID,
    @TargetNode HIERARCHYID
AS
BEGIN
    DECLARE @NewHierarchyID HIERARCHYID;
    DECLARE @CurrentNode HIERARCHYID;

    DECLARE MoveCursor CURSOR FOR
    SELECT Hierarchy_ID FROM Cars WHERE Hierarchy_ID.IsDescendantOf(@SourceNode) = 1;

    OPEN MoveCursor;
    FETCH NEXT FROM MoveCursor INTO @CurrentNode;

    WHILE @@FETCH_STATUS = 0
    BEGIN
        SET @NewHierarchyID = @TargetNode.GetDescendant(NULL, NULL);

        UPDATE Cars
        SET Hierarchy_ID = @NewHierarchyID
        WHERE Hierarchy_ID = @CurrentNode;

        FETCH NEXT FROM MoveCursor INTO @CurrentNode;
    END

    CLOSE MoveCursor;
    DEALLOCATE MoveCursor;
END;

DECLARE @SourceNode HIERARCHYID;
SET @SourceNode = (SELECT TOP 1 Hierarchy_ID FROM Cars WHERE Hierarchy_ID = HIERARCHYID::Parse('/1/1/1/'));
DECLARE @TargetNode HIERARCHYID;
SET @TargetNode = (SELECT TOP 1 Hierarchy_ID FROM Cars WHERE Hierarchy_ID = HIERARCHYID::Parse('/1/2/'));
EXEC MoveSubordinateBranch @SourceNode, @TargetNode;

DECLARE @CarsRoot HIERARCHYID = HIERARCHYID::GetRoot();
EXEC GetNodes @CarsRoot;

DROP PROCEDURE MoveSubordinateBranch;
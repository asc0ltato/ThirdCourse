use FirstDB

select * from TelemetryData;
select * from ReceivedTelemetry;

--delete from TelemetryData
--delete from ReceivedTelemetry
----------------------------------
use SecondDB

select * from TelemetryData;
select * from ReceivedTelemetry;

--delete from TelemetryData
--delete from ReceivedTelemetry
----------------------------------
use ServerDB

select * from TelemetryData;

--delete from TelemetryData
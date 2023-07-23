USE project_db;

CREATE TABLE `usertable` (
  `username` varchar(50) NOT NULL,
  `userpoints` int(11) NOT NULL,
  `datetime_stamps` DATETIME NOT NULL
  PRIMARY KEY (`userpoints`)
);

INSERT INTO `usertable` (`username`, `userpoints`, `datetime_stamps`) VALUES
('matt', 1001, '2023-04-01 10:00:01'),
('brian', 1002, '2023-04-02 10:00:02'),
('zack', 1003, '2023-04-03 10:00:03'),
('john', 1004, '2023-04-04 10:00:04'),
('james', 1005, '2023-04-05 10:00:05');
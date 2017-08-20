create database qqdb;

use qqdb;  

CREATE TABLE `qqdb`.`user` (
  `ID` INT NOT NULL,
  `userName` CHAR(20) NOT NULL,
  `passwd` CHAR(20) NOT NULL,
  `niName` CHAR(20) NOT NULL,
  `sex` CHAR(10) NOT NULL,
  `phone` CHAR(20) NULL,
  `address` CHAR(100) NULL,
  PRIMARY KEY (`ID`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = gb2312;

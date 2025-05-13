-- 创建数据库（如果不存在）
CREATE DATABASE IF NOT EXISTS online_gobang;

-- 使用该数据库
USE online_gobang;

-- 创建用户表（添加反引号或使用其他表名）
CREATE TABLE IF NOT EXISTS `user` (
    id INT PRIMARY KEY AUTO_INCREMENT,
    user_name VARCHAR(32) NOT NULL UNIQUE,
    password VARCHAR(32) NOT NULL, 
    score INT DEFAULT 0,
    total_count INT DEFAULT 0,
    win_count INT DEFAULT 0
); 
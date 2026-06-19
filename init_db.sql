-- Тип ENUM для времени суток
CREATE TYPE time_period AS ENUM ('DAY', 'NIGHT');

-- Таблица городов
CREATE TABLE cities (
    city_id SERIAL PRIMARY KEY,
    city_name VARCHAR(100) NOT NULL,
    city_code VARCHAR(10) NOT NULL,
    CONSTRAINT uq_city_name UNIQUE (city_name),
    CONSTRAINT uq_city_code UNIQUE (city_code)
);

-- Таблица абонентов
CREATE TABLE subscribers (
    subscriber_id SERIAL PRIMARY KEY,
    company_name VARCHAR(200) NOT NULL,
    tax_id VARCHAR(12) NOT NULL,
    bank_account VARCHAR(25) NOT NULL,
    phone_number VARCHAR(20) NOT NULL,
    city_id INTEGER NOT NULL,
    CONSTRAINT uq_subscriber_tax_id UNIQUE (tax_id),
    CONSTRAINT uq_subscriber_bank_account UNIQUE (bank_account),
    CONSTRAINT uq_subscriber_phone UNIQUE (phone_number),
    CONSTRAINT fk_subscriber_city FOREIGN KEY (city_id)
        REFERENCES cities(city_id) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT chk_tax_id_length CHECK (char_length(tax_id) IN (10, 12))
);

-- Таблица правил скидок
CREATE TABLE discount_rules (
    discount_rule_id SERIAL PRIMARY KEY,
    duration_from INTEGER NOT NULL,
    duration_to INTEGER NOT NULL,
    discount_percent NUMERIC(5,2) NOT NULL,
    destination_city_id INTEGER NOT NULL,
    CONSTRAINT fk_discount_city FOREIGN KEY (destination_city_id)
        REFERENCES cities(city_id) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT uq_discount_range UNIQUE (destination_city_id, duration_from, duration_to),
    CONSTRAINT chk_duration_range CHECK (duration_from >= 0 AND duration_to > duration_from),
    CONSTRAINT chk_discount_percent CHECK (discount_percent >= 0 AND discount_percent <= 100)
);

-- Таблица тарифов
CREATE TABLE tariffs (
    origin_city_id INTEGER NOT NULL,
    destination_city_id INTEGER NOT NULL,
    time_of_day time_period NOT NULL,
    price_per_minute NUMERIC(10,2) NOT NULL,
    CONSTRAINT pk_tariffs PRIMARY KEY (origin_city_id, destination_city_id, time_of_day),
    CONSTRAINT fk_tariff_origin_city FOREIGN KEY (origin_city_id)
        REFERENCES cities(city_id) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_tariff_destination_city FOREIGN KEY (destination_city_id)
        REFERENCES cities(city_id) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT chk_price CHECK (price_per_minute > 0)
);

-- Таблица звонков (скидка только для исходящих)
CREATE TABLE calls (
    call_id SERIAL PRIMARY KEY,
    call_date TIMESTAMP NOT NULL,
    duration_minutes INTEGER NOT NULL,
    time_of_day time_period NOT NULL,
    sender_subscriber_id INTEGER NOT NULL,
    receiver_subscriber_id INTEGER NOT NULL,
    discount_rule_id INTEGER NULL,
    CONSTRAINT fk_call_sender FOREIGN KEY (sender_subscriber_id)
        REFERENCES subscribers(subscriber_id) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_call_receiver FOREIGN KEY (receiver_subscriber_id)
        REFERENCES subscribers(subscriber_id) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_call_discount FOREIGN KEY (discount_rule_id)
        REFERENCES discount_rules(discount_rule_id) ON DELETE SET NULL ON UPDATE CASCADE,
    CONSTRAINT chk_call_duration CHECK (duration_minutes > 0),
    CONSTRAINT chk_different_subscribers CHECK (sender_subscriber_id <> receiver_subscriber_id)
);

-- Таблица ролей
CREATE TABLE roles (
    role_id SERIAL PRIMARY KEY,
    role_name VARCHAR(50) NOT NULL,
    CONSTRAINT uq_role_name UNIQUE (role_name)
);

-- Таблица пользователей (пароль хранится в виде хеша)
CREATE TABLE users (
    user_id SERIAL PRIMARY KEY,
    login VARCHAR(50) NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    full_name VARCHAR(200) NOT NULL,
    CONSTRAINT uq_user_login UNIQUE (login)
);

-- Таблица назначения ролей (M:N)
CREATE TABLE user_roles (
    user_id INTEGER NOT NULL,
    role_id INTEGER NOT NULL,
    CONSTRAINT pk_user_roles PRIMARY KEY (user_id, role_id),
    CONSTRAINT fk_user_roles_user FOREIGN KEY (user_id)
        REFERENCES users(user_id) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_user_roles_role FOREIGN KEY (role_id)
        REFERENCES roles(role_id) ON DELETE CASCADE ON UPDATE CASCADE
);

-- Индексы для производительности
CREATE INDEX idx_calls_sender ON calls(sender_subscriber_id);
CREATE INDEX idx_calls_receiver ON calls(receiver_subscriber_id);
CREATE INDEX idx_subscribers_city ON subscribers(city_id);

-- Начальные роли
INSERT INTO roles (role_name) VALUES ('admin'), ('operator'), ('manager');

-- Администратор (пароль: admin)
INSERT INTO users (login, password_hash, full_name)
VALUES ('admin', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918', 'Администратор Системы');

INSERT INTO user_roles (user_id, role_id) VALUES (1, 1);


-- Заполнение таблиц тестовыми данными
-- 1. Города
INSERT INTO cities (city_name, city_code) VALUES
('Москва', '495'),
('Санкт-Петербург', '812'),
('Новосибирск', '383'),
('Екатеринбург', '343'),
('Казань', '843');

-- 2. Абоненты (юридические лица)
INSERT INTO subscribers (company_name, tax_id, bank_account, phone_number, city_id) VALUES
('ООО "Ромашка"', '123456789012', '40702810123456789012', '84951234567', 1),
('ООО "Лютик"', '123456789013', '40702810123456789013', '84951234568', 1),
('ООО "Василёк"', '123456789014', '40702810123456789014', '88121234567', 2),
('ООО "Роза"', '123456789015', '40702810123456789015', '88121234568', 2),
('ООО "Тюльпан"', '123456789016', '40702810123456789016', '83831234567', 3),
('ООО "Подсолнух"', '123456789017', '40702810123456789017', '83831234568', 3),
('ООО "Незабудка"', '123456789018', '40702810123456789018', '83431234567', 4),
('ООО "Астра"', '123456789019', '40702810123456789019', '83431234568', 4),
('ООО "Фиалка"', '123456789020', '40702810123456789020', '88431234567', 5),
('ООО "Орхидея"', '123456789021', '40702810123456789021', '88431234568', 5);

-- 3. Тарифы
INSERT INTO tariffs (origin_city_id, destination_city_id, time_of_day, price_per_minute) VALUES
(1, 2, 'DAY', 3.50),
(1, 2, 'NIGHT', 2.00),
(1, 3, 'DAY', 5.00),
(1, 3, 'NIGHT', 3.50),
(2, 1, 'DAY', 3.50),
(2, 1, 'NIGHT', 2.00),
(2, 3, 'DAY', 6.00),
(2, 3, 'NIGHT', 4.00),
(3, 1, 'DAY', 5.00),
(3, 1, 'NIGHT', 3.50),
(3, 2, 'DAY', 6.00),
(3, 2, 'NIGHT', 4.00),
(4, 1, 'DAY', 5.50),
(4, 1, 'NIGHT', 3.50),
(4, 2, 'DAY', 6.50),
(4, 2, 'NIGHT', 4.50),
(5, 1, 'DAY', 4.50),
(5, 1, 'NIGHT', 3.00),
(5, 2, 'DAY', 5.50),
(5, 2, 'NIGHT', 3.50);

-- 4. Правила скидок
INSERT INTO discount_rules (duration_from, duration_to, discount_percent, destination_city_id) VALUES
(5, 10, 10, 1),
(11, 20, 15, 1),
(5, 10, 5, 2),
(11, 20, 10, 2),
(5, 10, 8, 3),
(11, 20, 12, 3),
(5, 10, 7, 4),
(11, 20, 11, 4),
(5, 10, 9, 5),
(11, 20, 14, 5);

-- 5. Звонки (с учётом, что скидка применяется только к исходящим, но в calls есть discount_rule_id, который может быть NULL)
INSERT INTO calls (call_date, duration_minutes, time_of_day, sender_subscriber_id, receiver_subscriber_id, discount_rule_id) VALUES
('2026-06-01 10:15:00', 12, 'DAY', 1, 3, 2),   -- скидка 15% для Москва->СПб (duration 12)
('2026-06-02 14:30:00', 7, 'DAY', 2, 4, 1),   -- скидка 10% для Москва->СПб (duration 7)
('2026-06-03 22:00:00', 25, 'NIGHT', 3, 5, 4), -- скидка 10% для СПб->Новосибирск (duration 25) – попадает в диапазон 11-20? нет, 25 не попадает, но мы задали 5-10 и 11-20, для 25 нет скидки, поэтому NULL
('2026-06-04 09:00:00', 8, 'DAY', 4, 6, 3),   -- скидка 5% для СПб->Новосибирск (duration 8)
('2026-06-05 18:00:00', 15, 'DAY', 5, 7, 6),  -- скидка 12% для Новосибирск->Екатеринбург
('2026-06-06 11:00:00', 6, 'DAY', 6, 8, 5),   -- скидка 8% для Новосибирск->Екатеринбург
('2026-06-07 20:00:00', 30, 'NIGHT', 7, 9, NULL), -- нет скидки
('2026-06-08 12:30:00', 18, 'DAY', 8, 10, 8), -- скидка 11% для Екатеринбург->Казань
('2026-06-09 23:00:00', 10, 'NIGHT', 9, 1, 10), -- скидка 14% для Казань->Москва
('2026-06-10 08:00:00', 4, 'DAY', 10, 2, NULL); -- нет скидки (длительность 4)

-- 6. Дополнительные пользователи (для тестирования ролей)
INSERT INTO users (login, password_hash, full_name) VALUES
('operator1', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918', 'Иванов Иван Иванович'),
('manager1', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918', 'Петров Пётр Петрович');

-- Назначение ролей
INSERT INTO user_roles (user_id, role_id) VALUES
(2, 2), -- operator1 -> роль operator (role_id=2)
(3, 3); -- manager1 -> роль manager (role_id=3)

-- Проверка количества записей
SELECT 'cities' AS table_name, COUNT(*) AS rows FROM cities
UNION ALL
SELECT 'subscribers', COUNT(*) FROM subscribers
UNION ALL
SELECT 'tariffs', COUNT(*) FROM tariffs
UNION ALL
SELECT 'discount_rules', COUNT(*) FROM discount_rules
UNION ALL
SELECT 'calls', COUNT(*) FROM calls
UNION ALL
SELECT 'users', COUNT(*) FROM users
UNION ALL
SELECT 'user_roles', COUNT(*) FROM user_roles;
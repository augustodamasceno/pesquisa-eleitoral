CREATE TABLE city (
    id    INTEGER PRIMARY KEY AUTOINCREMENT,
    state CHAR(2)      NOT NULL,
    name  VARCHAR(64)  NOT NULL, -- Edge Case: Vila Bela da Santíssima Trindade (size 32)
    tier  INTEGER      NOT NULL CHECK (tier BETWEEN 1 AND 4),
    UNIQUE (state, name)
);
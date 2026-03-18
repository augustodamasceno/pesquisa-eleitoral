PRAGMA journal_mode=WAL;
PRAGMA foreign_keys=ON;

-- city
CREATE TABLE city (
    id    INTEGER PRIMARY KEY AUTOINCREMENT,
    state CHAR(2)      NOT NULL,
    name  VARCHAR(64)  NOT NULL, -- Edge Case: Vila Bela da Santíssima Trindade (size 32)
    tier  INTEGER      NOT NULL CHECK (tier BETWEEN 0 AND 3),
    UNIQUE (state, name)
);
-- candidate
CREATE TABLE candidate (
    id    INTEGER PRIMARY KEY,
    name  VARCHAR(128),
    party VARCHAR(32)
);

-- vote
CREATE TABLE vote (
    id           INTEGER PRIMARY KEY,
    date         DATE    NOT NULL,
    city_id      INTEGER NOT NULL REFERENCES city(id),
    candidate_id INTEGER NOT NULL REFERENCES candidate(id)
);

-- result
CREATE TABLE result (
    id           INTEGER PRIMARY KEY AUTOINCREMENT,
    date         DATE    NOT NULL,
    position     INTEGER NOT NULL,
    votes        INTEGER NOT NULL,
    candidate_id INTEGER NOT NULL REFERENCES candidate(id)
);


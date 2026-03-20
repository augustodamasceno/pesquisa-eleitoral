CREATE TABLE log (
    id        INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp TEXT    NOT NULL DEFAULT (datetime('now')),
    level     VARCHAR(16) NOT NULL,
    source    VARCHAR(128),
    message   TEXT    NOT NULL
);

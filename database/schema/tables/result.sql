CREATE TABLE result (
    id           INTEGER PRIMARY KEY AUTOINCREMENT,
    date         DATE    NOT NULL,
    position     INTEGER NOT NULL,
    votes        INTEGER NOT NULL,
    proportion   REAL    NOT NULL,
    candidate_id INTEGER NOT NULL REFERENCES candidate(id)
);

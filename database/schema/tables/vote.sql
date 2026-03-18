CREATE TABLE vote (
    id           INTEGER PRIMARY KEY,
    date         DATE    NOT NULL,
    city_id      INTEGER NOT NULL REFERENCES city(id),
    candidate_id INTEGER NOT NULL REFERENCES candidate(id)
);

-- All candidates
SELECT * FROM candidate;

-- Candidates by party
SELECT * FROM candidate WHERE party = 'P1';

-- Candidate by name
SELECT * FROM candidate WHERE name LIKE '%Damasceno%';

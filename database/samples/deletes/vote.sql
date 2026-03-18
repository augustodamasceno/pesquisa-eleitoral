-- Delete a single vote record
DELETE FROM vote WHERE id = 10;

-- Delete all votes for a specific city
DELETE FROM vote WHERE city_id = 5;

-- Delete all votes for a specific candidate
DELETE FROM vote WHERE candidate_id = 3;

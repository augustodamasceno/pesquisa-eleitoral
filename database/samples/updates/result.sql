-- Correct the vote count for a result entry
UPDATE result SET votes = 46500 WHERE id = 1;

-- Update positions after a recount
UPDATE result SET position = 1 WHERE candidate_id = 2 AND date = '2026-10-04';
UPDATE result SET position = 2 WHERE candidate_id = 1 AND date = '2026-10-04';

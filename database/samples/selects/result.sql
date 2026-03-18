-- Full results with candidate details ordered by position
SELECT
    r.position,
    ca.name  AS candidate,
    ca.party,
    r.votes,
    r.date
FROM result r
JOIN candidate ca ON ca.id = r.candidate_id
ORDER BY r.position;

-- Results for a specific date
SELECT
    r.position,
    ca.name AS candidate,
    r.votes
FROM result r
JOIN candidate ca ON ca.id = r.candidate_id
WHERE r.date = '2026-10-04'
ORDER BY r.position;

-- Total votes across all results
SELECT SUM(votes) AS total_votes FROM result;

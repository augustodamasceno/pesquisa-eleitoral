-- All votes with city and candidate details
SELECT
    v.id,
    v.date,
    c.state,
    c.name  AS city,
    ca.name AS candidate,
    ca.party
FROM vote v
JOIN city      c  ON c.id  = v.city_id
JOIN candidate ca ON ca.id = v.candidate_id;

-- Votes for a specific city
SELECT
    v.id,
    v.date,
    ca.name  AS candidate,
    ca.party
FROM vote v
JOIN candidate ca ON ca.id = v.candidate_id
WHERE v.city_id = 1;

-- Number of votes registered per candidate
SELECT
    ca.name    AS candidate,
    ca.party,
    COUNT(v.id) AS total_polls
FROM vote v
JOIN candidate ca ON ca.id = v.candidate_id
GROUP BY ca.id
ORDER BY total_polls DESC;

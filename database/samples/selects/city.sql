-- All cities
SELECT * FROM city;

-- Cities by state
SELECT * FROM city WHERE state = 'SP';

-- Cities by tier
SELECT * FROM city WHERE tier = 3 ORDER BY name;

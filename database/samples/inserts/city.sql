INSERT INTO city (id, state, name, tier) VALUES
    -- Tier 1: less than 20 mil
    (1,  'RS', 'Bom Jesus',           1),
    (2,  'MG', 'Prados',              1),
    (3,  'CE', 'Mulungu',             1),

    -- Tier 2: between 20 mil and 100 mil
    (4,  'SP', 'São José dos Campos', 2),
    (5,  'RN', 'Natal',               2),
    (6,  'SC', 'Jaraguá do Sul',      2),

    -- Tier 3: between 100 mil and 1 million
    (7,  'PR', 'Curitiba',            3),
    (8,  'BA', 'Salvador',            3),
    (9,  'PE', 'Recife',              3),

    -- Tier 4: above 1 million
    (10, 'SP', 'São Paulo',           4),
    (11, 'RJ', 'Rio de Janeiro',      4),
    (12, 'MG', 'Belo Horizonte',      4);
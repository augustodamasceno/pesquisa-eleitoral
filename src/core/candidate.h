/*
 * Pesquisa Eleitoral - Candidate   
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#pragma once
#include <string>


struct Candidate {
    int         id    = 0;
    std::string name;        // VARCHAR(128)
    std::string party;       // VARCHAR(32)
};

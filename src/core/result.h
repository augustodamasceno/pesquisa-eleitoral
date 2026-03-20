/*
 * Pesquisa Eleitoral - Result   
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#pragma once
#include <string>


namespace pesquisae::core::database {

struct Result {
    int         id           = 0;
    std::string date;
    int         position     = 0;
    int         votes        = 0;  
    double      proportion   = 0.0; 
    int         candidate_id = 0;
};

} // namespace pesquisae::core::database

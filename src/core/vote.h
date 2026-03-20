/*
 * Pesquisa Eleitoral - Vote   
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

struct Vote {
    int         id           = 0;
    std::string date;        
    int         city_id      = 0;
    int         candidate_id = 0;
};

} // namespace pesquisae::core::database

/*
 * Pesquisa Eleitoral - Log
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

struct Log {
    int         id        = 0;
    std::string timestamp;   
    std::string level;       
    std::string source;      
    std::string message; 
};

} // namespace pesquisae::core::database

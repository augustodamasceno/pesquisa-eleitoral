/*
 * Pesquisa Eleitoral - City   
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#pragma once
#include <string>


struct City {
    int         id    = 0;
    std::string state;       
    std::string name;        
    int         tier  = 0;
};

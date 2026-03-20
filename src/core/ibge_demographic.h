/*
 * Pesquisa Eleitoral - IBGE Demographic Data Fetch      
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#pragma once
#include <vector>
#include <string>
#include <string_view>

#include "city.h"

namespace pesquisae::core::database {

inline constexpr std::string_view IBGE_API_URL =
"https://servicodados.ibge.gov.br/api/v3/agregados/4714/periodos/2022/variaveis/93?localidades=N6[all]";

struct IBGEFetchResult {
    bool        ok;
    std::string data;
};

IBGEFetchResult ibge_fetch_all_cities();
IBGEFetchResult ibge_fetch_cities(const std::string& url);
std::vector<City> ibge_parse_cities(const IBGEFetchResult& result);

} // namespace pesquisae::core::database

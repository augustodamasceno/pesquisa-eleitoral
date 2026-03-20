/*
 * Pesquisa Eleitoral - IBGE Demographic Data Fetch      
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "city.h"
#include "ibge_demographic.h"

namespace pesquisae::core::database {

IBGEFetchResult ibge_fetch_cities(const std::string& url) {
    cpr::Response r = cpr::Get(
        cpr::Url{url},
        cpr::Header{{"Accept", "application/json"}},
        cpr::VerifySsl{true},
        cpr::Timeout{30000}
    );

    if (r.error)
        return {false, "Request error: " + r.error.message};

    if (r.status_code != 200)
        return {false, "HTTP error: " + std::to_string(r.status_code)};

    return {true, std::move(r.text)};
}

IBGEFetchResult ibge_fetch_all_cities() {
    return ibge_fetch_cities(std::string(IBGE_API_URL));
}

std::vector<City> ibge_parse_cities(const IBGEFetchResult& result){
    if (!result.ok)
        return {};

    try {
        auto json = nlohmann::json::parse(result.data);

        auto population_to_tier = [](int pop) noexcept  {
            return 1 + (pop > 20000) + (pop > 100000) + (pop > 1000000);
        };
        (void)population_to_tier;

        std::vector<City> cities;

        const auto& series = json[0]["resultados"][0]["series"];
        cities.reserve(series.size());
        for (const auto& s : series) {
            const std::string nome    = s["localidade"]["nome"].get<std::string>();
            const std::string pop_str = s["serie"]["2022"].get<std::string>();

            City city;
            // IBGE format A: "City Name - ST"
            // IBGE format B: "City Name (ST)"
            const auto paren_open  = nome.rfind('(');
            const auto paren_close = nome.rfind(')');
            const auto dash        = nome.rfind(" - ");
            if (paren_open != std::string::npos && paren_close > paren_open) {
                city.state = nome.substr(paren_open + 1, paren_close - paren_open - 1);
                city.name  = nome.substr(0, paren_open > 0 ? paren_open - 1 : paren_open);
            } else if (dash != std::string::npos) {
                city.name  = nome.substr(0, dash);
                city.state = nome.substr(dash + 3);
            } else {
                city.name = nome;
            }
            city.tier = population_to_tier(std::stoi(pop_str));
            cities.push_back(std::move(city));
        }

        return cities;

    } catch (const std::exception&) {
        return {};
    }
}

} // namespace pesquisae::core::database

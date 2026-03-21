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
#include <vector>
#include <unordered_map>
#include <array>

#include "vote_intention.h"
#include "database_context.h"


namespace pesquisae::core::poll {

// Median of 2.85 mi for cities greater than 1mi
inline constexpr std::array<double, 4> kTierPopulationWeight = {10'000.0, 60'000.0, 550'000.0, 2'850'000.0};

class ResultProcessor {
public:
    ResultProcessor(const std::vector<VoteIntention>& vote_intentions, 
                    const pesquisae::core::database::DatabaseContext& db_context);
    bool update_counts(const VoteIntention& vi, int tier); 
    int city_tier(const std::string& state, const std::string& city);
    const std::string& get_date() const { return _date; }
private:
    std::unordered_map<int, std::array<int, 4>> votes_by_candidate;
    std::array<int, 4> votes_by_tier;
    const pesquisae::core::database::DatabaseContext& _db_context;
    std::string _date;
};

} // namespace pesquisae::core::poll

/*
 * Pesquisa Eleitoral - Result   
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include <string>
#include <vector>
#include <array>
#include <algorithm>

#include "result_processor.h"
#include "city.h"
#include "vote.h"

namespace pesquisae::core::poll {

ResultProcessor::ResultProcessor(const std::vector<VoteIntention>& vote_intentions, 
                                 const pesquisae::core::database::DatabaseContext& db_context)
    : _db_context(db_context), votes_by_tier{}
{
    if (!vote_intentions.empty())
        _date = vote_intentions.front().get_date();
        
    // Remove any existing data for this poll's date to ensure idempotent loading
    if (!vote_intentions.empty()) {
        const std::string& date = vote_intentions.front().get_date();
        _db_context.get_results().remove_by_date(date);
        _db_context.get_votes().remove_by_date(date);
    }

    // Organize data and update database
    for (const auto& vi : vote_intentions){
        int tier = this->city_tier(vi.get_state(), vi.get_city());
        bool is_new_candidate = this->update_counts(vi, tier);

        // Add Candidate to database if not exists
        if (is_new_candidate){
            this->_db_context.get_candidates().insert({vi.get_candidate_id(), "", ""});
        }

        // Add Vote to database
        pesquisae::core::database::Vote v;
        v.id = 0;
        v.date = vi.get_date();
        int city_id = 0;
        try {
            city_id = this->_db_context.get_cities().find_by_state_and_name(vi.get_state(), vi.get_city()).id;
        } catch (const std::exception&) {}
        v.city_id = city_id;
        v.candidate_id = vi.get_candidate_id();
        this->_db_context.get_votes().insert(v);
    }

    // Process Result
    if (!vote_intentions.empty()) {
        const std::string& date = vote_intentions.front().get_date();

        //   score = sum of ( weight[t] * (votes_candidate[t] / votes[t]) )
        // tier 1 with 10 votes out of 100:
        //    10k * (10/100) = 1k weighted votes
        // {candidate_id, weighted_score}
        std::vector<std::pair<int, double>> ranked; 
        double total_weighted = 0.0;
        for (const auto& [candidate_id, tier_counts] : votes_by_candidate) {
            double score = 0.0;
            for (int t = 0; t < 4; ++t) {
                if (votes_by_tier[t] == 0) continue;
                score += kTierPopulationWeight[t]
                         * (static_cast<double>(tier_counts[t]) / votes_by_tier[t]);
            }
            ranked.push_back({candidate_id, score});
            total_weighted += score;
        }
        std::sort(ranked.begin(), ranked.end(),
                  [](const auto& a, const auto& b){ return a.second > b.second; });

        auto raw_votes = [&](int candidate_id) {
            int total = 0;
            for (int t = 0; t < 4; ++t)
                total += votes_by_candidate.at(candidate_id)[t];
            return total;
        };

        int position = 1;
        for (const auto& [candidate_id, score] : ranked) {
            pesquisae::core::database::Result r;
            r.id           = 0;
            r.date         = date;
            r.position     = position++;
            r.votes        = raw_votes(candidate_id);
            r.proportion   = (total_weighted > 0.0) ? score / total_weighted : 0.0;
            r.candidate_id = candidate_id;
            this->_db_context.get_results().insert(r);
        }
    }
}

bool ResultProcessor::update_counts(const VoteIntention& vi, int tier){
    bool is_new_candidate = true;
    int tier_index = tier - 1;
    auto it = this->votes_by_candidate.find(vi.get_candidate_id());
    if (it != this->votes_by_candidate.end()) [[unlikely]] {
        it->second[tier_index] += 1;
        is_new_candidate = false;
    } else [[likely]] {
        this->votes_by_candidate[vi.get_candidate_id()] = {0, 0, 0, 0};
        this->votes_by_candidate[vi.get_candidate_id()][tier_index]++;
    }

    this->votes_by_tier[tier_index]++;
    return is_new_candidate;
}


int ResultProcessor::city_tier(const std::string& state, const std::string& city) {
    try {
        return _db_context.get_cities().find_by_state_and_name(state, city).tier;
    } catch (const std::exception&) {}
    return 1;
}

} // namespace pesquisae::core::poll

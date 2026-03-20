/*
 * Pesquisa Eleitoral - Election Poll   
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include "vote_intention.h"
#include <algorithm>
#include <sstream>
#include <utility>


namespace pesquisae::core::poll {

VoteIntention::VoteIntention(int id, std::string date, std::string city,
                             std::string state, int candidate_id)
    : id(id), date(std::move(date)), city(std::move(city)),
      state(std::move(state)), candidate_id(candidate_id) {}

void VoteIntention::setId(int value)            { id           = value; }
void VoteIntention::setDate(std::string value)  { date         = std::move(value); }
void VoteIntention::setCity(std::string value)  { city         = std::move(value); }
void VoteIntention::setState(std::string value) { state        = std::move(value); }
void VoteIntention::setCandidateId(int value)   { candidate_id = value; }

bool VoteIntention::operator==(const VoteIntention& other) const {
    return id           == other.id
        && date         == other.date
        && city         == other.city
        && state        == other.state
        && candidate_id == other.candidate_id;
}

bool VoteIntention::operator!=(const VoteIntention& other) const {
    return !(*this == other);
}

std::string VoteIntention::to_string() const {
    std::ostringstream oss;
    oss << "VoteIntention{"
        << "id="             << id
        << ", date="         << date
        << ", city="         << city
        << ", state="        << state
        << ", candidate_id=" << candidate_id
        << "}";
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const VoteIntention& vi) {
    os << vi.to_string();
    return os;
}

std::istream& operator>>(std::istream& is, VoteIntention& vi) {
    std::string line1, line5;

    if (!std::getline(is, line1))         return is;
    if (!std::getline(is, vi.date))       return is;
    if (!std::getline(is, vi.city))       return is;
    if (!std::getline(is, vi.state))      return is;
    if (!std::getline(is, line5))         return is;

    // Translate long state name to abbreviation if present in the table.
    auto it = std::find_if(VoteIntention::kStateNames.begin(),
                           VoteIntention::kStateNames.end(),
                           [&vi](const VoteIntention::StatePair& p) {
                               return vi.state == p.full;
                           });
    if (it != VoteIntention::kStateNames.end())
        vi.state = it->abbr;

    try {
        vi.id           = std::stoi(line1);
        vi.candidate_id = std::stoi(line5);
    } catch (...) {
        is.setstate(std::ios::failbit);
    }
    return is;
}

} // namespace pesquisae::core::poll

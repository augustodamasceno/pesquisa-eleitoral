/*
 * Pesquisa Eleitoral - Election Poll Parser 
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

#include "vote_intention.h"


namespace pesquisae::core::poll {

class PollParser {
public:
    PollParser(const std::string& filename);
    const std::vector<VoteIntention>& get_vote_intentions() const {return this->vote_intentions;};
private:
    std::vector<VoteIntention> vote_intentions;
};


} // namespace pesquisae::core::poll

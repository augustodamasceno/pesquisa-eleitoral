/*
 * Pesquisa Eleitoral - Election Poll Parser 
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <limits>

#include "poll_parser.h"


namespace pesquisae::core::poll {

PollParser::PollParser(const std::string&filename){
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << "\n";
        return;
    }

    VoteIntention vi;
    size_t line_counter = 1;

    while (file) {
        if (file.peek() == EOF)
            break;

        if (file >> vi) {
            vote_intentions.push_back(std::move(vi));
            line_counter += 5;
        } 
        else if (file.eof()) {
            std::cerr << "Warning: Unexpected End of File near line " << line_counter << "\n";
            break;
        }
        else if (file.fail() && !file.bad()) {
            std::cerr << "Warning: Malformed data (failbit) near line " << line_counter
                      << ". Skipping record.\n";
            file.clear();
            line_counter += 5;
        }
        else {
            std::cerr << "Error: Unrecoverable stream error near line " << line_counter << "\n";
            break;
        }
    }
}


} // namespace pesquisae::core::poll

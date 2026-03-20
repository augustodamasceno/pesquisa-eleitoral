/*
 * Pesquisa Eleitoral - Election Poll   
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#pragma once
#include <array>
#include <iostream>
#include <string>


namespace pesquisae::core::poll {

class VoteIntention {
public:
    VoteIntention() = default;

    VoteIntention(int id, std::string date, std::string city,
                  std::string state, int candidate_id);
    int                getId()          const { return id; }
    const std::string& getDate()        const { return date; }
    const std::string& getCity()        const { return city; }
    const std::string& getState()       const { return state; }
    int                getCandidateId() const { return candidate_id; }

    void setId(int value);
    void setDate(std::string value);
    void setCity(std::string value);
    void setState(std::string value);
    void setCandidateId(int value);

    bool operator==(const VoteIntention& other) const;
    bool operator!=(const VoteIntention& other) const;

    std::string to_string() const;

    friend std::ostream& operator<<(std::ostream& os, const VoteIntention& vi);
    friend std::istream& operator>>(std::istream& is, VoteIntention& vi);

private:
    struct StatePair { 
        std::string_view full; 
        std::string_view abbr; 
    };

static constexpr std::array<StatePair, 27> kStateNames = {{
    {"Acre",                "AC"},
    {"Alagoas",             "AL"},
    {"Amazonas",            "AM"},
    {"Amapá",               "AP"},
    {"Bahia",               "BA"},
    {"Ceará",               "CE"},
    {"Distrito Federal",    "DF"},
    {"Espírito Santo",      "ES"},
    {"Goiás",               "GO"},
    {"Maranhão",            "MA"},
    {"Minas Gerais",        "MG"},  
    {"Mato Grosso do Sul",  "MS"},
    {"Mato Grosso",         "MT"},
    {"Pará",                "PA"},
    {"Paraíba",             "PB"},
    {"Pernambuco",          "PE"},
    {"Piauí",               "PI"},
    {"Paraná",              "PR"},
    {"Rio de Janeiro",      "RJ"},
    {"Rio Grande do Norte", "RN"},
    {"Rondônia",            "RO"},
    {"Roraima",             "RR"},
    {"Rio Grande do Sul",   "RS"},
    {"Santa Catarina",      "SC"},
    {"Sergipe",             "SE"},
    {"São Paulo",           "SP"},
    {"Tocantins",           "TO"}
}};

    int         id;
    std::string date;
    std::string city;
    std::string state;
    int         candidate_id;
};


} // namespace pesquisae::core::poll

/*
 * Pesquisa Eleitoral - IBGE Demographic Tests
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>

#include <gtest/gtest.h>

#include "ibge_demographic.h"

// 8-city URL covering 2 states and all 4 tiers:
//   Frutuoso Gomes-RN  (2404002)  pop      4,122 → tier 1
//   Acari-RN           (2400109)  pop     10,597 → tier 1
//   Caicó-RN           (2402006)  pop     61,146 → tier 2
//   Pau dos Ferros-RN  (2409407)  pop     30,479 → tier 2
//   Parnamirim-RN      (2403251)  pop    252,716 → tier 3
//   São José dos Campos-SP (3549904) pop  697,054 → tier 3
//   Natal-RN           (2408102)  pop    751,300 → tier 3
//   São Paulo-SP        (3550308) pop 11,451,999 → tier 4
static constexpr std::string_view EIGHT_CITIES_URL =
    "https://servicodados.ibge.gov.br/api/v3/agregados/4714/periodos/2022"
    "/variaveis/93?localidades=N6[2404002,2400109,2402006,2409407,2403251,3549904,2408102,3550308]";

using City = pesquisae::core::database::City;

class IBGEDemographicTest : public ::testing::Test {
protected:
    std::vector<City> cities;

    void SetUp() override {
        pesquisae::core::database::IBGEFetchResult result =
            pesquisae::core::database::ibge_fetch_cities(std::string(EIGHT_CITIES_URL));
        cities = pesquisae::core::database::ibge_parse_cities(result);
    }

    const City* find(const std::string& name, const std::string& state) const {
        auto it = std::find_if(cities.begin(), cities.end(),
            [&](const City& c) { return c.name == name && c.state == state; });
        return it != cities.end() ? &(*it) : nullptr;
    }
};

TEST_F(IBGEDemographicTest, ReturnsEightCities) {
    ASSERT_EQ(cities.size(), 8u);
}

TEST_F(IBGEDemographicTest, AllCitiesHaveNonEmptyFields) {
    for (const auto& city : cities) {
        EXPECT_FALSE(city.name.empty());
        EXPECT_FALSE(city.state.empty());
    }
}

TEST_F(IBGEDemographicTest, AllCitiesHaveValidTier) {
    for (const auto& city : cities) {
        EXPECT_GE(city.tier, 1);
        EXPECT_LE(city.tier, 4);
    }
}

// Tier 1: up to 20,000 inhabitants
TEST_F(IBGEDemographicTest, FrutuosoGomesRN_Tier1) {
    const City* c = find("Frutuoso Gomes", "RN");
    ASSERT_NE(c, nullptr) << "Frutuoso Gomes (RN) not found";
    EXPECT_EQ(c->tier, 1); // pop 4,122
}

TEST_F(IBGEDemographicTest, AcariRN_Tier1) {
    const City* c = find("Acari", "RN");
    ASSERT_NE(c, nullptr) << "Acari (RN) not found";
    EXPECT_EQ(c->tier, 1); // pop 10,597
}

// Tier 2: 20,001–100,000 inhabitants
TEST_F(IBGEDemographicTest, CaicoRN_Tier2) {
    const City* c = find("Caicó", "RN");
    ASSERT_NE(c, nullptr) << "Caicó (RN) not found";
    EXPECT_EQ(c->tier, 2); // pop 61,146
}

TEST_F(IBGEDemographicTest, PauDosFerrosRN_Tier2) {
    const City* c = find("Pau dos Ferros", "RN");
    ASSERT_NE(c, nullptr) << "Pau dos Ferros (RN) not found";
    EXPECT_EQ(c->tier, 2); // pop 30,479
}

// Tier 3: 100,001–1,000,000 inhabitants
TEST_F(IBGEDemographicTest, ParnamirimRN_Tier3) {
    const City* c = find("Parnamirim", "RN");
    ASSERT_NE(c, nullptr) << "Parnamirim (RN) not found";
    EXPECT_EQ(c->tier, 3); // pop 252,716
}

TEST_F(IBGEDemographicTest, SaoJoseDosCamposSP_Tier3) {
    const City* c = find("São José dos Campos", "SP");
    ASSERT_NE(c, nullptr) << "São José dos Campos (SP) not found";
    EXPECT_EQ(c->tier, 3); // pop 697,054
}

TEST_F(IBGEDemographicTest, NatalRN_Tier3) {
    const City* c = find("Natal", "RN");
    ASSERT_NE(c, nullptr) << "Natal (RN) not found";
    EXPECT_EQ(c->tier, 3); // pop 751,300
}

// Tier 4: above 1,000,000 inhabitants
TEST_F(IBGEDemographicTest, SaoPauloSP_Tier4) {
    const City* c = find("São Paulo", "SP");
    ASSERT_NE(c, nullptr) << "São Paulo (SP) not found";
    EXPECT_EQ(c->tier, 4); // pop 11,451,999
}


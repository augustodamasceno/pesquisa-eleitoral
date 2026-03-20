/*
 * Pesquisa Eleitoral - Repository Interface   
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#pragma once

#include <vector>

namespace pesquisae::core::database {

template <typename T>
class IRepository {
public:
    virtual ~IRepository() = default;

    virtual void           insert(const T& entity)  = 0;
    virtual void           update(const T& entity)   = 0;
    virtual T              find_by_id(int id)        = 0;
    virtual std::vector<T> find_all()               = 0;
    virtual void           remove(int id)           = 0;

};

} // namespace pesquisae::core::database
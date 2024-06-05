#pragma once

#include "commands.hpp"

namespace poke {

    void Initialize();
    void Daily(const dpp::slashcommand_t& event);
    void Wish(const dpp::slashcommand_t& event);
    void Wishes(const dpp::slashcommand_t& event);

}
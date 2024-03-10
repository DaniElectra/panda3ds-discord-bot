#pragma once

#include <dpp/dpp.h>

namespace commands {
    using CommandFunction = std::function<void(dpp::cluster&, const dpp::slashcommand_t&)>;

    void Settings(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void AddAdmin(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void RemoveAdmin(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void AddMacro(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void AddMacroFile(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void RemoveMacro(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void Download(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void ClearCache(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void StartPuzzle(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void SolvePuzzle(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void CurrentPuzzle(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void Panda(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void AskPanda(dpp::cluster& bot, const dpp::slashcommand_t& event);
}
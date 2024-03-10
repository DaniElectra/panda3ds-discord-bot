#include "ai.hpp"
#include "restresults.h"
#include <liboai.h>
#include <fstream>
#include <iostream>
#include <functional>
#include <dpp/dpp.h>

liboai::OpenAI oai;
bool initialized;
uint64_t lastImageTimestamp = 0;
uint64_t lastQuestionTimestamp = 0;
liboai::Conversation convo;

namespace artificial {
    
    void Initialize() {
        std::string token;
        std::ifstream keyFile("ai_key.txt");
        if (keyFile.is_open()) {
            std::getline(keyFile, token);
            keyFile.close();
        } else {
            std::cerr << "Failed to open AI token file" << std::endl;
            return;
        }

        bool success = oai.auth.SetKey(token);

        if (!success) {
            std::cerr << "Failed to set OpenAI API key" << std::endl;
            return;
        }

        std::ifstream promptFile("ai_prompt.txt");
        std::string data;
        // read everything into data
        promptFile.seekg(0, std::ios::end);
        data.resize(promptFile.tellg());
        promptFile.seekg(0, std::ios::beg);
        promptFile.read(&data[0], data.size());
        promptFile.close();

        convo.AddUserData("Hello AI. Your prompt is: " + data);
        convo.AddUserData("Stay in character after this line. You will be conversated with. The non-AI messages will be prepended with \'HUMAN:\'");
        convo.AddUserData("---");

        initialized = true;
    }

    void GenerateImage(const dpp::slashcommand_t& event, const std::string& prompt) {
        if (!initialized) {
            event.reply(dpp::message("AI not initialized").set_flags(dpp::m_ephemeral));
            return;
        }

        uint64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (timestamp - lastImageTimestamp < 7200) {
            event.reply(dpp::message("You can only generate an image every 2 hours, or else Paris is going to go bankrupt. Next image available <t:" + std::to_string(lastImageTimestamp + 7200) + ":R>"));
            return;
        }

        lastImageTimestamp = timestamp;
        event.thinking(false, [event, prompt](const dpp::confirmation_callback_t& callback) {
            std::thread t([event, prompt] {
                liboai::Response res = oai.Image->create(
                    prompt,
                    1,
                    "256x256"
                );

                std::string url = res["data"][0]["url"];
                dpp::embed embed = dpp::embed()
                    .set_image(url)
                    .set_title("Your artificial panda is ready!");
                event.edit_original_response(dpp::message(event.command.channel_id, embed));
            });
            t.detach();
        });
    }

    void AskQuestion(const dpp::slashcommand_t& event, const std::string& prompt) {
        if (event.command.channel_id != 1118695733449723906) {
            event.reply(dpp::message("This command is only available in the <#1118695733449723906> channel").set_flags(dpp::m_ephemeral));
            return;
        }

        if (!initialized) {
            event.reply(dpp::message("AI not initialized").set_flags(dpp::m_ephemeral));
            return;
        }

        uint64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (timestamp - lastQuestionTimestamp < 60) {
            event.reply(dpp::message("You can only ask a question every minute, or else Paris is going to go bankrupt. Next question available <t:" + std::to_string(lastQuestionTimestamp + 60) + ":R>"));
            return;
        }

        lastQuestionTimestamp = timestamp;
        convo.AddUserData("HUMAN: " + prompt);
        event.thinking(false, [event, prompt](const dpp::confirmation_callback_t& callback) {
            std::thread t([event, prompt] {
                liboai::Response response = oai.ChatCompletion->create(
                    "gpt-3.5-turbo", convo
                );
                convo.Update(response);
                event.edit_original_response(dpp::message(convo.GetLastResponse()));
            });
            t.detach();
        });
    }

}
#include "MyBot.h"
#include <dpp/dpp.h>
#define canal_sesiones 110743729237053XXXX

/* Be sure to place your token in the line below.
 * Follow steps here to get a token:
 * https://dpp.dev/creating-a-bot-application.html
 * When you invite the bot, be sure to invite it with the 
 * scopes 'bot' and 'applications.commands', e.g.
 * https://discord.com/oauth2/authorize?client_id=940762342495518720&scope=bot+applications.commands&permissions=139586816064
 */
const std::string    BOT_TOKEN    = "MTEwNzQyODE0NDkwODA3OTExNA.GTgwUl.tfajIGNsu3gXZVJZBmfbZtbNrLwAgRSXXXXX";


int main()
{
    /*int canal_sesiones = 1107437292370534410;*/

    dpp::cluster bot(BOT_TOKEN);
    bot.on_log(dpp::utility::cout_logger());

    dpp::message message(canal_sesiones, "Nueva sesion de bot iniciada");
    bot.message_create(message);


    /* Handle slash command */
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
         if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
         if (event.command.get_command_name() == "chatgpt") {
             event.thinking(false);
             std::string comando = std::get<1>(event.get_parameter("comando"));
             dpp::message msg = dpp::message::message(comando);
             event.edit_response(msg);
         }
    });

    /* Register slash command here in on_ready */
    bot.on_ready([&bot](const dpp::ready_t& event) {
        /* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand chatgpt("chatgpt", "Pregunta algo a chat gpt, haz una consulta.", bot.me.id);
            chatgpt.add_option(dpp::command_option::command_option(dpp::command_option_type::co_string, "comando", "Especifica el comando a enviar.", true));
            bot.global_command_create(chatgpt);
            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
        }
    });



    /* Start the bot */
    bot.start(dpp::st_wait);

    return 0;
}

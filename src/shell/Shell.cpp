/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** Shell
*/

#include <string>

#include "../kitchens/Kitchens.hpp"
#include "../reception/Reception.hpp"



Generator tmp;




std::vector<std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize>> shellParsing(std::string line);

void shell(size_t cookingTime, size_t nbCooks, size_t time)
{
    std::string commandLine;
    pid_t parentPid = getpid();
    plazza::reception::Reception IPC;

    while (getpid() == parentPid) {
        std::getline(std::cin, commandLine);
        for (auto pizza: shellParsing(commandLine)) {
            while (!IPC.FoundAvailaibleKitchen(pizza)) {
                IPC.CreateNewKitchens(nbCooks, time, cookingTime);
            }
        }
        IPC.ServePizza();
    }
}

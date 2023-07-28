/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** ShellParsing
*/

#include "Pizza.hpp"

#include <string>
#include <vector>
#include <regex>
#include <bits/stdc++.h>

plazza::pizza::PizzaType getPizzaType(std::string type)
{
    if (!type.compare("regina"))
        return (plazza::pizza::PizzaType::Regina);
    if (!type.compare("margarita"))
        return (plazza::pizza::PizzaType::Margarita);
    if (!type.compare("americana"))
        return (plazza::pizza::PizzaType::Americana);
    if (!type.compare("fantasia"))
        return (plazza::pizza::PizzaType::Fantasia);
    return (plazza::pizza::PizzaType::EmptyType);
}

plazza::pizza::PizzaSize getPizzaSize(std::string size)
{
    if (!size.compare("S"))
        return (plazza::pizza::PizzaSize::S);
    if (!size.compare("M"))
        return (plazza::pizza::PizzaSize::M);
    if (!size.compare("L"))
        return (plazza::pizza::PizzaSize::L);
    if (!size.compare("XL"))
        return (plazza::pizza::PizzaSize::XL);
    if (!size.compare("XXL"))
        return (plazza::pizza::PizzaSize::XXL);
    return (plazza::pizza::PizzaSize::EmptySize);
}

std::vector<std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize>> generateNewCommand(std::string newCommand)
{
    std::stringstream split(std::regex_replace(newCommand, std::regex("^ +| +$|( ) +"), "$1"));
    std::vector<std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize>> nextCommand;
    std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize> newPizza;
    std::string type;
    std::string size;
    std::string multiplier;
    int n;

    split >> type >> size >> multiplier;
    multiplier.erase(0, 1);
    newPizza.first = getPizzaType(type);
    newPizza.second = getPizzaSize(size);
    if (newPizza.first == plazza::pizza::PizzaType::EmptyType || newPizza.second == plazza::pizza::PizzaSize::EmptySize)
        return {};
    n = stoi(multiplier);
    for (int i = 0; i < n; i++)
        nextCommand.push_back(newPizza);
    return (nextCommand);
}

std::vector<std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize>> shellParsing(std::string line)
{
    std::stringstream split(std::regex_replace(line, std::regex("^ +| +$|( ) +"), "$1"));
    std::vector<std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize>> allCommands = {};
    std::vector<std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize>> nextCommand;
    std::string command;

    while (getline(split, command, ';')) {
        nextCommand = generateNewCommand(command);
        for (auto data: nextCommand) {
            allCommands.push_back(data);
        }
    }
    return (allCommands);
}

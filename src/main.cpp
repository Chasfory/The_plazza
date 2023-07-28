/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** main
*/

#include <string>
#include <iostream>
#include <stdexcept>

void shell(size_t cookingTime, size_t nbCooks, size_t time);

bool handleArguments(int argc, char **argv)
{
    if (argc != 4) {
        return (false);
    }
    for (int i = 1; i < argc; i++) {
        try {
            std::stoi(argv[i], nullptr, 10);
        } catch (const std::invalid_argument&) {
            return (false);
        }
    }
    return (true);
}

int main(int argc, char **argv)
{
    if (handleArguments(argc, argv))
        shell(std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3]));
    else
        return (84);
    return (0);
}

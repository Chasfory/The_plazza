/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-laetitia.bousch
** File description:
** ACommunication
*/

#include "../include/ACommunication.hpp"
#include "./kitchens/cooks/Cooks.hpp"

namespace plazza::communication {
    void Communication::CreateThreadForNewCooks(std::function<void()> f)
    {
        _threads.emplace_back(f);
    }

    std::vector<std::thread>  &Communication::GetThreads()
    {
        return _threads;
    }
}

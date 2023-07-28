/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-laetitia.bousch
** File description:
** reception
*/

#ifndef RECEPTION_HPP_
    #define RECEPTION_HPP_

    #include <iostream>
    #include <memory>
    #include <vector>
    #include <ctime>
    #include <sys/types.h>
    #include <sys/wait.h>

    #include "../../include/Pizza.hpp"
    #include "../../include/MessageQueue.hpp"

namespace plazza::kitchens {
    class Kitchens;
}

// :D
namespace plazza::reception {

    class Reception
    {
        protected:
            MsgLifeKitchen_s LifeKitchen;
            MsgSendPizza_s SendPizza;
            MsgStatusQueue_s StatusQueue;
            MsgStatusReserve_s StatusReserve;
            MsgPizzaIsReady_s PizzaReady;
        public:
            Reception();
            ~Reception() = default;

            void CreateNewKitchens(size_t nbrCooks, std::time_t timeRefillIngredients, size_t tmpTime);
            void DestroyKitchens();
            bool FoundAvailaibleKitchen(std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize> pizza);
            void ServePizza();

        private:
            std::vector<std::unique_ptr<plazza::kitchens::Kitchens>> _kitchens;
            std::vector<int> _msgIDs;
    };
}

#endif /* !RECEPTION_HPP_ */

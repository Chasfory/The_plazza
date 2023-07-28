/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** Reception
*/

#include "Reception.hpp"
#include "../kitchens/Kitchens.hpp"

namespace plazza::reception {

    Reception::Reception()
    {
        SendPizza.TypeMsg = static_cast<long>(TypeMsg::MsgSendPizza);
        StatusQueue.TypeMsg = static_cast<long>(TypeMsg::MsgStatusQueue);
        StatusReserve.TypeMsg = static_cast<long>(TypeMsg::MsgStatusReserve);
        PizzaReady.TypeMsg = static_cast<long>(TypeMsg::MsgPizzaIsReady);
    }

    void Reception::CreateNewKitchens(size_t nbrCooks, std::time_t timeRefillIngredients, size_t tmpTime)
    {
        std::unique_ptr<plazza::kitchens::Kitchens> newKitchen = std::make_unique<plazza::kitchens::Kitchens>(nbrCooks, timeRefillIngredients, tmpTime);
        int newID = newKitchen->GetMsgID();

        //std::cout << "NewID de la nouvelle kitchen dans Reception: " << newID << std::endl;
        _kitchens.push_back(std::move(newKitchen));
        _msgIDs.push_back(newID);
        sleep(1);
    }

    void Reception::DestroyKitchens()
    {
    }

    bool Reception::FoundAvailaibleKitchen(std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize> pizza)
    {
        auto it = _kitchens.begin();
        auto msgID = _msgIDs.begin();
        while (it != _kitchens.end()) {
            LifeKitchen.TypeMsg = static_cast<long>(TypeMsg::MsgLifeKitchen);
            LifeKitchen.status = true;
            msgsnd((*it)->GetMsgID(), &LifeKitchen, sizeof(LifeKitchen), static_cast<long>(plazza::reception::TypeMsg::MsgLifeKitchen));
            LifeKitchen.TypeMsg = static_cast<long>(TypeMsg::MsgLifeKitchenReply);
            msgrcv((*it)->GetMsgID(), &LifeKitchen, sizeof(LifeKitchen), static_cast<long>(plazza::reception::TypeMsg::MsgLifeKitchenReply), 0);
            msgrcv((*it)->GetMsgID(), &StatusQueue, sizeof(StatusQueue), static_cast<long>(plazza::reception::TypeMsg::MsgStatusQueue), 0);
            if (!LifeKitchen.status || !StatusQueue.status) {
                waitpid((*it)->GetPID(), NULL, 0);
                while (msgID != _msgIDs.end()) {
                    if ((*msgID) == (*it)->GetMsgID()) {
                        _msgIDs.erase(msgID);
                        break;
                    }
                    msgID++;
                }
                it = _kitchens.erase(it);
                continue;
            }
            SendPizza.pizza = pizza;
            std::cout << "======SENT========" << std::endl;
            msgsnd((*it)->GetMsgID(), &SendPizza, sizeof(SendPizza), static_cast<long>(plazza::reception::TypeMsg::MsgSendPizza));
            if (LifeKitchen.status && StatusQueue.status) {
                std::cout << "======FOUND========" << std::endl;
                return (true);
            }
            it++;
        }
        std::cout << "======ENDSENT========" << std::endl;
        return (false);
    }

    void Reception::ServePizza()
    {
        for (auto _msgID: _msgIDs) {
            if (msgrcv(_msgID, &PizzaReady, sizeof(PizzaReady), static_cast<long>(TypeMsg::MsgPizzaIsReady), IPC_NOWAIT) > 0) {
                if (PizzaReady.pizza.first == plazza::pizza::PizzaType::Regina)
                    std::cout << "Pizza Regina finish!" << std::endl;
                if (PizzaReady.pizza.first == plazza::pizza::PizzaType::Margarita)
                    std::cout << "Pizza Margarita finish!" << std::endl;
                if (PizzaReady.pizza.first == plazza::pizza::PizzaType::Americana)
                    std::cout << "Pizza Americana finish!" << std::endl;
                if (PizzaReady.pizza.first == plazza::pizza::PizzaType::Fantasia)
                    std::cout << "Pizza Fantasia finish!" << std::endl;
            }
        }
    }

}

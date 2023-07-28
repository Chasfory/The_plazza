/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-laetitia.bousch
** File description:
** Cooks
*/

#include "Cooks.hpp"
#include <chrono>
#include <thread>

namespace plazza::kitchens::cooks {
    Cooks::Cooks(std::shared_ptr<plazza::kitchens::SafeQueue> &queue, std::shared_ptr<plazza::kitchens::Reserve> &reserve, size_t tmpTime, int msgID):
    _pizza(), _queue(queue), _reserve(reserve), _timePizza(tmpTime), _timeCurrentPizza(0), _timeFinishPizza(0), _msgID(msgID)
    {
        _status = std::make_shared<plazza::kitchens::cooks::StatusCooks>();
    }

    bool StatusCooks::GetStatus()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _status;
    }

    void StatusCooks::SetStatus(bool tmp)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _status = tmp;
    }

    std::shared_ptr<StatusCooks> &Cooks::GetStatus()
    {
        return _status;
    }

    bool Cooks::StartPizza()
    {
        std::optional<std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize>> newPizza = _queue->GetQueue();
        if (newPizza.has_value() == false || _reserve->RemoveIngredients(newPizza.value().first) == false) {
            return false;
        }
        _pizza = newPizza.value();
        _timeFinishPizza = (intmax_t)newPizza.value().first * (intmax_t)newPizza.value().second * (intmax_t)_timePizza;

        std::cout << "pizza current:" << _timeFinishPizza << std::endl;
        this->_status->SetStatus(true);
        std::this_thread::sleep_for(std::chrono::seconds{_timeFinishPizza});
        this->_status->SetStatus(false);
        PizzaReady.pizza = _pizza;
        if (PizzaReady.pizza.first == plazza::pizza::PizzaType::Regina)
            std::cout << "Pizza Regina finish!" << std::endl;
        if (PizzaReady.pizza.first == plazza::pizza::PizzaType::Margarita)
            std::cout << "Pizza Margarita finish!" << std::endl;
        if (PizzaReady.pizza.first == plazza::pizza::PizzaType::Americana)
            std::cout << "Pizza Americana finish!" << std::endl;
        if (PizzaReady.pizza.first == plazza::pizza::PizzaType::Fantasia)
            std::cout << "Pizza Fantasia finish!" << std::endl;
        msgsnd(_msgID, &PizzaReady, sizeof(PizzaReady), static_cast<long>(plazza::reception::TypeMsg::MsgPizzaIsReady));
        return true;
    }


    void Cooks::loop()
    {
        while (StartPizza()) {}
    }
}

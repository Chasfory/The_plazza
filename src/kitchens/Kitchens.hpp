/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-laetitia.bousch
** File description:
** Kitchens
*/

#ifndef KITCHENS_HPP_
    #define KITCHENS_HPP_

    #include <ctime>
    #include <optional>
    #include <algorithm>
    #include <unistd.h>
    #include <sys/ipc.h>
    #include <sys/msg.h>
    #include <functional>

    #include "./cooks/Cooks.hpp"
    #include "../../include/Pizza.hpp"
    #include "../../include/ACommunication.hpp"
    #include "../../include/MessageQueue.hpp"

namespace plazza::kitchens::cooks {
    class Cooks;
    class StatusCooks;
}

namespace plazza::communication {
    class Communication;
}

namespace plazza::kitchens {
    class SafeQueue {
        public :
            SafeQueue() = default;
            ~SafeQueue() = default;

            void Push(std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize> pizza);
            bool TryPop(std::size_t nbr);
            std::mutex &getMutex();
            std::condition_variable &getCondVar();
            std::optional<std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize>> GetQueue();

        private:
            std::vector<std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize>> _queue;
            std::mutex _mutex;
            std::condition_variable _condVar;
    };

    class Reserve {
        public:
            Reserve(std::time_t timeRefillIngredients);
            ~Reserve() = default;

            bool RemoveIngredients(plazza::pizza::PizzaType type);
            void RefillIngredients();
            void CloseReserve() {
                std::unique_lock<std::mutex> lock(_mutex);
                _alive = false;
            };
            std::mutex &getMutex();
            std::condition_variable &getCondVar();
            std::vector<std::pair<plazza::pizza::Ingredients, size_t>> GetReserve();

        private:
            std::vector<std::pair<plazza::pizza::Ingredients, size_t>> _reserve;
            std::time_t _timeRefillIngredients;
            std::time_t _finishTimeRefillIngredients;
            std::mutex _mutex;
            std::condition_variable _condVar;
            bool _alive = true;
    };

    // :(
    class Kitchens {
        protected:
            plazza::reception::MsgLifeKitchen_s LifeKitchen;
            plazza::reception::MsgSendPizza_s SendPizza;
            plazza::reception::MsgStatusQueue_s StatusQueue;
            plazza::reception::MsgStatusReserve_s StatusReserve;
            plazza::reception::MsgPizzaIsReady_s PizzaReady;
        public:
            Kitchens(size_t nbrCooks, std::time_t timeRefillIngredients, size_t tmpTime);
            ~Kitchens() = default;

            void DisplayStatus();
            bool GetTryPopQueue(std::size_t nbr);
            bool isKitchenAlive();
            void KitchenLoop();
            void PutQueue(std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize> newPizza);
            pid_t GetPID();
            int GetMsgID();
        private:
            std::vector<plazza::kitchens::cooks::Cooks> _cooks;
            std::time_t _timeAlive;
            bool _unemployment = false;
            plazza::communication::Communication _communication;
            std::shared_ptr<SafeQueue> _queue;
            std::shared_ptr<Reserve> _reserve;
            pid_t _pid;
        	int _msgID;
    };
}

#endif /* !KITCHENS_HPP_ */

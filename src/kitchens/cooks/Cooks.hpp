/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-laetitia.bousch
** File description:
** Cooks
*/

#ifndef COOKS_HPP_
    #define COOKS_HPP_

    #include <memory>
    #include <mutex>
    #include <condition_variable>

    #include "../Kitchens.hpp"
    #include "../../../include/MessageQueue.hpp"

namespace plazza::kitchens {
    class SafeQueue;
    class Reserve;
}

namespace plazza::kitchens::cooks {
    class StatusCooks {
        public:
            StatusCooks() = default;
            ~StatusCooks() = default;

            bool GetStatus();
            void SetStatus(bool tmp);
        private:
            bool _status;
            std::mutex _mutex;
    };

    class Cooks {
        public:
            Cooks(std::shared_ptr<plazza::kitchens::SafeQueue> &queue, std::shared_ptr<plazza::kitchens::Reserve> &reserve, size_t tmpTime, int msgID);
            ~Cooks() = default;

            bool StartPizza();
            void loop();
            std::shared_ptr<StatusCooks> &GetStatus();
        protected:
            plazza::reception::MsgPizzaIsReady_s PizzaReady;

        private:
            std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize> _pizza;
            std::shared_ptr<plazza::kitchens::SafeQueue> _queue;
            std::shared_ptr<plazza::kitchens::Reserve> _reserve;
            std::time_t _timePizza;
            std::time_t _timeCurrentPizza;
            intmax_t _timeFinishPizza;
            int _msgID;
            std::shared_ptr<StatusCooks> _status;
    };
}

#endif /* !COOKS_HPP_ */

/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** MessageQueue
*/

#ifndef MESSAGEQUEUE_HPP_
    #define MESSAGEQUEUE_HPP_

    #include <iostream>
    #include <unistd.h>
    #include <sys/ipc.h>
    #include <sys/msg.h>

    #include "Pizza.hpp"

namespace plazza::reception {

    enum class TypeMsg {
        MsgLifeKitchen = 1,
        MsgSendPizza = 2,
        MsgStatusQueue = 3,
        MsgStatusReserve = 4,
        MsgPizzaIsReady = 5,
        MsgLifeKitchenReply = 6,
    };

    struct MsgLifeKitchen_s { // Mutuel
        long TypeMsg;
        bool status;
    };

    struct MsgSendPizza_s { // Plazza to Kitchen
        long TypeMsg;
        std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize> pizza;
    };

    struct MsgStatusQueue_s { // Kitchen to Plazza
        long TypeMsg;
        bool status;
    };

    struct MsgStatusReserve_s { // Kitchen to Plazza
        long TypeMsg;
        bool status;
    };

    struct MsgPizzaIsReady_s { // Kitchen(Cook) to Plazza
        long TypeMsg;
        std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize> pizza;
    };

    class MessageQueue
    {
        protected:
            MsgLifeKitchen_s LifeKitchen;
            MsgSendPizza_s SendPizza;
            MsgStatusQueue_s StatusQueue;
            MsgStatusReserve_s StatusReserve;
            MsgPizzaIsReady_s PizzaReady;
        public:
            MessageQueue() = default;
            ~MessageQueue() = default;

            void SendPingKitchenAlive(int msgID, long typeMsg, bool status);
            void SendMsgPizza(int msgID, long typeMsg, std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize>);
            void SendMsgQueueStatus(int msgID, long typeMsg, bool status);

            bool GetPingKitchenAlive(int msgID, size_t size, long typeMsg, bool wait);
            std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize> GetMsgPizza(int msgID, size_t size, long typeMsg, bool wait);
            bool GetMsgQueueStatus(int msgID, long typeMsg, bool wait);
    };

}

#endif /* !MESSAGEQUEUE_HPP_ */

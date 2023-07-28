/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** MessageQueue
*/

#include "../../include/MessageQueue.hpp"

namespace plazza::reception
{

    void MessageQueue::SendPingKitchenAlive(int msgID, long typeMsg, bool status)
    {
        LifeKitchen.TypeMsg = typeMsg;
        LifeKitchen.status = status;
        msgsnd(msgID, &LifeKitchen, sizeof(LifeKitchen), typeMsg);
    }

    void MessageQueue::SendMsgPizza(int msgID, long typeMsg, std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize> pizza)
    {
        SendPizza.TypeMsg = typeMsg;
        SendPizza.pizza = pizza;
        msgsnd(msgID, &SendPizza, sizeof(SendPizza), typeMsg);
    }

    void MessageQueue::SendMsgQueueStatus(int msgID, long typeMsg, bool status)
    {
        StatusQueue.TypeMsg = typeMsg;
        StatusQueue.status = typeMsg;
        msgsnd(msgID, &StatusQueue, sizeof(StatusQueue), typeMsg);
    }

    bool MessageQueue::GetPingKitchenAlive(int msgID, size_t size, long typeMsg, bool wait)
    {
        LifeKitchen.TypeMsg = typeMsg;
        if (wait) {
          msgrcv(msgID, &LifeKitchen, sizeof(LifeKitchen), typeMsg, MSG_NOERROR);
        } else {
          if (msgrcv(msgID, &LifeKitchen, sizeof(LifeKitchen), typeMsg, MSG_NOERROR | IPC_NOWAIT) <= 0)
                return (false);
        }
        return (LifeKitchen.status);
    }

    std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize> MessageQueue::GetMsgPizza(int msgID, size_t size, long typeMsg, bool wait)
    {
        PizzaReady.TypeMsg = typeMsg;
        if (wait) {
          msgrcv(msgID, &PizzaReady, sizeof(PizzaReady), typeMsg, MSG_NOERROR);
        } else {
          msgrcv(msgID, &PizzaReady, sizeof(PizzaReady), typeMsg, MSG_NOERROR | IPC_NOWAIT);
        }
        return (PizzaReady.pizza);
    }

    bool MessageQueue::GetMsgQueueStatus(int msgID, long typeMsg, bool wait)
    {
        StatusQueue.TypeMsg = typeMsg;

        if (wait) {
            msgrcv(msgID, &StatusQueue, sizeof(StatusQueue), typeMsg, MSG_NOERROR);
        } else {
            msgrcv(msgID, &StatusQueue, sizeof(StatusQueue), typeMsg, MSG_NOERROR | IPC_NOWAIT);
        }
        return (StatusQueue.status);
    }

}

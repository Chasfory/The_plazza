/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-laetitia.bousch
** File description:
** Kitchens
*/

#include "Kitchens.hpp"

namespace plazza::kitchens {
    void SafeQueue::Push(std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize> pizza)
    {
        std::unique_lock<std::mutex> lock(_mutex);

        //_condVar.wait(lock);
        _queue.push_back(pizza);
        _condVar.notify_one();
    }

    bool SafeQueue::TryPop(std::size_t nbr)
    {

        if (_queue.size() == nbr) {
            return false;
        }
        return true;
    }

    std::optional<std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize>> SafeQueue::GetQueue()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _condVar.wait(lock);
        if (_queue.size() != 0) {
            std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize> newPizza = _queue.at(0);
            _queue.erase(_queue.begin());
            return newPizza;
        }
        return std::nullopt;
    }

    std::mutex &SafeQueue::getMutex()
    {
        return _mutex;
    }

    std::condition_variable &SafeQueue::getCondVar()
    {
        return _condVar;
    }

    std::mutex &Reserve::getMutex()
    {
        return _mutex;
    }

    std::condition_variable &Reserve::getCondVar()
    {
        return _condVar;
    }

    Kitchens::Kitchens(size_t nbrCooks, std::time_t timeRefillIngredients, size_t tmpTime)
    {
        std::cout << "===========NEW KITCHEN===========" << std::endl;
        _pid = fork();
        if (_pid != 0) {
            key_t key = ftok("plazza", _pid);
            _msgID = msgget(key, 0666 | IPC_CREAT);
            return;
        }
        key_t key = ftok("plazza", getpid());
        _msgID = msgget(key, 0666 | IPC_CREAT);
        std::shared_ptr<SafeQueue> tmpQueue = std::make_shared<SafeQueue>();
        std::shared_ptr<Reserve> tmpReserve = std::make_shared<Reserve>(timeRefillIngredients);

        _queue = tmpQueue;
        _reserve = tmpReserve;
        for (size_t tmp = 0; tmp < nbrCooks; tmp++) {
            _cooks.push_back(cooks::Cooks(_queue, _reserve, tmpTime, _msgID));
            auto &cook = _cooks.at(tmp);
            _communication.CreateThreadForNewCooks([&cook]() {
                cook.loop();
            });
        }
        _timeAlive = 0;
        KitchenLoop();
    }

    void Kitchens::KitchenLoop()
    {
        SendPizza.TypeMsg = static_cast<long>(plazza::reception::TypeMsg::MsgSendPizza);
        StatusQueue.TypeMsg = static_cast<long>(plazza::reception::TypeMsg::MsgStatusQueue);
        StatusReserve.TypeMsg = static_cast<long>(plazza::reception::TypeMsg::MsgStatusReserve);
        PizzaReady.TypeMsg = static_cast<long>(plazza::reception::TypeMsg::MsgPizzaIsReady);
        while (isKitchenAlive()) {
            LifeKitchen.TypeMsg = static_cast<long>(plazza::reception::TypeMsg::MsgLifeKitchen);
            if (msgrcv(_msgID, &LifeKitchen, sizeof(LifeKitchen), static_cast<long>(plazza::reception::TypeMsg::MsgLifeKitchen), IPC_NOWAIT) > 0) {
                LifeKitchen.status = true;
                LifeKitchen.TypeMsg = static_cast<long>(plazza::reception::TypeMsg::MsgLifeKitchenReply);
                msgsnd(_msgID, &LifeKitchen, sizeof(LifeKitchen), static_cast<long>(plazza::reception::TypeMsg::MsgLifeKitchenReply));
                StatusQueue.status = GetTryPopQueue(_cooks.size());
                sleep(2);
                msgsnd(_msgID, &StatusQueue, sizeof(StatusQueue), static_cast<long>(plazza::reception::TypeMsg::MsgStatusQueue));
                if (!StatusQueue.status)
                {
                    continue;
                }
                msgrcv(_msgID, &SendPizza, sizeof(SendPizza), static_cast<long>(plazza::reception::TypeMsg::MsgSendPizza), 0);
                if (SendPizza.pizza.first == plazza::pizza::PizzaType::Regina)
                    std::cout << "get REGINA" << std::endl;
                if (SendPizza.pizza.first == plazza::pizza::PizzaType::Margarita)
                    std::cout << "get MARGARITA" << std::endl;
                if (SendPizza.pizza.first == plazza::pizza::PizzaType::Americana)
                    std::cout << "get AMERICANA" << std::endl;
                if (SendPizza.pizza.first == plazza::pizza::PizzaType::Fantasia)
                    std::cout << "get FANTASIA" << std::endl;
                if (StatusQueue.status)
                    PutQueue(SendPizza.pizza);
            }
        }
        std::cout << "Fermeture DE LA KITCHENS!!!" << std::endl;
        msgrcv(_msgID, &LifeKitchen, sizeof(LifeKitchen), static_cast<long>(plazza::reception::TypeMsg::MsgLifeKitchen), 0);
        LifeKitchen.status = false;
        LifeKitchen.TypeMsg = static_cast<long>(plazza::reception::TypeMsg::MsgLifeKitchenReply);
        msgsnd(_msgID, &LifeKitchen, sizeof(LifeKitchen), static_cast<long>(plazza::reception::TypeMsg::MsgLifeKitchenReply));
        sleep(2);
        msgsnd(_msgID, &StatusQueue, sizeof(StatusQueue), static_cast<long>(plazza::reception::TypeMsg::MsgStatusQueue));
        _queue->getCondVar().notify_all();
        for (auto &thread: _communication.GetThreads()) {
            thread.join();
        }
        exit(0);
    }

    void Kitchens::PutQueue(std::pair<plazza::pizza::PizzaType, plazza::pizza::PizzaSize> newPizza)
    {
        //std::cout << "Put pizza in queue because IL Y A DE LA PLACE" << std::endl;
        _queue->Push(newPizza);
        //std::cout << "Fini de mettre la new pizza dans la queue" << std::endl;
    }

    bool Kitchens::isKitchenAlive()
    {
        std::time_t end;

        //std::this_thread::sleep_for(std::chrono::seconds{1});
        for (size_t i = 0; i < _cooks.size(); i++) {
            if (_cooks.at(i).GetStatus()->GetStatus() == true) {
                _unemployment = false;
            }
        }
        //std::cout << "employment " << _unemployment << std::endl;
        if (_unemployment) {
            end = std::time(nullptr);
            //std::cout << "time " << (intmax_t)end - (intmax_t)_timeAlive << std::endl;
            if ((intmax_t)end - (intmax_t)_timeAlive > 5) {
                return false;
            }
        } else {
            _timeAlive = std::time(nullptr);
            _unemployment = true;
        }
        return true;
    }

    void Kitchens::DisplayStatus()
    {
        std::unique_lock<std::mutex> lock(_reserve->getMutex());
        _reserve->getCondVar().wait(lock);
        std::cout << "Kitchen Ingredient:" << std::endl;
        if (_reserve->GetReserve().size() == 0) {
            std::cout << "Empty Ingredients !" << std::endl;
            return;
        }
        std::cout << "Dough: " << _reserve->GetReserve().at(0).second << std::endl;
        std::cout << "Dough: " <<  _reserve->GetReserve().at(1).second << std::endl;
        std::cout << "Tomato: " << _reserve->GetReserve().at(2).second << std::endl;
        std::cout << "Gruyere: " << _reserve->GetReserve().at(3).second << std::endl;
        std::cout << "Ham: " << _reserve->GetReserve().at(4).second << std::endl;
        std::cout << "Mushrooms: " << _reserve->GetReserve().at(5).second << std::endl;
        std::cout << "Steak: " << _reserve->GetReserve().at(6).second << std::endl;
        std::cout << "Eggplant: " << _reserve->GetReserve().at(7).second << std::endl;
        std::cout << "Goat Cheese: " <<  _reserve->GetReserve().at(8).second << std::endl;
        std::cout << "Chief Love: " << _reserve->GetReserve().at(9).second << std::endl;
        _reserve->getCondVar().notify_all();
        lock.unlock();
    }

    std::vector<std::pair<plazza::pizza::Ingredients, size_t>> Reserve::GetReserve()
    {
        return _reserve;
    }

    Reserve::Reserve(std::time_t timeRefillIngredients)
    {
        plazza::pizza::Ingredients ingredients;

        for (size_t i = 0; i < 9; i++) {
            ingredients = static_cast<plazza::pizza::Ingredients>(i);
            _reserve.push_back(std::make_pair(ingredients, 5));
        }
        _timeRefillIngredients = std::time(nullptr);
        _finishTimeRefillIngredients = timeRefillIngredients;
    }

    void Reserve::RefillIngredients()
    {
        std::time_t end = std::time(nullptr);

        if ((intmax_t)end - (intmax_t)_timeRefillIngredients >= _finishTimeRefillIngredients) {
            std::unique_lock<std::mutex> lock(_mutex);
            for (size_t i = 0; i < _reserve.size(); i++) {
                if (_reserve.at(i).second < 5) {
                    _reserve.at(i).second += 1;
                }
            }
            _condVar.notify_all();
            _timeRefillIngredients = std::time(nullptr);
        }
    }

    bool Kitchens::GetTryPopQueue(std::size_t nbr)
    {
        return _queue->TryPop(nbr);
    }

    bool Reserve::RemoveIngredients(plazza::pizza::PizzaType type)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        switch (type) {
            case plazza::pizza::PizzaType::Regina:
                while (!(_reserve.at(0).second > 0 && _reserve.at(1).second > 0 && _reserve.at(2).second > 0)) {
                    if (!_alive) {
                        return false;
                    }
                    _condVar.wait(lock);
                }
                _reserve.at(0).second -= 1;
                _reserve.at(1).second -= 1;
                _reserve.at(2).second -= 1;
                break;
            case plazza::pizza::PizzaType::Margarita:
                while (!(_reserve.at(0).second > 0 && _reserve.at(1).second > 0 && _reserve.at(2).second > 0 && _reserve.at(3).second > 0 && _reserve.at(4).second > 0)) {
                    if (!_alive) {
                        return false;
                    }
                    _condVar.wait(lock);
                }
                _reserve.at(0).second -= 1;
                _reserve.at(1).second -= 1;
                _reserve.at(2).second -= 1;
                _reserve.at(3).second -= 1;
                _reserve.at(4).second -= 1;
                break;
            case plazza::pizza::PizzaType::Americana:
                while (!(_reserve.at(0).second > 0 && _reserve.at(1).second > 0 && _reserve.at(2).second > 0 && _reserve.at(5).second > 0)) {
                    if (!_alive) {
                        return false;
                    }
                    _condVar.wait(lock);
                }
                _reserve.at(0).second -= 1;
                _reserve.at(1).second -= 1;
                _reserve.at(2).second -= 1;
                _reserve.at(5).second -= 1;
                break;
            case plazza::pizza::PizzaType::Fantasia:
                while (!(_reserve.at(0).second > 0 && _reserve.at(1).second > 0 && _reserve.at(6).second > 0 && _reserve.at(7).second > 0 && _reserve.at(8).second > 0)) {
                    if (!_alive) {
                        return false;
                    }
                    _condVar.wait(lock);
                }
                _reserve.at(0).second -= 1;
                _reserve.at(1).second -= 1;
                _reserve.at(6).second -= 1;
                _reserve.at(7).second -= 1;
                _reserve.at(8).second -= 1;
                break;
            default:
                break;
        }
        return true;
    }

    pid_t Kitchens::GetPID()
    {
        return (_pid);
    }

    int Kitchens::GetMsgID()
    {
        return (_msgID);
    }

}

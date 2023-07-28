/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-laetitia.bousch
** File description:
** ACommunication
*/

#ifndef ACOMMUNICATION_HPP_
    #define ACOMMUNICATION_HPP_

    #include <memory>
    #include <vector>
    #include <mutex>
    #include <thread>
    #include <condition_variable>
    #include <functional>

namespace plazza::kitchens::cooks {
    class Cooks;
}

namespace plazza::communication {
    class Communication {
        public:
            Communication() = default;
            ~Communication() = default;

            void CreateThreadForNewCooks(std::function<void()>);
            std::vector<std::thread> &GetThreads();
        private:
            std::vector<std::thread> _threads;
    };
}

#endif /* !ACOMMUNICATION_HPP_ */

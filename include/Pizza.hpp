/*
** EPITECH PROJECT, 2023
** Plazza
** File description:
** Pizza
*/

#ifndef PIZZA_HPP_
    #define PIZZA_HPP_

namespace plazza::pizza {

    enum class Ingredients {
        dough,
        tomato,
        gruyere,
        ham,
        mushrooms,
        steak,
        eggplant,
        goat_cheese,
        chief_love,
    };

    enum class PizzaType {
        EmptyType = 0,
        Regina = 1,
        Margarita = 2,
        Americana = 4,
        Fantasia = 8
    };

    enum class PizzaSize {
        EmptySize = 0,
        S = 1,
        M = 2,
        L = 4,
        XL = 8,
        XXL = 16
    };

}

#endif /* !PIZZA_HPP_ */

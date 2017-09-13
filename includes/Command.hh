
#ifndef COMMAND_HH_
# define COMMAND_HH_

#include <list>
#include <string>
#include <iostream>
#include "Plazza.hh"

namespace Plazza
{
  class Command
  {
  private:
    PizzaType::ePizzaType			_pizzaType;
    PizzaSize::ePizzaSize			_size;
    std::list<Ingredient::eIngredient>		_ingredientsNeeded;
    
  public:
    Command(PizzaType::ePizzaType pizzaType, PizzaSize::ePizzaSize size, std::list<Ingredient::eIngredient> ingredientsNeeded);
    ~Command();
    Command(Command const & other);
    Command & operator=(Command const & other);
    PizzaType::ePizzaType getType() const;
    PizzaSize::ePizzaSize getSize() const;
    std::list<Ingredient::eIngredient> const & getIngredient() const;
  };
}

#endif

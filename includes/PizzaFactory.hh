
#ifndef PIZZAFACTORY_HH_
# define PIZZAFACTORY_HH_

#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "Command.hh"
#include "Plazza.hh"

namespace Plazza
{
  class PizzaFactory
  {

  public:
    PizzaFactory();
    ~PizzaFactory();
    PizzaFactory(PizzaFactory const & other);
    PizzaFactory & operator=(PizzaFactory const & other);

  public:
    static Plazza::Command	* createPizza(std::string const & msg);
    static Plazza::Command	* regina(PizzaType::ePizzaType typePizza, PizzaSize::ePizzaSize size);
    static Plazza::Command	* margarita(PizzaType::ePizzaType typePizza, PizzaSize::ePizzaSize size);
    static Plazza::Command	* americaine(PizzaType::ePizzaType typePizza, PizzaSize::ePizzaSize size);
    static Plazza::Command	* fantasia(PizzaType::ePizzaType typePizza, PizzaSize::ePizzaSize size);

  private:
    static PizzaType::ePizzaType  get_typePizza(std::string const & str);
    static PizzaSize::ePizzaSize  get_pizzaSize(std::string const & str);
  };
};

#endif

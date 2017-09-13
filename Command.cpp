
#include "Command.hh"

Plazza::Command::Command(PizzaType::ePizzaType pizzaType, PizzaSize::ePizzaSize size, std::list<Ingredient::eIngredient> ingredientsNeeded) :
  _pizzaType(pizzaType), _size(size), _ingredientsNeeded(ingredientsNeeded)
{}

Plazza::Command::~Command()
{}

PizzaType::ePizzaType Plazza::Command::getType() const
{
  return (this->_pizzaType);
}

PizzaSize::ePizzaSize Plazza::Command::getSize() const
{
  return (this->_size);
}

std::list<Ingredient::eIngredient> const & Plazza::Command::getIngredient() const
{
  return (this->_ingredientsNeeded);
}

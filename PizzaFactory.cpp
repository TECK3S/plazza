
#include <list>
#include "PizzaFactory.hh"

static   Plazza::Command * (*ptr[])(PizzaType::ePizzaType, PizzaSize::ePizzaSize) =
{
  &Plazza::PizzaFactory::regina,
  &Plazza::PizzaFactory::margarita,
  &Plazza::PizzaFactory::americaine,
  &Plazza::PizzaFactory::fantasia,
  0
};

static std::map<PizzaType::ePizzaType, std::string>create_ptype()
{
  std::map<PizzaType::ePizzaType, std::string> typeTab;
 
  typeTab[PizzaType::Regina] = PizzaName::Regina;
  typeTab[PizzaType::Margarita] = PizzaName::Margarita;
  typeTab[PizzaType::Americaine] = PizzaName::Americaine;
  typeTab[PizzaType::Fantasia] = PizzaName::Fantasia;
  return (typeTab);
}

static std::map<PizzaSize::ePizzaSize, std::string>create_psize()
{
  std::map<PizzaSize::ePizzaSize, std::string> typeTab;

  typeTab[PizzaSize::S] = PizzaName::S;
  typeTab[PizzaSize::M] = PizzaName::M;
  typeTab[PizzaSize::L] = PizzaName::L;
  typeTab[PizzaSize::XL] = PizzaName::XL;
  typeTab[PizzaSize::XXL] = PizzaName::XXL; 
  return (typeTab);
}

std::map<PizzaType::ePizzaType, std::string> typeTab = create_ptype();
std::map<PizzaSize::ePizzaSize, std::string> sizeTab = create_psize();

static bool		cmp_case_insen(char d_assault, char ade)
{
  if (tolower(d_assault) == tolower(ade))
    return (true);
  return (false);
}

PizzaType::ePizzaType  Plazza::PizzaFactory::get_typePizza(std::string const & str)
{   
  std::map<PizzaType::ePizzaType, std::string>::const_iterator it;
  for (it = typeTab.begin(); it != typeTab.end(); ++it)
    {
      if (std::equal(str.begin(),str.end(),(*it).second.begin(), &cmp_case_insen))
	  return ((*it).first);
    }
  return(PizzaType::Regina);
}

PizzaSize::ePizzaSize  Plazza::PizzaFactory::get_pizzaSize(std::string const & str)
{
  std::map<PizzaSize::ePizzaSize, std::string>::const_iterator it;
  for (it = sizeTab.begin(); it != sizeTab.end(); ++it)
    {
      if (std::equal(str.begin(), str.end(), (*it).second.begin(), &cmp_case_insen))
	return ((*it).first);
    }
  return(PizzaSize::S);
}

Plazza::Command	* Plazza::PizzaFactory::createPizza(std::string const & msg)
{
  int i = 0;
  PizzaType::ePizzaType typePizza;
  PizzaSize::ePizzaSize pizzaSize;
  std::list<Ingredient::eIngredient> ing;
  
  typePizza = get_typePizza(msg.substr(0, msg.find_first_of(' ')));
  pizzaSize = get_pizzaSize(msg.substr(msg.find_first_of(' ') + 1, msg.size() - (msg.find_first_of(' ') + 1)));  
  while (ptr[i])
    {
      if (i == typePizza)
	return (ptr[i](typePizza, pizzaSize));
      ++i;
    }
  return (NULL);
}

Plazza::Command * Plazza::PizzaFactory::regina(PizzaType::ePizzaType typePizza, PizzaSize::ePizzaSize size)
{
  //pate,tomate,gruyere, jambon, champignon
  std::list<Ingredient::eIngredient>ingrelol;
  ingrelol.push_back(Ingredient::pate);
  ingrelol.push_back(Ingredient::tomate);
  ingrelol.push_back(Ingredient::gruyere);
  ingrelol.push_back(Ingredient::jambon);
  ingrelol.push_back(Ingredient::champignon);
  Command *com = new Command(typePizza, size, ingrelol);
  return (com);
}

Plazza::Command * Plazza::PizzaFactory::margarita(PizzaType::ePizzaType typePizza, PizzaSize::ePizzaSize size)
{
  //pate,tomate,gruyere
  std::list<Ingredient::eIngredient>ingrelol;
  ingrelol.push_back(Ingredient::pate);
  ingrelol.push_back(Ingredient::tomate);
  ingrelol.push_back(Ingredient::gruyere);
  Command *com = new Command(typePizza, size, ingrelol);
  return (com);
}

Plazza::Command * Plazza::PizzaFactory::americaine(PizzaType::ePizzaType typePizza, PizzaSize::ePizzaSize size)
{
  //pate, tomate, gruyere, steack
  std::list<Ingredient::eIngredient>ingrelol;
  ingrelol.push_back(Ingredient::pate);
  ingrelol.push_back(Ingredient::tomate);
  ingrelol.push_back(Ingredient::gruyere);
  ingrelol.push_back(Ingredient::steack);
  Command *com = new Command(typePizza, size, ingrelol);
  return (com);
}

Plazza::Command	* Plazza::PizzaFactory::fantasia(PizzaType::ePizzaType typePizza, PizzaSize::ePizzaSize size)
{
  //pate, tomate, aubergine, chevre, amour
  std::list<Ingredient::eIngredient>ingrelol;
  ingrelol.push_back(Ingredient::pate);
  ingrelol.push_back(Ingredient::tomate);
  ingrelol.push_back(Ingredient::aubergine);
  ingrelol.push_back(Ingredient::chevre);
  ingrelol.push_back(Ingredient::amour);
  Command *com = new Command(typePizza, size, ingrelol);
  return (com);
}

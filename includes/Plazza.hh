
#ifndef PLAZZA__
#define PLAZZA__

#include <queue>
#include <list>
#include <unistd.h>
#include "Communicator.hh"
#include "KitchenState.hh"
#include "Gui.hh"

namespace PizzaType
{
  enum ePizzaType
    {
      Regina = 0,
      Margarita = 1,
      Americaine = 2,
      Fantasia = 3
    };
}

namespace PizzaSize
{
  enum ePizzaSize
    {
      S = 1,
      M = 2,
      L = 4,
      XL = 8,
      XXL = 16
    };
}

namespace PizzaRoastTime
{
  enum ePizzaRoastTime
    {
      Regina = 2,
      Margarita = 1,
      Americaine = 2,
      Fantasia = 4     
    };
}

namespace Ingredient
{
  enum eIngredient
    {
      pate = 0,
      tomate,
      gruyere,
      jambon,
      champignon,
      steack,
      aubergine,
      chevre,
      amour
    };
}

namespace PizzaName
{
  const std::string Regina = "regina";
  const std::string Margarita = "margarita";
  const std::string Americaine = "americaine";
  const std::string Fantasia = "fantasia";
  const std::string S = "s";
  const std::string M = "m";
  const std::string L = "l";
  const std::string XL = "xl";
  const std::string XXL = "xxl";
  const std::string One = "1";
  const std::string Two = "2";
  const std::string Four = "4";
  const std::string Height = "8";
  const std::string Sixteen = "16";
  const char Pipe = '|';
  const std::string FifoName = "fifoPlazza";
}

namespace Commands
{
  const std::string TryCmd = "c";
  const std::string OKCmd = "o";
  const std::string KOCmd = "k";
  const std::string StateCmd = "s";
  const std::string DieCmd = "d";
}

namespace ReadParam
{
  const int BUFSIZE = 4096;
  const char END_OF_CMD = '#';
}

namespace LexType
{
  enum eLexType
    {
      Type = 0,
      Size,
      Number,
      Separator
    };
}

namespace Plazza
{
  struct Lexeme
  {
    LexType::eLexType	type;
    std::string		value;
  };

  struct HomeCmd
  {
    std::string		pizzaType;
    std::string		pizzaSize;
  };
}

namespace Plazza
{
  class Home
  {
  private:
    std::list<Communicator *>	_communicators;
    float			_cookTimeMult;
    int				_nbCook;
    int				_reloadIngredientTime;
    Gui				_gui;

  public:
    Home(float cookTimeMult, int nbCook, int reloadIngredientTime);
    ~Home();
    Home(Home const & other);
    Home & operator=(Home const & other);
    int  start_plazza();

  private:
    void		waitZombie(pid_t pid);
    std::list<Lexeme> *	lexe_cmd(std::string const & cmd);
    void		parse_cmd(std::list<Lexeme> * lex_list, std::queue<Plazza::HomeCmd> & homeCmds);
    bool		throw_command(Plazza::HomeCmd command);
    KitchenState &	get_kitchen_state(int pendingsCommands);
    void		createKitchen();
  };
}

#endif

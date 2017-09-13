
#ifndef KITCHEN_HH_
# define KITCHEN_HH_

#include <time.h>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include "Thread.hpp"
#include "Plazza.hh"
#include "Command.hh"
#include "SafeMap.hpp"
#include "SafeQueue.hpp"

namespace Plazza
{
  class Kitchen
  {
  private:
    Communicator &				_communicator;
    std::vector<Thread<Kitchen *> >		_cook;
    Plazza::SafeQueue<Command *>		_commandList;
    int						_totalCmd;
    int						_regeneration;
    float					_cookTimeMult;
    SafeMap<Ingredient::eIngredient, int>	_bacIngredients;
    bool					_acceptCmd;
    bool					_im_dead;
    bool					_end;
    bool					_remind;
    Mutex					_poolMutex;
    CondVar					_poolVar;
    Mutex					_listMutex;
    Mutex					_ingMutex;
    CondVar					_ingVar;

  public:
    Kitchen(Communicator & com, float cookTimeMult, int nbCook, int reloadIngredientTime);
    ~Kitchen();
    Kitchen(Kitchen const & other);
    Kitchen & operator=(Kitchen const & other);
    void	startKitchen();
    void	setThreadState(pthread_t id, ThreadState::eThreadState state);

  private:
    void	*cookPizza(Command * commande);
    void	timer_de_oof();
    void	regenerate_ingredient();
    void	call_father();
    static void *	pool(Kitchen * thus);

  public:
    void	c_handling();
    void	s_handling();

  public:
    void	roast_regina();
    void	roast_margarita();
    void	roast_americaine();
    void	roast_fantasia();
  };

  struct	s_cmd_handling
  {
    std::string	s;
    void	(Kitchen::*pt)();
  };
}

#endif

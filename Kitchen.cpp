
#include <sstream>
#include "Kitchen.hh"
#include "PizzaFactory.hh"

Plazza::Kitchen::Kitchen(Communicator & com, float cookTimeMult, int nbCook, int reloadIngredientTime)
  : _communicator(com),
    _cook(),
    _commandList(),
    _totalCmd(0),
    _regeneration(reloadIngredientTime),
    _cookTimeMult(cookTimeMult),
    _bacIngredients(),
    _acceptCmd(false),
    _im_dead(false),
    _end(false),
    _remind(false),
    _poolMutex(),
    _poolVar(this->_poolMutex),
    _listMutex(),
    _ingMutex(),
    _ingVar(this->_ingMutex)
{
  this->_bacIngredients[Ingredient::pate] = 5;
  this->_bacIngredients[Ingredient::tomate] = 5;
  this->_bacIngredients[Ingredient::gruyere] = 5;
  this->_bacIngredients[Ingredient::jambon] = 5;
  this->_bacIngredients[Ingredient::champignon] = 5;
  this->_bacIngredients[Ingredient::steack] = 5;
  this->_bacIngredients[Ingredient::aubergine] = 5;
  this->_bacIngredients[Ingredient::chevre] = 5;
  this->_bacIngredients[Ingredient::amour] = 5;
  this->_cook.resize(nbCook, Plazza::Thread<Plazza::Kitchen *>(&Plazza::Kitchen::pool, this));
  for (int i = 0; i < nbCook; ++i)
    this->_cook[i].Run();  
}

Plazza::Kitchen::~Kitchen()
{
  this->_poolVar.broadcast();
  for (size_t i = 0; i < this->_cook.size(); ++i)
    {
      this->_cook[i].KillMe();
      this->_cook[i].WaitForEnd();
    }
  exit(EXIT_SUCCESS);
}

void	 Plazza::Kitchen::startKitchen()
{
  while (!this->_end)
    {
      this->call_father();
      if (!this->_commandList.empty())
      	{
	  this->_poolVar.signal();
	  this->_remind = false;
	}
      this->regenerate_ingredient();
      this->timer_de_oof();
    }
}

void *	Plazza::Kitchen::pool(Plazza::Kitchen * thus)
{
  struct timespec	t;
  
  t.tv_sec = 1;
  t.tv_nsec = 0;
  while (!thus->_end)
    {
      thus->setThreadState(pthread_self(), ThreadState::Waiting);
      thus->_poolVar.timedwait(&t);
      if (!thus->_commandList.empty())
	{
	  thus->cookPizza(thus->_commandList.pop());
	}
    }
  return (NULL);
}

void Plazza::Kitchen::roast_regina()
{
  sleep(PizzaRoastTime::Regina * this->_cookTimeMult);
}

void Plazza::Kitchen::roast_margarita()
{
  sleep(PizzaRoastTime::Margarita * this->_cookTimeMult);
}

void Plazza::Kitchen::roast_americaine()
{
  sleep(PizzaRoastTime::Americaine * this->_cookTimeMult);
}

void Plazza::Kitchen::roast_fantasia()
{
  sleep(PizzaRoastTime::Fantasia * this->_cookTimeMult);
}

static   void (Plazza::Kitchen::*roast_pizza[])() =
{
  &Plazza::Kitchen::roast_regina,
  &Plazza::Kitchen::roast_margarita,
  &Plazza::Kitchen::roast_americaine,
  &Plazza::Kitchen::roast_fantasia,
  0
};

void *	 Plazza::Kitchen::cookPizza(Command * commande)
{
  std::list<Ingredient::eIngredient>::const_iterator it;
  int	tab_idx = 0;
  int	enum_idx = 1;

  this->setThreadState(pthread_self(), ThreadState::Running);
  for (it = commande->getIngredient().begin(); it != commande->getIngredient().end(); ++it)
    {
      while (this->_bacIngredients[*it] == 0)
	this->_ingVar.wait();
      this->_bacIngredients[*it] -= 1;
    }
  while (roast_pizza[tab_idx])
    {
      if (enum_idx == commande->getType())
	(this->*roast_pizza[tab_idx])();
      ++tab_idx;
      enum_idx *= 2;
    }
  this->_totalCmd += 1;
  this->_remind = false;
  return (NULL);
}

void	 Plazza::Kitchen::timer_de_oof()
{
  unsigned int	 seconds;
  struct timeval end, start;

  if (this->_remind == false)
    gettimeofday(&start, NULL);
  gettimeofday(&end, NULL);
  seconds = end.tv_sec - start.tv_sec;
  if (seconds >= 5)
    this->_im_dead = true;
  this->_remind = true;
}

void	Plazza::Kitchen::regenerate_ingredient()
{
  SafeMap<Ingredient::eIngredient, int>::Iterator it;
  struct timeval end, start;
  static int	remind = 0;
  int	diff_time;

  if (remind == 0)
    gettimeofday(&start, NULL);
  gettimeofday(&end, NULL);
  if (start.tv_usec <= end.tv_usec)
    diff_time = end.tv_usec - start.tv_usec;
  else
    diff_time = (end.tv_usec + 1000) - start.tv_usec;
  if (diff_time > this->_regeneration)
    {
      std::map<Ingredient::eIngredient, int> & smap = this->_bacIngredients.getMap();
      for (it = smap.begin(); it != smap.end(); ++it)
	{
	  this->_bacIngredients.setValue(it->first, it->second + 1);
	}
      this->_ingVar.broadcast();
    }
}

static Plazza::s_cmd_handling	call[]=
  {
    {Commands::TryCmd, &Plazza::Kitchen::c_handling},
    {Commands::StateCmd, &Plazza::Kitchen::s_handling},
    {"", 0}
  };

void	Plazza::Kitchen::call_father()
{
  std::string *	msg;
  int		i = 0;
  
  msg = this->_communicator.receive();
  if (msg != NULL)
    {
      while (call[i].pt)
	{
	  if (call[i].s == *msg)
	    {
	      (this->*call[i].pt)();
	      break;
	    }
	  ++i;
	}
      if (!call[i].pt && this->_acceptCmd)
	{
	  this->_commandList.push(Plazza::PizzaFactory::createPizza(*msg));
	  this->_acceptCmd = false;
	  this->_remind = false;
	}
    }
}

void	Plazza::Kitchen::c_handling()
{
  if (this->_commandList.size() >= this->_cook.size())
    this->_communicator.send(Commands::KOCmd);
  else
    {
      this->_communicator.send(Commands::OKCmd);
      this->_acceptCmd = true;
    }
  this->_remind = false;
}

void	Plazza::Kitchen::s_handling()
{
  std::stringstream	ss;
  int		count = 0;

  if (this->_im_dead)
    {
      this->_communicator.send(Commands::DieCmd);
      this->_end = true;
    }
  else
    {
      for (std::vector<Thread<Kitchen *> >::const_iterator it = this->_cook.begin();
	   it != this->_cook.end(); ++it)
	{
	  if ((*it).GetState() == ThreadState::Running)
	    count++;
	}
      ss << count
	 << PizzaName::Pipe
	 << this->_commandList.size()
	 << PizzaName::Pipe
	 << this->_totalCmd;
      this->_communicator.send(ss.str());
    }
}

void	Plazza::Kitchen::setThreadState(pthread_t id, ThreadState::eThreadState state)
{
  for (std::vector<Thread<Kitchen *> >::iterator it = this->_cook.begin();
       it != this->_cook.end(); ++it)
    {
      if ((*it).GetId() == id)
	{
	  (*it).SetState(state);
	  break;
	}
    }
}

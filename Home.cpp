

#include <sstream>
#include <queue>
#include <list>
#include <sys/types.h>
#include <sys/wait.h>
#include "Plazza.hh"
#include "Kitchen.hh"
#include "PizzaFactory.hh"
#include "Err.hh"
#include "Communicator.hh"
#include "KitchenState.hh"

Plazza::Home::Home(float mult, int cook, int ingredient_time) :
  _cookTimeMult(mult),
  _nbCook(cook),
  _reloadIngredientTime(ingredient_time),
  _gui(cook)
{ }

Plazza::Home::~Home()
{ }

int Plazza::Home::start_plazza()
{
  int				err_code = 0;
  std::queue<Plazza::HomeCmd>	homeCmds;

  while (this->_gui.is_opened())
    {
      this->parse_cmd(this->lexe_cmd(this->_gui.getInput()), homeCmds);
      if (!homeCmds.empty())
	{
	  if (this->throw_command(homeCmds.front()))
	    homeCmds.pop();
	}
      this->_gui.display(this->get_kitchen_state(homeCmds.size()));
    }
  return (err_code);
}

void	Plazza::Home::waitZombie(pid_t pid)
{
  for (std::list<Communicator *>::iterator it = this->_communicators.begin();
       it != this->_communicators.end(); ++it)
    {
      if ((*it)->getPid() == pid)
	{
	  delete *it;
	  this->_communicators.erase(it);
	  break;
	}
    }
  waitpid(pid, NULL, 0);
}

static std::vector<std::string> create_sizes()
{
  std::vector<std::string>	si;

  si.push_back(PizzaName::S);
  si.push_back(PizzaName::M);
  si.push_back(PizzaName::L);
  si.push_back(PizzaName::XL);
  si.push_back(PizzaName::XXL);
  return (si);
}

static std::vector<std::string>	sizes = create_sizes();

static bool		cmp_case_insen(char d_assault, char ade)
{
  if (tolower(d_assault) == tolower(ade))
    return (true);
  return (false);
}

static bool		toktok_size(std::string::const_iterator &it, const std::string::const_iterator &it_end, std::list<Plazza::Lexeme> &lex)
{
  unsigned int		i = 0;
  Plazza::Lexeme	maillon;

  while (i < sizes.size())
    {
      if (it + sizes[i].size() < it_end && std::equal(it, it + sizes[i].size(), sizes[i].begin(), &cmp_case_insen))
	{
	  maillon.type = LexType::Size;
	  maillon.value.resize(sizes[i].size());
	  std::copy(it, it + sizes[i].size(), maillon.value.begin());
	  lex.push_back(maillon);
	  it += (sizes[i].size() - 1);
	  return (true);
	}
      ++i;
    }
  return (false);
}

static bool		toktok_number(std::string::const_iterator &it, const std::string::const_iterator &it_end, std::list<Plazza::Lexeme> &lex)
{
  int			i = 1;
  Plazza::Lexeme	maillon;

  if (tolower(*it) == 'x' && it + 1 != it_end && isdigit(*(it + 1)) && *(it + 1) != '0')
    {
      while (it + i < it_end && isdigit(*(it + i)))
	++i;
      maillon.type = LexType::Number;
      maillon.value.resize(i);
      std::copy(it, it + i, maillon.value.begin());
      lex.push_back(maillon);
      it += (i - 1);
      return (true);
    }
  return (false);  
}

static std::vector<std::string> create_pizzas()
{
  std::vector<std::string>	piz;

  piz.push_back(PizzaName::Regina);
  piz.push_back(PizzaName::Margarita);
  piz.push_back(PizzaName::Americaine);
  piz.push_back(PizzaName::Fantasia);
  return (piz);
}

static std::vector<std::string> pizzas = create_pizzas();

static bool	is_known_pizza(std::string const &pizzaName)
{
  for (unsigned int i = 0; i < pizzas.size(); ++i)
    {
      if (pizzas[i].size() == pizzaName.size()
	  && std::equal(pizzas[i].begin(),
			pizzas[i].end(),
			pizzaName.begin(),
			&cmp_case_insen))
	return (true);
    }
  return (false);
}

static bool		toktok_type(std::string::const_iterator &it, const std::string::const_iterator &it_end, std::list<Plazza::Lexeme> &lex)
{
  int			i = 1;
  Plazza::Lexeme	maillon;

  if (isalpha(*it))
    {
      while (it + i < it_end && isalpha(*(it + i)))
	++i;
      maillon.type = LexType::Type;
      maillon.value.resize(i);
      std::copy(it, it + i, maillon.value.begin());
      if (is_known_pizza(maillon.value))
	{ 
	  lex.push_back(maillon);
	  it += (i - 1);
	  return (true);
	}
    }
    return (false);
}

static bool		toktok_sep(std::string::const_iterator &it, const std::string::const_iterator &, std::list<Plazza::Lexeme> &lex)
{
  Plazza::Lexeme	maillon;

  if (*it == ';')
    {
      maillon.type = LexType::Separator;
      maillon.value = ";";
      lex.push_back(maillon);
      return (true);
    }
  return (false);  
}

static bool (*toktok[])(std::string::const_iterator &, const std::string::const_iterator &, std::list<Plazza::Lexeme> &)=
    {
      &toktok_type,
      &toktok_size,
      &toktok_number,
      &toktok_sep,
      0
    };

std::list<Plazza::Lexeme> *	Plazza::Home::lexe_cmd(std::string const & cmd)
{
  std::list<Plazza::Lexeme> *	lex = new std::list<Plazza::Lexeme>();
  int				i = 0;

  for (std::string::const_iterator it = cmd.begin(); it != cmd.end(); ++it)
    {
      if (*it != ' ' && *it != '\t')
	{
	  i = 0;
	  while (toktok[i])
	    {
	      if (toktok[i](it, cmd.end(), *lex))
		break;
	      ++i;
	    }
	  if (!toktok[i])
	    break;
	}
    }
  return (lex);
}

static bool	sep_rule(std::list<Plazza::Lexeme> * lex_list)
{
  if (lex_list->front().type == LexType::Separator)
    {
      lex_list->pop_front();
      return (true);
    }
  return (false);
}

static bool	cmd_rule(std::list<Plazza::Lexeme> * lex_list, std::queue<Plazza::HomeCmd> & homeCmds)
{
  std::list<Plazza::Lexeme>::const_iterator	it;
  std::list<Plazza::Lexeme>::const_iterator	end;
  std::stringstream				ss;
  unsigned int					nb;

  it = lex_list->begin();
  end = lex_list->end();
  if (it != end
      && (*it).type == LexType::Type
      && ++it != end
      && (*it).type == LexType::Size
      && ++it != end
      && (*it).type == LexType::Number)
    {
      ss << (*it).value.substr(1);
      ss >> nb;
      --it;
      while (nb > 0)
	{
	  homeCmds.push(Plazza::HomeCmd());
	  homeCmds.back().pizzaType = lex_list->front().value;
	  homeCmds.back().pizzaSize = (*it).value;
	  --nb;
	}
      lex_list->pop_front();
      lex_list->pop_front();
      lex_list->pop_front();
      return (true);
    }
  return (false);
}

void		Plazza::Home::parse_cmd(std::list<Plazza::Lexeme> * lex_list, std::queue<Plazza::HomeCmd> & homeCmds)
{
  bool		please_dont_stop_the_music = true;

  please_dont_stop_the_music = cmd_rule(lex_list, homeCmds);
  while (please_dont_stop_the_music && !lex_list->empty())
    {
      please_dont_stop_the_music = sep_rule(lex_list);
      if (please_dont_stop_the_music)
	please_dont_stop_the_music = cmd_rule(lex_list, homeCmds);
    }
  delete lex_list;
}

bool		Plazza::Home::throw_command(Plazza::HomeCmd command)
{
  std::string *	answer;
  std::string	cmd;
  bool		enough = false;

  cmd = command.pizzaType;
  cmd += " ";
  cmd += command.pizzaSize;
  for (std::list<Plazza::Communicator *>::iterator it = this->_communicators.begin();
       it != this->_communicators.end(); ++it)
    {
      (*it)->send(Commands::TryCmd);
      answer = (*it)->receive();
      if (*answer == Commands::OKCmd)
	{
	  (*it)->send(cmd);
	  enough = true;
	  break;
	}
    }
  if (!enough)
    {
      this->createKitchen();
      return (false);
    }
  return (true);
}

void			Plazza::Home::createKitchen()
{
  pid_t			pid;
  Plazza::Pipe2	*	pipe = new Plazza::Pipe2();

  if ((pid = fork()) == -1)
    Plazza::Err::show_err_msg(err_code::fork_err_code);
  if (pid == 0)
    {
      Plazza::Communicator	child(0, *pipe, CommunicatorType::ChildCom);
      Kitchen	kitch(child, this->_cookTimeMult, this->_nbCook, this->_reloadIngredientTime);

      kitch.startKitchen();
    }
  else
    {
      Plazza::Communicator *dad = new Plazza::Communicator(pid, *pipe, CommunicatorType::DadCom);
      this->_communicators.push_back(dad);
    }
}

Plazza::KitchenState &	Plazza::Home::get_kitchen_state(int pendingCommands)
{
  Plazza::KitchenState *	state = new Plazza::KitchenState();
  std::string *		answer;
  int			value;

  state->waitingTotal = 0;
  state->ongoingTotal = 0;
  state->totalTotal = 0;
  state->pendingCommands = pendingCommands;
  for (std::list<Plazza::Communicator *>::iterator it = this->_communicators.begin();
       it != this->_communicators.end(); ++it)
    {
      (*it)->send(Commands::StateCmd);
      while ((answer = (*it)->receive()) == NULL);
      if (*answer == Commands::DieCmd)
	{
	  this->waitZombie((*it)->getPid());
	  break;
	}
      else
	{
	  std::istringstream	ss(answer->substr(0, answer->find_first_of(PizzaName::Pipe)));
	  ss >> value;
	  state->ongoingKitchenCmds.push_back(value);
	  state->ongoingTotal += value;
	  std::istringstream	ss2(answer->substr(answer->find_first_of(PizzaName::Pipe) + 1, answer->find_last_of(PizzaName::Pipe) - (answer->find_first_of(PizzaName::Pipe) + 1)));
	  ss2 >> value;
	  state->waitingKitchenCmds.push_back(value);
	  state->waitingTotal += value;
	  std::istringstream	ss3(answer->substr(answer->find_last_of(PizzaName::Pipe) + 1));
	  ss3 >> value;
	  state->totalKitchenCmds.push_back(value);
	  state->totalTotal += value;
	}
    }
  return (*state);
}

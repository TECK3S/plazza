
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "Plazza.hh"
#include "Communicator.hh"

Plazza::Communicator::Communicator(pid_t pid, Pipe2 & pipes, CommunicatorType::eCommunicatorType com)
  : _pid(pid), _pipes(pipes)
{
  if (com == CommunicatorType::ChildCom)
    this->_pipes.do_child_com();
  else
    this->_pipes.do_dad_com();
}

Plazza::Communicator::~Communicator()
{ }

void Plazza::Communicator::send(std::string const &data)
{
  this->_pipes.send(this->serialize(data));
}

std::string * Plazza::Communicator::receive()
{
  std::string *	msg;

  msg = this->_pipes.receive();
  if (msg == NULL)
    return (NULL);
  return (this->deserialize(*msg));
}

static std::map<std::string, int> create_cor_serial()
{
  std::map<std::string, int>	cor;

  cor[PizzaName::Regina] = PizzaType::Regina;
  cor[PizzaName::Margarita] = PizzaType::Margarita;
  cor[PizzaName::Americaine] = PizzaType::Americaine;
  cor[PizzaName::Fantasia] = PizzaType::Fantasia;
  cor[PizzaName::S] = PizzaSize::S;
  cor[PizzaName::M] = PizzaSize::M;
  cor[PizzaName::L] = PizzaSize::L;
  cor[PizzaName::XL] = PizzaSize::XL;
  cor[PizzaName::XXL] = PizzaSize::XXL;
  return (cor);
}

static std::map<std::string, int> cor_serial = create_cor_serial();

std::string & Plazza::Communicator::serialize(std::string const& msg_to_serialize)
{
  std::stringstream	ss(std::stringstream::in);
  std::string *		res;

  //ss << cor_serial[pizzaType];
  ss << PizzaName::Pipe;
  //ss << cor_serial[pizzaSize];
  res = new std::string(msg_to_serialize);
  return (*res);
}

static std::map<std::string, std::string> create_cor_deserial_name()
{
  std::map<std::string, std::string>	cor;

  cor[PizzaName::One] = PizzaName::Regina;
  cor[PizzaName::Two] = PizzaName::Margarita;
  cor[PizzaName::Four] = PizzaName::Americaine;
  cor[PizzaName::Height] = PizzaName::Fantasia;
  return (cor);
}

static std::map<std::string, std::string> cor_deserial_name = create_cor_deserial_name();

static std::map<std::string, std::string> create_cor_deserial_size()
{
  std::map<std::string, std::string>	cor;

  cor[PizzaName::One] = PizzaName::S;
  cor[PizzaName::Two] = PizzaName::M;
  cor[PizzaName::Four] = PizzaName::L;
  cor[PizzaName::Height] = PizzaName::XL;
  cor[PizzaName::Sixteen] = PizzaName::XXL;
  return (cor);
}

static std::map<std::string, std::string> cor_deserial_size = create_cor_deserial_size();

std::string * Plazza::Communicator::deserialize(std::string const& msg_to_deserialize)
{
  std::string *	res = new std::string(msg_to_deserialize);

  /*  *res += cor_deserial_name[msg_to_deserialize.substr(0, msg_to_deserialize.find(PizzaName::Pipe))];
  *res += ' ';
  *res += cor_deserial_size[msg_to_deserialize.substr(msg_to_deserialize.find(PizzaName::Pipe) + 1)];*/
  return (res);
}

pid_t	Plazza::Communicator::getPid() const
{
  return (this->_pid);
}

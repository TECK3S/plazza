
#ifndef COMMUNICATOR__
#define COMMUNICATOR__

#include <string>
#include <sys/types.h>
#include "Pipe2.hh"

namespace CommunicatorType
{
  enum eCommunicatorType
    {
      ChildCom = 0,
      DadCom
    };
}

namespace Plazza
{
  class Communicator
  {
  private:
    pid_t		_pid;
    Pipe2 &		_pipes;
    
  public:
    Communicator(pid_t pid, Pipe2 & pipes, CommunicatorType::eCommunicatorType com);
    ~Communicator();
    Communicator(Communicator const & other);
    Communicator & operator=(Communicator const & other);
    void send(std::string const &data);
    std::string * receive();
    pid_t	getPid() const;

  private:
    std::string & serialize(std::string const& msg_to_serialize);
    std::string * deserialize(std::string const& msg_to_deserialize);
  };
}

#endif


#ifndef PIPE2__
#define PIPE2__

#include <string>

namespace Plazza
{
  class Pipe2
  {
  private:
    int		in;
    int		out;
    std::string	_fifo1;
    std::string	_fifo2;

  public:
    static int	fifoNum;

  public:
    Pipe2();
    ~Pipe2();
    Pipe2(Pipe2 const &other);
    Pipe2 & operator=(Pipe2 const & other);

  public:
    void  send(std::string & msg);
    std::string * receive();
    void	do_child_com();
    void	do_dad_com();
  };
}

#endif

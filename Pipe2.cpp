
#include <unistd.h>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "Pipe2.hh"
#include "Err.hh"
#include "Plazza.hh"

int Plazza::Pipe2::fifoNum = 0;

Plazza::Pipe2::Pipe2()
{
  std::stringstream	ss, ssd;

  ss << PizzaName::FifoName << Plazza::Pipe2::fifoNum;
  if (mkfifo(ss.str().c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1)
    Plazza::Err::show_err_msg(err_code::pipe_err_code);
  this->_fifo1 = ss.str();
  Plazza::Pipe2::fifoNum = Plazza::Pipe2::fifoNum + 1;
  ssd << PizzaName::FifoName << Plazza::Pipe2::fifoNum;
  if (mkfifo(ssd.str().c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1)
    Plazza::Err::show_err_msg(err_code::pipe_err_code);
  this->_fifo2 = ssd.str();
  Plazza::Pipe2::fifoNum = Plazza::Pipe2::fifoNum + 1;
}

Plazza::Pipe2::~Pipe2()
{
  close(this->in);
  close(this->out);
}

void  Plazza::Pipe2::send(std::string & msg)
{
  int	w;

  msg.push_back(ReadParam::END_OF_CMD);
  if ((w = write(this->out, msg.c_str(), msg.size())) < 0)
    Plazza::Err::show_err_msg(err_code::write_err_code);
}

std::string * Plazza::Pipe2::receive()
{
  static char	rbuf[ReadParam::BUFSIZE];
  static int	buf_end = 0;
  int		b_r = 0;
  int		i = 0;
  std::string *	msg = NULL;

  b_r = read(this->in, &(rbuf[buf_end]), ReadParam::BUFSIZE - buf_end);
  if (b_r > 0)
    buf_end += b_r;
  while (i < buf_end)
    {
      if (rbuf[i] == ReadParam::END_OF_CMD)
	{
	  msg = new std::string();
	  msg->assign(rbuf, i);
	  memmove(rbuf, &(rbuf[i + 1]), buf_end - 1 - i);
	  buf_end -= (i + 1);
	  break;
	}
      ++i;
    }
  return (msg);
}

void	Plazza::Pipe2::do_child_com()
{
  int	in0 = 0;
  int	out0 = 0;

  if ((in0 = open(this->_fifo1.c_str(), O_RDONLY)) == -1)
    Plazza::Err::show_err_msg(err_code::pipe_err_code);
  if ((out0 = open(this->_fifo2.c_str(), O_WRONLY)) == -1)
    Plazza::Err::show_err_msg(err_code::pipe_err_code);
  this->in = in0;
  fcntl(this->in, F_SETFL, O_NONBLOCK);
  this->out = out0;
}

void	Plazza::Pipe2::do_dad_com()
{
  int	in0 = 0;
  int	out0 = 0;

  if ((out0 = open(this->_fifo1.c_str(), O_WRONLY)) == -1)
    Plazza::Err::show_err_msg(err_code::pipe_err_code);
  if ((in0 = open(this->_fifo2.c_str(), O_RDONLY)) == -1)
    Plazza::Err::show_err_msg(err_code::pipe_err_code);
  this->in = in0;
  this->out = out0;
}

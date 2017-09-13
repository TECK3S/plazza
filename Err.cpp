
#include <iostream>
#include "Plazza.hh"
#include "Err.hh"

static std::vector<std::string> create_code()
{
  std::vector<std::string>	err_code;

  err_code.push_back(err_msg::nothing_msg);
  err_code.push_back(err_msg::usage_msg);
  err_code.push_back(err_msg::pipe_error_msg);
  err_code.push_back(err_msg::open_error_msg);
  err_code.push_back(err_msg::fork_error_msg);
  err_code.push_back(err_msg::write_error_msg);
  return (err_code);
}

std::vector<std::string> Plazza::Err::err_code = create_code();

void	Plazza::Err::show_err_msg(int err)
{
  std::cerr << Plazza::Err::err_code.at(err) << std::endl;
}

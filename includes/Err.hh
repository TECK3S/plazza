
#ifndef ERR__
#define ERR__

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

namespace err_code
{
  const int usage = 1;
  const int pipe_err_code = 2;
  const int open_err_code = 3;
  const int fork_err_code = 4;
  const int write_err_code = 5;
};

namespace err_msg
{
  const std::string nothing_msg = "";
  const std::string usage_msg   = "Usage: ./plazza \"multiplicator\" \"nb_cook\" \"ingredient_time\"";
  const std::string pipe_error_msg = "Pipe already exist";
  const std::string open_error_msg = "Cannot open the file";
  const std::string fork_error_msg = "Cannot fork";
  const std::string write_error_msg = "Cannot write";
}

namespace Plazza
{
  class Err
  {
  private:
    static std::vector<std::string> err_code;

  public:
    Err();
    ~Err();
    Err(Err const &other);
    Err & operator=(Err const &other);

  public:
    static void  show_err_msg(int err);
  };
}

#endif

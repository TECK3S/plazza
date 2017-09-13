
#include <iostream>
#include <sstream>
#include <string>
#include "Communicator.hh"
#include "Command.hh"
#include "Err.hh"

int main(int ac, char **av)
{
  int err = 0;
  int mult, cook, ingredient_time;
  QApplication	app(ac, av);

  if (ac == 4)
    {
      std::istringstream m(av[1]), c(av[2]), i(av[3]);
      m >> mult;
      c >> cook;
      i >> ingredient_time;
      Plazza::Home canteen(mult, cook, ingredient_time);
      err = canteen.start_plazza();
    }
  else
    err = err_code::usage;
  Plazza::Err::show_err_msg(err);
  return (err);
}

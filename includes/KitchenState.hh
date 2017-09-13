
#ifndef KITCHEN_STATE_HH_
#define KITCHEN_STATE_HH_

#include <list>

namespace Plazza
{
  struct KitchenState
  {
    std::list<int>	waitingKitchenCmds;
    std::list<int>	ongoingKitchenCmds;
    std::list<int>	totalKitchenCmds;
    int			waitingTotal;
    int			ongoingTotal;
    int			totalTotal;
    int			pendingCommands;
  };
}

#endif

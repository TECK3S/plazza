
#ifndef CONDVAR__
# define CONDVAR__

#include <pthread.h>
#include "Mutex.hh"

class CondVar 
{
public:
  CondVar(Mutex & Mu);
  ~CondVar();
  CondVar(const CondVar & other);
  CondVar & operator=(const CondVar & other);

public:
  void wait(void);
  void timedwait(const struct timespec * t);
  void signal(void);
  void broadcast(void);

private:
  pthread_cond_t 	_cond;
  Mutex &      		_mu;
};

#endif /* !CONDVAR__ */

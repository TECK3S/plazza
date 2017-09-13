
#ifndef SCOPEDLOCK__
# define SCOPEDLOCK__

#include "Mutex.hh"

class ScopedLock
{
public:
  ScopedLock(Mutex & mutex);
  ~ScopedLock();
 
public:
  void trylock();

private:
  ScopedLock(const ScopedLock & other);
  ScopedLock & operator=(const ScopedLock & other);
  
  Mutex &	_mutex;
};

# endif /* !SCOPEDLOCK */

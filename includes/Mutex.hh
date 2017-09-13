
#ifndef MUTEX__
# define MUTEX__

#include <pthread.h>

class Mutex
{
public:
  Mutex();
  ~Mutex(void);
  
  bool			lock(void);
  bool			unlock(void);
  bool			trylock(void);
  int			getMutexCtr() const;
  pthread_mutex_t &	getMutex();
  
public:
  Mutex(const Mutex & other);
  Mutex & operator=(const Mutex & other);

private:
  pthread_mutex_t	_mutex;
  int			_mutex_ctr;

};

# endif /* !MUTEX__ */

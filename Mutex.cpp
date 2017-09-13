
#include <stdlib.h>
#include "Mutex.hh"
#include "Err.hh"

Mutex::Mutex() : _mutex(), _mutex_ctr(0)
{
  pthread_mutex_init(&(this->_mutex), NULL);
}

Mutex::~Mutex()
{
  this->_mutex_ctr = 0;
  pthread_mutex_destroy(&(this->_mutex));
}
		
Mutex::Mutex(const Mutex & other) : _mutex(other._mutex), _mutex_ctr(other._mutex_ctr) {}

Mutex & Mutex::operator=(const Mutex & other)
{
  this->_mutex_ctr = other._mutex_ctr;
  if (this != &other)
    {
      pthread_mutex_destroy(&(this->_mutex));
      this->_mutex = other._mutex;
    }
  return (*this);
}

int		Mutex::getMutexCtr() const
{
  return (this->_mutex_ctr);
}

pthread_mutex_t	&	Mutex::getMutex()
{
  return (this->_mutex);
}

bool			Mutex::lock(void)
{
  if (pthread_mutex_lock(&(this->_mutex)))
    {
      Plazza::Err::show_err_msg(1);
      return (false);
    }
  this->_mutex_ctr += 1;
  return (true);
}

bool		Mutex::unlock(void)
{
  if (pthread_mutex_unlock(&(this->_mutex)))
    {
      Plazza::Err::show_err_msg(2);
      return (false);
    }
  return (true);
}
 
bool		Mutex::trylock(void)
{
  if (!pthread_mutex_trylock(&(this->_mutex)))
    return (true);
  return (false);
}

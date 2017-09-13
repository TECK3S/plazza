
#include "ScopedLock.hh"


ScopedLock::ScopedLock(Mutex & mutex) : _mutex(mutex)
{
  this->_mutex.lock();
}

ScopedLock::~ScopedLock()
{
  this->_mutex.unlock();
}


ScopedLock::ScopedLock(const ScopedLock & other) : _mutex(other._mutex) {}

ScopedLock & ScopedLock::operator=(const ScopedLock & other)
{
  if (this != &other)
    this->_mutex = other._mutex;
  return (*this);
}
  
void	ScopedLock::trylock()
{
  this->_mutex.trylock();
}

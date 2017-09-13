
#include "CondVar.hh"
#include "ScopedLock.hh"

CondVar::CondVar(Mutex & Mu) : _cond(), _mu(Mu)
{
  pthread_cond_init(&(this->_cond), NULL);
}

CondVar::~CondVar()
{
  pthread_cond_destroy(&(this->_cond));
}

CondVar::CondVar(const CondVar & other) : _cond(other._cond), _mu(other._mu)
{ }

CondVar & CondVar::operator=(const CondVar & other)
{
   if (this != &other)
     {
       this->_cond = other._cond;
     }
   return (*this);
}

void	CondVar::wait(void)
{
  pthread_cond_wait(&(this->_cond), &(this->_mu.getMutex()));
}

void CondVar::timedwait(const struct timespec * t)
{
  pthread_cond_timedwait(&(this->_cond), &(this->_mu.getMutex()), t);
}

void CondVar::signal(void)
{
  pthread_cond_signal(&(this->_cond));
}

void CondVar::broadcast(void)
{
  pthread_cond_broadcast(&(this->_cond));
}

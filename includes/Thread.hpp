
#ifndef THREAD_HPP_
# define THREAD_HPP_

#include <pthread.h>
#include <signal.h>

namespace ThreadState
{
  enum eThreadState
    {
      Created = 0,
      Running,
      Waiting,
      Dead
    };
}

namespace Plazza
{
  template<typename T = void *, typename U = void *>
  class Thread
  {
  private:
    pthread_t			_thread;
    ThreadState::eThreadState	_state;
    U				(*_start_routine)(T);
    T				_param;

  public:
    Thread(U (*start_routine)(T), T);
    ~Thread();
    Thread(Thread const & other);
    Thread & operator=(Thread const & other);

  public:
    bool			Run();
    U				WaitForEnd();
    void			KillMe();
    ThreadState::eThreadState	GetState() const;
    pthread_t			GetId() const;
    void			SetState(ThreadState::eThreadState);
    
  public:
    void				_runRoutine();
  };
}

template<typename T, typename U>
Plazza::Thread<T, U>::Thread(U (*start_routine)(T), T param)
  : _thread(), _state(ThreadState::Created), _start_routine(start_routine), _param(param)
{ }

template<typename T, typename U>
Plazza::Thread<T, U>::~Thread()
{ }

template<typename T, typename U>
Plazza::Thread<T, U>::Thread(Thread const & other)
  : _thread(), _state(other._state), _start_routine(other._start_routine), _param(other._param)
{ }

template<typename T, typename U>
Plazza::Thread<T, U> & Plazza::Thread<T, U>::operator=(Plazza::Thread<T, U> const & other)
{
  if (this != &other)
    {
      this->_thread = pthread_t();
      this->_state = other._state;
      this->_start_routine = other._start_routine;
      this->_param = other._param;
    }
  return (*this);
}

#pragma GCC diagnostic ignored "-Wreturn-type"
template<typename T, typename U>
static void *	my_start_routine(void * p)
{
  reinterpret_cast<Plazza::Thread<T, U> *>(p)->_runRoutine();
}
#pragma GCC diagnostic warning "-Wreturn-type"

template<typename T, typename U>
bool	Plazza::Thread<T, U>::Run()
{
  if (pthread_create(&this->_thread, NULL, &my_start_routine<T, U>, this) == 0)
    {
      this->_state = ThreadState::Running;
      return (true);
    }
  return (false);
}

template<typename T, typename U>
void	Plazza::Thread<T, U>::_runRoutine()
{
  this->_start_routine(this->_param);
}

template<typename T, typename U>
U	Plazza::Thread<T, U>::WaitForEnd()
{
  void *	r;

  pthread_join(this->_thread, &r);
  this->_state = ThreadState::Dead;
  return (static_cast<U>(r));
}

template<typename T, typename U>
void	Plazza::Thread<T, U>::KillMe()
{
  pthread_kill(this->_thread, SIGUSR1);
}

template<typename T, typename U>
ThreadState::eThreadState Plazza::Thread<T, U>::GetState() const
{
  return (this->_state);
}

template<typename T, typename U>
pthread_t	Plazza::Thread<T, U>::GetId() const
{
  return (this->_thread);
}

template<typename T, typename U>
void	Plazza::Thread<T, U>::SetState(ThreadState::eThreadState state)
{
  this->_state = state;
}

#endif

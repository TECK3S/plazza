
#ifndef SAFEQUEUE_HPP_
# define SAFEQUEUE_HPP_

#include <queue>
#include <string>
#include "Mutex.hh"
#include "CondVar.hh"
#include "ScopedLock.hh"

namespace Plazza
{		
  template<typename T>
  class SafeQueue
  {
  private:
    std::queue<T>		_queue;
    Mutex			_mumut;
    bool			_finished;
    CondVar			_cond;

  public:
    SafeQueue();
    ~SafeQueue();
    SafeQueue(SafeQueue const & other);
    SafeQueue & operator=(SafeQueue const & other);
    
  public:
    void		push(T value);
    T			pop();
    bool		tryPop(T * value);
    bool		isFinished();
    void		setFinished();
    size_t		size();
    bool		empty();

    class SafeQueueException : public std::exception
    {
    private:
      std::string	_err_msg;
      
    public:
      SafeQueueException() throw();
      ~SafeQueueException() throw();
      const char * what() const throw();
    };
  };
}

template<typename T>
Plazza::SafeQueue<T>::SafeQueue()
  : _queue(), _mumut(), _finished(false), _cond(this->_mumut)
{ }

template<typename T>
Plazza::SafeQueue<T>::~SafeQueue()
{ }

template<typename T>
void Plazza::SafeQueue<T>::push(T value)
{
  ScopedLock	locking(this->_mumut);

  if (!this->_finished)
    {
      this->_queue.push(value);
      this->_cond.signal();
    }
}

template<typename T>
T Plazza::SafeQueue<T>::pop()
{
  ScopedLock	locking(this->_mumut);
  T		value;

  if (this->_finished && this->_queue.empty())
    throw Plazza::SafeQueue<T>::SafeQueueException();
  if (this->_queue.empty())
    this->_cond.wait();
  if (this->_finished && this->_queue.empty())
    throw Plazza::SafeQueue<T>::SafeQueueException();
  value = this->_queue.front();
  this->_queue.pop();
  if (!this->_queue.empty())
    this->_cond.signal();
  return (value);
}

template<typename T>
bool Plazza::SafeQueue<T>::tryPop(T * value)
{
  ScopedLock	locking(this->_mumut);

  if (this->_queue.empty())
    return (false);
  if (value != NULL)
    *value = this->_queue.front();
  this->_queue.pop();
  return (true);
}

template<typename T>
bool Plazza::SafeQueue<T>::isFinished()
{
  ScopedLock	locking(this->_mumut);

  if (this->_finished && this->_queue.empty())
    return (true);
  return (false);
}

template<typename T>
void Plazza::SafeQueue<T>::setFinished()
{
  ScopedLock	(this->_mumut);

  this->_finished = true;
  this->_cond.broadcast();
}

template<typename T>
size_t	Plazza::SafeQueue<T>::size()
{
  ScopedLock	(this->_mumut);

  return(this->_queue.size());
}

template<typename T>
bool	Plazza::SafeQueue<T>::empty()
{
  ScopedLock	(this->_mumut);

  return(this->_queue.empty());
}

static const std::string	SAFE_QUEUE_ERR_MSG("Queue will not be pushed anymore.");

template<typename T>
Plazza::SafeQueue<T>::SafeQueueException::SafeQueueException() throw()
  : _err_msg(SAFE_QUEUE_ERR_MSG)
{ }

template<typename T>
Plazza::SafeQueue<T>::SafeQueueException::~SafeQueueException() throw()
{ }

template<typename T>
const char * Plazza::SafeQueue<T>::SafeQueueException::what() const throw()
{
  return (this->_err_msg.c_str());
}

#endif

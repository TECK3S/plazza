
#ifndef SAFEMAP__
# define SAFEMAP__

#include <map>
#include <utility>
#include <exception>
#include <stdexcept>
#include <iterator>
#include <string>
#include "Mutex.hh"
#include "CondVar.hh"
#include "ScopedLock.hh"

namespace Plazza
{		
  template<typename T, typename U>
  class SafeMap
  {
  public:
    typedef typename std::map<T, U>::iterator Iterator;
    typedef typename std::map<T, U>::const_iterator const_Iterator;
    
  private:
    std::map<T, U>		_map;
    Mutex			_mutex;
    bool			_finished;
    CondVar			_cond;
   
  public:
    SafeMap();
    ~SafeMap();
    SafeMap(SafeMap const & other);
    SafeMap & operator=(SafeMap const & other);
    
  public:
    bool				empty() const;
    size_t				size() const;
    size_t				max_size () const;
    U &					operator[](const T & key);
    std::pair<Iterator, bool>		insert(const std::pair<T, U> & value);
    void				erase(Iterator position);
    size_t				erase(const T & key);
    void				erase(Iterator first, Iterator last);
    void				clear();
    void				setValue(const T & key, const U & value);
    std::map<T, U> &			getMap();

    class SafeMapException : public std::exception
    {
    private:
      std::string	_err_msg;
      
    public:
      SafeMapException() throw();
      ~SafeMapException() throw();
      const char * what() const throw();
    };
  };
}

template<typename T, typename U>
Plazza::SafeMap<T, U>::SafeMap()
  : _map(), _mutex(), _finished(false), _cond(this->_mutex)
{ }

template<typename T, typename U>
Plazza::SafeMap<T, U>::~SafeMap()
{ }

template<typename T, typename U>
bool Plazza::SafeMap<T, U>::empty() const
{
  ScopedLock	locking(this->_mutex);

  return (this->_map.empty());
}

template<typename T, typename U>
size_t Plazza::SafeMap<T, U>::size() const
{
  ScopedLock	locking(this->_mutex);

  return (this->_map.size());
}

template<typename T, typename U>
size_t Plazza::SafeMap<T, U>::max_size() const
{
  ScopedLock	locking(this->_mutex);

  return (this->_map.max_size());
}

template<typename T, typename U>
std::pair<typename std::map<T, U>::iterator, bool>	Plazza::SafeMap<T, U>::insert(const std::pair<T, U> & value)
{
  ScopedLock	locking(this->_mutex);
  
  if ((this->_map.insert(value)).second == false)
    throw SafeMap::SafeMapException();
  return (this->_map.insert(value));
}

template<typename T, typename U>
U &	Plazza::SafeMap<T, U>::operator[](const T & value)
{
  ScopedLock	locking(this->_mutex);
  
  return (this->_map[value]);
}

template<typename T, typename U>
void	Plazza::SafeMap<T, U>::erase(Iterator position)
{
  ScopedLock	locking(this->_mutex);

  this->_map.erase(position);
}

template<typename T, typename U>
size_t	Plazza::SafeMap<T, U>::erase(const T & key)
{
  ScopedLock	locking(this->_mutex);

  return (this->_map.erase(key));
}
 
template<typename T, typename U>
void	Plazza::SafeMap<T, U>::erase(typename std::map<T, U>::iterator first, typename std::map<T, U>::iterator last)
{
  ScopedLock	locking(this->_mutex);
  
  this->_map.erase(first, last);
}

template<typename T, typename U>
void	Plazza::SafeMap<T, U>::clear()
{
  ScopedLock	locking(this->_mutex);

  this->_map.clear();
}

template<typename T, typename U>
void	 Plazza::SafeMap<T, U>::setValue(const T & key, const U & value)
{
  ScopedLock	locking(this->_mutex);

  if (value < 0)
    throw SafeMap::SafeMapException(); 
  this->_map[key] = value;
}

static const std::string	SAFE_MAP_ERR_MSG("Wrong map value or trying to insert an element already in the map!");

template<typename T, typename U>
Plazza::SafeMap<T, U>::SafeMapException::SafeMapException() throw()
  : _err_msg(SAFE_MAP_ERR_MSG)
{ }

template<typename T, typename U>
Plazza::SafeMap<T, U>::SafeMapException::~SafeMapException() throw()
{ }

template<typename T, typename U>
const char * Plazza::SafeMap<T, U>::SafeMapException::what() const throw()
{
  return (this->_err_msg.c_str());
}

template<typename T, typename U>
std::map<T, U> &		Plazza::SafeMap<T, U>::getMap()
{
  ScopedLock	locking(this->_mutex);
  return (this->_map);
}

#endif /* !SAFEMAP__ */

#ifndef _BIGRAM_H_
#define _BIGRAM_H_

#include <string>
#include <sstream>
#include <boost/functional/hash/hash.hpp>

class Bigram
{
public:
  std::string first;
  std::string second;
  Bigram(std::string s1, std::string s2); 
  const std::string to_string() const;	  
  std::size_t hash_value() const;	  
  bool equals(const Bigram& other) const; 
};

inline Bigram::Bigram(std::string s1, std::string s2)
{
  first = s1;
  second = s2;
}

inline const std::string Bigram::to_string() const 
{
  std::stringstream ss;
  ss << "(" << first << " | " << second << ")";
  return ss.str();
}

inline std::size_t Bigram::hash_value() const
{
  std::size_t seed = 0;
  boost::hash_combine(seed, first);
  boost::hash_combine(seed, second);
  return seed;
}

inline bool Bigram::equals(const Bigram& other) const
{
  return ((other.first == first) && (other.second == second));
}

/* hashing functor */
struct bigram_hash {
  std::size_t operator()(const Bigram& b) const {
    return b.hash_value();
  }
};

/* equality functor */
struct bigram_equals {
  bool operator()(const Bigram& b1, const Bigram& b2) const {
    return b1.equals(b2);
  }
};


#endif /* _BIGRAM_H_ */

#ifndef _BIGRAMMODEL_H_
#define _BIGRAMMODEL_H_

#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/unordered_map.hpp>
#include <boost/tokenizer.hpp>
#include "bigram.hpp"

/* k for good turing smoothing */
const int k = 5; 

typedef boost::unordered_map<Bigram, int, bigram_hash, bigram_equals> bigram_map;
typedef boost::unordered_map<std::string, int> unigram_map;

void read_corpus(std::vector< std::vector<std::string> > &corpus, std::string filename);

class BigramModel
{
private:
  int bins[k+2];
  int total_count;
  void build_bins();
public:
  bigram_map bm;
  unigram_map um;
  BigramModel();
  void process_corpus(std::string filename);
  void process_corpus(std::vector< std::vector<std::string> > &corpus);
  double rfe_smoothed(std::string first, std::string second);
  double rfe_smoothed(Bigram b);
  double surprisal(std::string first, std::string second);
  double surprisal(Bigram b);
  double rfe_unsmoothed(std::string first, std::string second);
  double rfe_unsmoothed(Bigram b);
  double seq_prob_smoothed(std::vector<std::string>& seq);
  double seq_surprisal(std::vector<std::string>& seq);
  double seq_prob_unsmoothed(std::vector<std::string>& seq);
  void print_bm();
  void save(std::string filename);
  void load(std::string filename);
};

inline BigramModel::BigramModel()
{
  total_count = 0;
}

inline void BigramModel::process_corpus(std::string filename)
{
  std::vector< std::vector<std::string> > corpus;
  read_corpus(corpus, filename);
  process_corpus(corpus);
}

/* calculate unsmoothed relative frequence estimate of (second | first) */
inline double BigramModel::rfe_unsmoothed(std::string first, std::string second)
{
  Bigram b(first, second);
  return rfe_unsmoothed(b);
}

inline double BigramModel::rfe_unsmoothed(Bigram b)
{
  return static_cast<double>(bm[b]) / um[b.first];
}

inline double BigramModel::surprisal(std::string first, std::string second)
{
  Bigram b(first, second);
  return surprisal(b);
}

inline double BigramModel::surprisal(Bigram b)
{
  return -1. * log2(rfe_smoothed(b));
}

inline double BigramModel::rfe_smoothed(std::string first, std::string second)
{
  Bigram b(first, second);
  return rfe_smoothed(b);
}

inline double BigramModel::seq_surprisal(std::vector<std::string> &seq)
{
  double s = 0.;
  for(int i = 1; i <seq.size(); ++i)
    {
      s += surprisal(seq[i-1], seq[i]);
    }
  return s;
}

inline double BigramModel::rfe_smoothed(Bigram b)
{
  int count = bm[b];
  double rfe;
  if (count <= k) {
    rfe =  ((count + 1) * (static_cast<double>(bins[count+1]) / bins[count])) / total_count;
  } else {
    rfe = static_cast<double>(count) / um[b.first];
  }
  return rfe;
}

inline double BigramModel::seq_prob_smoothed(std::vector<std::string> &seq)
{
  double prob = 1.;
  for (int i = 1; i < seq.size(); ++i)
    {
      prob *= rfe_smoothed(seq[i-1], seq[i]);
    }
  return prob;
}

inline double BigramModel::seq_prob_unsmoothed(std::vector<std::string> &seq)
{
  double prob = 1.;
  for (int i = 1; i < seq.size(); ++i)
    {
      prob *= rfe_unsmoothed(seq[i-1], seq[i]);
    }
  return prob;
}
#endif /* _BIGRAMMODEL_H_ */

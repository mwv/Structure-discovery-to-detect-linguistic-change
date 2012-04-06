#include <cmath>
#include <iostream>
#include <string>
#include <boost/unordered_set.hpp>
#include "bigram.hpp"
#include "bigrammodel.hpp"
using namespace std;

typedef boost::unordered_set<Bigram, bigram_hash, bigram_equals> bigram_set;

void analyze(const string, const string, const string);
inline double kl(vector<double>&, vector<double>&);
inline double skl(vector<double>&, vector<double>&);

void distribution_analysis(bigram_set &merged_set, 
			   BigramModel &bmA, 
			   BigramModel &bmB, 
			   BigramModel &bmC);

void surprisal_analysis(BigramModel &bmA, 
			BigramModel &bmB, 
			BigramModel &bmC,
			vector< vector<string> > &corpusA,
			vector< vector<string> > &corpusB,
			vector< vector<string> > &corpusC);

int main(int argc, char *argv[])
{
  if (argc != 4) {
    cout << "bigram_analysis: analyze distance between corpora through bigram language models" << endl;
    cout << "usage:\n\t[1] corpus file 1\n\t[2] corpus file 2\n\t[3] corpus file 3\n\n";
  } else {
    analyze(argv[1], argv[2], argv[3]);
  }
  return 0;
}

void analyze(const string corpusfileA, const string corpusfileB, const string corpusfileC)
{

  vector< vector<string> > corpusA;
  read_corpus(corpusA, corpusfileA);
  vector< vector<string> > corpusB;
  read_corpus(corpusB, corpusfileB);
  vector< vector<string> > corpusC;
  read_corpus(corpusC, corpusfileC);

  BigramModel bmA;
  bmA.process_corpus(corpusA);
  BigramModel bmB;
  bmB.process_corpus(corpusB);
  BigramModel bmC;
  bmC.process_corpus(corpusC);

  bigram_set bs;
  for(bigram_map::iterator itA = bmA.bm.begin(); itA != bmA.bm.end(); ++itA) {
    bs.insert((itA -> first));
  }
  for(bigram_map::iterator itB = bmB.bm.begin(); itB != bmB.bm.end(); ++itB) {
    bs.insert((itB -> first));
  }
  for(bigram_map::iterator itC = bmC.bm.begin(); itC != bmC.bm.end(); ++itC) {
    bs.insert((itC -> first));
  }
  distribution_analysis(bs, bmA, bmB, bmC);
  surprisal_analysis(bmA,bmB,bmC,corpusA,corpusB,corpusC);
}


void surprisal_analysis(BigramModel &bmA, 
			BigramModel &bmB, 
			BigramModel &bmC,
			vector< vector<string> > &corpusA,
			vector< vector<string> > &corpusB,
			vector< vector<string> > &corpusC)
{
  // total distance of corpus A to corpus B is the total surprisal of observing corpus B given a language model based on corpus A
  
  double sBA = 0.;
  double sCA = 0.;
  for(vector< vector<string> >::iterator it = corpusA.begin(); it != corpusA.end(); ++it) {
    sBA += bmB.seq_surprisal(*it);
    sCA += bmC.seq_surprisal(*it);
  }
  sBA /= corpusA.size();
  sCA /= corpusA.size();

  double sAB = 0.;
  double sCB = 0.;
  for(vector< vector<string> >::iterator it = corpusB.begin(); it != corpusB.end(); ++it) {
    sAB += bmA.seq_surprisal(*it);
    sCB += bmC.seq_surprisal(*it);
  }
  sAB /= corpusB.size();
  sCB /= corpusB.size();

  double sAC = 0.;
  double sBC = 0.;
  for(vector< vector<string> >::iterator it = corpusC.begin(); it != corpusC.end(); ++it) {
    sAC += bmA.seq_surprisal(*it);
    sBC += bmB.seq_surprisal(*it);
  }
  sAC /= corpusC.size();
  sBC /= corpusC.size();

  double dAB = (sAB + sBA)/2;
  double dBC = (sBC + sCB)/2;
  double dAC = (sAC + sCA)/2;

  cout << "Analysis of average surprisal distance:\n";
  cout << "\tDistance from A to B: " << dAB << endl;
  cout << "\tDistance from A to C: " << dAC << endl;
  cout << "\tDistance from B to C: " << dBC << endl;
  cout << endl;
}

inline double kl(vector<double>& p1, vector<double>& p2) 
{
  double r = 0.;
  for (int i = 0; i < p1.size(); ++i)
    {
      r += p1[i] * log2(p1[i] / p2[i]);
    }
  return r;
}

inline double skl(vector<double>& p1, vector<double>& p2)
{
  double r = 0.;
  for (int i = 0; i < p1.size(); ++i)
    {
      r += p1[i] * log2(p1[i] / p2[i]) + p2[i] * log2(p2[i] / p1[i]);
    }
  return r;
}

void distribution_analysis(bigram_set& merged_set, 
			   BigramModel& bmA, 
			   BigramModel& bmB, 
			   BigramModel& bmC)
{
  vector<double> pA;
  vector<double> pB;
  vector<double> pC;

  
  for(bigram_set::iterator it = merged_set.begin();
      it != merged_set.end(); ++it) {
    pA.push_back(bmA.rfe_smoothed(*it));
    pB.push_back(bmB.rfe_smoothed(*it));
    pC.push_back(bmC.rfe_smoothed(*it));
  }
  double dAB = skl(pA, pB);
  double dAC = skl(pA, pC);
  double dBC = skl(pB, pC);

  cout << "Analysis of distances between bigram model distributions:\n";

  cout << "\tDistance from A to B: " << dAB << endl;
  cout << "\tDistance from A to C: " << dAC << endl;
  cout << "\tDistance from B to C: " << dBC << endl;
  cout << endl;
}  


#include "bigrammodel.hpp"

void read_corpus(std::vector< std::vector<std::string> > &corpus, std::string filename)
{
  std::string temp;
  std::ifstream indata((char *) filename.c_str());
  if (!indata) {
    std::cerr << "Corpus file could not be opened" << std::endl;
    exit(1);
  }
  while(getline(indata, temp)) {
    std::vector<std::string> line;
    line.push_back("DOT");
    boost::tokenizer<> tok(temp);
    for(boost::tokenizer<>::iterator it = tok.begin();
	it != tok.end(); ++it) {
      line.push_back(*it);
    }
    line.push_back("DOT");
    corpus.push_back(line);
  }
  indata.close();
}

void BigramModel::print_bm()
{
  for (bigram_map::iterator it = bm.begin(); it != bm.end(); ++it) {
    std::cout << (it -> first).to_string() << "\t" << it -> second << std::endl;
  }
}

void BigramModel::process_corpus(std::vector< std::vector<std::string> > &corpus)
{
  for(std::vector< std::vector<std::string> >::iterator lines_it = corpus.begin(); lines_it != corpus.end(); ++lines_it) {
    std::vector<std::string> line = *lines_it;
    for (int i = 0; i < line.size() - 1; ++i)
      {
	Bigram b(line[i],line[i+1]);
	if (bm.find(b) != bm.end()) {
	  ++bm[b];
	} else {
	  bm[b] = 1;
	}
	++total_count;
      
	if (um.find(line[i]) != um.end()) {
	  ++um[line[i]];
	} else {
	  um[line[i]] = 1;
	}
      }
  }
  build_bins();
}

void BigramModel::build_bins()
{
  for (int i = 0; i < k+2; ++i)
    {
      bins[i] = 0;
    }
  for(unigram_map::iterator it2 = um.begin(); it2 != um.end(); ++it2) {
    for(unigram_map::iterator it1 = um.begin(); it1 != um.end(); ++it1) {
      Bigram b(it1 -> first, it2 -> first);
      int count = 0;
      if (bm.find(b) != bm.end()) {
	count = bm[b];
      }
      if (count <= k) {
	bins[count]++;
      }
    }
  }
}



void BigramModel::save(std::string filename)
{
  std::cerr << "BigramModel::save(std::string) not implemented" << std::endl;
  exit(1);
  // TODO: implement
}

void BigramModel::load(std::string filename)
{
  std::cerr << "BigramModel::load(std::string) not implemented" << std::endl;
  exit(1);
  // TODO: implement
}

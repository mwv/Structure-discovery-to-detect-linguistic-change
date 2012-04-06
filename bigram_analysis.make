bigram_analysis.out: bigram_analysis.o bigrammodel.o bigram.o
	g++ -o bigram_analysis.out bigram_analysis.o bigrammodel.o bigram.o
bigram_analysis.o: bigram_analysis.cpp bigrammodel.hpp bigram.hpp
	g++ -c bigram_analysis.cpp
bigrammodel.o: bigrammodel.cpp bigrammodel.hpp bigram.hpp
	g++ -c bigrammodel.cpp
bigram.o: bigram.cpp bigram.hpp
	g++ -c bigram.cpp
clean:
	rm bigram_analysis.o bigrammodel.o bigram.o bigram_analysis.out

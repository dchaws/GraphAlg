    
all:  graphalgtest listdircycles 

graphalgtest: graphalgtest.cpp graphalg.cpp graphalg.h 
	g++ -o graphalgtest graphalg.cpp graphalgtest.cpp 

listdircycles: listdircycles.cpp graphalg.cpp graphalg.h 
	g++ -o listdircycles graphalg.cpp listdircycles.cpp 

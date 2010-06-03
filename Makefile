all:
	g++ *.cpp -o seminar -O3 -lecf -lboost_program_options-mt -Wall -Wno-deprecated
clean:
	rm seminar

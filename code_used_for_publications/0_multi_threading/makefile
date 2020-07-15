ALLFLAGS = -std=c++14 -pthread -o
CNAME = multi_threader
TNAME = TEST_$(CNAME)
CFLAGS = $(ALLFLAGS) $(CNAME) -O3 -march=native
DBFLAGS = $(ALLFLAGS) $(CNAME) -g -Og -Wall
TFLAGS = $(ALLFLAGS) $(TNAME) -g -Og -Wall -lboost_unit_test_framework
ALLFILES =	multi_threader.cpp
CFILES =	$(ALLFILES)
TFILES =	$(ALLFILES)
all:
	g++ $(CFLAGS) $(CFILES)
dbg:
	g++ $(DBFLAGS) $(CFILES)
test:
	g++ $(TFLAGS) $(TFILES)
clean:
	rm -f $(CNAME)
	rm -f $(TNAME)
CXX = clang++
CXXFLAGS = -std=c++1y -g -Wall
.SUFFIXES: .cpp .o

program = circlize
objs = circlize.o main.o
libs = -lopencv_highgui -lopencv_core -lopencv_imgproc

$(program): $(objs)
	$(CXX) $(CXXFLAGS) -L/usr/local/lib $(libs) -o $@ $^

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

circlize.o: CXXFLAGS += -I/usr/local/include
circlize.o: circlize.h

main.o: CXXFLAGS += -I/usr/local/include
main.o: circlize.h

.PHONY: run
run: $(program)
	./$(program)

.PHONY: clean
clean:
	-$(RM) $(program) $(objs)

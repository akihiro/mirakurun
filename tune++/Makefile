CXXFLAGS+=-Os -Wall -std=c++14

.PHONY: install clean

tune++: tune++.o
	$(CXX) $(CXXFLAGS) -o $@ $^

install:
	install -o root -g root -m 755 tune++ /usr/local/bin/

clean:
	$(RM) tune++ tune++.o

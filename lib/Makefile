CXXFLAGS = -g -Os -Wall -pedantic -Werror $(shell pkg-config --cflags bullet)
CXXFLAGS = -g -Wall -pedantic -Werror -Wno-unused-variable $(shell pkg-config --cflags bullet)

lib4wd.a: $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(AR) rcs $@ $(filter %.o, $^)

clean:
	@$(RM) *.a *.o

.PHONY: clean

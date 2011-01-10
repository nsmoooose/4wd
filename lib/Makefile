CXXFLAGS = -g -Os -Wall -pedantic -Werror
CXXFLAGS = -g -Wall -pedantic -Werror

lib4wd.a: $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(AR) rcs $@ $(filter %.o, $^)

clean:
	@$(RM) *.a *.o

.PHONY: clean

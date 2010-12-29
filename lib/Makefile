CXXFLAGS = -g -Os -Wall -pedantic -Werror

lib4wd.a: $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(AR) rcs $@ $^

clean:
	@$(RM) *.a *.o


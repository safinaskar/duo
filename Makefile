CXX          = g++

CPPFLAGS     =
CXXFLAGS     = -O3 -g -Wall -Wextra -Wno-missing-field-initializers
ASFLAGS      =
LDFLAGS      = -O3

DUO_FLAGS    = -m32

# Unfortunately, -ffreestanding is valid for C++, but -fno-hosted is invalid o_O. -ffreestanding is equivalent to -fno-builtin, except for `main' function (proof: gcc sources)
DUO_CPPFLAGS = -MD -nostdinc -Ilibfreestanding/include
DUO_CXXFLAGS = -std=gnu++98 -ffreestanding

all: duo

src/start.o: src/start.S
	$(CXX) -c $(DUO_FLAGS) $(DUO_CPPFLAGS) $(CPPFLAGS) $(ASFLAGS) -o $@ src/start.S

src/io.o: src/io.cpp
	$(CXX) -c $(DUO_FLAGS) $(DUO_CPPFLAGS) $(CPPFLAGS) $(DUO_CXXFLAGS) $(CXXFLAGS) -o $@ src/io.cpp
src/console.o: src/console.cpp
	$(CXX) -c $(DUO_FLAGS) $(DUO_CPPFLAGS) $(CPPFLAGS) $(DUO_CXXFLAGS) $(CXXFLAGS) -o $@ src/console.cpp
src/kmesg.o: src/kmesg.cpp
	$(CXX) -c $(DUO_FLAGS) $(DUO_CPPFLAGS) $(CPPFLAGS) $(DUO_CXXFLAGS) $(CXXFLAGS) -o $@ src/kmesg.cpp
src/main.o: src/main.cpp
	$(CXX) -c $(DUO_FLAGS) $(DUO_CPPFLAGS) $(CPPFLAGS) $(DUO_CXXFLAGS) $(CXXFLAGS) -o $@ src/main.cpp

duo: src/start.o src/io.o src/console.o src/kmesg.o src/main.o
	$(CXX) $(DUO_FLAGS) -nostdlib -Wl,-Ttext-segment=0x100000 $(LDFLAGS) -o $@ src/start.o src/io.o src/console.o src/kmesg.o src/main.o -lgcc && chmod -x $@

clean:
	rm -f duo && find . -name '*.o' -delete

include src/start.d
include src/io.d
include src/console.d
include src/kmesg.d
include src/main.d

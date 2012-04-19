# "make clean" also recreates empty .d files

CXX            = g++

C_CXX_WARNINGS = -Wall -Wextra -Wformat=2 -Wuninitialized -Winit-self -Wmissing-include-dirs -Wswitch-default -Wunused -Wunused-local-typedefs -Wuninitialized -Wstrict-aliasing=1 -Wstrict-overflow=5 -Wfloat-equal -Wundef -Wshadow -Wunsafe-loop-optimizations -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wsign-conversion -Wlogical-op -Wmissing-declarations -Wmissing-noreturn -Wmissing-format-attribute -Wpacked -Wredundant-decls -Wunreachable-code -Winline -Wdisabled-optimization -Wstack-protector -Woverlength-strings -Wcomment -Wunused-macros
CXX_WARNINGS   = -Wctor-dtor-privacy -Wnon-virtual-dtor -Wstrict-null-sentinel -Wold-style-cast -Woverloaded-virtual -Wsign-promo

CPPFLAGS       =
CXXFLAGS       = -O3 -g $(C_CXX_WARNINGS) $(CXX_WARNINGS) -Wno-missing-field-initializers
ASFLAGS        =
LDFLAGS        = -O3

# Flags for all building stages
DUO_FLAGS      = -m32

# Unfortunately, -ffreestanding is valid for C++, but -fno-hosted is invalid o_O. -ffreestanding is equivalent to -fno-builtin, except for `main' function (proof: gcc sources)
DUO_CPPFLAGS   = -MMD -nostdinc -Ifrees/include
DUO_CXXFLAGS   = -std=gnu++98 -ffreestanding -fno-rtti

COMPILE        = echo "  CC      $@" && $(CXX) -c $(DUO_FLAGS) $(DUO_CPPFLAGS) $(CPPFLAGS) $(DUO_CXXFLAGS) $(CXXFLAGS) -o $@

all: duo

src/start.o: src/start.S
	$(CXX) -c $(DUO_FLAGS) $(DUO_CPPFLAGS) $(CPPFLAGS) $(ASFLAGS) -o $@ src/start.S

frees/string.o: frees/string.cpp
	@$(COMPILE) frees/string.cpp
frees/util.o: frees/util.cpp
	@$(COMPILE) frees/util.cpp

src/io.o: src/io.cpp
	@$(COMPILE) src/io.cpp
src/memory.o: src/memory.cpp
	@$(COMPILE) src/memory.cpp
src/console.o: src/console.cpp
	@$(COMPILE) src/console.cpp
src/kmesg.o: src/kmesg.cpp
	@$(COMPILE) src/kmesg.cpp

src/kprintf.cpp: frees/printf.sh
	VPREFIX="vk" PREFIX="k" SHORT_ARGS="" LONG_ARGS="" CHAR_ACTION="kmesg_fputc" DATA_ACTION="kmesg_write" BEFORE="$$(echo "#include \"kmesg.h\"" && echo "#include \"kprintf.h\"")" frees/printf.sh > $@
src/kprintf.o: src/kprintf.cpp
	@$(COMPILE) src/kprintf.cpp

src/main.o: src/main.cpp
	@$(COMPILE) src/main.cpp

duo: src/start.o frees/string.o frees/util.o src/io.o src/memory.o src/console.o src/kmesg.o src/kprintf.o src/main.o
	$(CXX) $(DUO_FLAGS) -static -nostdlib -Wl,-Ttext-segment=0x100000 $(LDFLAGS) -o $@ src/start.o frees/string.o frees/util.o src/io.o src/memory.o src/console.o src/kmesg.o src/kprintf.o src/main.o -lgcc && chmod -x $@

clean:
	rm -f duo && find . -name '*.o' -delete && find . -name '*.d' -exec sh -c ": > {}" ";"

include src/start.d

include frees/string.d
include frees/util.d

include src/io.d
include src/memory.d
include src/console.d
include src/kmesg.d
include src/kprintf.d
include src/main.d

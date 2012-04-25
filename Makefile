# No, I didn't steal Linux build system interface. I use plain makefile and messages such as "  CC      frees/string.o", because it is really comfortable. But I don't use "make mrproper" (if you want real cleanning, just type "make maintainer-clean")

CXX            = g++

WARNS          = -Wall -Wextra -Wformat=2 -Wuninitialized -Winit-self -Wmissing-include-dirs -Wswitch-default -Wunused -Wunused-local-typedefs -Wuninitialized -Wstrict-aliasing=1 -Wstrict-overflow=5 -Wfloat-equal -Wundef -Wshadow -Wunsafe-loop-optimizations -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wsign-conversion -Wlogical-op -Wmissing-declarations -Wmissing-noreturn -Wmissing-format-attribute -Wpacked -Wredundant-decls -Wunreachable-code -Winline -Wdisabled-optimization -Wstack-protector -Woverlength-strings -Wcomment -Wunused-macros
CXXWARNS       = $(WARNS) -Wctor-dtor-privacy -Wnon-virtual-dtor -Wstrict-null-sentinel -Wold-style-cast -Woverloaded-virtual -Wsign-promo

CPPFLAGS       =
CXXFLAGS       = -O3 -g $(CXXWARNS) -Wno-missing-field-initializers -fno-rtti
ASFLAGS        =
LDFLAGS        = -O3

# Flags for all building stages
DUO_FLAGS      = -m32

# Unfortunately, -ffreestanding is valid for C++, but -fno-hosted is invalid o_O. -ffreestanding is equivalent to -fno-builtin, except for `main' function (proof: gcc sources)
DUO_CPPFLAGS   = -MMD -nostdinc -Ifrees/include
DUO_CXXFLAGS   = -std=gnu++98 -ffreestanding -fno-rtti -fno-stack-protector

COMPILE        = echo "  CC      $@" && $(CXX) -c $(DUO_FLAGS) $(DUO_CPPFLAGS) $(CPPFLAGS) $(DUO_CXXFLAGS) $(CXXFLAGS) -o $@

all: duo

#### frees (libfreestanding) ####

frees/string.o: frees/string.cpp
	@$(COMPILE) frees/string.cpp
frees/util.o: frees/util.cpp
	@$(COMPILE) frees/util.cpp

#### kprintfm (`kprintf' module) ####

kprintfm/start.o: kprintfm/start.S
	@echo "  AS      $@" && $(CXX) -c $(DUO_FLAGS) $(DUO_CPPFLAGS) $(CPPFLAGS) $(ASFLAGS) -o $@ kprintfm/start.S
kprintfm/io.o: kprintfm/io.cpp
	@$(COMPILE) kprintfm/io.cpp
kprintfm/console.o: kprintfm/console.cpp
	@$(COMPILE) kprintfm/console.cpp
kprintfm/kmsg.o: kprintfm/kmsg.cpp
	@$(COMPILE) kprintfm/kmsg.cpp
kprintfm/kprintf.cpp: frees/printf.sh
	@echo "  GEN     $@" && VPREFIX="vk" PREFIX="k" SHORT_ARGS="" LONG_ARGS="" CHAR_ACTION="kmsg_fputc" DATA_ACTION="kmsg_write" BEFORE="$$(echo "#include \"kmsg.h\"" && echo "#include \"kprintf.h\"")" frees/printf.sh > $@
kprintfm/kprintf.o: kprintfm/kprintf.cpp
	@$(COMPILE) kprintfm/kprintf.cpp

#### main module ####

src/memory.o: src/memory.cpp
	@$(COMPILE) src/memory.cpp
src/main.o: src/main.cpp
	@$(COMPILE) src/main.cpp

#### kernel image ####

OBJS = frees/string.o frees/util.o kprintfm/start.o kprintfm/io.o kprintfm/console.o kprintfm/kmsg.o kprintfm/kprintf.o src/memory.o src/main.o

duo: $(OBJS)
	@echo "  LD      $@" && $(CXX) $(DUO_FLAGS) -static -nostdlib -Wl,-Ttext-segment=0x100000 $(LDFLAGS) -o $@ $(OBJS) -lgcc && chmod -x $@

#### etc ####

clean:
	find . -name '*.o' -delete && rm -f duo

distclean: clean

maintainer-clean: distclean
	find . -name '*.d' -delete && rm -f kprintfm/kprintf.cpp

include frees/string.d
include frees/util.d

include kprintfm/start.d
include kprintfm/io.d
include kprintfm/console.d
include kprintfm/kmsg.d
include kprintfm/kprintf.d

include src/memory.d
include src/main.d

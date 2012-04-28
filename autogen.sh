#!/bin/sh

# No, I didn't steal Linux build system interface. I use plain makefile and messages such as "  CC      frees/string.o", because it is really comfortable. But I don't use "make mrproper" (if you want real cleanning, just type "make maintainer-clean")

set -e

TAB="$(printf '\t\n')"

# $SOURCES will contain new line symbols on all machines I tested. But this doesn't matter for my code
SOURCES="$({ find * \( -name '*.S' -o -name '*.cpp' \) && echo kprintfm/kprintf.cpp && echo fus/printf.cpp; } | sort | uniq)"

# But $OBJS will not contain new line symbols. It will contain spaces
OBJS="$(echo "$SOURCES" | tr ' ' '\n' | sed 's/\.[^.]*$/.o/' | tr '\n' ' ')"

for SOURCE in $SOURCES; do
	if [ ! -f "${SOURCE%.*}.d" ]; then
		: > "${SOURCE%.*}.d"
	fi
done

{
	cat << "EOF"
# Generated by autogen.sh

CXX            = g++

WARNS          = -Wall -Wextra -Wformat=2 -Wuninitialized -Winit-self -Wmissing-include-dirs -Wswitch-default -Wunused -Wunused-local-typedefs -Wuninitialized -Wstrict-aliasing=1 -Wstrict-overflow=5 -Wfloat-equal -Wundef -Wshadow -Wunsafe-loop-optimizations -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wsign-conversion -Wlogical-op -Wmissing-declarations -Wmissing-noreturn -Wmissing-format-attribute -Wredundant-decls -Wunreachable-code -Winline -Wdisabled-optimization -Wstack-protector -Woverlength-strings -Wcomment -Wunused-macros
CXXWARNS       = $(WARNS) -Wctor-dtor-privacy -Wnon-virtual-dtor -Wstrict-null-sentinel -Wold-style-cast -Woverloaded-virtual -Wsign-promo

CPPFLAGS       =

CXXFLAGS       = -O3 -g $(CXXWARNS) -Wno-missing-field-initializers
ASFLAGS        =
LDFLAGS        = -O3

# Flags for all building stages
DUO_FLAGS      = -m32

# Unfortunately, -ffreestanding is valid for C++, but -fno-hosted is invalid o_O. -ffreestanding is equivalent to -fno-builtin, except for `main' function (proof: gcc sources)
DUO_CPPFLAGS   = -MMD -nostdinc -Ifrees/include -Ifus

# Strict aliasing is not for OS kernel
DUO_CXXFLAGS   = -std=gnu++98 -ffreestanding -fno-rtti -fno-stack-protector -fno-strict-aliasing -fno-exceptions

EOF

	cat << EOF
OBJS           = $OBJS

all: duo

EOF

	for SOURCE in $SOURCES; do
		cat << EOF
${SOURCE%.*}.o: $SOURCE
EOF

		case "$SOURCE" in
			*.cpp)
				sed 's/^TAB/\t/' << "EOF" # I use "TAB" to prevent editors from replacing tab with spaces
TAB@echo "  CXX     $@" && $(CXX) -c $(DUO_FLAGS) $(DUO_CPPFLAGS) $(CPPFLAGS) $(DUO_CXXFLAGS) $(CXXFLAGS) -o $@ $<

EOF
				;;
			*.S)
				sed 's/^TAB/\t/' << "EOF"
TAB@echo "  AS      $@" && $(CXX) -c $(DUO_FLAGS) $(DUO_CPPFLAGS) $(CPPFLAGS) $(ASFLAGS) -o $@ $<

EOF
				;;
			*)
				echo "${0##*/}: $SOURCE: unsupported extension"
				;;
		esac
	done

	sed 's/^TAB/\t/' << "EOF"
kprintfm/kprintf.cpp: frees/printf.sh
TAB@echo "  GEN     $@" && VPREFIX="vk" PREFIX="k" SHORT_ARGS="" LONG_ARGS="" CHAR_ACTION="kmsg_fputc" DATA_ACTION="kmsg_write"    BEFORE="$$(echo "#include \"kmsg.h\"" && echo "#include \"kprintf.h\"")" $< > $@

fus/printf.cpp: frees/printf.sh
TAB@echo "  GEN     $@" && VPREFIX="v"  PREFIX=""  SHORT_ARGS="" LONG_ARGS="" CHAR_ACTION="putchar"    DATA_ACTION="console_write" BEFORE="$$(echo "#include \"../kprintfm/console.h\"" && echo "#include <stdio.h>")" $< > $@

duo: $(OBJS)
TAB@echo "  LD      $@" && $(CXX) $(DUO_FLAGS) -static -nostdlib -Wl,-Ttext-segment=0x100000 $(LDFLAGS) -o $@ $(OBJS) -lgcc && chmod -x $@

clean:
TABfind . -name '*.o' -delete && rm -f duo

distclean: clean

maintainer-clean: distclean
TABfind . -name '*.d' -delete && rm -f Makefile kprintfm/kprintf.cpp fus/printf.cpp

rcs-clean: maintainer-clean

EOF

	for SOURCE in $SOURCES; do
		echo "include ${SOURCE%.*}.d"
	done
} > Makefile

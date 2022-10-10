################################################################################
# Specify makeflags
################################################################################

.DEFAULT_GOAL = primary

MAKEFLAGS += -j

################################################################################
# Specify make tools and makeflags
################################################################################

CC = gcc
CX = g++
CCFLAGS = -g -Wall -pthread -I/usr/local/Aria/include
LDFLAGS = -L/usr/local/Aria/lib -lAria -pthread -ldl -lrt

LINK    = -o $@ $^ $(LDFLAGS)
COMPILE = -c $< -o $@ $(CCFLAGS)

################################################################################
# Search C and CPP source files
################################################################################

root := $(realpath .)
dir := $(root)

TARGET_BINS :=
CLEAN_FILES :=

SRCS_$(dir) := $(wildcard $(dir)/*.cpp)
OBJS_$(dir) := $(SRCS_$(dir):.cpp=.o)
DEPS_$(dir) := $(SRCS_$(dir):.cpp=.d)

CLEAN_FILES += $(OBJS_$(dir)) $(DEPS_$(dir))

example: example.o
part_c: part_c.o
part_d: part_d.o
part_e: part_e.o

%: %.o
	@printf "  %-8s$@\n" "CX"
	$(Q)$(CX) $(LINK)

TARGET_BINS += example part_c part_d part_e
CLEAN_FILES += example part_c part_d part_e

################################################################################
# Declare make targets
################################################################################

.PHONY: primary
primary: $(TARGET_BINS)

.PHONY: clean
clean:
	@rm -f $(CLEAN_FILES)

.PHONY: distclean
distclean: clean

################################################################################
# Directory generic make targets
################################################################################

%.o: %.cpp
	@printf "  %-8s$@\n" "CX"
	$(Q)$(CX) $(COMPILE)

################################################################################
# Make options
################################################################################

V ?= 0
ifeq ($(V),0)
    Q = @
else
    Q =
endif
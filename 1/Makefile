# these can be changed
TARGET_EXEC ?= integral

BUILD_DIR ?= build
SRC_DIRS ?= src

CC = mpicc
NP = 2
NI = 1073741824


# start of wizardry
# just -I./**/
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))


# -MMD -MP is a trick so that compiler produces dependency graph in Makefile format
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

# helps identify strange bugs
SHARED_FLAGS ?= -Wall -Wextra -Werror

# last versions before they broke up :(
CFLAGS ?= $(SHARED_FLAGS) -std=c99
CXXFLAGS ?= $(SHARED_FLAGS) -std=c++11

# basically i stole those from make manual
LDFLAGS ?= -pthread
LDLIBS ?= -lstdc++ -lm


# more wizardry regarding proper file names
# basically, i append .o and .d, not replace something with them
# the reason originally was to not invent a wheel, but it really helps differentiate C and C++
SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# the main target
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

.PHONY: run
run: $(BUILD_DIR)/$(TARGET_EXEC)
	mpirun --mca opal_warn_on_missing_libcuda 0 --oversubscribe -np $(NP) $(BUILD_DIR)/$(TARGET_EXEC) $(NI)

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

# slash actually means directory
# who knew...
$(BUILD_DIR)%/:
	mkdir -p $@

# let the black magic begin
# IIRC, the prerequisites are expanded at the same time as target
# so by using second expansion we can define the directory where target should go
.SECONDEXPANSION:

$(BUILD_DIR)/%.c.o: %.c | $$(@D)/
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp | $$(@D)/
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# and don't forget those -MMD -MP things
-include $(DEPS)

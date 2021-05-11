
SRCDIR=.
BUILTINS_SRCDIR=$(SRCDIR)/builtins
BUILD_DIR=$(SRCDIR)/build


CC=gcc
LIBS=
CFLAGS=-Wall -Wextra -g -I$(SRCDIR)
LDFLAGS=-g


SRCS_BUILTINS=$(shell find $(SRCDIR)/builtins -name "*.c")

SRCS=main.c builtin.c executor.c argumentHandle.c
     $(SRCS_BUILTINS) 

OBJS=$(SRCS:%.c=$(BUILD_DIR)/%.o)


TARGET=shell


.PHONY: all
all: prep-build $(TARGET)

prep-build:
	mkdir -p $(BUILD_DIR)/builtins

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)


depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CC) $(CFLAGS) -MM $^ > ./.depend;

include .depend


$(BUILD_DIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET) core .depend
	$(RM) -r $(BUILD_DIR)

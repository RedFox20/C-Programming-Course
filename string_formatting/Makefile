# Generic Makefile
NAME = string_formatting
CFLAGS = -g -std=c11 -I.
OBJDIR = obj
SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

ifeq ($(OS),Windows_NT)
	OUT = $(NAME).exe
else
	OUT = $(NAME)
endif
#ifneq (,$(findstring MINGW,$(uname_S)))

all: $(OUT)
run: $(OUT)
	./$(OUT)
clean:
	rm -rf $(OBJDIR) $(OUT)

# declare all the autodeps
-include $(OBJDIR)/*.d

#ld: -Wl,-X: discard nasm locals
# OUT depends on OBJDIR, OBJS
$(OUT): $(OBJDIR) $(OBJS)
	gcc -g -o $(OUT) $(OBJDIR)/*.o

$(OBJDIR)/%.o: %.c
	gcc $(CFLAGS) -Wall -c $*.c -o $(OBJDIR)/$*.o -MD

$(OBJDIR):
	mkdir $(OBJDIR)
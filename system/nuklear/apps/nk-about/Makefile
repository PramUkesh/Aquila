export

MAKEFLAGS += --no-print-directory

include Build.mk

all: $(app)

$(app): builtin.o
	@echo -e "  LD      " $@
	@$(LD) $(LDFLAGS) $< -lnk $(LDLIBS) -o $@;

builtin.o: $(obj-y) $(dirs-y)
	@echo -e "  LD      " builtin.o;
	@$(LD) $(LDFLAGS) -r $(obj-y) $(patsubst %/,%/builtin.o, $(dirs-y)) -o builtin.o; 

.PHONY: $(dirs-y)
$(dirs-y): $(patsubst %/,%/Makefile, $(dirs-y))
	@echo -e "  MK      " $@
	@$(MAKE) -C $@ $(param)

%.o:%.c
	@echo -e "  CC      " $@;
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean: param = clean
clean: $(dirs-y)
	$(RM) -f builtin.o $(app) $(obj-y)

.PHONY: all clean

OUTDIR=build
HEXFILE=$(OUTDIR)/main.hex
SRC=main.c

all: $(HEXFILE)

$(HEXFILE): $(SRC)
	-mkdir -p $(OUTDIR)
	xc8 --chip=16f628a --outdir=$(OUTDIR) $(SRC)

clean:
	-rm -rf $(OUTDIR)/*

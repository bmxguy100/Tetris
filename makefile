# ----------------------------
# Set NAME to the program name
# Set ICON to the png icon file name
# Set DESCRIPTION to display within a compatible shell
# Set COMPRESSED to "YES" to create a compressed program
# ----------------------------

NAME        ?= TETRIS
COMPRESSED  ?= YES
ICON        ?= iconc.png
DESCRIPTION ?= "Tetris by Jack Gordon"

# ----------------------------

include $(CEDEV)/include/.makefile

install:
	tilp -n -s $(BINDIR)/*.8xp

.PHONY: install
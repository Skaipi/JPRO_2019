gcc -Wall -Wextra -o main.exe \
	main.c \
	fluid_block.c \
	fluid_simulation.c \
	fluid_graphics.c \
	fluid_output.c \
	fluid_modes.c \
	-lSDL2main -lSDL2 -lm

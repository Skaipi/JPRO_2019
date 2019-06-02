gcc -Wall -Wextra -o main.exe \
	main.c \
	fluid_block.c \
	fluid_simulation.c \
	fluid_graphics.c \
	output.c \
  modes.c \
	-lmingw32  -lSDL2main -lSDL2

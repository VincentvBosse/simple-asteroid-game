CFLAGS= -fsanitize=address -g -Wall


.PHONY: compile clean checkstyle format

compile: game

clean:
	rm -f *.o game game_test

checkstyle:
	clang-tidy --quiet $(wildcard *.c) $(wildcard *.h) --

format:
	clang-format -i $(wildcard *.c) $(wildcard *.h)


game: game.o game_lib.o tui_matrix.o tui_io.o ansi_codes.o tui.o vec.o
	gcc $(CFLAGS) game.o game_lib.o tui_matrix.o tui_io.o ansi_codes.o tui.o vec.o -o game

game.o: game.c tui.h tui_io.h tui_matrix.h ansi_codes.h
	gcc $(CFLAGS) -c game.c -o game.o

game_lib.o: game_lib.c game_lib.h tui.h tui_io.h tui_matrix.h ansi_codes.h
	gcc $(CFLAGS) -c game_lib.c -o game_lib.o

vec.o: vec.c vec.h
	gcc -fsanitize=address -g -c vec.c -o vec.o

tui.o: tui.c tui.h tui_matrix.h ansi_codes.h
	gcc $(CFLAGS) -c tui.c -o tui.o

tui_matrix.o: tui_matrix.c tui_matrix.h ansi_codes.h
	gcc $(CFLAGS) -c tui_matrix.c -o tui_matrix.o

tui_io.o: tui_io.c tui_io.h
	gcc $(CFLAGS) -c tui_io.c -o tui_io.o

ansi_codes.o: ansi_codes.c ansi_codes.h
	gcc $(CFLAGS) -c ansi_codes.c -o ansi_codes.o


release:
	gcc ./source/main.c -Wall -Wextra -o ./binary/list

debug:
	gcc ./source/main.c -g -Wall -Wextra -fsanitize=address -o ./binary/list

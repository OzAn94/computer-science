#include "SPMainAux.h"

int main() {
	unsigned int level;

	do {
		level = init();

		if (level == EXIT) {
			return 0;
		}

	} while (run_game(level) == RESTART);

	return 0;
}
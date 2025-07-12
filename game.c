/*
 * Copyright (c) 2020-2022, 2024-2025 Brian Callahan <bcallah@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <term.h>
#include <unistd.h>

#include "freebee.h"
#include "version.h"

/*
 * Fisher-Yates shuffle
 */
static void
shuffle(void)
{
	int i, j, t;

	for (i = 5; i > 0; i--) {
		j = arc4random_uniform(i + 1);
		t = letters[j];
		letters[j] = letters[i];
		letters[i] = t;
	}
}

static void
print_board(void)
{

	(void) printf("Free Bee %s | Score: %3d | Rank: %s\n\n", VERSION,
	    points, rank());
	(void) printf("              %c\n", letters[1]);
	(void) printf("          %c       %c\n", letters[0], letters[2]);
	(void) printf("              %c\n", letters[6]);
	(void) printf("          %c       %c\n", letters[3], letters[5]);
	(void) printf("              %c\n\n", letters[4]);
}

static void
show_answers(void)
{
	int i, printed = 8; /* Number of \n in heading */
	size_t j, queen_points;

	(void) putp(clear_screen);
	print_board();

	for (i = 0; i < count; i++) {
		for (j = 0; words[i][j] != '\0'; j++)
			putchar(words[i][j]);
		putchar('\n');
		if (++printed > rows - 3) {
			while (getchar() != '\n')
				;
			(void) putp(clear_screen);
			print_board();
			printed = 8;
		}
	}

	(void) printf("\n");
	if (++printed > rows - 3) {
		while (getchar() != '\n')
			;
		(void) putp(clear_screen);
		print_board();
		printed = 8;
	}
	(void) printf("Total words:  %d\n", count);
	if (++printed > rows - 3) {
		while (getchar() != '\n')
			;
		(void) putp(clear_screen);
		print_board();
		printed = 8;
	}
	(void) printf("Total points: %d\n", total);
	if (++printed > rows - 3) {
		while (getchar() != '\n')
			;
		(void) putp(clear_screen);
		print_board();
		printed = 8;
	}
	queen_points = total * 0.70;
	(void) printf("Points for Queen Bee: %zu\n", queen_points);

	while (getchar() != '\n')
		;
}

static void
show_found(void)
{
	int i, printed = 8;

	(void) putp(clear_screen);
	print_board();

	for (i = 0; i < found; i++) {
		(void) printf("%s\n", foundlist[i]);
		if (++printed > rows - 3) {
			while (getchar() != '\n')
				;
			(void) putp(clear_screen);
			print_board();
			printed = 8;
		}
	}

	(void) printf("\n");
	if (++printed > rows - 3) {
		while (getchar() != '\n')
			;
		(void) putp(clear_screen);
		print_board();
		printed = 8;
	}
	(void) printf("Words found: %d\n", found);

	while (getchar() != '\n')
		;
}

static int
check(char *guess)
{
	char *g;
	int center = 0;

	for (g = guess; *g != '\0'; ++g) {
		if (!isalpha((unsigned char) *g))
			return 0;
		if (isupper((unsigned char) *g))
			*g = tolower((unsigned char) *g);
		if (*g == letters[6])
			center = 1;
	}

	if (strlen(guess) < 4) {
		(void) printf("Too short\n");
		while (getchar() != '\n')
			;
		return 0;
	}

	if (center == 0) {
		(void) printf("Missing center letter\n");
		while (getchar() != '\n')
			;
		return 0;
	}

	return 1;
}

static void
add_points(const char *guess)
{
	int one, two, three, four, five, six;
	size_t i = 0, j = 0;

	one = two = three = four = five = six = 0;

	if ((i = strlen(guess)) < 7) {
		if (i == 4)
			i = 1;
		points += i;
		return;
	}

	i = 0;
	while (guess[i] != '\0') {
		for (j = 0; j < 7; j++) {
			if (guess[i] == letters[j]) {
				switch(j) {
				case 0:
					one = 1;
					break;
				case 1:
					two = 1;
					break;
				case 2:
					three = 1;
					break;
				case 3:
					four = 1;
					break;
				case 4:
					five = 1;
					break;
				case 5:
					six = 1;
				}
				break;
			}
		}
		++i;
	}

	if (one && two && three && four && five && six) {
		points += strlen(guess) + 7;
		(void) printf("Pangram!\n");
		while (getchar() != '\n')
			;
	} else {
		points += strlen(guess);
	}
}

static void
found_word(const char *guess)
{
	int i;

	for (i = 0; i < found; i++) {
		if (!strcmp(guess, foundlist[i])) {
			(void) printf("Already found\n");
			while (getchar() != '\n')
				;
			return;
		}
	}

	(void) strlcat(foundlist[i], guess, sizeof(foundlist[i]));

	++found;

	add_points(guess);
}

static void
find_word(const char *guess)
{
	int i;

	for (i = 0; i < count; i++) {
		if (!strcmp(guess, words[i])) {
			found_word(guess);
			return;
		}
	}

	(void) printf("Not in dictionary\n");
	while (getchar() != '\n')
		;
}

/*
 * Main loop
 */
void
play_game(void)
{
	char guess[32];
	int afirst = 0, qfirst = 0;

	restart = 0;
	found = 0;
	points = 0;
	(void) memset(foundlist, 0, sizeof(foundlist));

	while (1) {
		(void) putp(clear_screen);

		print_board();

		if (qfirst == 0 && !strcmp(rank(), "Queen Bee!")) {
			(void) printf("You have earned the rank of Queen Bee");
			(void) printf(" and won the game!\n");
			(void) printf("You may continue playing. Can you ");
			(void) printf("find all the words?\n\n");

			qfirst = 1;
		}

		if (afirst == 0 && found == count) {
			(void) printf("Congratulations! You found all the ");
			(void) printf("words!\n\n");

			afirst = 1;
		}

		(void) printf("? to shuffle\n");
		(void) printf("! to display found words\n");
		(void) printf("& to display answers\n");
		(void) printf("%% to start a new game\n");
		(void) printf("; to quit\n\n");

		(void) printf("Enter a word: ");
		(void) fgets(guess, sizeof(guess), stdin);
		guess[strlen(guess) - 1] = '\0';

		if (guess[0] == '\0')
			continue;

		if (guess[0] == ';')
			break;

		if (guess[0] == '?') {
			shuffle();
			continue;
		}

		if (guess[0] == '!') {
			show_found();
			continue;
		}

		if (guess[0] == '&') {
			show_answers();
			continue;
		}

		if (guess[0] == '%') {
			restart = 1;
			break;
		}

		if (check(guess) == 0)
			continue;

		find_word(guess);
	}
}

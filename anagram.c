/*
 * Copyright (c) 2020-2022, 2024 Brian Callahan <bcallah@openbsd.org>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <term.h>

#include "freebee.h"
#include "version.h"
#include "wordlist.h"

/*
 * The seventh letter is the center letter.
 * All valid words must have at least 1 occurance of it.
 */
void
create_anagrams(void)
{
	int notfirst = 0, pangram;
	int one, two, three, four, five, six;
	int loop;
	size_t i, j, yes;
	ssize_t special;

	/*
	 * Start by generating 7 distinct letters.
	 */
again:
	for (i = 0; i < 7; i++) {
bad:
		letters[i] = (unsigned char) arc4random_uniform(26) + 'a';
		for (j = 0; j < i; j++) {
			if (letters[j] == letters[i])
				goto bad;
		}
	}

	if (!notfirst) {
		++notfirst;
		putp(clear_screen);
		printf("Free Bee %s\n\n", VERSION);
		printf("Creating game, please wait...\n");
	}

	/* Create anagrams.  */
	(void) memset(words, 0, sizeof(words));
	total = 0;
	pangram = 0;
	count = 0;
	for (loop = 0; loop < 101359; ++loop) {
		j = 0;
		yes = 0;
		special = 0;
		one = two = three = four = five = six = 0;
		while (wordlist[loop][j] != '\0') {
			for (i = 0; i < 7; i++) {
				if (wordlist[loop][j] == letters[i]) {
					++yes;
					switch (i) {
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
						break;
					case 6:
						special = 1;
					}
					break;
				}
			}
			++j;
		}
		if (special && (strlen(wordlist[loop]) == yes)) {
			(void) strlcat(words[count], wordlist[loop],
			    sizeof(words[count]));
			/* More than 60 words is not a good game.  */
			if (++count > 60)
				goto again;
			if (special && one && two && three && four && five
			    && six) {
				pangram = 1;
				total += strlen(wordlist[loop]) + 7;
			} else {
				if ((i = strlen(wordlist[loop])) == 4)
					i = 1;
				total += i;
			}
		}
	}

	/* Not a good game, try again.  */
	if (!pangram || count < 20)
		goto again;
}

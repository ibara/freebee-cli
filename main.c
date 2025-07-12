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

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <term.h>
#include <unistd.h>

#include "freebee.h"
#include "version.h"

char *term;
char foundlist[60][18], words[60][18];
char letters[7];

int cols, rows;
int restart;

int count, found, points, total;
int newbie, novice, fine, skilled, excellent, superb, marvellous;
int outstanding, queen;

int
main(int argc, char *argv[])
{
	int status;

	(void) argc, (void) argv;

	if ((term = getenv("TERM")) == NULL)
		err(1, "getenv");
	setupterm(term, 1, &status);
	if (status == -1)
		errx(1, "setupterm");

#ifdef HAVE_PLEDGE
	if (pledge("stdio", NULL) == -1)
		err(1, "pledge");
#endif /* HAVE_PLEDGE */

	if ((cols = tgetnum("co")) == -1)
		cols = 80;
	if ((rows = tgetnum("li")) == -1)
		rows = 24;
	if (cols < 25 || rows < 20)
		errx(1, "terminal too small!");

	restart = 1;
	while (restart == 1) {
		create_anagrams();
		set_rank();
		play_game();
	}

	return 0;
}

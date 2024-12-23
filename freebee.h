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

extern char *term;
extern char foundlist[60][18], words[60][18];
extern char letters[7];

extern int cols, rows;
extern int restart;

extern int count, found, points, total;

/* Score ranks  */
extern int newbie, novice, fine, skilled, excellent, superb, marvellous;
extern int outstanding, queen;

extern const char *rank(void);

extern void create_anagrams(void);
extern void play_game(void);
extern void set_rank(void);

#ifndef HAVE_ARC4RANDOM_UNIFORM
extern unsigned int arc4random_uniform(unsigned int);
#endif /* !HAVE_ARC4RANDOM_UNIFORM */

#ifndef HAVE_STRLCAT
extern size_t strlcat(char *, const char *, size_t);
#endif /* !HAVE_STRLCAT */

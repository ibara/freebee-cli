Free Bee
========
Free Bee is an enhanced Free Software clone of The New York Times game
Spelling Bee.

In this game, your goal is to find as many words as you can with the seven
letters you are given. You don't have to use any letter except the middle
letter and letters can be used more than once in a word. Finding a word
that uses all seven letters yields bonus points! Every game has at least
one such word, and many games have multiple.

Earning enough points to reach the rank of Queen Bee wins the game!

Free Bee offers a nearly infinite number of computer-generated random
challenges. Unlike the original game, you are not restricted to only a
single game per day!

The only notable difference between Free Bee and the original game is the
use of the open source
[Wordnik wordlist](https://github.com/wordnik/wordlist)
instead of The New York Times wordlist.

Dependencies
------------
Free Bee depends on `ncurses` for screen handling routines.

Compiling
---------
```sh
$ ./configure
$ make
$ sudo make install
```

License
-------
ISC License. See
[`LICENSE`](https://github.com/ibara/freebee-cli/blob/main/LICENSE)
for details.

The Wordnik wordlist is
[MIT licensed](https://github.com/wordnik/wordlist/blob/main/LICENSE).

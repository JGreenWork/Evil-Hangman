# Evil-Hangman

This game was the final project for our CS102 class.  Your goal was the use the string and vector classes we had built
as the basis for the game of evil hangman.

The main premise is that the game cheats.  It keeps a dictionary of words stored as an AVL tree, and when you guess a letter,
it will intentionally choose a branch that does not contain that letter, if possible.

It's possible to win by giving yourself a lot of guesses, or by playing on easy mode (or both!).  Otherwise, you're pretty screwed.

You could store the dictionary was a linked list, which would get you a C, or an AVL tree (self-balancing binary search tree), which
would get you an A.  I first used the linked list to make sure I had the logic of the game correct, and then implemented the
AVL tree.

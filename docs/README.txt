This is a Tetris-clone practice program that I made in 2006.

I originally developed in Python, then ported the logic to
several others languages (VB, C#, C++, Java).  This is the
C++ port.   My main port of this example is an approach to
separate out the "business-logic" (processing rules) away
from any GUI good.   This makes the "game-processing" much
easier to port across systems and GUI libraries.

This is also a reasonable way to demonstrate how to implement
Tetris-like puzzle pieces, rotations, and overlaying them onto
a grid (game board).


Sub-folders:

\build		Source directory
		Contains:
		  BCB6 console build project (Blockmania_b6.bpr)
		  BCB6 GUI build project (Blockmania_GUI_b6.bpr)
		  vc2005 console project (Blockmania_console.sln)

\bin		Binary directory
		Contains:
		  BCB6 console and GUI built executables
		  DLL and BPL requires by BCB6 to execute

		NOTE: BCB6 project will output to this folder
		automatically.  The VC2005 project outputs
		to its Debug sub-folder.


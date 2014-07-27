nTowers
=======

Towers for the TI-NSpire calculator

About:
This is a game for the TI-nSpire!  It works on the TI-NSpire CX CAS with ndless 3.1 and should work on the TI-nspire models.  It is a puzzle game.  In each level you need to fill the board with numbers.  Each number represents a building of that height.  For an N by N board the numbers are 1 through N.  In each column and in each row are the numbers 1 through N.  The numbers on the side indicate that from the direction looking into the board are that many visible buildings.  So if you have a building of height 3 and if there is a building of height 2 behind it, then the building of height 2 is obsecured.  If there were a building of height 4 behind the building of height 2 then you could see the building of height 3 and the building of height 4 from that view point.

Using the side numbers as clues it is your goal to fill out the entire board.  I hope you enjoy it!

How to put on calculator:
Start up TI-NSpire Student Software or whatever you use to put it on the calculator.  Then just drag-n-drop the nTowers-sdl.tns file into the application to transfer it to your calculator.  You have to have ndless 3.1 on your calculator in order to run it!

After you run it; you can add and play additional levels.  With ndless it allows start up files; and so you can add more level files to your calculator and play them!

How to play:
Navigate the menu using the arrow keys (or 2 and 8 keys) and the Enter key to start a new game.
Use the arrow keys to move the selector around the board
Type the number or cycle through the numbers with the Enter (or Ctrl) key.
Use the - and + keys to undo and redo your moves.

Extra levels:
This program works with external level files!  There is only a sample level included.  See the source code file PuzzleLevels.h to see the make-up of the puzzle files should you want to make your own.  Extra level files which can be placed in any folder on the calculator.  A subfolder is a great place to put them.  When you first ran nTowers-sdl it registered the extension 'towers' and so because the levels have the extension 'towers' at the end it is associated with the nTowers-sdl program.  Let me know if you'd be interested in a level editor or additional levels!  The make-up of a level is rather simple; and so if you take a look at the source and/or the level file you'll probably be able to figure it out.

Source:
I included my source.  I programmed with C++ which you can find out more here: http://blog.tangrs.id.au/?p=712  It uses nSDL as well as the ndless Software Development Kit.

Contact Me:
A.J. Orians
E-mail: ajorians@gmail.com

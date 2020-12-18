# CandyCrushSagaGame_Allegro5

# OVERVIEW

The game is mostly based on the Candy Crush Saga Mobile.Candies show up in different colors and shapes.The basic logic of gameplay is in the form of drag and leave.The score is incremented after every move by 2 to the power of  the number of candies that exploded that turn.

# WHY DID WE USE ?
stdio.h: To use basic structures.
stdlib.h: To use arrays.
time.h: To give random candies.
allegro.h: To use Allegro’s main functions such as allegro display, allegro event system, etc.
primitives.h: To draw shapes like triangle, circle etc.
font.h: To create text lines on screen.
ttf.h: To use the text type that we want.
math.h: To calculate points.

# ADDONS

Primitives Addon: It used for ball and hoop drawing. 
Font and TTF Addons: It used to incorporate the Arial Font Type into our project.
Color Addon: It used for coloring candies, background, fonts and etc.

# FUNCTIONS

main: it contains the starting routines that are responsible for initiating necessary allegro functions which are essential for the formation of the program. Additionally the call of the functions that are responsible for making the program work without errors until the maximum movement count is also present at the main.

anaEkran: It draws background screen, types the numbers of remaining moves and the score.

clear_: It clears candies which are need to be clear.

controlUnit: It checks whether there is or there isn't a mistake of any kind before the game begins.

distance: In this function, the Pisagor distance gets calculated between the two elements of the matrix.

draw_candy: At this function candy gets drawn according to its type with respect to its right color and size.

generateRandomCandy: In this function, a random number from one to four which represents a geometric number each is generated. Each value of a geometric form i assigned by a definition at the very beginning of the file.

getCell: Calculations are made after the move has been made in order understand the case of the cell.

matrixTamamlayici: With this function, indice of matrix gets checked and if it is equal to 0 indice gets regenerated.

matrixYazici: It locates generated matrices at backend.

matrix_Update: It refilled matrices and it work together with “matrixTamamlayici”.

offset_Update: In this function, game matrix gets changed completely, if the element is not active its type gets changed as 0, that way its color becomes equal to black.

pause_: It stops timer.

processMatrix: In this function, all candy sequences gets checked and if it is a crush these sequences should be deleted by calling the clear_ function.Additionally score change can be done considering the state of the present matrix.

sign_: The source and the target elements get changed with the help of a variable.
Additionally, quantity of movements get reduced and an audio file is played in order to help user understand the operation.

startGame: It starts game and draw first candies randomly.

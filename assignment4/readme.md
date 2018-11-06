# Astroids Game
- compilation code : present in the file called 'compile'  

## brief :  
this is an advanced model of the previous one made in assignment 3.  
this one is made with GLUT, and adds some textures and lighting to the game as needed,  
it's a basic game with a space ship trying to avoid crashing with astroids,
the score is calculated by the number of seconds you manage to survive, my maximum score was 700+ not much,  
but don't think it's going to be easy to reach a 1000 :).  

## changes :  
- added a pausing mode : so you can stop the game and come back when ever you wish with your score untouched.  
- improved the interface with textures of astroids, space ship, and back ground space.  
- changed the controls of motion  
	- **'6'** : 6 from key pad goes right
	- **'4'** : 4 from key pad goes left
	- **ENTER** : continue playing when the game is either stoped(game over), or paused
	- **'p'**   : pause the game
- changed the project structure to make it more clean
	- **class SceneManager** : this class is resposible for initilaizing GLUT,  
	loading textures, writing text to the screen, drawing different objects in the scene.
	- **class GameManager** : this class is the controller for the game rules,  
	it specifies keyboard handlier, resize handler, some timers, and the display function which displays  
	the different screens in the game, while manipulating the functions provided by SceneManager for display details.  

## notes :  
- GLUT functions like (glutDisplayFunc) can't take a function of an object as its argument, it must be  
either a function (not declared within a class), or a static method, so in order to get my oop to work;  
i had to add some functions in main file to be used like a bridge to update my global game environment indirectly.
- the "ENTER" button in my code has 2 functions depending on the current game_state, unfortunately when the user  
presses it; it would trigger the first event correctly, the as the code is too fast, it will take the continuing press  
as a new press triggering the other functionality which is wrong, in order to avoid that  
a state variable *interruptable* was added to the GameManager class, when its value is 0; it means i can change  
the current *game_state*, but when it's 1, then i can't change *game_state*, this is the first part of the solution.  
the second part is to whenever we a key is presses (e.g. ENTER) we check if the current state is **interruptable == 0**  
if it isn't the case then we can't change a thing and simply retrun.  
if we can make an interrupt, i change the current game_state to the target one, *set interruptable = 1*  
and *add a timer interrupt is added*, this timer would wait for about 100ms or so, then *set interruptable = 0* again.  

## BUGS :  
there is still some problem with the coloring of the program and its texture, here is the deal,  
it's seems like initialization related problem.
to be more exact with **SceneManager::glut_init()** in the following line
	
	//glEnable(GL_COLOR_MATERIAL);

when this line is commented :  
texture is displayed correctly but colors wouldn't display correctly.   
when this line is un-commented:  
colors would be displayed correctly, but texture would have its colors messed up (colors get alot darker).  


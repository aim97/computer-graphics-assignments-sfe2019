# openGL-glut
## essential libraries :
for apple : <OpenGL/OpenGL.h>, <GLUT/glut.h>  
for any thing else : <GL/glut.h>  
### openGL initialization
write the following code as the first thing in main function, it's just initialization to openGL 
      
    //Initialize GLUT  
    glutInit(&argc, argv);  // argc and argv are command line arguments  
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  
    glutInitWindowSize(400, 400); //Set the window size (width, height)  
      
    //Create the window  
    glutCreateWindow("window title"); // this sets the title of the program window  
    initRendering(); //Initialize rendering (this is not a built-in function)  

the implementation is as follows  
    
    void initRendering() {  
        //Makes 3D drawing work when something is in front of something else  
        glEnable(GL_DEPTH_TEST);  
    }  
    
### openGL the real deal
this part should go after the initialization in the program main function.
here is where you really need to think about what you are doing, openGL provides some function to handle interrrupts  

1 - glutKeyboardFunc(handleKeypress):  
    **role** : this one handles the event of key being pressed on mouse click.  
    **parameters** : a pointer to a function like this one, it must have the same return type and parameters types,  
    and same applies to the rest of the functions to follow.
    
    //Called when a key is pressed
    // the parameters here will come from openGL itself, so you don't need to worry where or how they will be filled.
    void handleKeypress(unsigned char key, //The ascii code of the key that was pressed
                        int x, int y) {    //The current mouse coordinates
        // handling code goes here
        ...
    }
    
2 - glutReshapeFunc(handleResize):  
    **role** : this function handles the event of user trying to resize the program window  
    **parameters** : a pointer to a function like this one.  
    
    //Called when the window is resized
    // this function makes sure to resize the shapes in the drawing area when the window is resized with the same ratio.
    // w and h are the new width and height and will be passed to this function by openGL 
    void handleResize(int w, int h) {
        //Tell OpenGL how to convert from coordinates to pixel values
        glViewport(0, 0, w, h);

        glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

        //Set the camera perspective
        glLoadIdentity(); //Reset the camera
        gluPerspective(45.0,                  //The camera angle
                       (double)w / (double)h, //The width-to-height ratio
                       1.0,                   //The near z clipping coordinate
                       200.0);                //The far z clipping coordinate
    }
    
    
3 - glutTimerFunc(unsigned int msecs, void (*func)(int value), value):  
    **role** : this function is responsible for running a function AFTER a specific number of seconds. (handles a timer interrupt)  
    **parameters** :  
    -- msecs : the number of milliseocnds to wait (wait not sleep) before running the given function  
    -- func  : pointer to a function that takes an integer input value and output nothing, this is the function to be called to handle the interrupt  
    -- value : the parameter to be sent to this function when calling it.  
    **note**  : this function calls the given function AFTER the given time period is passed only once,  
              not periodically, in order to make it periodic you have to call it again from with the given   
              function **func**.  
                     
### openGL display function
this is where you should be making the magic, openGL provides a function **glutDisplayFunc** that takes the function you  
want to be responsible for the display of your program window content.  
it's called as  

    glutDisplayFunc(drawScene);
                    
                    
the display or draw scene function itself is what you need to worry about, becuase this is where all the drawing of your  
window content happen, it should be something like this  

      //Draws the 3D scene
      void drawScene() {
            //Clear information from last draw
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
            glLoadIdentity(); //Reset the drawing perspective
            
            // drawing code should go here.
            ...
            
            glutSwapBuffers(); //Send the 3D scene to the screen
      }

now with just that your code won't draw any thing because simply you didn't tell to actually draw any thing (drawscene isn't called yet).  
we make it called using **glutMainLoop()** function, which is simply an infinite loop, during its first iteration it calls
drawScene function (bcuase this is the one given to glutDisplayFunc) once, then as long as the scene is not changed,  
and you didn't ask it to redraw it, it just won't do anything, and from this point forward your code will be mainly be  
depending on: interrupts that are handled like the above ones, and re-drawing the scene by calling **glutPostRedisplay()**
when this function is called (probably from an interrupt handling function like **handlekeypress**) it says the following to 
**glutMainloop** : *"when this interrupt handling function finish, go and call the display function again becuase it seems 
like some variables are updated here and we want their effect to actually change some stuff in the display behaviour"*
and simply in the next iteration when the handling function does its job the **glutMainLoop** will call the drawscene 
function agian.
the glutMainLoop is called at the end of the main function with nothing after it (because it's an infinite loop anything 
after it won't be executed).

      glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
      return 0; //This line is never reached, but just add it so that the compiler wouldn't complain

### a brief on some openGL function
- [glEnable(GL_DEPTH_TEST)](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnable.xml): this function is called inside the initRendering function we just made, it initializes our 
rendering parameters, now all it does is to make sure an object shows up behind an object in front of it that has already 
been drawn, which we want to happen.  
  
- [gluPerspective(fovy, aspect, Znear, Zfar)](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml) : this function specifies how the user should watch the scene.  
fovy : controls your perspective angle.  
aspect : width to height ratio, ratio between current width and height of window, or the ratio between x and y scales
Znear : this makes any thing close to the user at position of z less than Znear invisible, so as it doesn't fill the screen.
Zfar : this one removes all objects further than Zfar, because normally you wouldn't see them.

## how drawing works on openGL

### openGL transformations (translation, rotation, and scaling)

### openGL writing text

### openGL light effects

### openGL texture





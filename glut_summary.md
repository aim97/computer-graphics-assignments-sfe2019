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

1 - **glutKeyboardFunc(handleKeypress):**  
    **role** : this one handles the event of key being pressed on mouse click.  
    **parameters** : a pointer to a function like this one, it must have the same return type and parameters types,  
    and same applies to the rest of the functions to follow.
    
    //Called when a key is pressed
    // the parameters here will come from openGL itself, so you don't need to worry 
    // where or how they will be filled.
    void handleKeypress(unsigned char key, //The ascii code of the key that was pressed
                        int x, int y) {    //The current mouse coordinates
        // handling code goes here
        ...
    }
    
2 - **glutReshapeFunc(handleResize):**  
    **role** : this function handles the event of user trying to resize the program window  
    **parameters** : a pointer to a function like this one.  
    
    //Called when the window is resized
    // this function makes sure to resize the shapes in the drawing area  
    // when the window is resized with the same ratio.
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
    
    
3 - **glutTimerFunc(unsigned int msecs, void (\*func)(int value), value):**  
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
  
- [glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glClear.xml) : this function is has an important role as it's the one responsible for clearing the screen before you start drawing any thing so it's critical to have it at the start of the display function to make sure it does its job.  

- [glutSwapBuffers()](https://www.opengl.org/resources/libraries/glut/spec3/node21.html): this function Sends the 3D scene to the screen, this means without adding it at the end of the display function nothing will display, so you may want to remember this as well.

- [glColor](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glColor.xml) : glColor is not the function name, 
but it's more like a family of functions sharing that prefix and used to specify the current color using by openGL, this  
color will be used by all drawing blocks after it until another one is specified.  
it takes color value as input which can be rgb or rgba depending on what function you call.  
for example :  
glColor3f(): takes 3 float parameters whose values range from 0.0 to 1.0 for red, green, and blue values in order.  
glColor4ui(): takes 4 uint parameters whose values range from 0 to 255 for red, green, blue, and alpha values in order.  
glColor4fv(): takes a pointer to a float array of size 4, holding the values of rgba of the color in order.  
generally the function name should be something like glColor\[3/4]\[data_type]
where 3 and 4 depends on how you want to represent the color rgb or rgba.
and data_type is either how you want to enter the color values: f->float, ui->uint, s->short ...etc

## how drawing works on openGL
the code to draw shapes with openGL is a block like code, it starts with **gLBegin** and ends with **glEnd**, and between 
those two you may add the vertices of the shape you want to draw, this is its simplest form.
      
    glBegin(GL_QUADS); //Begin quadrilateral coordinates
    
    //Trapezoid
    glVertex3f(-0.7f, -1.5f, -5.0f);
    glVertex3f(0.7f, -1.5f, -5.0f);
    glVertex3f(0.4f, -0.5f, -5.0f);
    glVertex3f(-0.4f, -0.5f, -5.0f);
    
    glEnd(); //End quadrilateral coordinates
    
- it's not only vertices that can go between begin and end, you can make loops and other code as well including 
other openGL function normally, but mainly vertices are what matters here.
- make sure the z value is between zNear and Zfar specified by the gluPerspective function so that it can be seen.
- as you see above the parameter sent to glBegin() is the one determining the types of shapes to be drawn within that
drawing block, which in this case quadrilateral shapes (GL_QUADS), other shapes can be drawn as well like.  
  - GL_POINTS for points
  - GL_LINES for lines
  - GL_LINE_STRIP for drawing a line string passing through all points in order
  - GL_LINE_LOOP for drawing a line string passing through all points in order and ends at its start
  - you can check the rest of its [documentation](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glBegin.xml).
- drawing blocks doesn't draw one object only like one quadrilateral, or one line, it depends on how many vertices you  
provide to it, for example if you are using GL_QUADS: it will group each consecutive 4 vertices into 1 quadrilateral, so
if you provide 16 vertices it will draw 4 quadrilaterals.

### openGL transformations (translation, rotation, and scaling)
in those transformations we are not actually moving the shapes them selves but the coordinates we are drawing on,
it's done by 3 transformation functions as follows:
- [glTranslate](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glTranslate.xml):  
  glTranslatef (for float parameters), and glTranslated (for double parameters), this function is used to translate the
  axis from (0, 0, 0) to the given coordinates, each of them takes 3 parameters for new origin x,y, and z respectively.

- [glRotate](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glRotate.xml):
  glRotatef (for float parameters), and glRotate (for double parameters), this function is used to rotate the
  axis with a given angle around the given vector, it takes 4 parameters : angle of rotation, and x, y & z values for the 
  vector to rotate around it.
  
- [glScale](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glScale.xml):
  glScalef (for float parameters), and glScaled (for double parameters), this function is used to scale the
  axis respictive lengths, it takes 3 parameters, the scale values in all x, y, and z.
  
as stated above those functions only affect the axes, so how will we use them to move shapes, we will simply have to do
the following steps
1 - apply transformations
2 - draw shape
3 - reverse transformations

in code 

      glPushMatrix(); // this function stores the current state of axes, ..etc to stack
      // apply transformations
      ...;
      // draw shapes that we want to apply those tranformations to them
      ...;
      glPopMatrix();
      
- [glPushMatrix()](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml): it's called to store  
the current state of our axes.

- [glPopMatrix()](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml): it's called to retrieve 
the previous state of our axes.

* by those two functions used to envelope our transformations in between make sure the transformations we made will only 
affect the objects drawn within that block only, and will be automatucally discarded outside the block.
* this way we can draw all shapes being drawn at the origin initially without caring about size of location then we can 
generally draw the same object over and over, and at each time have it moved to the correct spot, with the needed 
orientation and scale.
* it's also worth mentioning that the order in which the transformations occur actually matter at least for translation, 
usually you want to translate first then rotate for example which won't give the same output if order is reversed, but as 
for rotation and scaling the order between them doesn't actually matter.  

### openGL writing text
openGL provides some useful functions for writing text on screen, not many font types though but it should do,  
for now we will stick to this function here which simply writes the given string s to the specified location  
using the specified font.  
you may find those links interseting :  
- [glutBitmapCharacter](https://www.opengl.org/resources/libraries/glut/spec3/node76.html)
- [text section repo](https://github.com/HalaBadr/Computer_Graphic/tree/master/Text?fbclid=IwAR1WaiRae10wUcUqMBqH8Q1j0NztYjw2Xu78rP__G7cnl8XlPi5jI-AzcQ4)
- [seems useful](http://jeromebelleman.gitlab.io/posts/devops/gltext/)

      void renderbitmap(float x, float y, float z, void *font, string s) {
            glRasterPos3f(x, y, z);
            for (int i = 0; i < s.size(); i++) {
                  glutBitmapCharacter(font, (int)s[i]);
            }
      }
      
### openGL light effects

### openGL texture





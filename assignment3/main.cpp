#include <GL/glew.h>
#include <GL/glut.h>
#include <bits/stdc++.h>
#include "imageloader.h"
#include <GLFW/glfw3.h>
using namespace std;

#define point pair<float, float>
#define X first
#define Y second
#define mp make_pair
#define pb push_back


// GAME STATES
#define RUNNING 0
#define STOPED 1
#define PAUSED 2

// astroids motion rates
#define ASTROID_STEP_TIME 5
#define ASTROID_ADD_TIME 15
#define inter_transition_period 5

//The id of the texture
GLuint background_texture, astroid_texture, ship_texture; 
int ITP = inter_transition_period;
// creating the window pointer
int game_state = RUNNING;
GLFWwindow *window;

float screenWidth = 700;
float screenHieght = 700;

// astroids related data
vector<point> astroids;
int astroid_step = ASTROID_STEP_TIME, astroid_new = ASTROID_ADD_TIME;

// player related data
point player_location = mp(0.0, -0.8);
unsigned long long score = 0;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void drawCircle(float);
void error_callback(int error, const char* description);
void draw_astroid();
void game_screen();
void stop_screen();
void pause_screen();
void handle_lighting(float i);
void renderbitmap(float x, float y, float z, void *font, string s);
void positions_update(int v = 1);
void add_astroids(int v = 1);
void reset_game();
void back_ground();
void initRendering();
GLuint loadTexture(Image* image);
GLuint loadTextureA(Image* image);

int main(int argc, char** argv) {
	srand( (unsigned)time( NULL ) );
	//Initialize GLUT  
	glutInit(&argc, argv);  // argc and argv are command line arguments  
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	//initilize library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR : glfw couldn't be initilized\nprogram terminated\n");
		return -1;
	}

	// setting call error back handler for glfw
	glfwSetErrorCallback(error_callback);

	// --------------------------------------------------------------------------------------
	// setting the necessary versions of openGL used when compiling and executing the program
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_ALPHA_MASK, GL_TRUE);
	// ---------------------------------------------------------------------------------------

	// ---------------------------------------------------------------------------------------
	//Create Window
	window = glfwCreateWindow(screenWidth, screenHieght, "Astroids", NULL, NULL);
	if (!window){
		fprintf(stderr, "ERROR : program window couldn't be rendered\nprogram terminated\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback); // set the key press handler for created window
	//glfwWindowHint(GLFW_DEPTH_BITS, GL_TRUE);
	// ---------------------------------------------------------------------------------------
	
	glViewport(0.0f, 0.0f, screenWidth, screenHieght); //draw openGl in pixels 
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	initRendering();
	
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		back_ground();
		switch(game_state){
			case RUNNING:
				game_screen();
				break;
			case STOPED:
				stop_screen();
				break;
			case PAUSED:
				pause_screen();
				break;
		}
		ITP = max(0, ITP-1);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

void drawCircle(float r) {
	float a;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++){
		a = i * 3.142 / 180;
		glVertex3f(r*cos(a), r*sin(a), 0.0);
	}
	glEnd();
}

void draw_astroid(){
	glPushMatrix();
	glTranslatef(0.0,0.17,0.0);
	glColor4f(0.5f, 0.5f, 0.0f, 0.2f);
	drawCircle(0.01);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0,0.13,0.0);
	glColor4f(0.5f, 0.5f, 0.0f, 0.4f);
	drawCircle(0.02);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0,0.06,0.0);
	glColor4f(0.5f, 0.5f, 0.0f, 0.7f);
	drawCircle(0.05);
	glPopMatrix();

	glColor4f(0.5f, 0.5f, 0.0f, 0.8f);
	drawCircle(0.08);
	
	glColor3f(0.5f, 0.0f, 0.0f);
	drawCircle(0.05);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, astroid_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	float x = 0.05;
	//glColor4f(0.1, 0.0, 0.1, 0.5);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);glVertex3f(-x, x, 0);
	glTexCoord2f(1.0, 1.0);glVertex3f(x, x, 0);
	glTexCoord2f(1.0, 0.0);glVertex3f(x, -x, 0);
	glTexCoord2f(0.0, 0.0);glVertex3f(-x, -x, 0);
	glEnd();


	glDisable(GL_TEXTURE_2D);
}

void draw_player(){
	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.0, 0.5);
	glVertex3f(-0.07, 0, 0);
	glVertex3f(0.07, 0, 0);
	glVertex3f(0, 0.03, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-0.02, 0, 0);
	glVertex3f(0.02, 0, 0);
	glVertex3f(0, 0.1, 0);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ship_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	float x = 0.05;
	glColor4f(0.1, 0.0, 0.1, 0.5);
	
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);glVertex3f(-x, x, 0);
	glTexCoord2f(1.0, 1.0);glVertex3f(x, x, 0);
	glTexCoord2f(1.0, 0.0);glVertex3f(x, -x, 0);
	glTexCoord2f(0.0, 0.0);glVertex3f(-x, -x, 0);
	glEnd();


	glDisable(GL_TEXTURE_2D);
}


void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }else if(key == GLFW_KEY_RIGHT){
    	player_location.X = min(player_location.X + 0.02, 0.9);
    }else if(key == GLFW_KEY_LEFT){
    	player_location.X = max(player_location.X - 0.02, -0.9);
    }else if((key == GLFW_KEY_KP_ENTER || key == GLFW_KEY_ENTER) && !ITP){
    	if(game_state == PAUSED){
    		game_state = RUNNING;
    	}else{
    		reset_game();
    	}
    	ITP = inter_transition_period;
    }else if(key == GLFW_KEY_P && game_state == RUNNING && !ITP){
    	game_state = PAUSED;
    	ITP = inter_transition_period;
    }
}

void error_callback(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
}

void renderbitmap(float x, float y, float z, void *font, string s) {
	glRasterPos3f(x, y, z);
	for (int i = 0; i < s.size(); i++) {
		glutBitmapCharacter(font, (int)s[i]);
	}
}


void game_screen(){
	// update step
	if(astroid_new > 0)astroid_new--;
	else{
		add_astroids();
		astroid_new = ASTROID_ADD_TIME;
	}

	if(astroid_step > 0) astroid_step--;
	else{
		positions_update();
		astroid_step = ASTROID_STEP_TIME;
	}

	// update and display score
	score ++;
	char buf[20];
	sprintf(buf, "%lld", score/25);
	//printf("%lld\n", score);
	glColor4f(1.0, 1.0, 0.0, 1.0);
	renderbitmap(0.8,0.8,0, GLUT_BITMAP_TIMES_ROMAN_24, buf);

	// display astroids
	for(auto i : astroids){
		glPushMatrix();
		glTranslatef(i.X, i.Y, 0.0);
		draw_astroid();
		glPopMatrix();
	}

	// display player vehicle
	glPushMatrix();
	glTranslatef(player_location.X, player_location.Y, 0.0);
	draw_player();
	glPopMatrix();
}

void stop_screen(){
	glColor4f(1.0, 1.0, 0.0, 1.0);
	char buf[30];sprintf(buf, "score : %lld", score / 25);
	renderbitmap(-0.2, 0.1, 0, GLUT_BITMAP_TIMES_ROMAN_24, buf);
	
	glColor4f(1.0, 1.0, 1.0, 1.0);
	renderbitmap(-0.2,0,0, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER");
	glColor4f(0.0, 1.0, 0.0, 1.0);
	renderbitmap(-0.5,-0.1,0, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS ENTER TO TRY AGAIN");
	renderbitmap(-0.5,-0.2,0, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS ESC TO EXIT");
}

float distance(point a, point b){
	return sqrt((a.X-b.X)*(a.X-b.X) + (a.Y-b.Y)*(a.Y-b.Y));
}

void positions_update(int v){
	// update astroids positions
	switch(game_state){
		case RUNNING:
			for(auto i = astroids.begin();i != astroids.end();i++){
				i->Y -= 0.07;
				if(distance(player_location, *i) < 0.1){
					game_state = STOPED;
					break;
				}
				if(i->Y < -0.9)astroids.erase(i);
			}
			break;
		case STOPED:
			break;
	}
}

void add_astroids(int v){
	point ast = mp(rand() * 2.0 / RAND_MAX - 1.0, 1.0);
	astroids.pb(ast);
}


void reset_game(){
	astroid_step = ASTROID_STEP_TIME;
	astroid_new = ASTROID_ADD_TIME;
	astroids.clear();
	player_location = mp(0.0, -0.8);
	score = 0;
	game_state = RUNNING;
}

void handle_lighting(float i){
	GLfloat ambientLight[] = {1.0f, 0.0f, 1.0f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	GLfloat directedLight[] = {i, i, i, i};
	//GLfloat directedLightPos[] = {-10.0f, 15.0f, 20.0f, 0.0f};
	GLfloat directedLightPos[] = {0.0f, 0.0f, 1.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);
}

void pause_screen(){
	handle_lighting(0.3);
	renderbitmap(-0.5,0,0, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS \'ENTER\' to continue");
}

void back_ground(){
	handle_lighting(0.8f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, background_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	float x = 1.0;
	//glColor4f(0.1, 0.0, 0.1, 0.5);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);glVertex3f(-x, x, 0);
	glTexCoord2f(1.0, 1.0);glVertex3f(x, x, 0);
	glTexCoord2f(1.0, 0.0);glVertex3f(x, -x, 0);
	glTexCoord2f(0.0, 0.0);glVertex3f(-x, -x, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}


// adding some functions to handle textures and such stuff
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

GLuint loadTextureA(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGBA,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGBA, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void initRendering() {
	//glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	//glEnable(GL_COLOR_MATERIAL);
	
	Image* image = loadBMP("space.bmp");
	background_texture = loadTexture(image);
	//delete image;

	image = loadBMP("astroid.bmp");
	astroid_texture = loadTexture(image);

	image = loadBMP("ship.bmp");
	ship_texture = loadTexture(image);
	delete image;
}

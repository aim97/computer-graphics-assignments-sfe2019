#include "sceneManager.h"

// private member functions
void SceneManager::draw_circle(float r) {
	/**
	role : draw a circle with the given radius and center at 0.0
	in   : r > float, the radius of the circle to be drawn
	out  : void 
	*/
	float a;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++){
		a = i * 3.142 / 180;
		glVertex3f(r*cos(a), r*sin(a), 0.0);
	}
	glEnd();
}

GLuint SceneManager::loadTexture(Image* image) {
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


// public static member functions
void SceneManager::glut_init(){
	glEnable(GL_DEPTH_TEST); // allow depth
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // make sure to allow blinding for 4D colors
	glEnable(GL_BLEND); // enable blinding 
	glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	
	// read the required textures
	Image* image1 = loadBMP("space.bmp");
	GLuint background_texture = loadTexture(image1);
	tex[BACKGROUND_TEX] = background_texture;
	delete image1;

	Image* image2 = loadBMP("astroid.bmp");
	GLuint astroid_texture = loadTexture(image2);
	tex[ASTROID_TEX] = astroid_texture;	
	delete image2;

	Image* image3 = loadBMP("ship.bmp");
	GLuint ship_texture = loadTexture(image3);
	tex[SHIP_TEX] = ship_texture;
	delete image3;
}

void SceneManager::draw_astroid(){
	float astroid_raduis = 0.2;

	glPushMatrix();
	glTranslatef(0.0,0.70,-0.05);
	glColor4f(0.5f, 0.5f, 0.0f, 0.2f);
	draw_circle(astroid_raduis * 0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0,0.45,-0.04);
	glColor4f(0.5f, 0.5f, 0.0f, 0.4f);
	draw_circle(astroid_raduis * 0.4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0,0.15,-0.03);
	glColor4f(0.5f, 0.5f, 0.0f, 0.7f);
	draw_circle(astroid_raduis);
	glPopMatrix();
	

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[ASTROID_TEX]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);glVertex3f(-astroid_raduis, astroid_raduis, 0);
	glTexCoord2f(1.0, 1.0);glVertex3f(astroid_raduis, astroid_raduis, 0);
	glTexCoord2f(1.0, 0.0);glVertex3f(astroid_raduis, -astroid_raduis, 0);
	glTexCoord2f(0.0, 0.0);glVertex3f(-astroid_raduis, -astroid_raduis, 0);
	glEnd();

	glColor4f(0.5f, 0.5f, 0.0f, 0.8f);
	draw_circle(astroid_raduis * 1.4);
	
	glColor3f(0.5f, 0.0f, 0.0f);
	draw_circle(astroid_raduis);


	glDisable(GL_TEXTURE_2D);
}

void SceneManager::draw_space_ship(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[SHIP_TEX]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	float x = 0.25;
	glColor4f(0.1, 0.0, 0.1, 0.5);
	
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);glVertex3f(-x, x, 0);
	glTexCoord2f(1.0, 1.0);glVertex3f(x, x, 0);
	glTexCoord2f(1.0, 0.0);glVertex3f(x, -x, 0);
	glTexCoord2f(0.0, 0.0);glVertex3f(-x, -x, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.0, 0.5);
	glVertex3f(-0.35, 0, 0);
	glVertex3f(0.35, 0, 0);
	glVertex3f(0, 0.1, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-0.1, 0, 0);
	glVertex3f(0.1, 0, 0);
	glVertex3f(0, 0.35, 0);
	glEnd();
}

void SceneManager::draw_back_ground(float lvl){
	glPushMatrix();
	glTranslatef(0, 0, lvl);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[BACKGROUND_TEX]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	float x = 4.2;
	
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);glVertex3f(-x, x, 0);
	glTexCoord2f(1.0, 1.0);glVertex3f(x, x, 0);
	glTexCoord2f(1.0, 0.0);glVertex3f(x, -x, 0);
	glTexCoord2f(0.0, 0.0);glVertex3f(-x, -x, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void SceneManager::add_lighting(float li){
	/*
	role : it's responsible for adding light to the scene
	in   : li > float light intensity
	out  : void
	*/
	GLfloat ambientLight[] = {1.0f, 0.0f, 1.0f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	GLfloat directedLight[] = {li, li, li, 1.0};
	//GLfloat directedLightPos[] = {-10.0f, 15.0f, 20.0f, 0.0f};
	GLfloat directedLightPos[] = {0.0f, 0.0f, 1.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);
}

void SceneManager::render_bit_map(float x, float y, float z, void *font, string s) {
	glRasterPos3f(x, y, z);
	for (int i = 0; i < s.size(); i++) {
		glutBitmapCharacter(font, (int)s[i]);
	}
}

void SceneManager::draw_grid(int xlim, int ylim){
	glBegin(GL_LINES);
	for(int i = -xlim;i <= xlim;i++){
		glVertex3f(i, ylim, 0);glVertex3f(i, -ylim, 0);
	}

	for(int i = -ylim;i <= ylim;i++){
		glVertex3f(xlim, i, 0);glVertex3f(-xlim, i, 0);
	}
	glEnd();
}
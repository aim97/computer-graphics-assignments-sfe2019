#include "gameManager.h"

// constructor
GameManager::GameManager(){
	SM = SceneManager();
	SM.glut_init();
	player.set(mp(0.0, -Y_LIM + 0.5), 0);
	game_state = RUNNING;
	interruptable = 0;
}


// private member functions
void GameManager::game_screen(){
	// add some bright lighting
	SM.add_lighting(0.8);
	SM.draw_back_ground(BACKGROUND_LVL);

	// set moving objects z lvl
	glPushMatrix();
	glTranslatef(0, 0, GAME_LVL);

	// display score
	char buf[20];
	sprintf(buf, "%lld", player.score);
	glColor4f(1.0, 1.0, 0.0, 1.0);
	SM.render_bit_map(X_LIM - 1,Y_LIM - 1,0, GLUT_BITMAP_TIMES_ROMAN_24, buf);

	// display astroids
	for(auto i : astroids){
		glPushMatrix();
		glTranslatef(i.location.X, i.location.Y, 0.0);
		SM.draw_astroid();
		glPopMatrix();
	}

	// display player space ship
	glPushMatrix();
	glTranslatef(player.location.X, player.location.Y, 0.0);
	SM.draw_space_ship();
	glPopMatrix();

	glPopMatrix();
}

void GameManager::pause_screen(){
	// add some slight lighting
	SM.draw_back_ground(-10.0);
	SM.add_lighting(0.4);

	// display instructions message to player
	SM.render_bit_map(-2.0, 0, GAME_LVL, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS \'ENTER\' to continue");
}

void GameManager::stop_screen(){
	SM.draw_back_ground(-10.0);
	// display final user score
	glColor4f(1.0, 1.0, 0.0, 1.0);
	char buf[30];sprintf(buf, "score : %lld", player.score);
	SM.render_bit_map(-1.5, 1, GAME_LVL, GLUT_BITMAP_TIMES_ROMAN_24, buf);
	
	// display some instructions message
	glColor4f(1.0, 1.0, 1.0, 1.0);
	SM.render_bit_map(-1.5, 0.5, GAME_LVL, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER");
	glColor4f(0.0, 1.0, 0.0, 1.0);
	SM.render_bit_map(-3, 0, GAME_LVL, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS ENTER TO TRY AGAIN");
	SM.render_bit_map(-3, -0.5, GAME_LVL, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS ESC TO EXIT");
}

void GameManager::reset_game(){
	astroids.clear();
	player.set(mp(0.0, -Y_LIM + 0.5), 0);
	game_state = RUNNING;
}

float GameManager::distance(point a, point b){
	return sqrt((a.X-b.X)*(a.X-b.X) + (a.Y-b.Y)*(a.Y-b.Y));
}

// public member functions
void GameManager::runGame(){
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

	//glTranslatef(0.0, 0.0, -10);
	//SM.draw_grid(4, 4);
	
	printf("game is %d, i'm %s now\n", game_state, (interruptable)?"not interruptable":"interruptable");
	switch(game_state){
		case RUNNING:game_screen();break;
		case PAUSED:pause_screen();break;
		case STOPPED:stop_screen();break;
	}

	glutSwapBuffers(); //Send the 3D scene to the screen
}

void GameManager::timer_update_astroids(int v){
	if(game_state == RUNNING){
		for(auto it = astroids.begin();it != astroids.end();it++){
			it->location.Y -= ASTROID_STEP;
			if(distance(player.location, it->location) < DEATH_RADIUS){
				game_state = STOPPED;
				break;
			}
			if(it->location.Y < -Y_LIM)astroids.erase(it);
		}	
	}
}

void GameManager::timer_add_astroids(int v){
	// create a new astroid in a random location and add it to the list
	if(game_state == RUNNING){
		Astroid ast;ast.set(mp(X_LIM * (rand() * 2.0 / RAND_MAX - 1), Y_LIM));
		astroids.pb(ast);
		
		// increment player score
		player.score++;
	}
}

void GameManager::timer_enable_interrupt(int v){
	interruptable = 0;
}

bool GameManager::KB_handler(unsigned char key, int x, int y){
	switch(key){
		//----------------------------------------------------------------------------------
		case 27:
			printf("ESCAPE\n");
			exit(0);
			break;
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		case 13: // ENTER KEY
			printf("ENTER\n");
			if(!interruptable){
				if(game_state == PAUSED){
					game_state = RUNNING;
					interruptable = 1;
					printf("ENTER : game resumed\n");
					return true;
				}else if(game_state == RUNNING || game_state == STOPPED){
					reset_game();
					interruptable = 1;
					printf("ENTER : game re-started\n");
					return true;
				}
			}
			break;
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		case 112: // p
			printf("P\n");
			if(game_state == RUNNING){
				game_state = PAUSED;
				interruptable = 1;
				printf("P : game is paused\n");
				return true;
			}
			break;
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		case 32: // space
			printf("SPACE\n");
			// do nothing here so far
			break;
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		case 52: // going left
			printf("<-");
			if(game_state == RUNNING) player.location.X = max(player.location.X - SHIP_STEP, -X_LIM + 0.5);
			break;
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		case 54:
			printf("->\n");
			if(game_state == RUNNING) player.location.X = min(player.location.X + SHIP_STEP, X_LIM - 0.5);
			break;
		//----------------------------------------------------------------------------------
	}
	return false;
}

void GameManager::resize_handler(int w, int h){
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
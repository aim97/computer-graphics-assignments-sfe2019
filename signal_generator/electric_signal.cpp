Signal::Signal(float lvl, int type, ...){
	this->type = type;
	this->step = 0.0;
	this->lvl = lvl;
	int count;
	switch(type){
		case SIN_SIGNAL:
			count = 4;
			va_list list;
		    va_start(list, count); 
		    for(int j=0; j<4; j++){
		    	params[j] = (float)va_arg(list, double);
		    	printf("%f\n", params[j]);
		    }
		    va_end(list);
			break;
		case 100:
			printf("unknown signal type\n");
			exit(1);
	};
}

void Signal::handle_sin(){
	float a = params[0],
	      b = params[1],
	      c = params[2],
	      d = params[3],
	      yv = a * sin(b *(step - c)) + d;
	y_points.pb(yv);
}

void Signal::forward(){
	switch(type){
		case SIN_SIGNAL:
			handle_sin();
			break;
		case 100:
			exit(1);
	};
	if(y_points.size() >= 122)y_points.erase(y_points.begin());
	step += 0.1;
}

void Signal::display(){
	glPushMatrix();
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0, 0.0, 0.0);
	int count = 0;
	for (vector<float>::reverse_iterator i = y_points.rbegin();i != y_points.rend(); ++i,++count )
		glVertex3f(-6.0 + count * 0.1 , *i + lvl, -10.0);
	glEnd();
	glPopMatrix();
}

void Signal::reset(){
	y_points.clear();
	step = 0;
}
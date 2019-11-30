#include <iostream>
#include "../include/Session.h"
using namespace std;

int main(int argc, char** argv){
	if(argc<2)
	{
		cout << "usage splflix input_file" << endl;
		return 0;
	}
	Session *s = new Session(argv[1]);
	s->start();
	Session s2 = *s;
	delete s;
	s2.start();
	Session *s3 = new Session(argv[2]);
	delete s3;
	s3 = &s2;
	s3->start();
	//delete s3;
	Session* s4 = new Session(Session(argv[1]));
	s4->start();
	Session* s5 = s4;
	delete s5;
	s5 = new Session(argv[2]);
	s4 = new Session(*(s5));
	delete s5;
	s4->start();
	delete s4;
	return 0;
}

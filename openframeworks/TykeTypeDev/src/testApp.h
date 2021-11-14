#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#define MAX_ENTERED_TEXT_LENGTH 3000
#include "helper.h"

class testApp : public ofBaseApp{
	
public:
	
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	ofTrueTypeFont  theTypeface;
	char enteredTextBuffer [MAX_ENTERED_TEXT_LENGTH];
	string enteredTextString;
	string hypotheticalString;
	
	float marginL, marginR;
	float marginT, marginB, marginTtext;
	float positionX, positionY;
	float positionYGoal;
	
	bool bDisabledKeyRepeat;
	bool bSeparateLettersAndNumbers;
	bool bKeyWasPressed;
	bool bKeyWasReleased;
	bool bStartedTyping;
	int  previousKey;
	int  deleteLineCount; 
	
	bool bLineShifting;
	void updateLineShift();
	void initiateLineShift();
	
	char *heightString;
};

#endif	


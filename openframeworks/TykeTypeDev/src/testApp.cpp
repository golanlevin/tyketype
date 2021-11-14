#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	
	ofSetVerticalSync(true);
	ofSetEscapeQuitsApp(false);

	
	heightString = new char[1];
	sprintf(heightString, "$");
	
	// this load font loads the non-full character set (ie ASCII 33-128)
	// font name, size, anti-aliased, full character set
	float fontSize = 72.0/1280.0 * ofGetWidth();
	theTypeface.loadFont("vag.ttf", fontSize, true, true);
	
	enteredTextString = "TykeType! \nBy Golan Levin\nNov. 2021 v1.1\nType to begin!";
	enteredTextString.reserve (2048);
	
	marginL = 80.0/1024.0 * ofGetWidth();
	marginT = 80.0/1024.0 * ofGetWidth();
	marginR = ofGetWidth()  - marginL;
	marginB = ofGetHeight() - marginT;
	marginTtext = marginT + theTypeface.stringHeight(heightString); // a very tall letter
	
	positionYGoal = marginTtext;
	positionY     = positionYGoal;
	positionX     = marginL;
	
	deleteLineCount = 0; 
	
	bDisabledKeyRepeat = false;
	bSeparateLettersAndNumbers = true;
	bKeyWasPressed  = false;
	bKeyWasReleased = true;
	bLineShifting   = false;
	bStartedTyping  = false;
	previousKey = 0;
	
	
	CallSomeCocoaFunction(); // see helper.mm
	

	

	
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(255,255,255);	
	
	marginR = ofGetWidth()  - marginL;
	marginB = ofGetHeight() - marginT;
	updateLineShift();
	
}

//--------------------------------------------------------------
void testApp::draw(){
	glPushMatrix();	
	ofHideCursor();


	ofSetColor(0x000000);
	ofDisableAlphaBlending();
	theTypeface.drawString(enteredTextString, positionX, positionY);
	
	float W = ofGetWidth();
	float Mheight = theTypeface.stringHeight(heightString) * 1.25;
	float Lheight = theTypeface.getLineHeight();
	
	//----------------------------
	// Draw fadey quad at the top
	float fadeyQuadFactor = 1.125;
	ofEnableAlphaBlending();
	glBegin(GL_QUADS);
	
	glColor4f(1,1,1, 1); 
	glVertex2f(0,0); 
	glVertex2f(W,0);
	glVertex2f(W,marginT-Lheight+Mheight);
	glVertex2f(0,marginT-Lheight+Mheight);
	
	glColor4f(1,1,1,1);
	glVertex2f(0,marginT-Lheight+Mheight);
	glVertex2f(W,marginT-Lheight+Mheight);
	glColor4f(1,1,1,0); 
	glVertex2f(W,marginT * fadeyQuadFactor);
	glVertex2f(0,marginT * fadeyQuadFactor);
	glEnd();
	ofDisableAlphaBlending();
	
	//sprintf(enteredTextBuffer,"%i", (int)ofGetElapsedTimeMillis());
	//if (theTypeface.stringHeight(enteredTextString) > (marginB - marginT)){
	//ofRectangle rect = theTypeface.getStringBoundingBox(enteredTextString, 0,0);
	//if (rect.height > (marginB - marginT)){
	//printf("rectheight = %d, stringHeight = %d, H=%d\n", (int)rect.height, (int)theTypeface.stringHeight(enteredTextString), (int)(marginB - marginT));
	
	glPopMatrix();
}


//==========================================================
void testApp::initiateLineShift(){
	if (bLineShifting == false){
		bLineShifting = true;
		if (enteredTextString != "_"){
			positionYGoal -= theTypeface.getLineHeight();
		}
	}  else {
		
	}
}

//==========================================================
void testApp::updateLineShift(){
	
	float A = 0.7;
	float B = 1.0-A;
	positionX = marginL;
	positionY = A*positionY + B*positionYGoal;
	
	if (bLineShifting){
		
		bool bDoneLineShifting = false;
		if (ABS(positionYGoal - positionY) < 1.0){
			positionY = positionYGoal = marginTtext;
			bDoneLineShifting = true;
		}
		
		if (bDoneLineShifting){
			int whereIsFirstReturn = (int)(enteredTextString.find('\n', 0));
			if (whereIsFirstReturn > -1){
				int currentLength = enteredTextString.length();
				enteredTextString = enteredTextString.substr(whereIsFirstReturn+1, currentLength);
			} else {
				enteredTextString = "_";
			}
			bLineShifting = false;
		}
	}
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    
	
	if (bStartedTyping == false){
		enteredTextString.clear();
        enteredTextString = "_";
		bStartedTyping = true;
	}
	
	if (enteredTextString == "_"){ // god bless
        bool bDoClear = true;
        switch (key){
            case 13: // RETURN
            case 27: // ESCAPE
            case 127: // BACKSPACE
            case 8:
            case 9:
            case 32: // SPACE
                bDoClear = false;
                break;
        }
        if (bDoClear){
            enteredTextString.clear();
        } else {
            return;
        }
	}
	
	char keyChar = (char) key;
	char prevChar = (char) previousKey;
	int currentLength = enteredTextString.length();
	int whereIsFirstReturn = (int)(enteredTextString.find_first_of('\n', 0));
	
	
	switch (key){
		case 13: // RETURN
			//enteredTextString += '\n';
			enteredTextString.clear();
			enteredTextString = "_";
			break;
			
		case 27: // ESCAPE
			
			if ((currentLength > 0)){// && (whereIsFirstReturn > -1)){
				initiateLineShift();
			}
			break;
        
        case 8:
		case 127: // BACKSPACE
			if (currentLength > 0){
				
				if (currentLength > 1){
					char lastChar         = enteredTextString[currentLength-1];
					char secondToLastChar = enteredTextString[currentLength-2];
					if ((lastChar != '\n') && (secondToLastChar == '\n')){
						enteredTextString = enteredTextString.substr(0, currentLength-1);
					}
				}
				
				currentLength = enteredTextString.length();
				enteredTextString = enteredTextString.substr(0, currentLength-1);
				
				
			}
			if (enteredTextString.length() == 0){
				enteredTextString.clear();
				enteredTextString = "_";
			}
			break;
			
		case 9:
		case 32: // SPACE
			hypotheticalString = enteredTextString + ' ';
			if (theTypeface.stringWidth(hypotheticalString) >= (marginR - marginL)){
				enteredTextString += '\n';
			}
			enteredTextString += ' ';
			break;
			
		case OF_KEY_F12:
			printf("Got F12\n"); 
			break;
		case OF_KEY_UP:
			printf("Got UP\n"); 
			break;
			
		default:
			if ((key >= 33) && (key < 127)){
				if ((bDisabledKeyRepeat && bKeyWasReleased) || 
					(bKeyWasPressed && (key != previousKey)) || 
					(!bDisabledKeyRepeat)){
					
					if (bSeparateLettersAndNumbers){
						bool bCurrIsNumber = ((keyChar   >= '0') && (keyChar  <= '9'));
						bool bPrevIsNumber = ((prevChar  >= '0') && (prevChar <= '9'));
						bool bCurrIsLetter = (((keyChar  >= 'a') && (keyChar  <= 'z')) || ((keyChar >= 'A') && (keyChar <= 'Z')));
						bool bPrevIsLetter = (((prevChar >= 'a') && (prevChar <= 'z')) || ((prevChar >= 'A') && (prevChar <= 'Z')));
						
						if ((bCurrIsNumber && bPrevIsLetter) || (bCurrIsLetter && bPrevIsNumber)){
							enteredTextString += ' ';
						}
					}
					
					
					hypotheticalString = enteredTextString + keyChar;
					float widthOfEnteredString      = theTypeface.stringWidth(enteredTextString);
					float widthOfHypotheticalString = theTypeface.stringWidth(hypotheticalString);
					if (widthOfEnteredString >= (marginR - marginL)){
						int len = enteredTextString.length();
						if (len > 1){
							enteredTextString = enteredTextString.substr(0, len-2);
						}
					}
					
					if (widthOfHypotheticalString >= (marginR - marginL)){
						enteredTextString += '\n';
					}
					enteredTextString += keyChar;
					
					
					
					
				}
			}
			break;
	}
	
	// shift up if the bbox is too high
	int returnCount = 0;
	int len = enteredTextString.length();
	for (int i=0; i<len; i++){
		char c = enteredTextString[i];
		if (c == '\n'){
			returnCount++;
		}
	}
	float boxHeight = returnCount * theTypeface.getLineHeight();
	float textBottom = marginTtext + boxHeight;
	if (textBottom > marginB){
		initiateLineShift();
	}
	
	printf("Key = %d\n", key);
	//if (theTypeface.stringWidth (enteredTextString) > (marginR - marginL)){
	//	printf("too wide! %d\n", ofGetElapsedTimeMillis());
	//}
	
	bKeyWasPressed = true;
	bKeyWasReleased = false;
	previousKey = key;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	bKeyWasReleased = true;
	bKeyWasPressed  = false;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){;}
void testApp::mouseDragged(int x, int y, int button){;}
void testApp::mousePressed(int x, int y, int button){;}
void testApp::mouseReleased(int x, int y, int button){;}
void testApp::windowResized(int w, int h){;}

#pragma once

#include "ofPoint.h"

class ofBaseApp { // TZLA-619 //: public ofBaseSoundInput, public ofBaseSoundOutput{

	public:
        ofBaseApp() {
            mouseX = mouseY = 0;
        }

		virtual ~ofBaseApp(){}

		virtual void setup(){}
		virtual void update(){}
		virtual void draw(){}
		virtual void exit(){}

		virtual void windowResized(int w, int h){}

		virtual void keyPressed( int key ){}
		virtual void keyReleased( int key ){}

		virtual void mouseMoved( int x, int y ){}
		virtual void mouseDragged( int x, int y, int button ){}
		virtual void mousePressed( int x, int y, int button ){}
		virtual void mouseReleased(){}
		virtual void mouseReleased(int x, int y, int button ){}
		
		// TZLA-619 // virtual void dragEvent(ofDragInfo dragInfo) { }
		// TZLA-619 // virtual void gotMessage(ofMessage msg){ }
	
		virtual void windowEntry ( int state ) { }
		
		int mouseX, mouseY;			// for processing heads
};




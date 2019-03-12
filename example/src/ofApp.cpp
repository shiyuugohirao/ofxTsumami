#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    int w = ofGetWidth()/4;
    int h = ofGetHeight()/3;
    for(int i=0; i<6; i++){

        ofColor col = ofColor::fromHsb(ofRandom(255),5, 255);
        tsumami[i].setup(&f[i], 30, glm::vec2((1+i%3)*w, (1+i/3)*h), 0.,1.,"tsumami0"+to_string(i));
    }

    ofAddListener(tsumami[1].tsumamiEvent, this, &ofApp::changeVal);
}
void ofApp::changeVal(float &val){
    tsumami[4].setup(&f[4], 10+40*val, tsumami[4].getPos());
}
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255*f[0]);

    for(auto &t:tsumami){
        t.draw();
    }
}

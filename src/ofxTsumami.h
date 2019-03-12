//
//  ofxTsumami.h
//
//  Created by shugohirao on 2019/03/12
//

class ofxTsumami {
private:
    float *target;
    float radius;
    float minVal, maxVal, initVal, pressedPct;
    string name;
    int res;
    glm::vec2 pos, pressedPos;

    bool bVertical;
    bool bPressed,bHovered;
    ofColor mainColor;

    void mousePressed(ofMouseEventArgs & mouse){
        pressedPos = {mouse.x, mouse.y};
        if(distance(glm::vec2(mouse.x,mouse.y),pos)<radius){
            if(mouse.button == OF_MOUSE_BUTTON_LEFT){
                float tf = ofGetElapsedTimef();
                static float lastPressed = tf;
                if(tf - lastPressed < 0.5){
                    initVal = *target;
                }else{
                    pressedPct = (*target - minVal) / (maxVal - minVal);
                    bPressed = true;
                }
                lastPressed = tf;
            }
            else if(mouse.button == OF_MOUSE_BUTTON_RIGHT){
                *target = initVal;
            }
        }

    }
    void mouseReleased(ofMouseEventArgs & mouse){
        bPressed = false;
    }
    void mouseDragged(ofMouseEventArgs & mouse){
        if(bPressed){
            float move = bVertical ? pressedPos.y-mouse.y : pressedPos.x-mouse.x;
            *target = ofMap(move, -radius*2*pressedPct,radius*2*(1-pressedPct), minVal,maxVal,true);
            ofNotifyEvent(tsumamiEvent, *target);
        }
    }
    void mouseMoved(ofMouseEventArgs & mouse){
        bHovered = (distance(glm::vec2(mouse.x,mouse.y),pos)<radius);
    }
    void mouseScrolled(ofMouseEventArgs & mouse){
        if(bHovered){
            *target -= mouse.scrollY*0.005 * (maxVal - minVal);
            *target = ofClamp(*target, minVal,maxVal);
            ofNotifyEvent(tsumamiEvent, *target);
        }
    }

public:
    ofEvent<float> tsumamiEvent;

    ofxTsumami():target(nullptr),radius(50),minVal(0.),maxVal(1.),res(30){
        ofAddListener(ofEvents().mousePressed,this,&ofxTsumami::mousePressed);
        ofAddListener(ofEvents().mouseReleased,this,&ofxTsumami::mouseReleased);
        ofAddListener(ofEvents().mouseDragged,this,&ofxTsumami::mouseDragged);
        ofAddListener(ofEvents().mouseMoved,this,&ofxTsumami::mouseMoved);
        ofAddListener(ofEvents().mouseScrolled,this,&ofxTsumami::mouseScrolled);
        bVertical = true;
        initVal = (maxVal - minVal) / 2.0;
    }
    virtual ~ofxTsumami(){
        target = nullptr;
        ofRemoveListener(ofEvents().mousePressed,this,&ofxTsumami::mousePressed);
        ofRemoveListener(ofEvents().mouseReleased,this,&ofxTsumami::mouseReleased);
        ofRemoveListener(ofEvents().mouseDragged,this,&ofxTsumami::mouseDragged);
        ofRemoveListener(ofEvents().mouseMoved,this,&ofxTsumami::mouseMoved);
        ofRemoveListener(ofEvents().mouseScrolled,this,&ofxTsumami::mouseScrolled);
    }

    void setup(float *target, int radius, string name="", ofColor color=ofColor::ivory){
        setup(target, radius, glm::vec2(0,0), 0., 1., name, color);
    }
    void setup(float *target, int radius, float min,float max, string name="", ofColor color=ofColor::ivory){
        setup(target, radius, glm::vec2(0,0), min, max, name, color);
    }
    void setup(float *target, int radius, glm::vec2 pos, string name="", ofColor color=ofColor::ivory){
        setup(target, radius, pos, 0., 1., name, color);
    }
    void setup(float *target, int radius, glm::vec2 pos, float min,float max, string name="", ofColor color=ofColor::ivory){
        this->target = target;
        this->radius = radius;
        this->pos = pos;
        minVal = min;
        maxVal = max;
        this->name = name;
        this->mainColor = color;
    }

    void draw(){
        ofPushMatrix();
        ofTranslate(pos.x, pos.y);
        float deg = ofMap(*target,minVal,maxVal,0.,320.,true);
        ofPushStyle();
        ofSetCircleResolution(res);
        ofFill();
        ofSetLineWidth(radius*0.1);
        ofScale( bHovered? 1.05 : 1.0 );
        ofSetColor(mainColor, 100);
        ofDrawCircle(3,3, radius*1.02);
        ofRotateDeg(deg+200);
        ofSetColor(mainColor, 255);
        ofDrawCircle(0,0, radius);
        ofSetColor(bPressed?0:127);
        ofDrawLine(0,-radius,0,-radius*0.5);
        ofPopStyle();
        ofPopMatrix();

        //if(bPressed){
        if(bHovered){
            string s = name=="" ? ofToString(*target,4) : name+"\n("+ofToString(*target,4)+")";
            ofDrawBitmapStringHighlight(s, ofGetMouseX()+10,ofGetMouseY()-30);
        }
    }
    void draw(glm::vec2 pos){
        draw(pos.x,pos.y);
    }
    void draw(int x, int y){
        pos = {x,y};
        draw();
    }

    void setInitVal(float val){ initVal = val; }
    const string getName(){ return name; }
    const float getRadius(){ return radius; }
    const float getMinVal(){ return minVal; }
    const float getMaxVal(){ return maxVal; }
};

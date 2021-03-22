#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofSetCircleResolution(60);

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 20, true, true, true);

	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}

	this->box2d.init();
	this->box2d.setGravity(0, 50);
	this->box2d.createBounds();
	this->box2d.setFPS(60);
	this->box2d.registerGrabbing();
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = this->circle_list.size() - 1; i > -1; i--) {

		this->life_list[i] -= this->life_speed_list[i];

		if (this->life_list[i] < 0) {

			this->circle_list.erase(this->circle_list.begin() + i);
			this->radius_list.erase(this->radius_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);
			this->life_speed_list.erase(this->life_speed_list.begin() + i);
		}
	}

	if (ofGetFrameNum() % 15 == 0) {

		auto radius = ofRandom(25, 50);

		auto circle = make_shared<ofxBox2dCircle>();
		circle->setPhysics(0.5, 0.83, 0.3);
		circle->setup(this->box2d.getWorld(), ofRandom(radius, ofGetWidth() - radius), radius, radius);
		circle->setRotation(ofRandom(360));
		this->circle_list.push_back(circle);
		this->radius_list.push_back(radius);

		int color_index = ofRandom(this->base_color_list.size());
		this->color_list.push_back(this->base_color_list[color_index]);
		this->life_list.push_back(128);
		this->life_speed_list.push_back(ofRandom(0.5, 1.2));
	}

	this->box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->circle_list.size(); i++) {

		ofPushMatrix();
		ofTranslate(this->circle_list[i]->getPosition());

		auto color_value = this->life_list[i] < 30 ? ofMap(this->life_list[i], 0, 30, 239, 39) : 39;

		ofNoFill();
		ofSetColor(ofColor(this->color_list[i], ofMap(color_value, 39, 239, 255, 0)));
		ofDrawCircle(glm::vec3(), this->radius_list[i]);

		this->draw_arrow(glm::vec3(), this->circle_list[i]->getVelocity(), this->radius_list[i] * 0.8);

		ofPopMatrix();
	}
}

//--------------------------------------------------------------
void ofApp::draw_arrow(glm::vec2 location, glm::vec2 next_location, float size) {

	auto angle = std::atan2(next_location.y - location.y, next_location.x - location.x);

	ofFill();
	ofBeginShape();
	ofVertex(location + glm::vec2(size * cos(angle), size * sin(angle)));
	ofVertex(location + glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)));
	ofVertex(location + glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)));
	ofEndShape();

	ofBeginShape();
	ofVertex(location + glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25);
	ofVertex(location + glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25 - glm::vec2(size * cos(angle), size * sin(angle)) * 0.5);
	ofVertex(location + glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25 - glm::vec2(size * cos(angle), size * sin(angle)) * 0.5);
	ofVertex(location + glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25);
	ofEndShape();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
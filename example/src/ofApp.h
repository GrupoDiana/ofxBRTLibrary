#pragma once

#include "ofMain.h"
#include "ofxBRT.h"


#define SAMPLERATE 44100
#define BUFFER_SIZE 512

#define LISTENER_ID "listener1"

#define LISTENER_HRTF_MODEL_ID "listenerHRTF1"
#define SOFA1_FILEPATH "data/hrtf.sofa"
#define ILD_NearFieldEffect_44100 "data/NearFieldCompensation_ILD_44100.sofa"
#define HRTFRESAMPLINGSTEP 15


class ofApp : public ofBaseApp{

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		void audioOut(ofSoundBuffer & output);

		

	private:
	
		void ConfigureBRT();
		void LoadBRTResources(const std::string & listenerID);
		bool LoadHRTFSofaFile(const std::string & _filePath, std::shared_ptr<BRTServices::CHRTF> hrtf);
		bool LoadNearFieldSOSFilter(std::string _ildFilePath, std::shared_ptr<BRTServices::CSOSFilters> _sosFilter);
		std::shared_ptr<BRTSourceModel::CSourceSimpleModel> CreateSimpleSoundSource(std::string _soundSourceID);
		void ConnectSoundSource(const std::string & _soundSourceID);

		////////////////
		// Attributes		
		////////////////
		ofSoundStream soundStream;

		// BRT
		Common::CGlobalParameters globalParameters; // Class where the global BRT parameters are defined.
		BRTBase::CBRTManager brtManager; // BRT global manager interface
		std::shared_ptr<BRTBase::CListener> listener; // Pointer to listener model
		std::shared_ptr<BRTSourceModel::CSourceSimpleModel> source1BRT; // Pointers to each audio source model
		
};

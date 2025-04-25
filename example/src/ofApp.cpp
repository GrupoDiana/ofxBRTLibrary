#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	// Set up the sound stream
	ofSoundStreamSettings settings;	
	auto devices = soundStream.getDeviceList(ofSoundDevice::Api::MS_WASAPI);	
	settings.setOutDevice(devices[2]);	// Select the output device
	settings.setOutListener(this);
	settings.sampleRate = SAMPLERATE;
	settings.numOutputChannels = 2;
	settings.bufferSize = BUFFER_SIZE;
	soundStream.setup(settings);
	soundStream.stop();
	
	// Setup BRT
	ConfigureBRT();	
	LoadBRTResources(LISTENER_ID);
	
	// Set up sound source
	source1BRT = CreateSimpleSoundSource("source1");
	ConnectSoundSource("source1");	
	Common::CTransform source1Transform = Common::CTransform();
	source1Transform.SetPosition(Common::CVector3(3, 3, 0));
	source1BRT->SetSourceTransform(source1Transform); // Set source2 position
	
	// Start sound stream
	soundStream.start();

	// Informing user by the console to press to end the execution   
	std::cout << std::endl<< std::endl;
	std::cout << "Press ENTER to finish..." << std::endl;
	std::cout << std::endl<< std::endl;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_RETURN) {
		soundStream.stop();
		ofExit();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & output) {
	int uiBufferSize = output.getNumFrames();	
	
	if (uiBufferSize != BUFFER_SIZE) {
		std::cout << "Buffer size mismatch" << std::endl;
		return;
	}
	
	CMonoBuffer<float> source1Input(uiBufferSize);	

	for (size_t i = 0; i < output.getNumFrames(); i++) {
		float noise = ofRandom(-1.0f, 1.0f) * 0.2f; // white noise
		source1Input[i] = noise; 
		
	}
	source1BRT->SetBuffer(source1Input); // Set samples in the sound source
	brtManager.ProcessAll();
	
	Common::CEarPair<CMonoBuffer<float>> bufferProcessed;
	listener->GetBuffers(bufferProcessed.left, bufferProcessed.right); // Get out buffers

	for (size_t i = 0; i < uiBufferSize; i++) {
		output[i * 2] = bufferProcessed.left[i]; // canal izquierdo
		output[i * 2 + 1] = bufferProcessed.right[i]; // canal derecho
	}	
}


///////////////////////
// BRT SETUP
///////////////////////

void ofApp::ConfigureBRT() {
	/// BRT Global Parametert setup
	globalParameters.SetSampleRate(SAMPLERATE); // Setting sample rate
	globalParameters.SetBufferSize(BUFFER_SIZE); // Setting buffer size

	
	brtManager.BeginSetup();	
	// Listener Setup
	listener = brtManager.CreateListener<BRTBase::CListener>(LISTENER_ID);	
	// Listener Model Setup
	std::shared_ptr<BRTListenerModel::CListenerHRTFModel> listenerModel = brtManager.CreateListenerModel<BRTListenerModel::CListenerHRTFModel>(LISTENER_HRTF_MODEL_ID);	
	bool control = listener->ConnectListenerModel(listenerModel);
	if (!control) {
		std::cout << "Error connecting listener model" << std::endl;
	}
	brtManager.EndSetup();

	// Position the listener
	Common::CTransform listenerPosition = Common::CTransform(); // Setting listener in (0,0,0)
	listenerPosition.SetPosition(Common::CVector3(0, 0, 0));
	listener->SetListenerTransform(listenerPosition);
}

void ofApp::LoadBRTResources(const std::string & listenerID) {
	std::shared_ptr<BRTBase::CListener> listener = brtManager.GetListener(listenerID);

	/// Load HRTF
	std::shared_ptr<BRTServices::CHRTF> hrtf1 = std::make_shared<BRTServices::CHRTF>();
	bool hrtfSofaLoaded1 = LoadHRTFSofaFile(SOFA1_FILEPATH, hrtf1);
	// Set one for the listener. We can change it at runtime
	if (hrtfSofaLoaded1) {
		listener->SetHRTF(hrtf1);
	}

	/// LOAD NEARFIELD ILD coefficients
	std::shared_ptr<BRTServices::CSOSFilters> sosFilter = std::make_shared<BRTServices::CSOSFilters>();
	bool nearFieldFilterLoaded = LoadNearFieldSOSFilter(ILD_NearFieldEffect_44100, sosFilter);
	// Set to the listener
	if (nearFieldFilterLoaded) {
		listener->SetNearFieldCompensationFilters(sosFilter);
		// Another way to set the near field filters
		//brtManager.GetListenerModel<BRTListenerModel::CListenerModelBase>(LISTENER_MODEL_ID)->SetNearFieldCompensationFilters(sosFilter);
	}
}


bool ofApp::LoadHRTFSofaFile(const std::string & _filePath, std::shared_ptr<BRTServices::CHRTF> hrtf) {

	BRTReaders::CSOFAReader sofaReader;
	Common::CGlobalParameters globalParameters;

	int sampleRateInSOFAFile = sofaReader.GetSampleRateFromSofa(_filePath);
	if (sampleRateInSOFAFile == -1) {
		std::cout << ("Error loading HRTF Sofa file") << std::endl;
		return false;
	}
	if (globalParameters.GetSampleRate() != sampleRateInSOFAFile) {
		std::cout << "The sample rate in HRTF SOFA file doesn't match the configuration." << std::endl;
		return false;
	}
	std::cout << std::endl
			  << "Loadind HRTF SOFA File....." << std::endl
			  << std::endl;
	bool result = sofaReader.ReadHRTFFromSofa(_filePath, hrtf, HRTFRESAMPLINGSTEP, BRTServices::TEXTRAPOLATION_METHOD::nearest_point);
	if (result) {
		std::cout << ("HRTF Sofa file loaded successfully.") << std::endl;
		return true;
	} else {
		std::cout << ("Error loading HRTF") << std::endl;
		return false;
	}
}

bool ofApp::LoadNearFieldSOSFilter(std::string _ildFilePath, std::shared_ptr<BRTServices::CSOSFilters> _sosFilter) {

	BRTReaders::CSOFAReader sofaReader;
	Common::CGlobalParameters globalParameters;

	int sampleRateInSOFAFile = sofaReader.GetSampleRateFromSofa(_ildFilePath);
	if (sampleRateInSOFAFile == -1) {
		std::cout << ("Error loading ILD Sofa file") << std::endl;
		return false;
	}
	if (globalParameters.GetSampleRate() != sampleRateInSOFAFile) {
		std::cout << "The sample rate in ILD SOFA file" << std::endl;
		return false;
	}

	std::cout << std::endl
			  << "Loadind SOS File....." << std::endl;
	bool result = sofaReader.ReadSOSFiltersFromSofa(_ildFilePath, _sosFilter);
	if (result) {
		std::cout << "ILD Sofa file loaded successfully: " << std::endl;
		return true;
	} else {
		std::cout << "Error loading HRTF" << std::endl;
		return false;
	}
}


std::shared_ptr<BRTSourceModel::CSourceSimpleModel> ofApp::CreateSimpleSoundSource(std::string _soundSourceID) {
	brtManager.BeginSetup();
	std::shared_ptr<BRTSourceModel::CSourceSimpleModel> _brtSoundSource = brtManager.CreateSoundSource<BRTSourceModel::CSourceSimpleModel>(_soundSourceID);
	brtManager.EndSetup();
	if (_brtSoundSource == nullptr) {
		std::cout << "Error creating sound source" << std::endl;
	}
	return _brtSoundSource;
}

// Connect a sound source to the listener model
void ofApp::ConnectSoundSource(const std::string & _soundSourceID) {
	std::shared_ptr<BRTListenerModel::CListenerModelBase> listenerModel = brtManager.GetListenerModel<BRTListenerModel::CListenerModelBase>(LISTENER_HRTF_MODEL_ID);
	if (listenerModel != nullptr) {
		brtManager.BeginSetup();
		bool control = listenerModel->ConnectSoundSource(_soundSourceID);
		brtManager.EndSetup();
	}
}


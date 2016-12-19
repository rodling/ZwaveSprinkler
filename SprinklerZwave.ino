


// git update test vv
#define numRelays 8
#define maxRelayTime 3600000	// 1 hour in mills
#define pollRate 100			// minimum ms between pulse measurments
#define pulsePin 8
#define RELAYOPEN 1
#define RELAYCLOSED 0

int relayPins[numRelays] = { 14, 15, 16, 17, 18, 19, 6, 7 };
int relayState[numRelays];
int currentRelay=0;
int pulseRelay=0;
int lastPulseRelay = 0;
int sameRelayCount = 0;
int relayUpdateNeeded = false;


unsigned long lastPolledMills;
unsigned long lastChangeMills;	// if no relay change in maxRelayTime then reset all relays so water does not run forever



//StepPin = (arg == "x") ? xStep : yStep;		// convert x or Y to stepe pin number

void setup()
{
	pinMode(pulsePin, INPUT);
	for (int i = 0; i < numRelays; i++)
	{
		pinMode(relayPins[i], OUTPUT);	// set relay pins to outputs
		relayState[i] = RELAYOPEN;				// set relays to not pulled
		digitalWrite(relayPins[i], RELAYOPEN);
	}
	lastPolledMills = millis();			// set a start point for polling delay
	


}

void loop()
{

	if (millis() - lastPolledMills > pollRate){		//get a new relay reading from pulse detector 
		pulseRelay = getRelayPulse();
		lastPolledMills = millis();
		if (pulseRelay == lastPulseRelay){
			sameRelayCount++;
		}
		else{
			sameRelayCount = 0;
		}
		pulseRelay = lastPulseRelay;
		if (sameRelayCount == 10 && pulseRelay != currentRelay){
			clearAllRelayStates();
			currentRelay = pulseRelay;
			if (pulseRelay == 0){
				lastChangeMills = millis();
				// if zero do nothing as we already cleared all relay states
			}
			else if (pulseRelay > numRelays){
				lastChangeMills = millis();
				// if out of range do nothing as we already cleared all relay states
			}
			else{		// we have a good relay set so set it
				relayState[pulseRelay] = RELAYCLOSED;
				relayUpdateNeeded = true;
				lastChangeMills = millis();
				
			}
		}

	}
	if (relayUpdateNeeded){
		sameRelayCount = 0;
		updateRelays();
	}

	if (millis() - lastChangeMills > maxRelayTime){ // if no command in maxRelayTime the clear all relays
		clearAllRelayStates();
	}

}

int getRelayPulse(){
	int pulse;
	int mappedRelay;

	pulse = pulseIn(pulsePin, HIGH);		// get dimmer dellay
	mappedRelay = map(pulse, 1000, 25000, 0, 30);		// these valuse will need to be set after understanding mesurable values
	return mappedRelay;
	
}

void clearAllRelayStates(){
// clear all relay bits and set relaysNeedUpdating to TRUE

	for (int i = 0; i < numRelays; i++)
	{
		relayState[i] = RELAYOPEN;				// set relays to not pulled
	}
	relayUpdateNeeded = true;
}

void updateRelays(){
	for (int i = 0; i < numRelays; i++)
	{
		digitalWrite(relayPins[i], relayState[i]);
	}
	relayUpdateNeeded = false;
}
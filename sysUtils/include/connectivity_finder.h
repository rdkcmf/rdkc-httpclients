typedef enum MODE{
		CONNECTED_STATE = 0,
		DISCONNECTED_STATE,
		PAUSE_STATE
}MODE;

//curl to captive portal
//HTTP-204 -- Connected (NORMAL WORKING STATE)
//HTTP-200 -- Connected (PAUSE STATE)
//curl timeout -- Disconnected (WIFI DISCONNECTED STATE)
MODE stateFinder();

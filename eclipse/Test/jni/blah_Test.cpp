#include <jni.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <android/log.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "api.h"
#include "blah_Test.h"

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "rbd", __VA_ARGS__))

extern const uint8_t g_appkey[] = {
	0x01, 0x22, 0x49, 0x94, 0x8F, 0x52, 0xCB, 0xB0, 0xCD, 0x78, 0xE5, 0x1C, 0x9E, 0x43, 0x88, 0xA1,
	0xBE, 0x19, 0xDA, 0x7F, 0x11, 0xA7, 0xDD, 0x49, 0x8A, 0xBE, 0xD0, 0xA3, 0x61, 0x81, 0x7D, 0xBD,
	0x09, 0x47, 0xE3, 0x79, 0x87, 0x7D, 0x85, 0xD6, 0x3D, 0xDC, 0xD3, 0x3F, 0x0B, 0x7B, 0x6F, 0x6C,
	0x3C, 0x92, 0x56, 0xBC, 0x9A, 0xAA, 0x1F, 0x98, 0x58, 0xE2, 0x7A, 0x6F, 0x80, 0xCE, 0x80, 0xD0,
	0x23, 0xD9, 0x2B, 0xE4, 0xEC, 0x96, 0x40, 0x99, 0xBA, 0xC5, 0x8C, 0x98, 0xF4, 0x01, 0xF7, 0x76,
	0x10, 0xED, 0x8C, 0xC7, 0x7B, 0x0D, 0x90, 0xD3, 0x7E, 0x27, 0x74, 0xBC, 0x8B, 0x33, 0x48, 0x73,
	0xBD, 0x05, 0x5E, 0xF9, 0x44, 0xA6, 0xCC, 0x63, 0xC4, 0xE8, 0x37, 0x2F, 0xB2, 0x90, 0x8E, 0x31,
	0x0F, 0x33, 0xD1, 0x53, 0x45, 0xCC, 0x72, 0x82, 0x2E, 0xD3, 0xB5, 0x3B, 0xBD, 0x51, 0x07, 0xF9,
	0xE6, 0x1E, 0xDE, 0xB4, 0xBA, 0x16, 0x84, 0x64, 0xB1, 0xC3, 0x34, 0x81, 0x74, 0x48, 0x34, 0x18,
	0xC5, 0x4F, 0x96, 0xB8, 0xD8, 0x7C, 0x4B, 0xC7, 0x36, 0xD4, 0x98, 0xF1, 0xCA, 0x5E, 0xC5, 0xCA,
	0x6F, 0xAD, 0xDD, 0x34, 0x35, 0x08, 0xC3, 0x63, 0x4C, 0x8C, 0x49, 0xA3, 0xD9, 0xD1, 0x84, 0x89,
	0xAC, 0x2A, 0x8F, 0x8F, 0x60, 0x48, 0x09, 0xAA, 0xF6, 0x4C, 0x88, 0xF4, 0x9D, 0xEE, 0x08, 0xE1,
	0x0E, 0xA2, 0x46, 0xE6, 0x71, 0xDD, 0xA5, 0xF8, 0x24, 0xAD, 0x39, 0xB3, 0x6C, 0xB0, 0x28, 0x46,
	0x76, 0xD3, 0xC1, 0x5E, 0x56, 0x5A, 0x95, 0x0E, 0x33, 0xD5, 0x15, 0x77, 0x4B, 0x0B, 0x71, 0xB4,
	0x58, 0x6C, 0xE2, 0x21, 0xEB, 0xBB, 0xFA, 0xA0, 0xB0, 0xDA, 0x5D, 0x88, 0x90, 0xCD, 0x0E, 0x45,
	0x10, 0xB3, 0x4E, 0x5A, 0xF7, 0x04, 0x23, 0x14, 0x85, 0x0A, 0x86, 0xE3, 0x11, 0xE3, 0x40, 0xBA,
	0x1C, 0xB6, 0x3A, 0x7B, 0xAF, 0x01, 0x32, 0xEC, 0xB0, 0xB0, 0x45, 0x13, 0x3A, 0x3A, 0x75, 0xE6,
	0xCA, 0xF6, 0x14, 0xAE, 0xCC, 0xFE, 0xC3, 0x49, 0xF1, 0x15, 0xA3, 0xB5, 0x5F, 0x66, 0x60, 0xE9,
	0x17, 0x8B, 0xA8, 0xD0, 0x46, 0x9B, 0x6A, 0x39, 0x96, 0xA3, 0x9C, 0x32, 0xEC, 0x11, 0x1D, 0x65,
	0x50, 0x92, 0x2E, 0xEB, 0x8D, 0x82, 0xFC, 0xF6, 0xB3, 0x0F, 0x5D, 0x18, 0xF0, 0x59, 0x0E, 0x9E,
	0x45,
};
extern const size_t g_appkey_size = sizeof(g_appkey);

sp_session* g_Session = NULL;

sp_link* g_Link = NULL;
sp_track* g_Track = NULL;
bool loginSuc = false;
bool didLink = false;
bool didTrack = false;

pthread_cond_t notifyLoop;
pthread_mutex_t notifyLock;
bool spotNotify = false;
bool slSuccessful = false;
bool bufferReady = true;

SLObjectItf slEngine = NULL;
SLObjectItf slOutputMix = NULL;
SLEngineItf slEngineEngine = NULL;
SLObjectItf slBufferQueueObj = NULL;
SLPlayItf slPlayer = NULL;
SLBufferQueueItf slBufferQueue = NULL;

pthread_mutex_t bufferLock;
#define BUF_SAMPLE_NUM (44100*20)
#define SAMPLE_BYTES 2
short myBuffers[2][BUF_SAMPLE_NUM];
//const int myBufferSize = 44100*2;//44100*2*60;
int myBufferPlayOffset = 0;
int myBufferEndOffset = 0;
long totalRecieved=0;
long totalPlayed=0;

int myCurBuf = 0;
int myBufferIdx = 0;
int bufsFilled = 0;

int outfile;

extern "C" {

void doLink();
void doTrack();
void doName();
void containerLoaded(sp_playlistcontainer*, void*);
void playlistRenamed(sp_playlist*, void*);
void bufferQueueReady(SLBufferQueueItf, void*);

bool initAudio() {
	SLresult res = slCreateEngine(&slEngine, 0, NULL, 0, NULL, NULL);
	if(res != SL_RESULT_SUCCESS) {
		LOGD("error creating SL engine");
		return false;
	}

	res = (**slEngine).Realize(slEngine, SL_BOOLEAN_FALSE);
	if(res != SL_RESULT_SUCCESS) {
		LOGD("error realizing SL engine");
		return false;
	}

	res = (**slEngine).GetInterface(slEngine, SL_IID_ENGINE, &slEngineEngine);
	if(res != SL_RESULT_SUCCESS) {
		LOGD("error getting SL engine engine");
		return false;
	}

	const SLInterfaceID ids[] = {SL_IID_VOLUME};
	const SLboolean req[] = {SL_BOOLEAN_FALSE};
	res = (**slEngineEngine).CreateOutputMix(slEngineEngine, &slOutputMix, 1, ids, req);
	if(res != SL_RESULT_SUCCESS) {
		LOGD("error getting SL output mix");
		return false;
	}

	res = (**slOutputMix).Realize(slOutputMix, SL_BOOLEAN_FALSE);
	if(res != SL_RESULT_SUCCESS) {
		LOGD("error realizing SL output mix");
		return false;
	}

	SLDataLocator_AndroidSimpleBufferQueue bufferQueue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
	SLDataFormat_PCM formatPCM = {SL_DATAFORMAT_PCM, 2, SL_SAMPLINGRATE_44_1,
									SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
									0, SL_BYTEORDER_LITTLEENDIAN};
	SLDataSource audioSrc = {&bufferQueue, &formatPCM};

	SLDataLocator_OutputMix outMixLoc = {SL_DATALOCATOR_OUTPUTMIX, slOutputMix};
	SLDataSink audioSink = {&outMixLoc, NULL};

	const SLInterfaceID playerIIDs[] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
	const SLboolean playerIReq[] = {SL_BOOLEAN_TRUE};
	res = (**slEngineEngine).CreateAudioPlayer(slEngineEngine, &slBufferQueueObj, &audioSrc, &audioSink, 1, playerIIDs, playerIReq);
	if(res != SL_RESULT_SUCCESS) {
		LOGD("error creating SL audio player");
		return false;
	}

	res = (**slBufferQueueObj).Realize(slBufferQueueObj, SL_BOOLEAN_FALSE);
	if(res != SL_RESULT_SUCCESS) {
		LOGD("error realizing SL audio player");
		return false;
	}

	res = (**slBufferQueueObj).GetInterface(slBufferQueueObj, SL_IID_PLAY, &slPlayer);
	if(res != SL_RESULT_SUCCESS) {
		LOGD("error getting SL player interface");
		return false;
	}

	res = (**slBufferQueueObj).GetInterface(slBufferQueueObj, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &slBufferQueue);
	if(res != SL_RESULT_SUCCESS) {
		LOGD("error getting SL buffer queue");
		return false;
	}

	res = (**slBufferQueue).RegisterCallback(slBufferQueue, bufferQueueReady, NULL);
	if(res != SL_RESULT_SUCCESS) {
		LOGD("error setting SL buffer callback");
		return false;
	}

//	myBuffer = malloc(myBufferSize);

	res = (**slBufferQueue).Enqueue(slBufferQueue, myBuffers[0], BUF_SAMPLE_NUM * SAMPLE_BYTES);
	if(res != SL_RESULT_SUCCESS) {
		LOGD("error enqueueing buffer one: %d", res);
		return false;
	}

	res = (**slBufferQueue).Enqueue(slBufferQueue, myBuffers[1], BUF_SAMPLE_NUM * SAMPLE_BYTES);
	if(res != SL_RESULT_SUCCESS) {
		LOGD("error enqueueing buffer two: %d", res);
		return false;
	}

	umask(0);
	outfile = open("/tmp/spotify/pcm", O_WRONLY | O_TRUNC | O_CREAT, 511);

	return true;
}

void bufferQueueReady(SLBufferQueueItf bq, void* context) {
////	LOGD("bqr 1");
//	pthread_mutex_lock(&notifyLock);
////	LOGD("bqr 2");
//	bufferReady = true;
//	pthread_cond_signal(&notifyLoop);
////	LOGD("bqr 3");
//	pthread_mutex_unlock(&notifyLock);

	myCurBuf ^= 1;
	myBufferIdx = 0;
	SLresult res = (**slBufferQueue).Enqueue(slBufferQueue, myBuffers[myCurBuf], BUF_SAMPLE_NUM * SAMPLE_BYTES);
	if(res != SL_RESULT_SUCCESS) {
		LOGD("eror enqueueing new buffer: %d", res);
	}
}

void loggedIn(sp_session* sess, sp_error error) {
	char msg[100];
	strcpy(msg, "loggedIn called");
	if(error != SP_ERROR_OK) {
		strcat(msg, ", error: ");
		strcat(msg, sp_error_message(error));
	} else {
		loginSuc = true;
	}
	LOGD(msg);

	sp_playlistcontainer_callbacks pc_callbacks;
	memset(&pc_callbacks, 0, sizeof(pc_callbacks));
	pc_callbacks.container_loaded = &containerLoaded;

	sp_playlistcontainer* pc = sp_session_playlistcontainer(sess);
	sp_playlistcontainer_add_callbacks(pc, &pc_callbacks, NULL);
}

void containerLoaded(sp_playlistcontainer* pc, void* userData) {
	int numPlaylists = sp_playlistcontainer_num_playlists(pc);
//	LOGD("%d playlists", numPlaylists);

	sp_playlist_callbacks pl_callbacks;
	memset(&pl_callbacks, 0, sizeof(pl_callbacks));
	pl_callbacks.playlist_renamed = &playlistRenamed;

	for(int i = 0; i<numPlaylists; ++i) {
		sp_playlist* playlist = sp_playlistcontainer_playlist(pc, i);
		sp_playlist_add_callbacks(playlist, &pl_callbacks, NULL);
		//LOGD("%d: %s", i, sp_playlist_name(playlist));
	}
}

void playlistRenamed(sp_playlist* pl, void* userData) {
//	LOGD("pl renamed: %s", sp_playlist_name(pl));
}

void connectionError(sp_session* sess, sp_error error) {
	char msg[100];
	strcpy(msg, "connectionError called");
	if(error != SP_ERROR_OK) {
		strcat(msg, ", error: ");
		strcat(msg, sp_error_message(error));
	}
	LOGD(msg);
}

void connStateUpdated(sp_session* sess) {
	LOGD("connStateUpdated called");
}

void logMessage(sp_session* sess, const char* message) {
	//strcpy(supMsg, "logMessage called: ");
	//strcat(supMsg, message);
//	LOGD(message);
}

void offlineError(sp_session* sess, sp_error error) {
	char msg[100];
	strcpy(msg, "offlineError called");
	if(error != SP_ERROR_OK) {
		strcat(msg, ", error: ");
		strcat(msg, sp_error_message(error));
	}
	LOGD(msg);
}

void offlineStatusUpdated(sp_session* sess) {
	LOGD("offlineStatusUpdated called");
}

void userInfoUpdated(sp_session* sess) {
//	LOGD("userInfoUpdated called");
}

void notifyMainThread(sp_session* sess) {
//	LOGD("nmt 1");
	pthread_mutex_lock(&notifyLock);
//	LOGD("nmt 2");
	spotNotify = true;
	pthread_cond_signal(&notifyLoop);
//	LOGD("nmt 3");
	pthread_mutex_unlock(&notifyLock);
}

void metadataUpdated(sp_session* sess) {
//	LOGD("metadata updated");
}

int musicDelivery(sp_session* sess, const sp_audioformat* format, const void* frames, int numFrames) {
////	LOGD("md 1");
//	pthread_mutex_lock(&bufferLock);
////	LOGD("md 2");
//
//	int copySpace = myBufferPlayOffset - myBufferEndOffset;
//	if(copySpace <= 0) copySpace += myBufferSize;
//	if(copySpace > numFrames*2) copySpace = numFrames*2;
////	LOGD("cs: %d, context: %d, %d, %d, %d", copySpace, myBufferSize, myBufferPlayOffset, myBufferEndOffset, numFrames);
//
//	int numCopy = copySpace;
//	if(myBufferSize-myBufferEndOffset < numFrames*2)
//		numCopy = myBufferSize - myBufferEndOffset;
////	LOGD("nc: %d", numCopy);
//	memcpy(myBuffer + myBufferEndOffset, frames, numCopy);
//	myBufferEndOffset = (myBufferEndOffset + numCopy) % myBufferSize;
//	if(numCopy != copySpace) {
//		if(myBufferEndOffset != 0) LOGD("wtf? %d", myBufferEndOffset);
//		memcpy(myBuffer + myBufferEndOffset, frames + numCopy, copySpace - numCopy);
//		myBufferEndOffset += copySpace - numCopy;
//	}
//
////	LOGD("md 3: %d, %d", myBufferPlayOffset, myBufferEndOffset);
//	totalRecieved += copySpace;
//	pthread_mutex_unlock(&bufferLock);
//	return copySpace / 2;

//	LOGD("md 1");

//	LOGD("format: %d, %d, %d", format->channels, format->sample_rate, format->sample_type);
//	pthread_mutex_lock(&bufferLock);
//
//	int spaceLeft = myBufferSize - myBufferIdx;
//	int numCopy;
//	if(numFrames*2 < spaceLeft)
//		numCopy = numFrames*2;
//	else
//		numCopy = spaceLeft;
//
//	memcpy(myBuffer + myBufferIdx, frames, numCopy);
//	myBufferIdx += numCopy;
//	totalRecieved += numCopy;
//
//	pthread_mutex_unlock(&bufferLock);
//	return numCopy/2;

	write(outfile, frames, numFrames*4);

	int samplesLeft = BUF_SAMPLE_NUM - myBufferIdx;
	int numSamplesCopy;
	if(numFrames*2 < samplesLeft) {
		numSamplesCopy = numFrames*2;
	} else {
		numSamplesCopy = samplesLeft;
	}

//	LOGD("copying %d to %d[%d]", numSamplesCopy, myCurBuf, myBufferIdx);
//	LOGD("     ( %d, %d )", numFrames, samplesLeft);
	memcpy(&myBuffers[myCurBuf][myBufferIdx], frames, numSamplesCopy * SAMPLE_BYTES);
//	LOGD("%d?",myBuffers[myCurBuf][myBufferIdx]);

	myBufferIdx += numSamplesCopy;
	totalRecieved += numSamplesCopy;

	if (numSamplesCopy == samplesLeft && samplesLeft > 0) {
		LOGD("filled buffer %d with %d samples", ++bufsFilled, totalRecieved);
		//totalRecieved = 0;
		if(bufsFilled<2) {
			myCurBuf ^= 1;
			myBufferIdx = 0;
		}
	}

	return numSamplesCopy/2;
}

void end_of_track(sp_session* sess) {
	close(outfile);
	LOGD("track ended %d",totalRecieved);
}

void credBlob(sp_session* sess, const char* blob) {
	LOGD("BLOB: %s", blob);
}

void create_session(char* err) {
	int next_timeout=0;

	if (g_Session != NULL) {
		strcpy(err, "session already created");
		return;
	}

	sp_error error;
	sp_session_config config;

	memset(&config, 0, sizeof(config));
	config.application_key = g_appkey;
	config.application_key_size = g_appkey_size;
	config.user_agent = "songflow";
	config.api_version = SPOTIFY_API_VERSION;
	config.tracefile = "/tmp/spotify/trace";
	config.cache_location = "/tmp/spotify/cache/";
	config.settings_location = "cache_location";

	sp_session_callbacks callbacks;
	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.logged_in = loggedIn;
	callbacks.connection_error = connectionError;
	callbacks.connectionstate_updated = connStateUpdated;
	callbacks.log_message = logMessage;
	callbacks.offline_error = offlineError;
	callbacks.offline_status_updated = offlineStatusUpdated;
	callbacks.userinfo_updated = userInfoUpdated;
	callbacks.notify_main_thread = notifyMainThread;
	callbacks.metadata_updated = metadataUpdated;
	callbacks.music_delivery = musicDelivery;
	callbacks.end_of_track = end_of_track;
	callbacks.credentials_blob_updated = credBlob;
	
	config.callbacks = &callbacks;

	error = sp_session_create(&config, &g_Session);
	if(error != SP_ERROR_OK) {
		strcpy(err, sp_error_message(error));
	}

	char* blob = "8i7ivTM6dZkxtxIdRDJNOttp8PYM28Mx2HivZ95PZOi4D5Sct8X1JNoJfdKSeRdZQcTPkQQkmDWwu1mwB/k9DPCgc5rrumt9zAf7rPRfo5TMWlNz3mEd3zML+aTEzU7OVPhC1drp/fdtxtwoe02pdAMeIWTY8AhIPFBYSardeYvis8tjFDxIIx6W/RZcUq9tjzM7hkiJnF3JwIPAUv+tuL0dCD+o3s6uY6Y9AXkIXxwkQsKrZ5hiWY5WhERSICAOvqEPMn6gNQaPgDgIHtPIrILc0lWwtIRn6fJY+Usb+1aa9DKwf1ng+i4iMKIclSmvFF+v42Wg3CjsoSl3qpzjFtvg90ZhBpx25gkVePquesR1nlpdRRdCjwLRqXyfDGDr";
	//error = sp_session_login(g_Session, "ryguy_@hotmail.com", "hohojaws9", true, NULL);
	error = sp_session_login(g_Session, "ryguy_@hotmail.com", NULL, true, blob);
	if(error != SP_ERROR_OK) {
		strcpy(err, sp_error_message(error));
	}
}

void* loop(void* args) {
	int next_timeout = 0;
	bool didName = false;

	timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	time_t secs = time.tv_sec;

	bool played = false;
	pthread_mutex_lock(&notifyLock);
	while(1) {
//		LOGD("loop 1");
		while(!spotNotify && !bufferReady) {
//			LOGD("sn: %d br: %d", spotNotify, bufferReady);
			pthread_cond_wait(&notifyLoop, &notifyLock);
		}
//		LOGD("loop 2");

		bool sn = spotNotify;
		bool br = bufferReady;
		spotNotify = false;
//		bufferReady = false;
		pthread_mutex_unlock(&notifyLock);

		if(loginSuc) {
			if(!didLink) {
				doLink();
			} else if(!didTrack) {
				doTrack();
			} else if(!sp_track_is_loaded(g_Track)) {
//				LOGD("waiting for track");
			} else if(!didName) {
				LOGD("track loaded, %d", sp_track_is_loaded(g_Track));
				doName();
				didName = true;
			}
		}

//		clock_gettime(CLOCK_MONOTONIC, &time);
//		if(time.tv_sec - secs > 20) {
//			SLresult res = (**slPlayer).SetPlayState(slPlayer, SL_PLAYSTATE_PLAYING);
//			if(res != SL_RESULT_SUCCESS) {
//				LOGD("error playing SL");
//				//return false;
//			} else LOGD("playing");
//
//			secs = time.tv_sec;
//		}

		if(!played && bufsFilled > 1) {
			SLresult res = (**slPlayer).SetPlayState(slPlayer, SL_PLAYSTATE_PLAYING);
			if(res != SL_RESULT_SUCCESS) {
				LOGD("error playing SL");
				//return false;
			} else LOGD("playing");
			played = true;
		}

//		if(br) {
////			LOGD("loop 3");
//			pthread_mutex_lock(&notifyLock);
//			pthread_mutex_lock(&bufferLock);
//
////			while(bufferReady && myBufferPlayOffset != myBufferEndOffset) {
////				int endPoint = myBufferEndOffset;
////				if(endPoint < myBufferPlayOffset) endPoint = myBufferSize;
////				int numEnqueue = endPoint - myBufferPlayOffset;
////
////				SLresult res = (**slBufferQueue).Enqueue(slBufferQueue, myBuffer + myBufferPlayOffset, numEnqueue);
////				if(res == SL_RESULT_BUFFER_INSUFFICIENT) {
////					bufferReady = false;
////				} else if(res != SL_RESULT_SUCCESS) {
////					LOGD("error enqueuing frames at one: %d", res);
////					LOGD("params: %d, %d, %d, %d",slBufferQueue,myBuffer,myBufferPlayOffset,numEnqueue);
////					bufferReady = false;
////				} else {
////					myBufferPlayOffset = (myBufferPlayOffset + numEnqueue) % myBufferSize;
////					totalPlayed += numEnqueue;
////					LOGD("totals: %d / %d", totalPlayed, totalRecieved);
////				}
////			}
//
//			if(myBufferIdx > 0) {
//				SLresult res = (**slBufferQueue).Enqueue(slBufferQueue, myBuffer, myBufferIdx);
//				if(res == SL_RESULT_BUFFER_INSUFFICIENT) {
//					bufferReady = false;
//				} else if(res != SL_RESULT_SUCCESS) {
//					LOGD("error enqueuing frames at one: %d", res);
//					bufferReady = false;
//				} else {
//					//bufferReady = false;
//					totalPlayed += myBufferIdx;
//					myBufferIdx = 0;
////					LOGD("totals: %d / %d", totalPlayed, totalRecieved);
//				}
//			}
//
////			LOGD("loop 4");
//			pthread_mutex_unlock(&bufferLock);
//			pthread_mutex_unlock(&notifyLock);
//		}

		//LOGD("notified");
		if(sn) {
			do {
				sp_session_process_events(g_Session, &next_timeout);
			} while (next_timeout == 0);
		}

//		LOGD("loop 5");
		pthread_mutex_lock(&notifyLock);
	}

	return 0;
}

JNIEXPORT jstring JNICALL
Java_blah_Test_sup(JNIEnv *env, jobject obj) {
	slSuccessful = initAudio();

	char err[100] = "";
	create_session(err);
	if(strlen(err) > 0) {
		return env->NewStringUTF(err);
	}

	pthread_t thread;
	pthread_create(&thread, NULL, loop, NULL);

	return env->NewStringUTF("alright");
}

void doLink() {
	didLink = true;

	LOGD("linkbefore");
	g_Link = sp_link_create_from_string("spotify:track:3ZUMC5OazTA8QtLpdlPdz9");
	if (!g_Link) {
		LOGD("null link");
		return;
	}
	LOGD("link created: %d", sp_link_type(g_Link));
}

void doTrack() {
	didTrack = true;
	g_Track = sp_link_as_track(g_Link);
	if (g_Track == NULL) {
		LOGD("null track");
		return;
	}
	sp_error error = sp_track_error(g_Track);
	if(error != SP_ERROR_OK) {
		LOGD("track error: %s", sp_error_message(error));
		return;
	}

	LOGD("track created: %d", g_Track);
	LOGD("track loaded: %d", sp_track_is_loaded(g_Track));
}

void doName() {
	LOGD("track name: %s", sp_track_name(g_Track));

//	sp_error error = sp_link_release(g_Link);
//	if(error != SP_ERROR_OK) {
//		LOGD("link release error: %s", sp_error_message(error));
//		return;
//	}

	sp_error error = sp_session_player_load(g_Session, g_Track);
	if(error != SP_ERROR_OK) {
		LOGD("player load error: %s", sp_error_message(error));
		return;
	}

	error = sp_session_player_play(g_Session, true);
	if(error != SP_ERROR_OK) {
		LOGD("player play error: %s", sp_error_message(error));
		return;
	}
	LOGD("now playing");
}

}

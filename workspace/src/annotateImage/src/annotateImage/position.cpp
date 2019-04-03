#include "annotateImage/position.h"

namespace annotateImage {

	Position::Position(){
		x = 0;
		y = 0;
		uncertainty = 0;
		time_stamp = 0;
	}

	Position::~Position(){
	}

	void Position::setPosition(float xp, float yp,  uint64_t t_s){
		x = xp;
		y = yp;
		time_stamp = t_s;
	}

	void Position::setTimeStamp(uint64_t t_s){
		time_stamp = t_s;
	}

}

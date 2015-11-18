#ifndef LS7366_h
#define LS7366_h

class LS7366 {
	int cs_pin;
public:
	LS7366(int pin);
	void begin();
	void configure();
	long read();
	void clear();
};


#endif

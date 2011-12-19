#ifndef RBG_DEBOUNCE
#define RBG_DEBOUNCE

typedef enum {SteadyLow = 0, LowToHigh, SteadyHigh, HighToLow} DebounceState ;

typedef struct {
	volatile DebounceState ButtonState;
	volatile unsigned char ChangeFlag;
	volatile int StateCount;
	volatile int LowToHighThresh;
	volatile int HighToLowThresh;
	volatile int RepeatThresh;
} DebounceStorage;

DebounceStorage InitializeDebounce(int LowToHighT, int HighToLowT, int RepeatT);
unsigned char GetButtonState(DebounceStorage * Button);
unsigned char GetButtonChanged(DebounceStorage * Button);
unsigned char GetButtonStateRepeat(DebounceStorage * Button);
void RegisterDebounceHigh(DebounceStorage * Button);
void RegisterDebounceLow(DebounceStorage * Button);
#endif

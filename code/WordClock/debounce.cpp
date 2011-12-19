#include "debounce.h"

DebounceStorage InitializeDebounce(int LowToHighT, int HighToLowT, int RepeatT)
{
	DebounceStorage myDS;
	
	myDS.ButtonState = SteadyLow;
	myDS.StateCount = 0;
	myDS.ChangeFlag = 0;
	myDS.LowToHighThresh = LowToHighT;
	myDS.HighToLowThresh = HighToLowT;
	myDS.RepeatThresh = RepeatT;

	return myDS;
}

unsigned char GetButtonState(DebounceStorage* Button)
{
	if((Button->ButtonState == SteadyHigh) || (Button->ButtonState == HighToLow))
	{
		return 1;
	} else {
		return 0;
	}
}

unsigned char GetButtonChanged(DebounceStorage* Button)
{
	if(Button->ChangeFlag == 1)
	{
		Button->ChangeFlag = 0;
		return 1;
	} else {
		return 0;
	}
}

unsigned char GetButtonStateRepeat(DebounceStorage* Button)
{
	if((Button->ButtonState == SteadyHigh) && Button->ChangeFlag)
	{
		Button->ChangeFlag = 0;
		return 1;
	} else {
		return 0;
	}
}

void RegisterDebounceHigh(DebounceStorage* Button)
{
	switch(Button->ButtonState)
	{
		case SteadyLow:
			Button->ButtonState = LowToHigh;
			Button->StateCount = 0;
			break;
		case LowToHigh:
			Button->StateCount++;
			if(Button->StateCount > Button->LowToHighThresh)
			{
				Button->ButtonState = SteadyHigh;
				Button->ChangeFlag = 1;
				Button->StateCount = 0;
			}
			break;
		case SteadyHigh:
			if(Button->RepeatThresh > 0)
			{
				Button->StateCount++;
				if(Button->StateCount > Button->RepeatThresh)
				{
					Button->ButtonState = SteadyHigh;
					Button->ChangeFlag = 1;
					Button->StateCount = 0;
				}
			}
			break;
		case HighToLow:
			Button->ButtonState = SteadyHigh;
			Button->StateCount = 0;
			break;
	}
}

void RegisterDebounceLow(DebounceStorage* Button)
{
	switch(Button->ButtonState)
	{
		case SteadyLow:
			if(Button->RepeatThresh > 0)
			{
				Button->StateCount++;
				if(Button->StateCount > Button->RepeatThresh)
				{
					Button->ButtonState = SteadyLow;
					Button->ChangeFlag = 1;
					Button->StateCount = 0;
				}
			}
			break;	
		case LowToHigh:
			Button->ButtonState = SteadyLow;
			Button->StateCount = 0;
			break;
		case SteadyHigh:
			Button->ButtonState = HighToLow;
			Button->StateCount = 0;
			break;
		case HighToLow:
			Button->StateCount++;
			if(Button->StateCount > Button->HighToLowThresh)
			{
				Button->ButtonState = SteadyLow;
				Button->ChangeFlag = 1;
				Button->StateCount = 0;
			}
			break;
	}
}

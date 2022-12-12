/*
 * timer.c
 *
 *  Created on: Oct 27, 2022
 *      Author: PC
 */
#define NUM_OF_TIMER 4
int timer_duration[NUM_OF_TIMER] = {0, 0, 0, 0};
int timer_flag[NUM_OF_TIMER] = {0, 0, 0, 0};
#define TIMER_TICK 10;
#define FLAG_ON 	1
#define FLAG_OFF 	0

void setTimer(int index, int duration)
{
	timer_duration[index] = duration/TIMER_TICK;
	timer_flag[index] = FLAG_OFF;
}

int timer_timeout(int index)
{
	if (timer_flag[index] == FLAG_ON) return 1;
	return 0;
}

int timer_remaining(int index)
{
	return ((timer_duration[index] / 100) + 1);
}

void timerRun()
{
	for (int i = 0; i < NUM_OF_TIMER; i++)
	{
		if(timer_duration[i] > 0)
		{
			timer_duration[i]--;
			if (timer_duration[i] == 0)
			{
				timer_flag[i] = FLAG_ON;
			}
		}
	}
}



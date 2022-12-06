NUM_OF_TIMER = 2
timer_dur = [0, 0]
timer_flag = [0, 0]

def timerRun():
    for i in range(NUM_OF_TIMER):
        timer_dur[i] = timer_dur[i] - 1
        if timer_dur[i] == 0:
            timer_flag[i] = 1

def setTimer(index, dur):
    timer_dur[index] = dur

def timerTimeout(index):
    if timer_flag[index] == 1:
        timer_flag[index] = 0
        return 1
    else:
        return 0
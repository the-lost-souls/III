.386
.Model flat, C
.code

public TimerTick

include w:\general\var.inc

TimerTick    PROC
		inc	SYS_TimerTick
		ret
TimerTick    ENDP


END

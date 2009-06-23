set(zynaddsubfx_controls_SRCS
	Control.C
    Event.cpp
	ControlContainer.cpp
	InstrumentContainer.cpp
	FloatControl.cpp
	CharControl.cpp
	DelayCtl.C
)

add_library(zynaddsubfx_controls STATIC
	${zynaddsubfx_controls_SRCS} 
	)

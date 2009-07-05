set(zynaddsubfx_controls_SRCS
	Control.cpp
    Event.cpp
    EventClasses.cpp
    Node.cpp
    Job.cpp
    #ControlContainer.cpp
	InstrumentContainer.cpp
	FloatControl.cpp
	CharControl.cpp
	DelayCtl.cpp
)

add_library(zynaddsubfx_controls STATIC
	${zynaddsubfx_controls_SRCS} 
	)

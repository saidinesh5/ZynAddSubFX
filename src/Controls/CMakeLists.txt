set(zynaddsubfxgui_controls_SRCS
	Control.C
    Event.cpp
	ControlContainer.cpp
	InstrumentContainer.cpp
	FloatControl.cpp
	CharControl.cpp
	DelayCtl.C
)

add_library(zynaddsubfxgui_controls STATIC
	${zynaddsubfxgui_controls_SRCS} 
	)

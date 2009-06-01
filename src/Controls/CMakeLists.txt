set(zynaddsubfxgui_controls_SRCS
	Control.C
	FloatControl.cpp
	DelayCtl.C
)

add_library(zynaddsubfxgui_controls STATIC
	${zynaddsubfxgui_controls_SRCS} 
	)

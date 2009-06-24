
set(zynaddsubfx_gui_SRCS
	dial.cpp
	siblingbar.cpp
	controlhelper.cpp
    eventhelper.cpp
	bankui.cpp
	masterui.cpp
	addnoteui.cpp
	unitbox.cpp
	lfobox.cpp
	envelopebox.cpp
	)

SET( zynaddsubfx_gui_MOC_HDRS
	masterui.h
    eventhelper.h
	bankui.h
	addnoteui.h
	unitbox.h
	lfobox.h
	envelopebox.h
	siblingbar.h
)

SET( zynaddsubfx_gui_UIS
	masterUI.ui
	addnoteui.ui
	lfobox.ui
	envelopebox.ui
)

qt4_wrap_ui(zynaddsubfx_gui_UIS_HDRS ${zynaddsubfx_gui_UIS} )
qt4_automoc(${zynaddsubfx_gui_SRCS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(zynaddsubfx_gui STATIC
	${zynaddsubfx_gui_SRCS}
	${zynaddsubfx_gui_UIS}
	)
target_link_libraries(zynaddsubfx_gui)

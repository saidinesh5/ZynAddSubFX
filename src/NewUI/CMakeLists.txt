
set(zynaddsubfx_gui_SRCS
	dial.cpp
    checkbox.cpp
    combobox.cpp
	siblingbar.cpp
	controlhelper.cpp
    eventhelper.cpp
	bankui.cpp
	masterui.cpp
	addnoteui.cpp
	unitbox.cpp
	lfobox.cpp
	envelopebox.cpp
    debuginterface.cpp
    collapsableframe.cpp
    voicelist.cpp
    voicewidget.cpp
    oscil.cpp
    slider.cpp
    menu.cpp
    oscilwidget.cpp
	)

SET( zynaddsubfx_gui_MOC_HDRS
	masterui.h
    eventhelper.h
    checkbox.h
    combobox.h
    dial.h
	bankui.h
	addnoteui.h
	unitbox.h
	lfobox.h
	envelopebox.h
	siblingbar.h
    collapsableframe.h
    voicelist.h
    voicewidget.h
    oscil.h
    slider.h
    menu.h
    oscilwidget.h
)

SET( zynaddsubfx_gui_UIS
	masterUI.ui
    #addnoteui.ui
    newaddnote.ui
	lfobox.ui
	envelopebox.ui
    collapsableframe.ui
    voicelist.ui
    voicewidgetexpanded.ui
    oscil.ui
)

qt4_wrap_ui(zynaddsubfx_gui_UIS_HDRS ${zynaddsubfx_gui_UIS} )
qt4_automoc(${zynaddsubfx_gui_SRCS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(zynaddsubfx_gui STATIC
	${zynaddsubfx_gui_SRCS}
	${zynaddsubfx_gui_UIS}
	)

set(GUI_LIBRARIES
    ${QT_LIBRARIES}
    zynaddsubfx_gui
    PARENT_SCOPE
    )

target_link_libraries(zynaddsubfx_gui)

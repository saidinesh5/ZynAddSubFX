
set(zynaddsubfx_gui_SRCS
    AddnoteUI.cpp
    BankUI.cpp
    CheckBox.cpp
    CollapsableFrame.cpp
    ComboBox.cpp
    ControlHelper.cpp
    DebugInterface.cpp
    Dial.cpp
    EnvelopeBox.cpp
    EventHelper.cpp
    LFOBox.cpp
    MasterUI.cpp
    Menu.cpp
    Oscil.cpp
    OscilWidget.cpp
    SiblingBar.cpp
    Slider.cpp
    SpectrumWidget.cpp
    UnitBox.cpp
    VoiceList.cpp
    VoiceWidget.cpp
	)

SET( zynaddsubfx_gui_UIS
    CollapsableFrame.ui
    EnvelopeBox.ui
    LFOBox.ui
    MasterUI.ui
    NewAddNote.ui
    Oscil.ui
    VoiceList.ui
    VoiceWidgetExpanded.ui
)

set( zynaddsubfx_gui_RC_FILES
    main.qrc
)

qt4_wrap_ui(zynaddsubfx_gui_UIS_HDRS ${zynaddsubfx_gui_UIS} )
qt4_automoc(${zynaddsubfx_gui_SRCS})
qt4_add_resources(zynaddsubfx_gui_RCS ${zynaddsubfx_gui_RC_FILES} )

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(zynaddsubfx_gui STATIC
	${zynaddsubfx_gui_SRCS}
	${zynaddsubfx_gui_UIS}
    ${zynaddsubfx_gui_RCS}
	)

set(GUI_LIBRARIES
    ${QT_LIBRARIES}
    zynaddsubfx_gui
    PARENT_SCOPE
    )

target_link_libraries(zynaddsubfx_gui)

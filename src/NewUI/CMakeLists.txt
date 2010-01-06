
set(zynaddsubfx_gui_SRCS
    Addnote/AddnoteUI.cpp
    Addnote/VoiceList.cpp
    Addnote/VoiceWidget.cpp
    Addnote/VoiceParameters.cpp
    BankUI.cpp
    Button.cpp
    CheckBox.cpp
    CollapsableFrame.cpp
    ComboBox.cpp
    ControlHelper.cpp
    DebugInterface.cpp
    Dial.cpp
    EnvelopeBox.cpp
	GroupBox.cpp
    LFOBox.cpp
    MasterUI.cpp
    Menu.cpp
    Oscil.cpp
    OscilWidget.cpp
    SiblingBar.cpp
    Slider.cpp
    SpectrumWidget.cpp
    SpinBox.cpp
    UnitBox.cpp
	)

SET( zynaddsubfx_gui_UIS
    Addnote/NewAddNote.ui
    Addnote/VoiceList.ui
    Addnote/VoiceWidgetExpanded.ui
    Addnote/VoiceParameters.ui
    CollapsableFrame.ui
    EnvelopeBox.ui
    LFOBox.ui
    MasterUI.ui
    Oscil.ui
)

set( zynaddsubfx_gui_RC_FILES
    main.qrc
)


qt4_wrap_ui(zynaddsubfx_gui_UIS_HDRS ${zynaddsubfx_gui_UIS} )
qt4_automoc(${zynaddsubfx_gui_SRCS})
qt4_add_resources(zynaddsubfx_gui_RCS ${zynaddsubfx_gui_RC_FILES} )

include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Addnote)

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

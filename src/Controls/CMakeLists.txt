set(zynaddsubfx_controls_SRCS
    GenControl.cpp
    #Control.cpp
    Event.cpp
    EventClasses.cpp
    Node.cpp
    Job.cpp
    #ControlContainer.cpp
    InstrumentContainer.cpp
    InstrumentControl.cpp
    BankControl.cpp
    FakeChildFactory.cpp
    #FloatControl.cpp
    #CharControl.cpp
    #DelayCtl.cpp
    DescRanger.cpp
    DetuneControlSet.cpp
    Ranger.cpp
    Selector.cpp
    Toggle.cpp
    Trigger.cpp
    ArrayControl.cpp
)

add_library(zynaddsubfx_controls STATIC
	${zynaddsubfx_controls_SRCS} 
	)

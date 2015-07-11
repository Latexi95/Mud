#-------------------------------------------------
#
# Project created by QtCreator 2014-10-06T18:01:46
#
#-------------------------------------------------



CONFIG += c++11
CONFIG -= qt

TARGET = Mud
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += jsoncpp/include

#DEFINES += HAVE_ZLIB
win32 {
        INCLUDEPATH += "$$(BOOST_INCLUDE)"
        LIBS += -L"$$(BOOST_LIB)" -lWs2_32 -lMswsock -liconv
        CONFIG(debug, debug|release) {
                LIBS += -lboost_system-mgw49-mt-d-1_58 -lboost_chrono-mgw49-mt-d-1_58 -lboost_locale-mgw49-mt-d-1_58 -lboost_atomic-mgw49-mt-d-1_58 -lboost_thread-mgw49-mt-d-1_58
    } else {
                LIBS += -lboost_system-mgw49-mt-1_58 -lboost_chrono-mgw49-mt-1_58 -lboost_locale-mgw49-mt-1_58 -lboost_atomic-mgw49-mt-1_58 -lboost_thread-mgw49-mt-1_58
    }
}
linux {
    LIBS += -lboost_system -lboost_chrono -lboost_locale -lboost_atomic -lboost_thread
}

LIBS += -lz


SOURCES += main.cpp \
    commandevent.cpp \
    item.cpp \
    character.cpp \
    dicerandom.cpp \
    dicesequence.cpp \
    skillcheck.cpp \
    event.cpp \
    commandservice.cpp \
    eventhandler.cpp \
    mud.cpp \
    mudserver.cpp \
    player.cpp \
    playerservice.cpp \
    jsoncpp/src/lib_json/json_internalarray.inl \
    jsoncpp/src/lib_json/json_internalmap.inl \
    jsoncpp/src/lib_json/json_reader.cpp \
    jsoncpp/src/lib_json/json_value.cpp \
    jsoncpp/src/lib_json/json_valueiterator.inl \
    jsoncpp/src/lib_json/json_writer.cpp \
    jsonserializable.cpp \
    resourceservice.cpp \
    jsonchecker.cpp \
    traits/itemtrait.cpp \
    traits/weapontrait.cpp \
    traits/containertrait.cpp \
    traits/lockedtrait.cpp \
    traits/landmarktrait.cpp \
    level.cpp \
    room.cpp \
    name.cpp \
    equipmentslots.cpp \
    messagehandler.cpp \
    client.cpp \
    joinmessagehandler.cpp \
    libtelnet.c \
    telnetserver.cpp \
    gamemessagehandler.cpp \
    wall.cpp \
    traits/roomtrait.cpp \
    position.cpp \
    box.cpp \
    charactercreationmessagehandler.cpp \
    textgen/color.cpp \
    textgen/textutils.cpp \
    commandparser.cpp \
    command.cpp \
    levelservice.cpp \
    characterservice.cpp \
    timedeventqueue.cpp \
    maineventqueue.cpp \
    leveleventqueue.cpp \
    messageevent.cpp \
    playereventhandler.cpp \
    messagebuilder.cpp \
    connectionevents.cpp

HEADERS += \
    commandevent.h \
    item.h \
    character.h \
    dicerandom.h \
    dicesequence.h \
    skillcheck.h \
    event.h \
    commandservice.h \
    eventhandler.h \
    mud.h \
    mudserver.h \
    player.h \
    playerservice.h \
    jsoncpp/src/lib_json/json_batchallocator.h \
    jsoncpp/src/lib_json/json_tool.h \
    jsoncpp/include/json/assertions.h \
    jsoncpp/include/json/autolink.h \
    jsoncpp/include/json/config.h \
    jsoncpp/include/json/features.h \
    jsoncpp/include/json/forwards.h \
    jsoncpp/include/json/json.h \
    jsoncpp/include/json/reader.h \
    jsoncpp/include/json/value.h \
    jsoncpp/include/json/version.h \
    jsoncpp/include/json/writer.h \
    jsonserializable.h \
    resourceservice.h \
    jsonchecker.h \
    traits/itemtrait.h \
    traits/weapontrait.h \
    traits/containertrait.h \
    traits/lockedtrait.h \
    traits/landmarktrait.h \
    level.h \
    room.h \
    name.h \
    stringbuilder.h \
    equipmentslots.h \
    messagehandler.h \
    client.h \
    joinmessagehandler.h \
    libtelnet.h \
    telnetserver.h \
    gamemessagehandler.h \
    wall.h \
    traits/roomtrait.h \
    position.h \
    box.h \
    charactercreationmessagehandler.h \
    textgen/color.h \
    textgen/textutils.h \
    commandparser.h \
    command.h \
    levelservice.h \
    characterservice.h \
    timedeventqueue.h \
    maineventqueue.h \
    common.h \
    leveleventqueue.h \
    messageevent.h \
    playereventhandler.h \
    messagebuilder.h \
    connectionevents.h

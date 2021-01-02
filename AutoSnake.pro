QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    astar_method.cpp \
    back_painter.cpp \
    bfs_method.cpp \
    body_painter.cpp \
    data_recorder.cpp \
    dijkstra_method.cpp \
    dstar_lite_method.cpp \
    dstar_method.cpp \
    head_painter.cpp \
    main.cpp \
    main_window.cpp \
    obstacle.cpp \
    obstacle_painter.cpp \
    route_method.cpp \
    screen_data.cc \
    seed.cc \
    seed_painter.cpp \
    snake.cc \
    wall_painter.cpp

HEADERS += \
    algorithm_selection.h \
    astar_method.h \
    back_painter.h \
    bfs_method.h \
    body_painter.h \
    data_recorder.h \
    dijkstra_method.h \
    direction.h \
    dstar_lite_method.h \
    dstar_method.h \
    game_control.h \
    head_painter.h \
    index_heap.h \
    main_window.h \
    mode_selection.h \
    obstacle.h \
    obstacle_painter.h \
    route_method.h \
    screen_data.h \
    screen_painter.h \
    seed.h \
    seed_painter.h \
    snake.h \
    speed_selection.h \
    wall_painter.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    back.jpg \
    body.jpg \
    head_down.jpg \
    head_left.jpg \
    head_right.jpg \
    head_up.jpg \
    seed1.jpg \
    seed10.jpg \
    seed11.jpg \
    seed12.jpg \
    seed13.jpg \
    seed2.jpg \
    seed3.jpg \
    seed4.jpg \
    seed5.jpg \
    seed6.jpg \
    seed7.jpg \
    seed8.jpg \
    seed9.jpg \
    wall.jpg

RESOURCES += \
    res.qrc

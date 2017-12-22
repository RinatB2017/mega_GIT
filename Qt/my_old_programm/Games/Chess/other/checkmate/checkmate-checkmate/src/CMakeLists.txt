set(checkmate_SRCS 
    commandhighlighter.cpp
    commandedit.cpp
    gameconsole.cpp
    chessutilities.cpp
    piece.cpp
    board.cpp
    gnuchessengine.cpp
    main.cpp
    checkmate.cpp
    gameinstance.cpp
    abstractengine.cpp
    themeprovider.cpp
    promotiondialog.cpp
    movetablemodel.cpp
    movetableitem.cpp
    movement.cpp
    movelist.cpp
    advancegame.cpp
    pieceinstance.cpp
  )

include_directories(${KDEGAMES_INCLUDE_DIRS})

kde4_add_kcfg_files(checkmate_SRCS settings.kcfgc)

kde4_add_executable(checkmate ${checkmate_SRCS})

target_link_libraries(checkmate ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBRARY} ${KDEGAMES_LIBRARY})

install(TARGETS checkmate ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES CheckMateui.rc  DESTINATION  ${DATA_INSTALL_DIR}/CheckMate)
install(FILES checkmate.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES checkmate.kcfg DESTINATION  ${KCFG_INSTALL_DIR})

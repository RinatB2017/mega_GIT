package com.psbelov.rpi.leds.data;

import java.io.Serializable;

public enum CommandsEnum implements Serializable {
    CONNECT,
    DISCONNECT,
    KILL,
    CANCEL,
    VERSION,
    LENGTH,
    ON,
    OFF,
    COLOR,
    INDEX,
    FILL,
    INDEX_REVERTED,
    FILL_REVERTED,
    CHASE,
    CHASE_CYCLED,
    RANDOM,
    RANDOM_SOLID,
    RANDOM2,
    STARS,
    STARS2,
    RAINBOW,
    RAINBOW2,
    RAINBOW_DYNAMIC,
    COLORS_LIST,
    LINEAR_ANIMATION,
    ANIMATION_OF_COLORS_SETS
}
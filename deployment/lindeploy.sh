#!/bin/bash
cqtdeployer -bin "$1" -qmake "$2" $$QMAKE_QMAKE -libDir "$3" -extraData \
    "$3"/bootdirs/bootata,"$3"/bootdirs/bootdxl,"$3"/bootdirs/bootmyd,"$3"/bootdirs/bootpic,"$3"/bootdirs/bootsma,"$3"/bootdirs/bootspa,\
    "$3"/atari/rcl_SpartaDos,"$3"/atari/rcl_RealDos,"$3"/atari/atari_8-bit_Menu\
    -description "RespeQt" -deployVersion "$4" \
    -tr "$3"/i18n/respeqt_de.qm,"$3"/i18n/respeqt_es.qm,"$3"/i18n/respeqt_pl.qm,"$3"/i18n/respeqt_ru.qm,"$3"/i18n/respeqt_sk.qm,"$3"/i18n/respeqt_tr.qm \
    zip

cqtdeployer -bin "$1" -qmake "$2" $$QMAKE_QMAKE -libDir "$3" -extraData \
    "$3"/bootdirs/bootata,"$3"/bootdirs/bootdxl,"$3"/bootdirs/bootmyd,"$3"/bootdirs/bootpic,"$3"/bootdirs/bootsma,"$3"/bootdirs/bootspa,\
    "$3"/atari/rcl_SpartaDos,"$3"/atari/rcl_RealDos,"$3"/atari/atari_8-bit_Menu\
    -description "RespeQt" -deployVersion "$4" \
    -tr "$3"/i18n/respeqt_de.qm,"$3"/i18n/respeqt_es.qm,"$3"/i18n/respeqt_pl.qm,"$3"/i18n/respeqt_ru.qm,"$3"/i18n/respeqt_sk.qm,"$3"/i18n/respeqt_tr.qm \
    qif

cqtdeployer -bin "$1" -qmake "$2" $$QMAKE_QMAKE -libDir "$3" -extraData \
    "$3"/bootdirs/bootata,"$3"/bootdxl,"$3"/bootdirs/bootmyd,"$3"/bootdirs/bootpic,"$3"/bootdirs/bootsma,"$3"/bootdirs/bootspa,\
    "$3"/atari/rcl_SpartaDos,"$3"/atari/rcl_RealDos,"$3"/atari/atari_8-bit_Menu\
    -description "RespeQt" -deployVersion "$4" \
    -tr "$3"/i18n/respeqt_de.qm,"$3"/i18n/respeqt_es.qm,"$3"/i18n/respeqt_pl.qm,"$3"/i18n/respeqt_ru.qm,"$3"/i18n/respeqt_sk.qm,"$3"/i18n/respeqt_tr.qm \
    deb

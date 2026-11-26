#!/bin/bash
# ftp_lite ni qulaganda qayta ishga tushiradi; ASan hisobotlari asan.<pid> fayllariga yoziladi
export ASAN_OPTIONS=log_path=asan
while true; do
    ./ftp_lite
    echo "[!] qayta ishga tushirilmoqda"
    sleep 0.2
done

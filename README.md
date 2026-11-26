# A11 · Fuzz testing amaliyoti (Codespaces)

Ushbu repozitoriy brauzerda ishlaydigan tayyor Linux muhitini ochadi: AFL++, ASan, gcc va boofuzz.
Kompyuteringizga hech narsa o'rnatish shart emas.

> Diqqat: dasturlarda ataylab xato qoldirilgan. `ftp_lite` faqat 127.0.0.1 da tinglaydi.

## 0. Muhitni ochish (dars oldidan, bir marta)

1. github.com da hisob oching (bepul) va tizimga kiring.
2. O'qituvchi yuborgan repozitoriy havolasini oching.
3. Yashil **Code** tugmasi → **Codespaces** yorlig'i → **Create codespace on main**.
4. Birinchi marta muhit bir necha daqiqa tayyorlanadi. Terminalda
   `MUHIT TAYYOR` yozuvi chiqishini kuting.
5. Keyingi safar: github.com/codespaces sahifasidan o'sha codespace'ni oching (yangisini yaratmang).

Foydali: **Terminal → New Terminal** — yangi terminal; terminal panelidagi `+` yonidagi
bo'lish belgisi — ikki terminalni yonma-yon ochish.

## 1. Kompilyatsiya (10 daq)

```bash
AFL_USE_ASAN=1 afl-cc -g -o parser_afl parser.c            # AFL++ uchun
gcc -g -fsanitize=address,undefined -o parser parser.c     # qayta tiklash uchun
./parser seeds/ok.img                                       # -> logo: 64x32 (nisbat 2)
```
Zoom chatiga yozing: `1-qadam: logo: 64x32`

## 2. AFL++ fuzzing (15 daq)

```bash
afl-fuzz -i seeds -o out -x img.dict -V 900 -- ./parser_afl @@
```
Status ekranidagi `saved crashes` 0 dan katta bo'lganda skrinshot oling.
15 daqiqadan so'ng fuzzer o'zi to'xtaydi. Natijalar: `out/default/`.

## 3. Triage (15 daq)

```bash
ls out/default/crashes/
./parser out/default/crashes/id:000000*
for f in out/default/crashes/id*; do ./parser "$f" 2>&1 | grep -m1 SUMMARY; done | sort | uniq -c
afl-tmin -i out/default/crashes/id:000001* -o min.img -- ./parser_afl @@
xxd min.img
```
`id:000001*` o'rniga o'zingizdagi kerakli fayl nomini qo'ying.
Qulash faylini ko'rish: chap paneldagi faylni o'ng tugma bilan → **Open With… → Hex Editor**.

## 4. Tuzatish va regressiya

`parser.c` dan `parser_fixed.c` nusxa oling va tuzating, so'ng:
```bash
gcc -g -fsanitize=address,undefined -o parser_fixed parser_fixed.c
for f in seeds/ok.img out/default/crashes/id*; do ./parser_fixed "$f" >/dev/null 2>&1; echo "$f rc=$?"; done
```

## 5. boofuzz (15 daq, ikkita terminal)

1-terminal:
```bash
gcc -g -fsanitize=address -o ftp_lite ftp_lite.c
./run_target.sh
```
2-terminal:
```bash
python boofuzz_ftp.py
```
Veb-interfeys: pastdagi **Ports** yorlig'ida 26000-port → globus belgisi.
Qulash hisobotlari: `asan.*` fayllari. To'xtatish: ikkala terminalda `Ctrl+C`.

FAIL bo'lgan sinovni qo'lda takrorlash (3-terminal, server ishlab turganda):
```bash
printf 'USER anonymous\r\n' | nc -q1 127.0.0.1 2121                                  # javob keladi
python3 -c "print('USER ' + 'A'*40, end='\r\n')" | nc -q1 127.0.0.1 2121           # javob yo'q — qulash
ls asan.*
```

## 6. Natijalarni yuklab olish va topshirish

```bash
zip -r natijalar.zip out/default/crashes out/default/fuzzer_stats min*.img asan.* boofuzz-results parser_fixed.c ftp_lite_fixed.c
```
Chap panelda `natijalar.zip` → o'ng tugma → **Download**.
Xato hisobotlari (BUG-01…03) PDF ko'rinishida alohida topshiriladi.
CVSS kalkulyatori: https://www.first.org/cvss/calculator/3.1

## Darsdan keyin

github.com/codespaces → codespace yonidagi `…` → **Stop codespace**.
Baholash tugagach, **Delete** qiling (bepul limit saqlanadi).

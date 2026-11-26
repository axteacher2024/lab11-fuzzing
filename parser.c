#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct img { char name[16]; uint8_t w, h; };

int parse(const uint8_t *data, size_t len) {
    struct img im;
    if (len < 7 || memcmp(data, "IMG1", 4) != 0)
        return 0;
    uint8_t nlen = data[4];
    if (len < 5 + (size_t)nlen + 2)
        return 0;
    memcpy(im.name, data + 5, nlen);
    im.w = data[5 + nlen];
    im.h = data[6 + nlen];
    int ratio = im.w / im.h;
    printf("%.*s: %ux%u (nisbat %d)\n",
           nlen, im.name, im.w, im.h, ratio);
    return 1;
}

int main(int argc, char **argv) {
    FILE *f = argc > 1 ? fopen(argv[1], "rb") : stdin;
    if (!f) { perror("fopen"); return 1; }
    static uint8_t buf[4096];
    size_t len = fread(buf, 1, sizeof buf, f);
    if (f != stdin) fclose(f);
    parse(buf, len);
    return 0;
}

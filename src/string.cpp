extern "C" {

int memcmp(const void* a, const void* b, unsigned long size) {
    const unsigned char* x = (const unsigned char*)a;
    const unsigned char* y = (const unsigned char*)b;

    for (unsigned long i = 0; i < size; i++) {
        if (x[i] != y[i])
            return x[i] - y[i];
    }
    return 0;
}

int strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(unsigned char*)a - *(unsigned char*)b;
}

}

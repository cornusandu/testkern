#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BLOCK_SIZE 512
#define TOYFS_MAX_FILES 64

// -------- Superblock (LBA 0) --------
typedef struct {
    char     magic[5];       // "TOYFS"
    uint32_t root_offset;    // LBA of root directory
} ToySuperblock;

// -------- File entry --------
typedef struct {
    char     name[32];       // filename (null-terminated)
    uint32_t start_lba;      // starting block
    uint32_t size_in_blocks; // size in blocks
} ToyFile;

// -------- Root directory structure --------
typedef struct {
    uint32_t count;                  // file count
    ToyFile  entries[TOYFS_MAX_FILES];
} ToyRootDir;

// -------- Global root directory --------
extern ToyRootDir root_dir;

// -------- API exposed to kernel --------
extern "C" bool mount_toyfs();
extern "C" ToyFile* toyfs_find(const char* name);
extern "C" void toyfs_read(ToyFile* file, void* buffer);

// Kernel must provide these:
extern "C" int read_block(uint32_t lba, void* buffer);
extern "C" int read_blocks(uint32_t lba, uint32_t count, void* buffer);

#ifdef __cplusplus
}
#endif

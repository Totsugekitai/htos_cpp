#ifndef _LOADER_H_
#define _LOADER_H_

#include <stdint.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>

#include "bootinfo.h"

extern void jump_to_kernel(bootinfo_t *binfo, uint64_t *kernel_addr);

#define KERNEL_FILE_NAME L"htos.elf"
#define BUF_256B (UINTN)256
#define KERNEL_START (uint64_t *)0x100000

#endif

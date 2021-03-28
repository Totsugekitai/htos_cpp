#include "htloader.h"
#include "bootinfo.h"
#include "elf.h"

void halt()
{
    while(1);
}

EFI_STATUS
EFIAPI
UefiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    bootinfo_t binfo;
    EFI_STATUS status;

    // GraphicsOutputProtocolを取得
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    do {
        status = gBS->LocateProtocol(&gop_guid, NULL, (void **)&gop);
    } while (EFI_ERROR(status));

    // カーネルに渡す引数を設定
    // video_infoを詰める
    binfo.vinfo.frame_buffer_base = (pixel_t *)gop->Mode->FrameBufferBase;
    binfo.vinfo.frame_buffer_size = (uint64_t)gop->Mode->FrameBufferSize;
    binfo.vinfo.horizonal_resolution = (uint32_t)gop->Mode->Info->HorizontalResolution;
    binfo.vinfo.vertical_resolution = (uint32_t)gop->Mode->Info->VerticalResolution;
    binfo.vinfo.pixels_per_scan_line = (uint32_t)gop->Mode->Info->PixelsPerScanLine;
    switch (gop->Mode->Info->PixelFormat) {
    case PixelRedGreenBlueReserved8BitPerColor:
        binfo.vinfo.format = RGBColor;
        break;
    case PixelBlueGreenRedReserved8BitPerColor:
        binfo.vinfo.format = BGRColor;
        break;
    default:
        Print(L"Pixel Format is not supported\n");
        halt();
    }

    Print(L"[OK] prepare bootinfo\n");

    // SimpleFileSystemProtocolを取得
    EFI_GUID sfsp_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *sfsp;
    do {
        status = gBS->LocateProtocol(&sfsp_guid, NULL, (void **)&sfsp);
    } while(EFI_ERROR(status));

    Print(L"[OK] get SimpleFileSystemProtocol\n");

    // rootディレクトリを開く
    EFI_FILE_PROTOCOL *root;
    do {
        sfsp->OpenVolume(sfsp, &root);
    } while(EFI_ERROR(status));

    Print(L"[OK] open root directory\n");

    // カーネルのファイルを開く
    EFI_FILE_PROTOCOL *kernel_file;
    CHAR16 *file_name = (CHAR16 *)KERNEL_FILE_NAME;
    UINT64 file_mode = (UINT64)EFI_FILE_READ_ONLY;
    do {
        status = root->Open(root, &kernel_file, file_name, file_mode, 0);
    } while(EFI_ERROR(status));

    Print(L"[OK] open kernel file\n");

    // カーネルのファイルサイズを取得
    EFI_FILE_INFO file_info;
    EFI_GUID fi_guid = EFI_FILE_INFO_ID;
    UINTN buf_size = BUF_256B;
    do {
        status = kernel_file->GetInfo(kernel_file, &fi_guid, &buf_size, &file_info);
    } while(EFI_ERROR(status));
    UINTN file_size = file_info.FileSize;

    Print(L"[OK] get kernel file size\n");

    // カーネルファイルをメモリに読み込む
    char *kernel_program = NULL;
    do {
        status = gBS->AllocatePool(EfiLoaderData, file_size, (void **)(&kernel_program));
    } while(EFI_ERROR(status));

    Print(L"[OK] allocate buffer pool\n");

    do {
        status = kernel_file->Read(kernel_file, &file_size, kernel_program);
    } while(EFI_ERROR(status));

    Print(L"[OK] read kernel file\n");

    // bodyをメモリに書き込む
    uint64_t *start_addr = KERNEL_START;
    elf64_load_segments((char *)kernel_program);

    Print(L"[OK] copy kernel file to memory\n");

    // メモリマップ取得のための変数
    UINTN mmapsize = 0, mapkey, descsize;
    UINT32 descver;
    EFI_MEMORY_DESCRIPTOR *mmap = NULL;
    // ExitBootService()の処理を開始
    do {
        status = gBS->GetMemoryMap(&mmapsize, mmap, &mapkey, &descsize, &descver);
        while (status == EFI_BUFFER_TOO_SMALL) {
            if (mmap) {
                gBS->FreePool(mmap);
            }
            mmapsize += 0x1000;
            // メモリマップの領域を確保
            status = gBS->AllocatePool(EfiLoaderData, mmapsize, (void **)&mmap);
            // メモリマップを取得
            status = gBS->GetMemoryMap(&mmapsize, mmap, &mapkey, &descsize, &descver);
        }
        // ExitBootServices
        status = gBS->ExitBootServices(ImageHandle, mapkey);
    } while (EFI_ERROR(status));

    //for (uint64_t i = 0; i < binfo.vinfo.frame_buffer_size; i++) {
    //    pixel_t iro = {{0xff, 0xff, 0x00}, 0x00};
    //    binfo.vinfo.frame_buffer_base[i] = iro; // なんか適当な色で塗る
    //}

    void (*kernel_entry)(bootinfo_t *) = (void (*)(bootinfo_t *))start_addr;

    // カーネルに渡す情報をレジスタに格納
    // スタックポインタを設定しカーネルへジャンプ
    kernel_entry(&binfo);
    //jump_to_kernel(&binfo, start_addr);

    return EFI_SUCCESS;
}

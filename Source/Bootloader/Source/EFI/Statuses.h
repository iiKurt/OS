#pragma once

// Page 2212 - UEFI Specs 2.8b
#define EFI_SUCCESS               0x0000000000000000
#define EFI_ERROR                 0x8000000000000000 
#define EFI_LOAD_ERROR            (EFI_ERROR | 0x0000000000000001)
#define EFI_INVALID_PARAMETER     (EFI_ERROR | 0x0000000000000002)
#define EFI_UNSUPPORTED           (EFI_ERROR | 0x0000000000000003)
#define EFI_BAD_BUFFER_SIZE       (EFI_ERROR | 0x0000000000000004)
#define EFI_BUFFER_TOO_SMALL      (EFI_ERROR | 0x0000000000000005)
#define EFI_NOT_READY             (EFI_ERROR | 0x0000000000000006)
#define EFI_DEVICE_ERROR          (EFI_ERROR | 0x0000000000000007)
#define EFI_WRITE_PROTECTED       (EFI_ERROR | 0x0000000000000008)
#define EFI_OUT_OF_RESOURCES      (EFI_ERROR | 0x0000000000000009)
#define EFI_VOLUME_CORRUPTED      (EFI_ERROR | 0x000000000000000A)
#define EFI_VOLUME_FULL           (EFI_ERROR | 0x000000000000000B)
#define EFI_NO_MEDIA              (EFI_ERROR | 0x000000000000000C)
#define EFI_MEDIA_CHANGED         (EFI_ERROR | 0x000000000000000D)
#define EFI_NOT_FOUND             (EFI_ERROR | 0x000000000000000E)
#define EFI_ACCESS_DENIED         (EFI_ERROR | 0x000000000000000F)
#define EFI_NO_RESPONSE           (EFI_ERROR | 0x0000000000000010)
#define EFI_NO_MAPPING            (EFI_ERROR | 0x0000000000000011)
#define EFI_TIMEOUT               (EFI_ERROR | 0x0000000000000012)
#define EFI_NOT_STARTED           (EFI_ERROR | 0x0000000000000013)
#define EFI_ALREADY_STARTED       (EFI_ERROR | 0x0000000000000014)
#define EFI_ABORTED               (EFI_ERROR | 0x0000000000000015)
#define EFI_ICMP_ERROR            (EFI_ERROR | 0x0000000000000016)
#define EFI_TFTP_ERROR            (EFI_ERROR | 0x0000000000000017)
#define EFI_PROTOCOL_ERROR        (EFI_ERROR | 0x0000000000000018)
#define EFI_INCOMPATIBLE_VERSION  (EFI_ERROR | 0x0000000000000019)
#define EFI_SECURITY_VIOLATION    (EFI_ERROR | 0x000000000000001A)
#define EFI_CRC_ERROR             (EFI_ERROR | 0x000000000000001B)
#define EFI_END_OF_MEDIA          (EFI_ERROR | 0x000000000000001C)
#define EFI_END_OF_FILE           (EFI_ERROR | 0x000000000000001D)
#define EFI_INVALID_LANGUAGE      (EFI_ERROR | 0x000000000000001E)
#define EFI_COMPROMISED_DATA      (EFI_ERROR | 0x000000000000001F)
#define EFI_IP_ADDRESS_CONFLICT   (EFI_ERROR | 0x0000000000000020)
#define EFI_HTTP_ERROR            (EFI_ERROR | 0x0000000000000021)

// Page 2214 - UEFI Specs 2.8b
#define EFI_WARN_UNKNOWN_GLYPH     0x0000000000000001
#define EFI_WARN_DELETE_FAILURE    0x0000000000000002
#define EFI_WARN_WRITE_FAILURE     0x0000000000000003
#define EFI_WARN_BUFFER_TOO_SMALL  0x0000000000000004
#define EFI_WARN_STALE_DATA        0x0000000000000005
#define EFI_WARN_FILE_SYSTEM       0x0000000000000006
#define EFI_WARN_RESET_REQUIRED    0x0000000000000007

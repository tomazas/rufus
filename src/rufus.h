/*
 * Rufus: The Reliable USB Formatting Utility
 * Copyright © 2011-2016 Pete Batard <pete@akeo.ie>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <windows.h>
#include <malloc.h>
#include <inttypes.h>

#if defined(_MSC_VER)
// Disable some VS2012 Code Analysis warnings
#pragma warning(disable: 4996)		// Ignore deprecated (eg. GetVersionEx()), as we have to contend with XP
#pragma warning(disable: 28159)		// We use GetTickCount64() where possible, but it's not available on XP
#pragma warning(disable: 6258)		// I know what I'm using TerminateThread for
// Burn in HELL Windows XP!!!
#ifdef DDKBUILD
#if (_WIN32_WINNT < _WIN32_WINNT_VISTA)
#error The Windows XP target is no longer supported for WDK compilation.
#endif
#endif
#endif

#pragma once

/* Program options */
#define RUFUS_DEBUG                 // print debug info to Debug facility
/* Features not ready for prime time and that may *DESTROY* your data - USE AT YOUR OWN RISKS! */
// #define RUFUS_TEST

#define APPLICATION_NAME            "Rufus"
#define COMPANY_NAME                "Akeo Consulting"
#define STR_NO_LABEL                "NO_LABEL"
// Yes, there exist characters between these seemingly empty quotes!
#define LEFT_TO_RIGHT_MARK          "‎"
#define RIGHT_TO_LEFT_MARK          "‏"
#define LEFT_TO_RIGHT_EMBEDDING     "‪"
#define RIGHT_TO_LEFT_EMBEDDING     "‫"
#define POP_DIRECTIONAL_FORMATTING  "‬"
#define RIGHT_TO_LEFT_OVERRIDE      "‮"
#define DRIVE_ACCESS_TIMEOUT        15000		// How long we should retry drive access (in ms)
#define DRIVE_ACCESS_RETRIES        60			// How many times we should retry
#define DRIVE_INDEX_MIN             0x00000080
#define DRIVE_INDEX_MAX             0x000000C0
#define MIN_DRIVE_SIZE              8			// Minimum size a drive must have, to be formattable (in MB)
#define MIN_EXTRA_PART_SIZE         (1024*1024)	// Minimum size of the extra partition, in bytes
#define MAX_DRIVES                  (DRIVE_INDEX_MAX - DRIVE_INDEX_MIN)
#define MAX_TOOLTIPS                128
#define MAX_SIZE_SUFFIXES           6			// bytes, KB, MB, GB, TB, PB
#define MAX_CLUSTER_SIZES           18
#define MAX_PROGRESS                (0xFFFF-1)	// leave room for 1 more for insta-progress workaround
#define MAX_LOG_SIZE                0x7FFFFFFE
#define MAX_REFRESH                 25			// How long we should wait to refresh UI elements (in ms)
#define MAX_GUID_STRING_LENGTH      40
#define MAX_GPT_PARTITIONS          128
#define MAX_SECTORS_TO_CLEAR        128			// nb sectors to zap when clearing the MBR/GPT (must be >34)
#define MBR_UEFI_MARKER             0x49464555	// 'U', 'E', 'F', 'I', as a 32 bit little endian longword
#define STATUS_MSG_TIMEOUT          3500		// How long should cheat mode messages appear for on the status bar
#define WRITE_RETRIES               3
#define FS_DEFAULT                  FS_FAT32
#define SINGLE_CLUSTERSIZE_DEFAULT  0x00000100
#define BADBLOCK_PATTERNS           {0xaa, 0x55, 0xff, 0x00}
#define LARGE_FAT32_SIZE            (32*1073741824LL)	// Size at which we need to use fat32format
#define UDF_FORMAT_SPEED            3.1f		// Speed estimate at which we expect UDF drives to be formatted (GB/s)
#define UDF_FORMAT_WARN             20			// Duration (in seconds) above which we warn about long UDF formatting times
#define MAX_FAT32_SIZE              2.0f		// Threshold above which we disable FAT32 formatting (in TB)
#define FAT32_CLUSTER_THRESHOLD     1.011f		// For FAT32, cluster size changes don't occur at power of 2 boundaries but sligthly above
#define DD_BUFFER_SIZE              65536		// Minimum size of the buffer we use for DD operations
#define RUFUS_URL                   "http://rufus.akeo.ie"
#define DOWNLOAD_URL                RUFUS_URL "/downloads"
#define FILES_URL                   RUFUS_URL "/files"
#define SEVENZIP_URL                "http://sourceforge.net/projects/sevenzip/files/7-Zip/"
#define FILES_DIR                   "rufus_files"
#define IGNORE_RETVAL(expr)         do { (void)(expr); } while(0)
#ifndef ARRAYSIZE
#define ARRAYSIZE(A)                (sizeof(A)/sizeof((A)[0]))
#endif
#ifndef STRINGIFY
#define STRINGIFY(x)                #x
#endif
#define IsChecked(CheckBox_ID)      (IsDlgButtonChecked(hMainDialog, CheckBox_ID) == BST_CHECKED)
#define MB_IS_RTL                   (right_to_left_mode?MB_RTLREADING|MB_RIGHT:0)
#define CHECK_FOR_USER_CANCEL       if (IS_ERROR(FormatStatus)) goto out

#define safe_free(p) do {free((void*)p); p = NULL;} while(0)
#define safe_mm_free(p) do {_mm_free((void*)p); p = NULL;} while(0)
#define safe_min(a, b) min((size_t)(a), (size_t)(b))
#define safe_strcp(dst, dst_max, src, count) do {memcpy(dst, src, safe_min(count, dst_max)); \
	((char*)dst)[safe_min(count, dst_max)-1] = 0;} while(0)
#define safe_strcpy(dst, dst_max, src) safe_strcp(dst, dst_max, src, safe_strlen(src)+1)
#define safe_strncat(dst, dst_max, src, count) strncat(dst, src, safe_min(count, dst_max - safe_strlen(dst) - 1))
#define safe_strcat(dst, dst_max, src) safe_strncat(dst, dst_max, src, safe_strlen(src)+1)
#define safe_strcmp(str1, str2) strcmp(((str1==NULL)?"<NULL>":str1), ((str2==NULL)?"<NULL>":str2))
#define safe_strstr(str1, str2) strstr(((str1==NULL)?"<NULL>":str1), ((str2==NULL)?"<NULL>":str2))
#define safe_stricmp(str1, str2) _stricmp(((str1==NULL)?"<NULL>":str1), ((str2==NULL)?"<NULL>":str2))
#define safe_strncmp(str1, str2, count) strncmp(((str1==NULL)?"<NULL>":str1), ((str2==NULL)?"<NULL>":str2), count)
#define safe_strnicmp(str1, str2, count) _strnicmp(((str1==NULL)?"<NULL>":str1), ((str2==NULL)?"<NULL>":str2), count)
#define safe_closehandle(h) do {if ((h != INVALID_HANDLE_VALUE) && (h != NULL)) {CloseHandle(h); h = INVALID_HANDLE_VALUE;}} while(0)
#define safe_release_dc(hDlg, hDC) do {if ((hDC != INVALID_HANDLE_VALUE) && (hDC != NULL)) {ReleaseDC(hDlg, hDC); hDC = NULL;}} while(0)
#define safe_sprintf(dst, count, ...) do {_snprintf(dst, count, __VA_ARGS__); (dst)[(count)-1] = 0; } while(0)
#define static_sprintf(dst, ...) safe_sprintf(dst, sizeof(dst), __VA_ARGS__)
#define safe_strlen(str) ((((char*)str)==NULL)?0:strlen(str))
#define safe_strdup _strdup
#if defined(_MSC_VER)
#define safe_vsnprintf(buf, size, format, arg) _vsnprintf_s(buf, size, _TRUNCATE, format, arg)
#else
#define safe_vsnprintf vsnprintf
#endif

#ifdef RUFUS_DEBUG
extern void _uprintf(const char *format, ...);
#define uprintf(...) _uprintf(__VA_ARGS__)
#define ubclear() do { ubuffer[0] = 0; } while (0);
#define ubpushf(...) static_sprintf(ubuffer, __VA_ARGS__)
#define ubpop() uprintf("%s", ubuffer)
#define vuprintf(...) if (verbose) _uprintf(__VA_ARGS__)
#define vvuprintf(...) if (verbose > 1) _uprintf(__VA_ARGS__)
#define suprintf(...) if (!bSilent) _uprintf(__VA_ARGS__)
#define uuprintf(...) if (usb_debug) _uprintf(__VA_ARGS__)
#ifdef _DEBUG
#define duprintf(...) _uprintf(__VA_ARGS__)
#else
#define duprintf(...)
#endif
#else
#define uprintf(...)
#define vuprintf(...)
#define vvuprintf(...)
#define duprintf(...)
#define suprintf(...)
#endif

/* Custom Windows messages */
enum user_message_type {
	UM_FORMAT_COMPLETED = WM_APP,
	UM_MEDIA_CHANGE,
	UM_PROGRESS_INIT,
	UM_PROGRESS_EXIT,
	UM_NO_UPDATE,
	UM_SET_PARTITION_SCHEME_TOOLTIP,
	UM_ISO_SCAN_COMPLETE,
	// Start of the WM IDs for the language menu items
	UM_LANGUAGE_MENU = WM_APP + 0x100
};

/* Custom notifications */
enum notification_type {
	MSG_INFO,
	MSG_WARNING,
	MSG_ERROR,
	MSG_QUESTION,
};
typedef INT_PTR (CALLBACK *Callback_t)(HWND, UINT, WPARAM, LPARAM);
typedef struct {
	WORD id;
	Callback_t callback;
} notification_info;	// To provide a "More info..." on notifications

/* Status Bar sections */
#define SB_SECTION_LEFT         0
#define SB_SECTION_MIDDLE       1
#define SB_SECTION_RIGHT        2
#define SB_TIMER_SECTION_SIZE   58.0f

/* Timers used throughout the program */
enum timer_type {
	TID_MESSAGE_INFO = 0x1000,
	TID_MESSAGE_STATUS,
	TID_OUTPUT_INFO,
	TID_OUTPUT_STATUS,
	TID_BADBLOCKS_UPDATE,
	TID_APP_TIMER,
	TID_BLOCKING_TIMER,
	TID_REFRESH_TIMER
};

/* Action type, for progress bar breakdown */
enum action_type {
	OP_ANALYZE_MBR,
	OP_BADBLOCKS,
	OP_ZERO_MBR,
	OP_PARTITION,
	OP_FORMAT,
	OP_CREATE_FS,
	OP_FIX_MBR,
	OP_DOS,
	OP_FINALIZE,
	OP_MAX
};

/* File system indexes in our FS combobox */
enum fs_type {
	FS_UNKNOWN = -1,
	FS_FAT16 = 0,
	FS_FAT32,
	FS_NTFS,
	FS_UDF,
	FS_EXFAT,
	FS_REFS,
	FS_MAX
};

enum boot_type {
	BT_MSDOS = 0,
	BT_FREEDOS,
	BT_ISO,
	BT_IMG,
	BT_SYSLINUX_V4,		// Start of indexes that only display in advanced mode
	BT_SYSLINUX_V6,
	BT_REACTOS,
	BT_GRUB4DOS,
	BT_GRUB2,
	BT_UEFI_NTFS,
	BT_MAX
};

enum target_type {
	TT_BIOS = 0,
	TT_UEFI,
	TT_MAX
};
// For the partition types we'll use Microsoft's PARTITION_STYLE_### constants
#define GETTARGETTYPE(x) (((x)>0)?(((x) >> 16) & 0xFFFF):0)
#define GETPARTTYPE(x)   (((x)>0)?((x) & 0xFFFF):0);

enum checksum_type {
	CHECKSUM_MD5 = 0,
	CHECKSUM_SHA1,
	CHECKSUM_SHA256,
	CHECKSUM_MAX
};

/* Special handling for old .c32 files we need to replace */
#define NB_OLD_C32          2
#define OLD_C32_NAMES       { "menu.c32", "vesamenu.c32" }
#define OLD_C32_THRESHOLD   { 53500, 148000 }

/* ISO details that the application may want */
#define WINPE_MININT        0x2A
#define WINPE_I386          0x15
#define MAX_WIM_VERSION     0x000E0000
#define HAS_SYSLINUX(r)     (r.sl_version != 0)
#define HAS_INSTALL_WIM(r)  (r.install_wim_path[0] != 0)
#define HAS_TOGO(r)         (r.has_bootmgr && r.has_efi && HAS_INSTALL_WIM(r) && (r.install_wim_version < MAX_WIM_VERSION))
#define IS_WINPE(r)         (((r & WINPE_MININT) == WINPE_MININT)||(( r & WINPE_I386) == WINPE_I386))
#define IS_WIN7_EFI(r)      ((r.has_efi == 1) && HAS_INSTALL_WIM(r))
#define IS_REACTOS(r)       (r.reactos_path[0] != 0)
#define IS_GRUB(r)          ((r.has_grub2) || (r.has_grub4dos))
#define IS_FAT(fs)          ((fs == FS_FAT16) || (fs == FS_FAT32))

typedef struct {
	char label[192];			/* 3*64 to account for UTF-8 */
	char usb_label[192];		/* converted USB label for workaround */
	char cfg_path[128];			/* path to the ISO's isolinux.cfg */
	char reactos_path[128];		/* path to the ISO's freeldr.sys or setupldr.sys */
	char install_wim_path[64];	/* path to install.wim or install.swm */
	uint64_t projected_size;
	uint32_t install_wim_version;
	BOOLEAN is_iso;
	BOOLEAN is_bootable_img;
	uint8_t winpe;
	uint8_t has_efi;
	BOOLEAN has_4GB_file;
	BOOLEAN has_long_filename;
	BOOLEAN has_symlinks;
	BOOLEAN has_bootmgr;
	BOOLEAN has_autorun;
	BOOLEAN has_old_c32[NB_OLD_C32];
	BOOLEAN has_old_vesamenu;
	BOOLEAN has_efi_syslinux;
	BOOLEAN needs_syslinux_overwrite;
	BOOLEAN has_grub4dos;
	BOOLEAN has_grub2;
	BOOLEAN has_kolibrios;
	BOOLEAN uses_minint;
	BOOLEAN compression_type;
	BOOLEAN is_vhd;
	uint16_t sl_version;	// Syslinux/Isolinux version
	char sl_version_str[12];
	char sl_version_ext[32];
	char grub2_version[32];
} RUFUS_IMG_REPORT;

/* Isolate the Syslinux version numbers */
#define SL_MAJOR(x) ((uint8_t)((x)>>8))
#define SL_MINOR(x) ((uint8_t)(x))

typedef struct {
	uint16_t version[3];
	uint32_t platform_min[2];		// minimum platform version required
	char* download_url;
	char* release_notes;
} RUFUS_UPDATE;

typedef struct {
	DWORD DeviceNum;
	char* path;
} VHD_SAVE;

/*
 * Structure and macros used for the extensions specification of FileDialog()
 * You can use:
 *   EXT_DECL(my_extensions, "default.std", __VA_GROUP__("*.std", "*.other"), __VA_GROUP__("Standard type", "Other Type"));
 * to define an 'ext_t my_extensions' variable initialized with the relevant attributes.
 */
typedef struct ext_t {
	const size_t count;
	const char* filename;
	const char** extension;
	const char** description;
} ext_t;

#ifndef __VA_GROUP__
#define __VA_GROUP__(...)  __VA_ARGS__
#endif
#define EXT_X(prefix, ...) const char* _##prefix##_x[] = { __VA_ARGS__ }
#define EXT_D(prefix, ...) const char* _##prefix##_d[] = { __VA_ARGS__ }
#define EXT_DECL(var, filename, extensions, descriptions)                   \
	EXT_X(var, extensions);                                                 \
	EXT_D(var, descriptions);                                               \
	ext_t var = { ARRAYSIZE(_##var##_x), filename, _##var##_x, _##var##_d }

/* Duplication of the TBPFLAG enum for Windows 7 taskbar progress */
typedef enum TASKBAR_PROGRESS_FLAGS
{
	TASKBAR_NOPROGRESS = 0,
	TASKBAR_INDETERMINATE = 0x1,
	TASKBAR_NORMAL = 0x2,
	TASKBAR_ERROR = 0x4,
	TASKBAR_PAUSED = 0x8
} TASKBAR_PROGRESS_FLAGS;

/* Windows versions */
enum WindowsVersion {
	WINDOWS_UNDEFINED = -1,
	WINDOWS_UNSUPPORTED = 0,
	WINDOWS_XP = 0x51,
	WINDOWS_2003 = 0x52,	// Also XP x64
	WINDOWS_VISTA = 0x60,
	WINDOWS_7 = 0x61,
	WINDOWS_8 = 0x62,
	WINDOWS_8_1 = 0x63,
	WINDOWS_10_PREVIEW1 = 0x64,
	WINDOWS_10 = 0xA0,
	WINDOWS_MAX
};


/*
 * Globals
 */
extern HINSTANCE hMainInstance;
extern HWND hMainDialog, hLogDlg, hStatus, hDeviceList, hCapacity;
extern HWND hPartitionScheme, hFileSystem, hClusterSize, hLabel, hBootType, hNBPasses, hLog;
extern HWND hInfo, hProgress, hDiskID, hStatusToolbar;
extern float fScale;
extern char szFolderPath[MAX_PATH], app_dir[MAX_PATH], system_dir[MAX_PATH], sysnative_dir[MAX_PATH];
extern char* image_path;
extern DWORD FormatStatus, DownloadStatus, MainThreadId;
extern BOOL PromptOnError;
extern unsigned long syslinux_ldlinux_len[2];
extern const int nb_steps[FS_MAX];
extern BOOL use_own_c32[NB_OLD_C32], detect_fakes, iso_op_in_progress, format_op_in_progress, right_to_left_mode;
extern BOOL allow_dual_uefi_bios, togo_mode;
extern RUFUS_IMG_REPORT img_report;
extern int64_t iso_blocking_status;
extern uint16_t rufus_version[3], embedded_sl_version[2];
extern int nWindowsVersion;
extern char WindowsVersionStr[128];
extern char ubuffer[256];
extern char embedded_sl_version_str[2][12];
extern RUFUS_UPDATE update;
extern int dialog_showing;
extern WORD selected_langid;

/*
 * Shared prototypes
 */
extern void GetWindowsVersion(void);
extern BOOL is_x64(void);
extern const char *WindowsErrorString(void);
extern void DumpBufferHex(void *buf, size_t size);
extern void PrintStatusInfo(BOOL info, BOOL debug, unsigned int duration, int msg_id, ...);
#define PrintStatus(...) PrintStatusInfo(FALSE, FALSE, __VA_ARGS__)
#define PrintStatusDebug(...) PrintStatusInfo(FALSE, TRUE, __VA_ARGS__)
#define PrintInfo(...) PrintStatusInfo(TRUE, FALSE, __VA_ARGS__)
#define PrintInfoDebug(...) PrintStatusInfo(TRUE, TRUE, __VA_ARGS__)
extern void UpdateProgress(int op, float percent);
extern const char* StrError(DWORD error_code, BOOL use_default_locale);
extern char* GuidToString(const GUID* guid);
extern char* SizeToHumanReadable(uint64_t size, BOOL copy_to_log, BOOL fake_units);
extern HWND MyCreateDialog(HINSTANCE hInstance, int Dialog_ID, HWND hWndParent, DLGPROC lpDialogFunc);
extern INT_PTR MyDialogBox(HINSTANCE hInstance, int Dialog_ID, HWND hWndParent, DLGPROC lpDialogFunc);
extern void CenterDialog(HWND hDlg);
extern void ResizeMoveCtrl(HWND hDlg, HWND hCtrl, int dx, int dy, int dw, int dh, float scale);
extern void CreateStatusBar(void);
extern void SetTitleBarIcon(HWND hDlg);
extern BOOL CreateTaskbarList(void);
extern BOOL SetTaskbarProgressState(TASKBAR_PROGRESS_FLAGS tbpFlags);
extern BOOL SetTaskbarProgressValue(ULONGLONG ullCompleted, ULONGLONG ullTotal);
extern INT_PTR CreateAboutBox(void);
extern BOOL CreateTooltip(HWND hControl, const char* message, int duration);
extern void DestroyTooltip(HWND hWnd);
extern void DestroyAllTooltips(void);
extern BOOL Notification(int type, const notification_info* more_info, char* title, char* format, ...);
extern int Selection(char* title, char* message, char* selection1, char* selection2);
extern SIZE GetTextSize(HWND hCtrl);
extern BOOL ExtractDOS(const char* path);
extern BOOL ExtractISO(const char* src_iso, const char* dest_dir, BOOL scan);
extern int64_t ExtractISOFile(const char* iso, const char* iso_file, const char* dest_file, DWORD attributes);
extern char* MountISO(const char* path);
extern void UnMountISO(void);
extern BOOL InstallSyslinux(DWORD drive_index, char drive_letter, int fs);
extern uint16_t GetSyslinuxVersion(char* buf, size_t buf_size, char** ext);
extern BOOL CreateProgress(void);
extern BOOL SetAutorun(const char* path);
extern char* FileDialog(BOOL save, char* path, const ext_t* ext, DWORD options);
extern BOOL FileIO(BOOL save, char* path, char** buffer, DWORD* size);
extern unsigned char* GetResource(HMODULE module, char* name, char* type, const char* desc, DWORD* len, BOOL duplicate);
extern DWORD GetResourceSize(HMODULE module, char* name, char* type, const char* desc);
extern DWORD RunCommand(const char* cmdline, const char* dir, BOOL log);
extern BOOL CompareGUID(const GUID *guid1, const GUID *guid2);
extern BOOL GetDevices(DWORD devnum);
extern BOOL SetLGP(BOOL bRestore, BOOL* bExistingKey, const char* szPath, const char* szPolicy, DWORD dwValue);
extern LONG GetEntryWidth(HWND hDropDown, const char* entry);
extern DWORD DownloadFile(const char* url, const char* file, HWND hProgressDialog);
extern HANDLE DownloadFileThreaded(const char* url, const char* file, HWND hProgressDialog);
extern INT_PTR CALLBACK UpdateCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
extern BOOL SetUpdateCheck(void);
extern BOOL CheckForUpdates(BOOL force);
extern void DownloadNewVersion(void);
extern BOOL IsShown(HWND hDlg);
extern char* get_token_data_file(const char* token, const char* filename);
extern char* set_token_data_file(const char* token, const char* data, const char* filename);
extern char* get_token_data_buffer(const char* token, unsigned int n, const char* buffer, size_t buffer_size);
extern char* insert_section_data(const char* filename, const char* section, const char* data, BOOL dos2unix);
extern char* replace_in_token_data(const char* filename, const char* token, const char* src, const char* rep, BOOL dos2unix);
extern char* replace_char(const char* src, const char c, const char* rep);
extern void parse_update(char* buf, size_t len);
extern uint8_t WimExtractCheck(void);
extern BOOL WimExtractFile(const char* wim_image, int index, const char* src, const char* dst);
extern BOOL WimApplyImage(const char* image, int index, const char* dst);
extern BOOL IsBootableImage(const char* path);
extern BOOL AppendVHDFooter(const char* vhd_path);
extern int IsHDD(DWORD DriveIndex, uint16_t vid, uint16_t pid, const char* strid);
extern LONG ValidateSignature(HWND hDlg, const char* path);
extern BOOL IsFontAvailable(const char* font_name);
extern BOOL WriteFileWithRetry(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,
	LPDWORD lpNumberOfBytesWritten, DWORD nNumRetries);
extern BOOL SetThreadAffinity(DWORD_PTR* thread_affinity, size_t num_threads);
extern BOOL HashFile(const unsigned type, const char* path, uint8_t* sum);
extern BOOL HashBuffer(const unsigned type, const unsigned char* buf, const size_t len, uint8_t* sum);
extern BOOL IsFileInDB(const char* path);
extern BOOL IsBufferInDB(const unsigned char* buf, const size_t len);
#define printbits(x) _printbits(sizeof(x), &x, 0)
#define printbitslz(x) _printbits(sizeof(x), &x, 1)
extern char* _printbits(size_t const size, void const * const ptr, int leading_zeroes);

DWORD WINAPI FormatThread(void* param);
DWORD WINAPI SaveImageThread(void* param);
DWORD WINAPI SumThread(void* param);

/* Hash tables */
typedef struct htab_entry {
	uint32_t used;
	char* str;
	void* data;
} htab_entry;
typedef struct htab_table {
	htab_entry *table;
	uint32_t size;
	uint32_t filled;
} htab_table;
#define HTAB_EMPTY {NULL, 0, 0}
extern BOOL htab_create(uint32_t nel, htab_table* htab);
extern void htab_destroy(htab_table* htab);
extern uint32_t htab_hash(char* str, htab_table* htab);

/* Basic String Array */
typedef struct {
	char**   String;
	uint32_t Index;		// Current array size
	uint32_t Max;		// Maximum array size
} StrArray;
extern void StrArrayCreate(StrArray* arr, uint32_t initial_size);
extern int32_t StrArrayAdd(StrArray* arr, const char* str);
extern void StrArrayClear(StrArray* arr);
extern void StrArrayDestroy(StrArray* arr);
#define IsStrArrayEmpty(arr) (arr.Index == 0)

/*
 * typedefs for the function prototypes. Use the something like:
 *   PF_DECL(FormatEx);
 * which translates to:
 *   FormatEx_t pfFormatEx = NULL;
 * in your code, to declare the entrypoint and then use:
 *   PF_INIT(FormatEx, Fmifs);
 * which translates to:
 *   pfFormatEx = (FormatEx_t) GetProcAddress(GetDLLHandle("fmifs"), "FormatEx");
 * to make it accessible.
 */
#define         MAX_LIBRARY_HANDLES 32
extern HMODULE  OpenedLibrariesHandle[MAX_LIBRARY_HANDLES];
extern uint16_t OpenedLibrariesHandleSize;
#define         OPENED_LIBRARIES_VARS HMODULE OpenedLibrariesHandle[MAX_LIBRARY_HANDLES]; uint16_t OpenedLibrariesHandleSize = 0
#define         CLOSE_OPENED_LIBRARIES while(OpenedLibrariesHandleSize > 0) FreeLibrary(OpenedLibrariesHandle[--OpenedLibrariesHandleSize])
static __inline HMODULE GetLibraryHandle(char* szLibraryName) {
	HMODULE h = NULL;
	if ((h = GetModuleHandleA(szLibraryName)) == NULL) {
		if (OpenedLibrariesHandleSize >= MAX_LIBRARY_HANDLES) {
			uprintf("Error: MAX_LIBRARY_HANDLES is too small\n");
		} else {
			h = LoadLibraryA(szLibraryName);
			if (h != NULL)
				OpenedLibrariesHandle[OpenedLibrariesHandleSize++] = h;
		}
	}
	return h;
}
#define PF_TYPE(api, ret, proc, args)		typedef ret (api *proc##_t)args
#define PF_DECL(proc)						static proc##_t pf##proc = NULL
#define PF_TYPE_DECL(api, ret, proc, args)	PF_TYPE(api, ret, proc, args); PF_DECL(proc)
#define PF_INIT(proc, name)					if (pf##proc == NULL) pf##proc = \
	(proc##_t) GetProcAddress(GetLibraryHandle(#name), #proc)
#define PF_INIT_OR_OUT(proc, name)			do {PF_INIT(proc, name);         \
	if (pf##proc == NULL) {uprintf("Unable to locate %s() in %s.dll: %s\n",  \
	#proc, #name, WindowsErrorString()); goto out;} } while(0)

/* Custom application errors */
#define FAC(f)                         (f<<16)
#define APPERR(err)                    (APPLICATION_ERROR_MASK|err)
#define ERROR_INCOMPATIBLE_FS          0x1201
#define ERROR_CANT_QUICK_FORMAT        0x1202
#define ERROR_INVALID_CLUSTER_SIZE     0x1203
#define ERROR_INVALID_VOLUME_SIZE      0x1204
#define ERROR_CANT_START_THREAD        0x1205
#define ERROR_BADBLOCKS_FAILURE        0x1206
#define ERROR_ISO_SCAN                 0x1207
#define ERROR_ISO_EXTRACT              0x1208
#define ERROR_CANT_REMOUNT_VOLUME      0x1209
#define ERROR_CANT_PATCH               0x120A
#define ERROR_CANT_ASSIGN_LETTER       0x120B
#define ERROR_CANT_MOUNT_VOLUME        0x120C

/* GetTickCount64 not being available on XP is a massive bother */
PF_TYPE(WINAPI, ULONGLONG, GetTickCount64, (void));
extern GetTickCount64_t pfGetTickCount64;
#define _GetTickCount64() ((pfGetTickCount64 != NULL)?(uint64_t)pfGetTickCount64():(uint64_t)GetTickCount())

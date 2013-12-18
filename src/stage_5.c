#include <string.h>
#include <windows.h>
#include <psapi.h>

#include "main.h"
#include "crc32.h"
static void placeholder_replace(MODULEINFO *mi,
                                IMAGE_EXPORT_DIRECTORY *export_dir,
                                char *placeholder_name,
                                char *new_name);

static IMAGE_EXPORT_DIRECTORY *exportdir_va(MODULEINFO *mi);
static MODULEINFO             *mod_by_name_crc32(uint32_t crc);
static char                   *exportname_gen();
static void                    dostuff();

int __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    (void)hinstDLL;
    (void)lpvReserved;

    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            dostuff();
            break;

        case DLL_PROCESS_DETACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
    }
    return 1;
}

/* Placeholder for generated time-based name.
 * Fuction must return 0x35371337
 */
int __stdcall FFFFFFFFFFFFFFFF(void)
{
    return 0x35371337;
}

/* Get the virtual address of the export directory.
*/
static IMAGE_EXPORT_DIRECTORY *exportdir_va(MODULEINFO *mi)
{
    IMAGE_DOS_HEADER *dos_head;
    IMAGE_NT_HEADERS *nt_head;
    IMAGE_EXPORT_DIRECTORY *export_dir;

    dos_head = mi->lpBaseOfDll;
    nt_head = mi->lpBaseOfDll + dos_head->e_lfanew;
    export_dir = mi->lpBaseOfDll +
                 nt_head->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

    return export_dir;
}

/* Search the loaded module by crc32.
*/
static MODULEINFO *mod_by_name_crc32(uint32_t crc)
{
    static MODULEINFO mi;
    char modname[100];
    size_t nmods;
    unsigned long needed;
    size_t i;

    HMODULE mods[100];

    /* Enumerate loaded modules.
     * No more than 100 modules will be enumerated.
     */
    if (EnumProcessModules(GetCurrentProcess(), mods, sizeof(mods), &needed) == 0){
        return NULL;
    }

    nmods = needed/sizeof(HMODULE);

    for (i = 0; i < nmods; i++){
        /* Compute crc32 of each module name and search for the needed crc32.
        *  Return a pointer to MODULEINFO if module with correct crc32 found.
        *  Or NULL otherwise.
        */
        if (GetModuleBaseName(GetCurrentProcess(), mods[i], modname, sizeof(modname)) == 0){
            return NULL;
        }

        if ( crc == crc32(modname, strlen(modname), sizeof(char))){

            if (GetModuleInformation(GetCurrentProcess(), mods[i], &mi, sizeof(mi)) == 0){
                return NULL;
            } else {
                return &mi;
            }
        }
    }
    return NULL;
}

static char *exportname_gen()
{
    static char name[16+1];
    SYSTEMTIME st;

    /* Generate the name of the exported function based
    *  on local time.
    */
    GetLocalTime(&st);
    sprintf(name,"%04X%04X%04X%04X", ~st.wYear  & 0x0000FFFF,
                                     ~st.wMonth & 0x0000FFFF,
                                     ~st.wDay   & 0x0000FFFF,
                                     ~st.wHour  & 0x0000FFFF);

    return &name[0];
}

static void placeholder_replace(MODULEINFO *mi, IMAGE_EXPORT_DIRECTORY *export_dir, char *placeholder_name, char *new_name)
{
    unsigned long *names_rva_arr = mi->lpBaseOfDll + export_dir->AddressOfNames;
    unsigned long  n = export_dir->NumberOfNames;
    char *name;
    size_t i;

    /* Replace the exported placeholder string to new one
    *  that is provided.
    */
    for (i = 0; i < n; i++){
        name = mi->lpBaseOfDll + names_rva_arr[i];
        if (strcmp(name, placeholder_name) == 0){
            strcpy(name, new_name);
        }
    }
}

void dostuff()
{
    MODULEINFO *mi;
    IMAGE_EXPORT_DIRECTORY *export_dir;
    unsigned long oldprot;

    mi = mod_by_name_crc32(0x35370000);
    if (mi == NULL){
        return;
    }

    /* Grant PAGE_EXECUTE_READWRITE access rights and
    *  replace the placeholder export to correct one.
    */
    VirtualProtect(mi->lpBaseOfDll, mi->SizeOfImage, PAGE_EXECUTE_READWRITE, &oldprot);
    export_dir = exportdir_va(mi);
    placeholder_replace(mi, export_dir, "FFFFFFFFFFFFFFFF", exportname_gen());
}

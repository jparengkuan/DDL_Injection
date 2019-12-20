#include <iostream>
#include <Windows.h>

int main() {

    // Pad naar het DLL Bestand
    LPCSTR dllPath = "";

    // Open een Handle naar het doel process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 000);

    // Gegeughen alloceren voor het pad naar het DLL bestand in het doel process
    // Lengte van de Pad String + een null terminator

    LPVOID pdllPath = VirtualAllocEx(hProcess, 0, strlen(dllPath) + 1,
                                     MEM_COMMIT, PAGE_READWRITE);

    // Schrijf het pad naar geheugen wat we zojuist hebben gealloceerd in het doel process
    WriteProcessMemory(hProcess, pdllPath, (LPVOID) dllPath, strlen(dllPath) + 1, 0);

    // Maak een remote thread aan in het doelprocess dat de functie
    // LoadLibraryA aanroept met onze dllPath als argument
    // Het doelProgramma laad ons DLL bestand

    HANDLE hLoadThread = CreateRemoteThread(hProcess, 0, 0,
                                            (LPTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandleA("kernel32.dll"),
                                                                                    "LoadLibraryA"), pdllPath, 0, 0);

    // Wacht totdat onze loaderThread is uitgevoerd
    WaitForSingleObject(hLoadThread, INFINITE);

    std::cout << "DLL Path gealloceerd -> " << dllPath << std::endl;
    std::cin.get();

    


    return 0;
}

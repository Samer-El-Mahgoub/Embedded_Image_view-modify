#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) { 
    if (argc != 3) {
        printf("Usage: %s <target_exe> <new_image.bmp>\n", argv[0]);
        return 1;
    }

    const char *exe_path = argv[1];
    const char *new_image_path = argv[2]; 

    FILE *file = fopen(new_image_path, "rb");
    if (!file) {
        printf("Error: Could not open the new image '%s'.\n", new_image_path);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *fileData = (char*)malloc(fileSize);
    fread(fileData, 1, fileSize, file);
    fclose(file);

    char *resourceData = fileData + 14; 
    DWORD resourceSize = fileSize - 14;

    printf("Injecting '%s' into '%s'...\n", new_image_path, exe_path);

    HANDLE hUpdate = BeginUpdateResource(exe_path, FALSE);
    if (hUpdate == NULL) {
        printf("Error: Could not open '%s' for updating.\n", exe_path);
        free(fileData);
        return 1;
    }

    // Update the bitmap resource (ID 101) with the new image data
    BOOL result = UpdateResource(
        hUpdate,
        RT_BITMAP,
        MAKEINTRESOURCE(101), 
        MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
        resourceData,
        resourceSize
    );

    if (!result) {
        //If the update fails, print the error code for debugging
        printf("Error: Failed to update the resource. Windows Error Code: %lu\n", GetLastError());
        EndUpdateResource(hUpdate, TRUE); 
        free(fileData);
        return 1;
    }

    EndUpdateResource(hUpdate, FALSE); 

    printf("Success: Image replaced successfully!\n");
    
    free(fileData);
    return 0;
}
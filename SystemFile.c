// import library yang diperlukan
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 255

// Struktur data untuk file
typedef struct File
{
    char name[MAX_NAME];
    struct File *next;
} File;

// Struktur data untuk direktori
typedef struct Directory
{
    char name[MAX_NAME];
    struct Directory *parent;
    struct Directory *subdirs;
    struct Directory *next;
    File *files;
} Directory;

// Fungsi untuk membuat direktori baru
Directory *create_directory(const char *name, Directory *parent);

// Fungsi untuk membuat file baru
File *create_file(const char *name);

// Fungsi untuk menambahkan subdirektori ke dalam direktori tertentu
void add_subdirectory(Directory *parent, Directory *subdir);

// Fungsi untuk menambahkan file ke dalam direktori tertentu
void add_file(Directory *dir, File *file);

// Fungsi untuk mengubah direktori saat ini
Directory *change_directory(Directory *current, const char *name);

// Fungsi untuk menghapus file dari sebuah direktori
void delete_file(Directory *dir, const char *name);

// Fungsi untuk menghapus sebuah direktori beserta isinya
void delete_directory(Directory *parent, const char *name);

// Fungsi untuk mengubah nama sebuah direktori
void rename_directory(Directory *dir, const char *new_name);

// Fungsi untuk mengubah nama sebuah file
void rename_file(File *file, const char *new_name);

// Fungsi untuk menampilkan isi direktori
void list_directory(Directory *dir);

// Fungsi untuk menghapus semua struktur data direktori secara rekursif
void free_directory(Directory *dir);

Directory *create_directory(const char *name, Directory *parent)
{
    Directory *dir = (Directory *)malloc(sizeof(Directory));
    strcpy(dir->name, name);
    dir->parent = parent;
    dir->subdirs = NULL;
    dir->next = NULL;
    dir->files = NULL;
    return dir;
}

File *create_file(const char *name)
{
    File *file = (File *)malloc(sizeof(File));
    strcpy(file->name, name);
    file->next = NULL;
    return file;
}

void add_subdirectory(Directory *parent, Directory *subdir)
{
    // Mengecek apakah sudah ada subdirektori dengan nama yang sama
    Directory *existing_subdir = parent->subdirs;
    while (existing_subdir)
    {
        if (strcmp(existing_subdir->name, subdir->name) == 0)
        {
            printf("Sudah ada direktori dengan nama tersebut\n");
            return;
        }
        existing_subdir = existing_subdir->next;
    }

    // Jika tidak ada subdirektori dengan nama yang sama, tambahkan subdirektori baru
    subdir->next = parent->subdirs;
    parent->subdirs = subdir;
}

void add_file(Directory *dir, File *file)
{
    // Mengecek apakah sudah ada file dengan nama yang sama
    File *existing_file = dir->files;
    while (existing_file)
    {
        if (strcmp(existing_file->name, file->name) == 0)
        {
            printf("Sudah ada file dengan nama tersebut\n");
            return;
        }
        existing_file = existing_file->next;
    }

    // Jika tidak ada file dengan nama yang sama, tambahkan file baru
    file->next = dir->files;
    dir->files = file;
}


Directory *change_directory(Directory *current, const char *name)
{
    if (strcmp(name, "..") == 0)
        return current->parent ? current->parent : current;

    Directory *subdir = current->subdirs;
    while (subdir)
    {
        if (strcmp(subdir->name, name) == 0)
            return subdir;
        subdir = subdir->next;
    }
    printf("Direktori tidak ditemukan\n");
    return current;
}

void delete_file(Directory *dir, const char *name)
{
    File *file = dir->files, *prev = NULL;
    while (file)
    {
        if (strcmp(file->name, name) == 0)
        {
            if (prev)
                prev->next = file->next;
            else
                dir->files = file->next;
            free(file);
            return;
        }
        prev = file;
        file = file->next;
    }
    printf("File tidak ditemukan\n");
}

void delete_directory(Directory *parent, const char *name)
{
    Directory *dir = parent->subdirs, *prev = NULL;
    while (dir)
    {
        if (strcmp(dir->name, name) == 0)
        {
            if (prev)
                prev->next = dir->next;
            else
                parent->subdirs = dir->next;
            free_directory(dir); // Free the memory used by the directory
            return;
        }
        prev = dir;
        dir = dir->next;
    }
    printf("Direktori tidak ditemukan\n");
}

void rename_directory(Directory *dir, const char *new_name)
{
    strcpy(dir->name, new_name);
}

void rename_file(File *file, const char *new_name)
{
    strcpy(file->name, new_name);
}

void list_directory(Directory *dir)
{
    printf("Direktori saat ini: %s\n", dir->name);
    printf("Subdirectories:\n");
    Directory *subdir = dir->subdirs;
    while (subdir)
    {
        printf("  %s/\n", subdir->name);
        subdir = subdir->next;
    }
    printf("Files:\n");
    File *file = dir->files;
    while (file)
    {
        printf("  %s\n", file->name);
        file = file->next;
    }
}

void free_directory(Directory *dir)
{
    while (dir->subdirs)
    {
        Directory *subdir = dir->subdirs;
        dir->subdirs = subdir->next;
        free_directory(subdir);
    }
    while (dir->files)
    {
        File *file = dir->files;
        dir->files = file->next;
        free(file);
    }
    free(dir);
}


int main()
{
    // Membuat direktori root
    Directory *root = create_directory("root", NULL);
    Directory *current = root;

    char command[MAX_NAME];
    char arg1[MAX_NAME];
    char arg2[MAX_NAME];
    while (1)
    {
        printf("\n%s> ", current->name);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Menghapus karakter newline dari input

        // Memproses perintah
        if (sscanf(command, "mkdir %s", arg1) == 1)
        {
            // Menambahkan subdirektori baru
            add_subdirectory(current, create_directory(arg1, current));
        }
        else if (sscanf(command, "cd %s", arg1) == 1)
        {
            // Mengubah direktori saat ini
            current = change_directory(current, arg1);
        }
        else if (sscanf(command, "mkfile %s", arg1) == 1)
        {
            // Menambahkan file baru
            add_file(current, create_file(arg1));
        }
        else if (sscanf(command, "rmfile %s", arg1) == 1)
        {
            // Menghapus file
            delete_file(current, arg1);
        }
        else if (sscanf(command, "rmdir %s", arg1) == 1)
        {
            // Menghapus direktori
            delete_directory(current, arg1);
        }
        else if (sscanf(command, "rename %s %s", arg1, arg2) == 2)
        {
            // Mengubah nama direktori atau file
            // Coba mengubah nama direktori terlebih dahulu
            Directory *subdir = current->subdirs;
            while (subdir)
            {
                if (strcmp(subdir->name, arg1) == 0)
                {
                    rename_directory(subdir, arg2);
                    break;
                }
                subdir = subdir->next;
            }
            // Jika tidak ditemukan direktori, coba mengubah nama file
            if (!subdir)
            {
                File *file = current->files;
                while (file)
                {
                    if (strcmp(file->name, arg1) == 0)
                    {
                        rename_file(file, arg2);
                        break;
                    }
                    file = file->next;
                }
                if (!file)
                    printf("File atau Direktori tidak ditemukan\n");
            }
        }
        else if (strcmp(command, "list") == 0)
        {
            // Menampilkan isi direktori
            list_directory(current);
        }
        else if (strcmp(command, "exit") == 0)
        {
            // Membebaskan memori dan keluar dari program
            free_directory(root);
            return 0;
        }
        else
        {
            printf("Invalid command\n");
        }
    }

    return 0;
}

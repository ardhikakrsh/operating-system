#include "shell.h"
#include "kernel.h"
#include "std_lib.h"
#include "filesystem.h"

void shell()
{
  char buf[64];
  char cmd[64];
  char arg[2][64];

  byte cwd = FS_NODE_P_ROOT;

  while (true)
  {
    printString("MengOS:");
    printCWD(cwd);
    printString("$ ");
    readString(buf);
    parseCommand(buf, cmd, arg);

    if (strcmp(cmd, "cd"))
      cd(&cwd, arg[0]);
    else if (strcmp(cmd, "ls"))
      ls(cwd, arg[0]);
    else if (strcmp(cmd, "mv"))
      mv(cwd, arg[0], arg[1]);
    else if (strcmp(cmd, "cp"))
      cp(cwd, arg[0], arg[1]);
    else if (strcmp(cmd, "cat"))
      cat(cwd, arg[0]);
    else if (strcmp(cmd, "mkdir"))
      mkdir(cwd, arg[0]);
    else if (strcmp(cmd, "clear"))
      clearScreen();
    else
      printString("Invalid command\n");
  }
}

// TODO: 4. Implement printCWD function
void printCWD(byte cwd)
{
  struct node_fs node_fs_buf;
  byte path[64];
  int i, path_len = 0;

  if (cwd == FS_NODE_P_ROOT)
  {
    printString("/");
    return;
  }

  clear((byte *)&node_fs_buf, sizeof(struct node_fs));

  // Read node sector into memory
  readSector(&node_fs_buf, FS_NODE_SECTOR_NUMBER);
  readSector((byte *)&node_fs_buf + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1);

  // Traverse from cwd to root
  while (cwd != FS_NODE_P_ROOT)
  {
    path[path_len++] = cwd;
    cwd = node_fs_buf.nodes[cwd].parent_index;
  }

  // Print the path from root to cwd
  for (i = path_len - 1; i >= 0; i--)
  {
    // jika awal path, print "/"
    if (i == path_len - 1)
    {
      printString("/");
    }
    printString(node_fs_buf.nodes[path[i]].node_name);
    printString("/");
  }
}

// TODO: 5. Implement parseCommand function
void parseCommand(char *buf, char *cmd, char arg[2][64])
{
  int i, j, k;

  // Clear the command and arguments
  for (i = 0; i < 64; i++)
  {
    cmd[i] = '\0';
  }
  for (j = 0; j < 2; j++)
  {
    for (k = 0; k < 64; k++)
    {
      arg[j][k] = '\0';
    }
  }

  // Parse the command
  i = 0;
  j = 0;
  while (buf[i] != ' ' && buf[i] != '\0')
  {
    cmd[j++] = buf[i++];
  }
  cmd[j] = '\0';

  // Parse the arguments
  for (j = 0; j < 2; j++)
  {
    while (buf[i] == ' ')
      i++;
    k = 0;
    while (buf[i] != ' ' && buf[i] != '\0')
    {
      arg[j][k++] = buf[i++];
    }
    arg[j][k] = '\0';
    if (buf[i] == '\0')
      break;
  }
}

// TODO: 6. Implement cd function
void cd(byte *cwd, char *dirname)
{
  struct node_fs node_fs_buf;
  int i;

  // if cd / then move to root
  if (strcmp(dirname, "/") && *cwd != FS_NODE_P_ROOT)
  {
    *cwd = FS_NODE_P_ROOT;
    return;
  }

  // cd .. untuk kembali ke directory sebelumnya
  if (strcmp(dirname, ".."))
  {
    if (*cwd == FS_NODE_P_ROOT)
    {
      printString("Already in root directory\n");
      return;
    }

    clear((byte *)&node_fs_buf, sizeof(struct node_fs));
    // Read node sector into memory
    readSector(&node_fs_buf, FS_NODE_SECTOR_NUMBER);
    readSector((byte *)&node_fs_buf + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1);

    // pindah ke direktori sebelumnya
    *cwd = node_fs_buf.nodes[*cwd].parent_index;
    return;
  }

  clear((byte *)&node_fs_buf, sizeof(struct node_fs));
  readSector(&node_fs_buf, FS_NODE_SECTOR_NUMBER);
  readSector((byte *)&node_fs_buf + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1);

  // Search for the directory in the current directory's subdirectories
  if (!strcmp(dirname, "/") && !strcmp(dirname, ".."))
  {
    for (i = 0; i < FS_MAX_NODE; i++)
    {
      if (strcmp(node_fs_buf.nodes[i].node_name, dirname))
      {
        if (node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR)
        {
          *cwd = i;
          return;
        }
        else
        { // Jika bukan direktori
          printString("Not a directory\n");
          return;
        }
      }
      // Jika tidak ada direktori yang ditemukan
      else if (i == FS_MAX_NODE)
      {
        printString("Directory not found\n");
        return;
      }
    }
  }

  // If no directory is found
  printString("Directory not found\n");
}

// TODO: 7. Implement ls function
void ls(byte cwd, char *dirname)
{
  struct node_fs node_fs_buf;
  byte target_cwd = cwd;
  int i;
  int data_index;

  clear((byte *)&node_fs_buf, sizeof(struct node_fs));
  // jika ada nama direktori dan bukan "."
  readSector(&node_fs_buf, FS_NODE_SECTOR_NUMBER);
  readSector((byte *)&node_fs_buf + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1);
  if (strlen(dirname) > 0 && !strcmp(dirname, "."))
  {
    for (i = 0; i < FS_MAX_NODE; i++)
    {
      if (node_fs_buf.nodes[i].parent_index == cwd && strcmp(node_fs_buf.nodes[i].node_name, dirname))
      {
        if (node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR)
        {
          target_cwd = i;
          break;
        }
        else
        {
          printString("ls: Notmake a directory\n");
          return;
        }
      }
    }

    // Jika tidak ada direktori yang ditemukan
    if (i == FS_MAX_NODE)
    {
      printString("ls: No such directory\n");
      return;
    }
  }

  // print semua list direktori sesuai target_cwd
  for (i = 0; i < FS_MAX_NODE; i++)
  {
    if (node_fs_buf.nodes[i].parent_index == target_cwd && node_fs_buf.nodes[i].node_name[0] != '\0')
    {
      printString(node_fs_buf.nodes[i].node_name);
      printString(" ");
    }
  }
  printString("\n");
}

// TODO: 8. Implement mv function
void mv(byte cwd, char *src, char *dst)
{
  struct node_fs node_fs_buf;
  int i;
  int src_index = -1;
  int dst_index = -1;
  byte dst_parent_index = cwd;
  char dst_name[MAX_FILENAME];

  // Initialize dst_name to be empty using a loop
  for (i = 0; i < MAX_FILENAME; i++)
  {
    dst_name[i] = '\0';
  }

  clear((byte *)&node_fs_buf, sizeof(struct node_fs));
  readSector(&node_fs_buf, FS_NODE_SECTOR_NUMBER);
  readSector((byte *)&node_fs_buf + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1);

  // Find source file index
  for (i = 0; i < FS_MAX_NODE; i++)
  {
    if (strcmp(node_fs_buf.nodes[i].node_name, src) && node_fs_buf.nodes[i].parent_index == cwd)
    {
      src_index = i;
      break;
    }
  }

  if (src_index == -1)
  {
    printString("mv: Source file not found\n");
    return;
  }

  // Check destination path
  if (dst[0] == '/')
  {
    dst_parent_index = FS_NODE_P_ROOT;
    strcpy(dst_name, dst + 1); // Copy destination name without the leading '/'
  }
  else if (dst[0] == '.' && dst[1] == '.')
  {
    dst_parent_index = node_fs_buf.nodes[cwd].parent_index;
    strcpy(dst_name, dst + 3); // Copy destination name without the leading '../'
  }
  else
  {
    char *slash = dst;
    while (*slash != '/' && *slash != '\0')
    {
      slash++;
    }
    if (*slash == '/')
    {
      *slash = '\0';
      strcpy(dst_name, slash + 1);

      // Check if the destination directory exists
      for (i = 0; i < FS_MAX_NODE; i++)
      {
        if (strcmp(node_fs_buf.nodes[i].node_name, dst) && node_fs_buf.nodes[i].parent_index == cwd)
        {
          if (node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR)
          {
            dst_parent_index = i;
          }
          else
          {
            printString("mv: Destination path is not a directory\n");
            return;
          }
          break;
        }
      }
      if (i == FS_MAX_NODE)
      {
        printString("mv: Destination directory not found\n");
        return;
      }
    }
    else
    {
      strcpy(dst_name, dst);
    }
  }

  for (i = 0; i < FS_MAX_NODE; i++)
  {
    if (strcmp(node_fs_buf.nodes[i].node_name, dst_name) && node_fs_buf.nodes[i].parent_index == dst_parent_index)
    {
      dst_index = i;
      break;
    }
  }

  if (dst_index != -1)
  {
    printString("mv: Destination file already exists\n");
    return;
  }

  // Move the file
  strcpy(node_fs_buf.nodes[src_index].node_name, dst_name);
  node_fs_buf.nodes[src_index].parent_index = dst_parent_index;

  writeSector(&node_fs_buf, FS_NODE_SECTOR_NUMBER);
  writeSector((byte *)&node_fs_buf + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1);

  printString("File moved successfully\n");
}

// TODO: 9. Implement cp function
void cp(byte cwd, char *src, char *dst)
{
  struct node_fs node_fs_buf;
  int i;
  int src_index = -1;
  int new_node_index = -1;
  byte dst_parent_index = cwd;
  char dst_name[MAX_FILENAME];

  // Initialize dst_name to be empty
  for (i = 0; i < MAX_FILENAME; i++)
  {
    dst_name[i] = '\0';
  }

  clear((byte *)&node_fs_buf, sizeof(struct node_fs));
  readSector(&node_fs_buf, FS_NODE_SECTOR_NUMBER);
  readSector((byte *)&node_fs_buf + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1);

  // Find source file index
  for (i = 0; i < FS_MAX_NODE; i++)
  {
    if (strcmp(node_fs_buf.nodes[i].node_name, src) && node_fs_buf.nodes[i].parent_index == cwd)
    {
      src_index = i;
      break;
    }
  }

  if (src_index == -1)
  {
    printString("cp: Source file not found\n");
    return;
  }

  // Check if the source is a directory
  if (node_fs_buf.nodes[src_index].data_index == FS_NODE_D_DIR)
  {
    printString("cp: Cannot copy directories\n");
    return;
  }

  // Process destination path
  if (dst[0] == '/')
  {
    dst_parent_index = FS_NODE_P_ROOT;
    strcpy(dst_name, dst + 1); // Copy destination name without the leading '/'
  }
  else if (dst[0] == '.' && dst[1] == '.')
  {
    dst_parent_index = node_fs_buf.nodes[cwd].parent_index;
    strcpy(dst_name, dst + 3); // Copy destination name without the leading '../'
  }
  else
  {
    char *slash = dst;
    while (*slash != '/' && *slash != '\0')
    {
      slash++;
    }
    if (*slash == '/')
    {
      *slash = '\0';
      strcpy(dst_name, slash + 1);

      // Check if the destination directory exists
      for (i = 0; i < FS_MAX_NODE; i++)
      {
        if (strcmp(node_fs_buf.nodes[i].node_name, dst) && node_fs_buf.nodes[i].parent_index == cwd)
        {
          if (node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR)
          {
            dst_parent_index = i;
          }
          else
          {
            printString("cp: Destination path is not a directory\n");
            return;
          }
          break;
        }
      }
      if (i == FS_MAX_NODE)
      {
        printString("cp: Destination directory not found\n");
        return;
      }
    }
    else
    {
      strcpy(dst_name, dst);
    }
  }

  // Find an empty node for the new file
  for (i = 0; i < FS_MAX_NODE; i++)
  {
    if (node_fs_buf.nodes[i].node_name[0] == '\0' && node_fs_buf.nodes[i].parent_index == 0x00)
    {
      new_node_index = i;
      break;
    }
  }

  if (new_node_index == -1)
  {
    printString("cp: No empty node found for new file\n");
    return;
  }

  // Copy the file attributes to the new node
  strcpy(node_fs_buf.nodes[new_node_index].node_name, dst_name);
  node_fs_buf.nodes[new_node_index].parent_index = dst_parent_index;
  node_fs_buf.nodes[new_node_index].data_index = node_fs_buf.nodes[src_index].data_index; // Copy data index (file or directory)

  writeSector(&node_fs_buf, FS_NODE_SECTOR_NUMBER);
  writeSector((byte *)&node_fs_buf + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1);

  printString("File copied successfully\n");
}

// TODO: 10. Implement cat function
void cat(byte cwd, char *filename)
{
  struct file_metadata metadata;
  enum fs_return status;

  strcpy(metadata.node_name, filename);
  metadata.parent_index = cwd;
  fsRead(&metadata, &status);

  if (status == FS_SUCCESS)
  {
    printString(metadata.buffer);
    printString("\n");
  }
  else
  {
    printString("File not found.\n");
  }
}

// TODO: 11. Implement mkdir function
void mkdir(byte cwd, char *dirname)
{
  struct node_fs node_fs_buf;
  int i;

  clear((byte *)&node_fs_buf, sizeof(struct node_fs));
  readSector(&node_fs_buf, FS_NODE_SECTOR_NUMBER);
  readSector((byte *)&node_fs_buf + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1);

  // Check if directory already exists
  for (i = 0; i < FS_MAX_NODE; i++)
  {
    if (strcmp(node_fs_buf.nodes[i].node_name, dirname) && node_fs_buf.nodes[i].parent_index == cwd)
    {
      printString("Directory already exists.\n");
      return;
    }
  }

  // Find an empty node to create a new directory
  for (i = 0; i < FS_MAX_NODE; i++)
  {
    if (node_fs_buf.nodes[i].node_name[0] == '\0')
    {
      strcpy(node_fs_buf.nodes[i].node_name, dirname);
      node_fs_buf.nodes[i].parent_index = cwd;
      node_fs_buf.nodes[i].data_index = FS_NODE_D_DIR; // mark as directory
      writeSector(&node_fs_buf, FS_NODE_SECTOR_NUMBER);
      writeSector((byte *)&node_fs_buf + SECTOR_SIZE, FS_NODE_SECTOR_NUMBER + 1);
      printString("Directory created successfully.\n");
      return;
    }
  }

  printString("Failed to create directory. No free nodes available.\n");
}

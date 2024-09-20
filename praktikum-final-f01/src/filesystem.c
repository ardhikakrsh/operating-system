#include "kernel.h"
#include "std_lib.h"
#include "filesystem.h"

void fsInit()
{
  struct map_fs map_fs_buf;
  int i = 0;

  readSector(&map_fs_buf, FS_MAP_SECTOR_NUMBER);
  for (i = 0; i < 16; i++)
    map_fs_buf.is_used[i] = true;
  for (i = 256; i < 512; i++)
    map_fs_buf.is_used[i] = true;
  writeSector(&map_fs_buf, FS_MAP_SECTOR_NUMBER);
}

// TODO: 2. Implement fsRead function
void fsRead(struct file_metadata *metadata, enum fs_return *status)
{
  struct node_fs node_fs_buf;
  struct data_fs data_fs_buf;
  int i;
  int node_index = -1;
  bool file_found = false;

  readSector(&data_fs_buf, FS_DATA_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  // Find the file in the node sector
  for (i = 0; i < FS_MAX_NODE; i++)
  {
    if (strcmp(node_fs_buf.nodes[i].node_name, metadata->node_name) && node_fs_buf.nodes[i].parent_index == metadata->parent_index)
    {
      file_found = true;
      node_index = i;
      break;
    }
  }

  // Check if the file was not found
  if (!file_found)
  {
    *status = FS_R_NODE_NOT_FOUND;
    return;
  }

  // Check if the node is a directory
  if (node_fs_buf.nodes[node_index].data_index == FS_NODE_D_DIR)
  {
    *status = FS_R_TYPE_IS_DIRECTORY;
    return;
  }

  // Read the file data
  metadata->filesize = 0;
  for (i = 0; i < FS_MAX_SECTOR; i++)
  {
    if (data_fs_buf.datas[node_fs_buf.nodes[node_index].data_index].sectors[i] == 0x00)
    {
      break;
    }
    readSector(metadata->buffer + (i * SECTOR_SIZE), data_fs_buf.datas[node_fs_buf.nodes[node_index].data_index].sectors[i]);
    metadata->filesize += SECTOR_SIZE;
  }

  *status = FS_SUCCESS;
}

// TODO: 3. Implement fsWrite function
void fsWrite(struct file_metadata *metadata, enum fs_return *status)
{
  struct map_fs map_fs_buf;
  struct node_fs node_fs_buf;
  struct data_fs data_fs_buf;
  int i, j, node_index = -1, data_index = -1, free_sectors = 0;
  bool node_found = false;
  bool data_found = false;

  readSector(&data_fs_buf, FS_DATA_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER);

  // cari node nama yang sama
  for (i = 0; i < FS_MAX_NODE; i++)
  {
    if (strcmp(node_fs_buf.nodes[i].node_name, metadata->node_name) &&
        node_fs_buf.nodes[i].parent_index == metadata->parent_index)
    {
      *status = FS_W_NODE_ALREADY_EXISTS;
      return;
    }
  }

  // cari node yang kosong (nama node adalah string kosong)
  for (i = 0; i < FS_MAX_NODE; i++)
  {
    if (node_fs_buf.nodes[i].node_name == '\0')
    {
      node_index = i;
      node_found = true;
      break;
    }
  }

  if (!node_found)
  {
    *status = FS_W_NO_FREE_NODE;
    return;
  }

  // iterasi untuk mencari data yang kosong
  for (i = 0; i < FS_MAX_DATA; i++)
  {
    if (data_fs_buf.datas[i].sectors[0] == 0x00)
    {
      data_index = i;
      data_found = true;
      break;
    }
  }

  // data kosong tidak ditemukan
  if (!data_found)
  {
    *status = FS_W_NO_FREE_DATA;
    return;
  }

  // hitung free sectors
  for (i = 0; i < SECTOR_SIZE; i++)
  {
    if (map_fs_buf.is_used[i] == false)
    {
      free_sectors++;
    }
  }

  // cek apakah ada cukup ruang
  if (free_sectors < ((metadata->filesize + SECTOR_SIZE - 1) / SECTOR_SIZE))
  {
    *status = FS_W_NOT_ENOUGH_SPACE;
    return;
  }

  // set nama node
  strcpy(node_fs_buf.nodes[node_index].node_name, metadata->node_name);
  node_fs_buf.nodes[node_index].parent_index = metadata->parent_index;
  node_fs_buf.nodes[node_index].data_index = data_index;

  j = 0;
  for (i = 0; i < SECTOR_SIZE; i++)
  {
    if (map_fs_buf.is_used[i] == false)
    {
      map_fs_buf.is_used[i] = true;
      data_fs_buf.datas[data_index].sectors[j] = i;
      writeSector(metadata->buffer + (j * SECTOR_SIZE), i);
      j++;
      // jika j sudah mengisi semua map, keluar dari loop
      if (j * SECTOR_SIZE >= metadata->filesize + SECTOR_SIZE - 1)
      {
        break;
      }
    }
  }

  writeSector(&map_fs_buf, FS_MAP_SECTOR_NUMBER);
  writeSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  writeSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER);
  writeSector(&data_fs_buf, FS_DATA_SECTOR_NUMBER);

  *status = FS_SUCCESS;
}

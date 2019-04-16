#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct mgos_sd;

/*
 * Unit type for reporting the filesystem size by mgos_sd_get_fs_size
 */
enum mgos_sd_fs_unit {
  SD_FS_UNIT_BYTES = 1,
  SD_FS_UNIT_KILOBYTES = 2,
  SD_FS_UNIT_MEGABYTES = 3,
  //SD_FS_UNIT_GIGABYTES = 4
};

/*
 * Initialize the SD card using the spi device of ESP32
 * mounts it at `mount_point`, format it if mount failed if
 * `format_if_mount_failed` is true.
 * Returns an opaque pointer if success, NULL otherwise
 */
struct mgos_sd *mgos_sd_open(const char *mount_point,
                             bool format_if_mount_failed);

/*
 * Get the global instance.
 * Valid only after mgos_sd_open.
 */
struct mgos_sd *mgos_sd_get_global();

/*
 * Closes the sd and deletes the `struct mgos_sd*`
 */
void mgos_sd_close();

/*
 * Prints information about the connected SD card
 */
void mgos_sd_print_info(struct json_out *out);

/*
 * Returns the mount point of the SD card.
 */
const char *mgos_sd_get_mount_point();

/*
 * Returns the size of the SD card using the units defined by `enum
 * mgos_sd_fs_unit`
 */
uint64_t mgos_sd_get_fs_size(enum mgos_sd_fs_unit unit);

/*
 * Returns the used size of the SD card using the units defined by `enum
 * mgos_sd_fs_unit`
 */
uint64_t mgos_sd_get_fs_used(enum mgos_sd_fs_unit unit);

/*
 * Returns the free size of the SD card using the units defined by `enum
 * mgos_sd_fs_unit`
 */
uint64_t mgos_sd_get_fs_free(enum mgos_sd_fs_unit unit);

#ifdef __cplusplus
}
#endif
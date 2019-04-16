#include <mgos.h>

#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_types.h"
#include "driver/sdmmc_defs.h"

#include "mgos_sd.h"


struct mgos_sd {
  sdmmc_card_t *card;
  char *mount_point;
  uint64_t size;
};

static struct mgos_sd *s_card = NULL;


static void unmount_sd_cb(int ev, void *ev_data, void *arg) {
  (void) ev;
  (void) ev_data;
  (void) arg;
  mgos_sd_close();
}

static bool get_size_used(uint64_t *total_size, const char *folder);

static struct mgos_sd *mgos_sd_common_init(const char *mount_point,
                                           bool format_if_mount_failed,
                                           const sdmmc_host_t *host,
                                           const void *slot_config) {
  // Options for mounting the filesystem.

  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
      .format_if_mount_failed = format_if_mount_failed,
      .max_files = 5,  ///< Max number of open files
      .allocation_unit_size = CONFIG_WL_SECTOR_SIZE};

  sdmmc_card_t *card = NULL;
  esp_err_t ret = esp_vfs_fat_sdmmc_mount(mount_point, host, slot_config,
                                          &mount_config, &card);

  if (ret != ESP_OK) {
    if (ret == ESP_FAIL) {
      LOG(LL_ERROR, ("Failed to mount filesystem. "
                     "If you want the card to be formatted, set "
                     "format_if_mount_failed = true."));
    } else {
      const char *err = esp_err_to_name(ret);
      LOG(LL_ERROR, ("Failed to initialize the card (%d - %s). ",
                     ret, err));
    }
    return NULL;
  }

  // Card has been initialized
  s_card = (struct mgos_sd *) calloc(1, sizeof(struct mgos_sd));
  s_card->card = card;
  s_card->size = ((uint64_t) card->csd.capacity) * card->csd.sector_size;
  s_card->mount_point = strdup(mount_point);

  /*
   * Add reboot handler to unmount the SD.
   */
  mgos_event_add_handler(MGOS_EVENT_REBOOT, unmount_sd_cb, NULL);

  return s_card;
}

static struct mgos_sd *mgos_sd_open_spi(const char *mount_point,
                                        bool format_if_mount_failed) {
  LOG(LL_INFO, ("Using SPI peripheral"));

  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.max_freq_khz = 19000; // need to lower the freq (<20k) for smaller cards, apparently (they don't work without this)
  sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
  slot_config.gpio_miso = mgos_sys_config_get_sd_spi_pin_miso();
  slot_config.gpio_mosi = mgos_sys_config_get_sd_spi_pin_mosi();
  slot_config.gpio_sck = mgos_sys_config_get_sd_spi_pin_clk();
  slot_config.gpio_cs = mgos_sys_config_get_sd_spi_pin_cs();

  return mgos_sd_common_init(mount_point, format_if_mount_failed, &host,
                             &slot_config);
}

struct mgos_sd *mgos_sd_open(const char *mount_point,
                             bool format_if_mount_failed) {
  if (NULL != s_card) {
    LOG(LL_ERROR, ("SD already created. Returns the existing instance "
                   "mounted at %s",
                   s_card->mount_point));
    return s_card;
  }
  return mgos_sd_open_spi(mount_point, format_if_mount_failed);
}

void mgos_sd_close() {
  if (NULL != s_card) {
    // All done, unmount partition and disable SDMMC or SPI peripheral
    esp_vfs_fat_sdmmc_unmount();
    if (NULL != s_card->mount_point) {
      free(s_card->mount_point);
    }
    free(s_card);
    s_card = NULL;
  }
}

struct mgos_sd *mgos_sd_get_global() {
  return s_card;
}

void mgos_sd_print_info(struct json_out *out) {
  if ((NULL != s_card) && (NULL != out)) {
    const sdmmc_card_t *card = s_card->card;
    json_printf(
        out, "{Name: %Q, Type: %Q, Speed: %Q, Size: %llu, SizeUnit:%Q, ",
        card->cid.name, ((card->ocr & SD_OCR_SDHC_CAP) ? "SDHC/SDXC" : "SDSC"),
        ((card->csd.tr_speed > 25000000) ? "high speed" : "default speed"),
        (((uint64_t) card->csd.capacity) * card->csd.sector_size /
         (1024 * 1024)),
        "MB");
    json_printf(out,
                "CSD:{ver:%d, sector_size:%d, capacity:%d, read_bl_len:%d}, ",
                card->csd.csd_ver, card->csd.sector_size, card->csd.capacity,
                card->csd.read_block_len);
    json_printf(out, "SCR:{sd_spec:%d, bus_width:%d}}", card->scr.sd_spec,
                card->scr.bus_width);
  }
}

const char *mgos_sd_get_mount_point() {
  return (NULL != s_card) ? s_card->mount_point : NULL;
}

bool get_size_used(uint64_t *total_size, const char *folder) {
  char full_path[256];
  struct stat buffer;
  int exists;
  bool resp = true;

  DIR *dir = opendir(folder);

  if (dir == NULL) {
    return false;
  }

  struct dirent *dir_data = readdir(dir);
  while (NULL != dir_data) {
    if (dir_data->d_type == DT_DIR) {
      if (dir_data->d_name[0] != '.') {
        snprintf(full_path, sizeof(full_path), "%s/%s", folder,
                 dir_data->d_name);
        if (false == get_size_used(total_size, full_path)) {
          resp = false;
        }
      }
    } else {
      snprintf(full_path, sizeof(full_path), "%s/%s", folder, dir_data->d_name);
      exists = stat(full_path, &buffer);
      if (exists < 0) {
        LOG(LL_ERROR, ("stat failed %s (%u)", full_path, errno));
        resp = false;
        continue;
      } else {
        (*total_size) += buffer.st_size;
      }
    }
    dir_data = readdir(dir);
  }
  closedir(dir);

  return resp;
}

uint64_t mgos_sd_get_fs_used(enum mgos_sd_fs_unit unit) {
  if (NULL == s_card) {
    return 0;
  }
  uint64_t total_size = 0;
  get_size_used(&total_size, s_card->mount_point);
  return total_size;
}

uint64_t mgos_sd_get_fs_size(enum mgos_sd_fs_unit unit) {
  if (NULL == s_card) {
    return 0;
  }
  uint64_t size = s_card->size;
  switch (unit) {
    // case SD_FS_UNIT_GIGABYTES:
    //    size /= 1024;
    case SD_FS_UNIT_MEGABYTES:
      size /= 1024;
    case SD_FS_UNIT_KILOBYTES:
      size /= 1024;
    case SD_FS_UNIT_BYTES:
      break;
  }
  return size;
}

uint64_t mgos_sd_get_fs_free(enum mgos_sd_fs_unit unit) {
  if (NULL == s_card) {
    return 0;
  }
  return mgos_sd_get_fs_size(unit) - mgos_sd_get_fs_used(unit);
}

bool mgos_sdlib_init() {
  return true;
}
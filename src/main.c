/* SD card and FAT filesystem example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "mgos.h"
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"

#define USE_SPI_MODE

static void sdcardtest(){
  LOG(LL_INFO, ("Initializing SD card"));

  LOG(LL_INFO, ("Using SPI peripheral"));
  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.max_freq_khz = 19000;
  sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
  slot_config.gpio_miso = mgos_sys_config_get_sd_spi_pin_miso();
  slot_config.gpio_mosi = mgos_sys_config_get_sd_spi_pin_mosi();
  slot_config.gpio_sck = mgos_sys_config_get_sd_spi_pin_clk();
  slot_config.gpio_cs = mgos_sys_config_get_sd_spi_pin_cs();
  // This initializes the slot without card detect (CD) and write protect (WP) signals.
  // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.

  // Options for mounting the filesystem.
  // If format_if_mount_failed is set to true, SD card will be partitioned and
  // formatted in case when mounting fails.
  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
      .format_if_mount_failed = true,
      .max_files = 5,
      .allocation_unit_size = 16 * 1024};

  // Use settings defined above to initialize SD card and mount FAT filesystem.
  // Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience function.
  // Please check its source code and implement error recovery when developing
  // production applications.
  sdmmc_card_t *card;
  LOG(LL_INFO, ("%d", slot_config.gpio_mosi));
  esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

  

  if (ret != ESP_OK)
  {
    if (ret == ESP_FAIL)
    {
      LOG(LL_ERROR, ("Failed to mount filesystem."));
    }
    else
    {
      LOG(LL_ERROR, ("Failed to initialize the card (%s).",esp_err_to_name(ret)));
    }
    return;
  }

  // Card has been initialized, print its properties
  sdmmc_card_print_info(stdout, card);

  // Use POSIX and C standard library functions to work with files.
  // First create a file.
  LOG(LL_INFO, ("Opening file"));
  FILE *f = fopen("/sdcard/hello.txt", "w");
  if (f == NULL)
  {
    LOG(LL_ERROR, ("Failed to open file for writing"));
    return;
  }
  fprintf(f, "Hello %s!\n", card->cid.name);
  fclose(f);
  LOG(LL_INFO, ("File written"));

  // Check if destination file exists before renaming
  struct stat st;
  if (stat("/sdcard/foo.txt", &st) == 0)
  {
    // Delete it if it exists
    unlink("/sdcard/foo.txt");
  }

  // Rename original file
  LOG(LL_INFO, ("Renaming file"));
  if (rename("/sdcard/hello.txt", "/sdcard/foo.txt") != 0)
  {
    LOG(LL_ERROR, ("Rename failed"));
    return;
  }

  // Open renamed file for reading
  LOG(LL_INFO, ("Reading file"));
  f = fopen("/sdcard/foo.txt", "r");
  if (f == NULL)
  {
    LOG(LL_ERROR, ("Failed to open file for reading"));
    return;
  }
  char line[64];
  fgets(line, sizeof(line), f);
  fclose(f);
  // strip newline
  char *pos = strchr(line, '\n');
  if (pos)
  {
    *pos = '\0';
  }
  LOG(LL_INFO, ("Read from file: '%s'", line));

  // All done, unmount partition and disable SDMMC or SPI peripheral
  esp_vfs_fat_sdmmc_unmount();
  LOG(LL_INFO, ("Card unmounted"));
}


enum mgos_app_init_result mgos_app_init(void) {

  //mgos_set_timer(1000, MGOS_TIMER_REPEAT, timer_cb, NULL);

  //sdcardtest();
  return MGOS_APP_INIT_SUCCESS;
}
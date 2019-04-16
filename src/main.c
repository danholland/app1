#include "mgos.h"
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "mgos_sd.h"



static void sdcardtest(){
  LOG(LL_INFO, ("Initializing SD card"));

  mgos_sd_open("/sdcard", true);

  // Use POSIX and C standard library functions to work with files.
  // First create a file.
  LOG(LL_INFO, ("Opening file"));
  FILE *f = fopen("/sdcard/hello.txt", "w");
  if (f == NULL)
  {
    LOG(LL_ERROR, ("Failed to open file for writing"));
    return;
  }
  fprintf(f, "Hello %s!\n", "text");
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
  mgos_sd_close();
  LOG(LL_INFO, ("Card unmounted"));
}

enum mgos_app_init_result mgos_app_init(void) {

  //mgos_set_timer(1000, MGOS_TIMER_REPEAT, timer_cb, NULL);

  sdcardtest();

  return MGOS_APP_INIT_SUCCESS;
}
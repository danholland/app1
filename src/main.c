#include "mgos.h"
#include "mgos_event.h"
#include "mgos_sd.h"
#include "mgos_webserver.h"
#include "mgos_wifi_extras_rpc.h"

enum mgos_app_init_result mgos_app_init(void)
{
  webserver_start();
  mgos_event_register_base(MGOS_WIFI_SETUP_EV_BASE, "WiFi Setup");
  mgos_wifi_rpc_init();
  //mgos_set_timer(1000, MGOS_TIMER_REPEAT, timer_cb, NULL);

  return MGOS_APP_INIT_SUCCESS;
}
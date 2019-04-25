#include <stdlib.h>

#include "mgos.h"
#include "mgos_rpc.h"
#include "mgos_event.h"
#include "mgos_config.h"

#if CS_PLATFORM == CS_P_ESP32
#include "esp_wifi.h"
#endif

char *s_ssid = NULL;
char *s_pass = NULL;
bool s_wifi_rpc_init = false;

bool mgos_wifi_setup_test(char *ssid, char *pass, wifi_setup_test_cb_t cb, void *userdata)
{
}

static void mgos_wifi_test_rpc_handler(struct mg_rpc_request_info *ri, void *cb_arg,
                                       struct mg_rpc_frame_info *fi,
                                       struct mg_str args)
{
  LOG(LL_INFO, ("Wifi.Test RPC Handler Parsing JSON: %.*s\n", args.len, args.p));
  char *ssid;
  char *pass;
  char *user;

  json_scanf(args.p, args.len, ri->args_fmt, &ssid, &pass, &user);

  if (mgos_conf_str_empty(ssid))
  {
    mg_rpc_send_errorf(ri, 400, "SSID is required!");
    return;
  }
  if (!mgos_conf_str_empty(user))
  {
    LOG(LL_INFO, ("Wifi.Test RPC Handler ssid: %s pass: %s user: %s", ssid, pass, user));
    bool result = mgos_captive_portal_wifi_setup_test_ent(ssid, pass, user, NULL, NULL);
    mg_rpc_send_responsef(ri, "{ ssid: %Q, pass: %Q, user: %Q, result: %B }", ssid, pass, user, result);
  }
  else
  {
    LOG(LL_INFO, ("Wifi.Test RPC Handler ssid: %s pass: %s", ssid, pass));
    bool result = mgos_captive_portal_wifi_setup_test(ssid, pass, NULL, NULL);
    mg_rpc_send_responsef(ri, "{ ssid: %Q, pass: %Q, result: %B }", ssid, pass, result);
  }

  (void)cb_arg;
  (void)fi;
}

bool mgos_wifi_rpc_force_apsta(void)
{
#if CS_PLATFORM == CS_P_ESP32
  // We have to call this to set device in AP+STA mode, as when the Scan is called, it will force the device into
  // AP+STA mode, causing a disconnect to the client if it's not already in this mode.
  // By adding this we can set the device in that mode immediately, to prevent that disconnect of the client device
  // Only relevant on ESP32 devices
  esp_wifi_set_mode(WIFI_MODE_APSTA);
#endif
  return true;
}

bool mgos_wifi_rpc_start(void)
{
  LOG(LL_INFO, ("Wifi RPC handlers start"));
  if (!s_wifi_rpc_init)
  {

    if (mgos_sys_config_get_wifi_rpc_apsta())
    {
      // Set AP+STA mode if device is ESP32
      mgos_wifi_rpc_force_apsta();
    }

    struct mg_rpc *c = mgos_rpc_get_global();
    mg_rpc_add_handler(c, "Wifi.Test", "{ssid: %Q, pass: %Q, user: %Q}", mgos_wifi_test_rpc_handler, NULL);
    s_wifi_rpc_init = true;
    return true;
  }

  return false;
}

bool mgos_wifi_rpc_init(void)
{
  LOG(LL_INFO, ("Wifi RPC handlers init"));
  if (mgos_sys_config_get_wifi_rpc_enable())
  {
    mgos_wifi_rpc_start();
  }
  /*
    if( mgos_sys_config_get_cportal_rpc_disable() ){
        mgos_event_add_handler(MGOS_CAPTIVE_PORTAL_WIFI_SETUP_TEST_SUCCESS, test_success_cb, NULL);
    }
    */
  return true;
}
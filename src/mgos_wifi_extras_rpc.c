#include <stdlib.h>

#include "mgos.h"
#include "mgos_rpc.h"
#include "mgos_event.h"
#include "mgos_config.h"
#include "mgos_wifi.h"
#include "mgos_wifi_extras_rpc.h"

#if CS_PLATFORM == CS_P_ESP32
#include "esp_wifi.h"
#endif

char *s_ssid = NULL;
char *s_pass = NULL;
bool s_wifi_rpc_init = false;
static int s_connection_retries = 0;

static struct mgos_config_wifi_sta *sp_test_sta_vals = NULL;

static void remove_event_handlers(void);
static void add_event_handlers(void);

static void reconnect(int ev, void *ev_data, void *userdata)
{
  s_connection_retries++;
  if (s_connection_retries < 3)
  {
    LOG(LL_INFO, ("Wifi disconnected - Retrying connection... Attempt %d", s_connection_retries));
    mgos_wifi_connect();
  }
  s_connection_retries = 0;
  (void)ev;
  (void)ev_data;
  (void)userdata;
}

static void ip_acquired(int ev, void *ev_data, void *userdata)
{
  char *connectedto = mgos_wifi_get_connected_ssid();

  LOG(LL_INFO, ("WiFi Setup -- IP Acquired from SSID %s", connectedto));
  free(connectedto);
  remove_event_handlers();

  int sta_index = 0;

  if (sp_test_sta_vals != NULL)
  {
    if (sta_index > -1)
    {
      LOG(LL_INFO, ("Copying SSID %s and Password %s to STA 1 config (wifi.sta)", sp_test_sta_vals->ssid, sp_test_sta_vals->pass));
      if (sta_index == 0)
      {
        mgos_sys_config_set_wifi_sta_enable(true);
        mgos_sys_config_set_wifi_sta_ssid(sp_test_sta_vals->ssid);
        mgos_sys_config_set_wifi_sta_pass(sp_test_sta_vals->pass);

        if (!mgos_conf_str_empty(sp_test_sta_vals->user))
        {
          mgos_sys_config_set_wifi_sta_user(sp_test_sta_vals->user);
          mgos_sys_config_set_wifi_sta_anon_identity(sp_test_sta_vals->user);
          mgos_sys_config_set_wifi_sta_ca_cert("");
        }
        else
        {
          mgos_sys_config_set_wifi_sta_user("");
          mgos_sys_config_set_wifi_sta_anon_identity("");
          mgos_sys_config_set_wifi_sta_ca_cert("");
        }
      }
      else if (sta_index == 1)
      {
        mgos_sys_config_set_wifi_sta1_enable(true);
        mgos_sys_config_set_wifi_sta1_ssid(sp_test_sta_vals->ssid);
        mgos_sys_config_set_wifi_sta1_pass(sp_test_sta_vals->pass);

        if (!mgos_conf_str_empty(sp_test_sta_vals->user))
        {
          mgos_sys_config_set_wifi_sta1_user(sp_test_sta_vals->user);
          mgos_sys_config_set_wifi_sta1_anon_identity(sp_test_sta_vals->user);
          mgos_sys_config_set_wifi_sta1_ca_cert("");
        }
        else
        {
          mgos_sys_config_set_wifi_sta1_user("");
          mgos_sys_config_set_wifi_sta1_anon_identity("");
          mgos_sys_config_set_wifi_sta1_ca_cert("");
        }
      }
      else if (sta_index == 2)
      {
        mgos_sys_config_set_wifi_sta2_enable(true);
        mgos_sys_config_set_wifi_sta2_ssid(sp_test_sta_vals->ssid);
        mgos_sys_config_set_wifi_sta2_pass(sp_test_sta_vals->pass);

        if (!mgos_conf_str_empty(sp_test_sta_vals->user))
        {
          mgos_sys_config_set_wifi_sta2_user(sp_test_sta_vals->user);
          mgos_sys_config_set_wifi_sta2_anon_identity(sp_test_sta_vals->user);
          mgos_sys_config_set_wifi_sta2_ca_cert("");
        }
        else
        {
          mgos_sys_config_set_wifi_sta2_user("");
          mgos_sys_config_set_wifi_sta2_anon_identity("");
          mgos_sys_config_set_wifi_sta2_ca_cert("");
        }
      }
    }
  }
}

static void add_event_handlers(void)
{
  mgos_event_add_handler(MGOS_WIFI_EV_STA_DISCONNECTED, reconnect, NULL);
  mgos_event_add_handler(MGOS_WIFI_EV_STA_IP_ACQUIRED, ip_acquired, NULL);
}

static void remove_event_handlers(void)
{
  mgos_event_remove_handler(MGOS_WIFI_EV_STA_DISCONNECTED, reconnect, NULL);
  mgos_event_remove_handler(MGOS_WIFI_EV_STA_IP_ACQUIRED, ip_acquired, NULL);
}

bool mgos_wifi_setup_test(char *ssid, char *user, char *pass, void *userdata)
{
  if (mgos_conf_str_empty(ssid))
  {
    return false;
  }

  if (sp_test_sta_vals == NULL)
  {
    // Allocate memory to store sta values in
    sp_test_sta_vals = (struct mgos_config_wifi_sta *)calloc(1, sizeof(*sp_test_sta_vals));
  }
  sp_test_sta_vals->enable = 0;
  sp_test_sta_vals->ssid = ssid;
  sp_test_sta_vals->pass = pass;
  sp_test_sta_vals->enable = true;
  sp_test_sta_vals->user = user;
  sp_test_sta_vals->anon_identity = user;
  sp_test_sta_vals->ca_cert = "";
  LOG(LL_INFO, ("Testing Wifi setup, SSID: %s, USER: %s, PASS: %s", ssid, user, pass));

  mgos_wifi_disconnect();
  if (!mgos_wifi_setup_sta(sp_test_sta_vals))
  {
    return false;
  }
  add_event_handlers();
  mgos_wifi_connect();
  char *status = mgos_wifi_get_status_str();
  LOG(LL_INFO, ("STA Status: %s", status));
  free(status);
  return false;
}

static void mgos_wifi_test_rpc_handler(struct mg_rpc_request_info *ri, void *cb_arg,
                                       struct mg_rpc_frame_info *fi,
                                       struct mg_str args)
{
  LOG(LL_INFO, ("Wifi.Test RPC Handler Parsing JSON: %.*s\n", args.len, args.p));
  char *ssid = NULL;
  char *pass = NULL;
  char *user = NULL;

  json_scanf(args.p, args.len, ri->args_fmt, &ssid, &pass, &user);

  if (mgos_conf_str_empty(ssid))
  {
    free(ssid);
    free(pass);
    free(user);
    mg_rpc_send_errorf(ri, 400, "SSID is required!");
    return;
  }

  LOG(LL_INFO, ("Wifi.Test RPC Handler ssid: %s, user: %s, pass: %s", ssid, user, pass));
  bool result = mgos_wifi_setup_test(ssid, user, pass, NULL);
  mg_rpc_send_responsef(ri, "{ ssid: %Q, user: %Q, pass: %Q, result: %B }", ssid, user, pass, result);

  free(ssid);
  free(pass);
  free(user);

  (void)cb_arg;
  (void)fi;
}

static void mgos_wifi_save()
{
  return;
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
#include <stdbool.h>
#include <mgos.h>
#include "mgos_event.h"

typedef void (*wifi_setup_test_cb_t)(bool result, char *ssid, char *password, void *userdata);
#define MGOS_WIFI_SETUP_EV_BASE MGOS_EVENT_BASE('W', 'F', 'S')

enum mgos_wifi_setup_event
{
  /**
     * Fired when WiFi Setup/Config testing is Started
     * 
     * ev_data: struct mgos_config_wifi_sta *sta
     */
  MGOS_WIFI_SETUP_TEST_START = MGOS_WIFI_SETUP_EV_BASE,
  /**
     * Fired when succesful connection test for Wifi
     * 
     * ev_data: struct mgos_config_wifi_sta *sta
     */
  MGOS_WIFI_SETUP_TEST_SUCCESS,
  /**
     * Fired when failed connection test Wifi
     * 
     * ev_data: struct mgos_config_wifi_sta *sta
     */
  MGOS_WIFI_SETUP_TEST_FAILED
};

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

  bool mgos_wifi_rpc_init(void);
  /**
 * Start wifi RPC handlers
 */

  bool mgos_wifi_rpc_start(void);

  /**
 * @brief Start WiFi Credential/Connection test for Standard OPEN/WEP/WPA2 networks
 * 
 * @param ssid 
 * @param pass 
 * @param cb 
 * @param userdata 
 * @param user 
 * @return true 
 * @return false 
 */
  void mgos_wifi_setup_test(char *ssid, char *user, char *pass, void *userdata);

#ifdef __cplusplus
}
#endif /* __cplusplus */
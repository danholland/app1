#include <stdbool.h>
#include <mgos.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

  bool mgos_wifi_rpc_init(void);
  /**
 * Start wifi RPC handlers
 */

  bool mgos_wifi_rpc_start(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
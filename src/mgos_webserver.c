#include <stdlib.h>
#include <string.h>

#include "mgos.h"
#include "mgos_utils.h"
#include "mgos_config.h"
#include "mgos_mongoose.h"
#include "mongoose.h"
#include "mgos_webserver.h"
#include "common/str_util.h"

static struct mg_serve_http_opts s_http_server_opts;

static void http_msg_print(const struct http_message *msg)
{
  LOG(LL_INFO, ("      method: \"%.*s\"", msg->method.len, msg->method.p));
  LOG(LL_INFO, ("         uri: \"%.*s\"", msg->uri.len, msg->uri.p));
}

// static bool accept_gzip_encoding(struct http_message *msg)
// {

//   struct mg_str *hdrval = mg_get_http_header(msg, "Accept-Encoding");

//   if (hdrval != NULL && strstr(hdrval->p, "gzip") != NULL)
//   {
//     return true;
//   }

//   return false;
// }

static bool is_gz(const char *string)
{
  LOG(LL_INFO, ("GZIP Check on %s", string));
  if (strlen(string) > 3 && !strcmp(string + strlen(string) - 3, ".gz"))
  {
    return true;
  }

  return false;
}

static void serve_handler(struct mg_connection *nc, int ev, void *p, void *user_data)
{
  (void)user_data;
  if (ev != MG_EV_HTTP_REQUEST)
    return;
  struct http_message *msg = (struct http_message *)(p);
  http_msg_print(msg);

  struct mg_serve_http_opts opts;
  memcpy(&opts, &s_http_server_opts, sizeof(opts));

  char uri[msg->uri.len + 1];
  snprintf(uri, msg->uri.len + 1, "%s\n", msg->uri.p);

  if (is_gz(uri))
  {
    LOG(LL_INFO, ("Set GZIP Header"));
    opts.extra_headers = "Access-Control-Allow-Origin: *\r\nContent-Encoding: gzip";
  }
  // free(uri);
  mg_serve_http(nc, (struct http_message *)p, opts);
}

// static void serve_gzip(struct mg_connection *nc, int ev, void *p, void *user_data)
// {
//   (void)user_data;
//   if (ev != MG_EV_HTTP_REQUEST)
//     return;
//   struct http_message *msg = (struct http_message *)(p);
//   http_msg_print(msg);

//   // struct mg_serve_http_opts opts;
//   // memcpy(&opts, &s_http_server_opts, sizeof(opts));
//   s_http_server_opts.extra_headers = "\r\nContent-Encoding: gzip\r\n";
//   mg_serve_http(nc, (struct http_message *)p, s_http_server_opts);
// }

bool webserver_start(void)
{
  memset(&s_http_server_opts, 0, sizeof(s_http_server_opts));
  s_http_server_opts.document_root = "/";
  // Add GZIP mime types for HTML, JavaScript, and CSS files
  s_http_server_opts.custom_mime_types = ".html.gz=text/html; charset=utf-8,.js.gz=application/javascript; charset=utf-8,.css.gz=text/css; charset=utf-8";
  // CORS
  s_http_server_opts.extra_headers = "Access-Control-Allow-Origin: *";

  mgos_register_http_endpoint("/", serve_handler, NULL);
  return true;
}
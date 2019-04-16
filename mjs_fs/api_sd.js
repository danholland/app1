let SD = {
  _open: ffi('void mgos_sd_open(char*, bool)'),
  _close: ffi('void mgos_sd_close()')
};

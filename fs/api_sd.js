let SD = {
  _open: ffi('void mgos_sd_open(char*, bool)'),
  _close: ffi('void mgos_sd_close()'),
  _getMount: ffi('char *mgos_sd_get_mount_point()'),
  _readFile: ffi('void mgos_sd_read_file(char*)'),
  _writeFile: ffi('void mgos_sd_write_file(char*, char*)'),
  _writeLine: ffi('void mgos_sd_write_line(char*, char*)'),
  _rename: ffi('void mgos_sd_rename_file(char*,char*)'),
  _delete: ffi('void mgos_sd_delete_file(char *)'),

  _proto: {
    open: function(mountPoint, formatOnFail) {
      SD._open(mountPoint, formatOnFail);
    },
    close: function() {
      SD._close();
    },
    readFile: function(filePath) {
      SD._readFile(filePath);
    },
    writeFile: function(filePath, content) {
      SD._writeFile(filePath, content);
    },
    writeLine: function(filePath, line) {
      SD._writeLine(filePath, line);
    },
    renameFile: function(source, dest) {
      SD._rename(source, dest);
    },
    deleteFile: function(filePath) {
      SD._delete(filePath);
    }
  },
  create: function() {
    let obj = null;
    obj = Object.create(SD._proto);
    return obj;
  }
};

var Wifi = {
  SSIDs: [],
  Networks: [],
  _msg_proto: function(elID) {
    return {
      $el: document.getElementById(elID),
      show: function(msg) {
        if (msg) {
          this.$el.innerHTML = msg;
        }
        this.$el.style.display = 'block';
      },
      hide: function() {
        this.$el.style.display = 'none';
      }
    };
  },
  init: function() {
    console.log('Wifi.init');
    Wifi.Buttons.init();
    Wifi.Info = new Wifi._msg_proto('info');
    Wifi.Error = new Wifi._msg_proto('error');
    Wifi.Scanning = new Wifi._msg_proto('scanning');
    Wifi.Creds = new Wifi._msg_proto('credswrapper');

    document.getElementById('networks').onchange = function(a) {
      var s = this.value || this.options[this.selectedIndex].value;
      Wifi.selectNetwork(s);
    };

    // Stupid iPhones @see https://stackoverflow.com/questions/8004227/ios-select-onchange-not-firing
    document.getElementById('networks').onblur = function(a) {
      var s = this.value || this.options[this.selectedIndex].value;
      Wifi.selectNetwork(s);
    };

    Wifi.scan();
  },
  scan: function() {
    Wifi.Info.hide();
    Wifi.Error.hide();
    Wifi.Creds.hide();
    Wifi.Buttons.disableAll();
    Wifi.Scanning.show();
    Wifi.rpcCall(
      'POST',
      'Wifi.Scan',
      'Scanning for wireless networks...',
      false,
      function(resp) {
        if (resp && resp.length > 0) {
          Wifi.SSIDs = [];
          Wifi.Networks = [];
          console.log(resp);
          Wifi.Scanning.hide();
          var netSelect = document.getElementById('networks');
          netSelect.removeAttribute('disabled');
          netSelect.innerHTML =
            '<option value="-1" disabled="disabled" selected="selected">Please select a network...</option>';

          resp.forEach(function(net) {
            if (Wifi.SSIDs.indexOf(net.ssid) > -1) {
              return;
            }
            var opt = document.createElement('option');
            var authInt = parseInt(net.auth);
            var imgLock =
              'data:image/svg+xml;base64,PHN2ZyB4bWxucz0naHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmcnIHdpZHRoPScxMCcgaGVpZ2h0PScxMCcgdmlld0JveD0iMCAwIDQ0OCA1MTIiPjxwYXRoIGZpbGw9IiMwMDAiIGQ9Ik00MDAgMjI0aC0yNHYtNzJDMzc2IDY4LjIgMzA3LjggMCAyMjQgMFM3MiA2OC4yIDcyIDE1MnY3Mkg0OGMtMjYuNSAwLTQ4IDIxLjUtNDggNDh2MTkyYzAgMjYuNSAyMS41IDQ4IDQ4IDQ4aDM1MmMyNi41IDAgNDgtMjEuNSA0OC00OFYyNzJjMC0yNi41LTIxLjUtNDgtNDgtNDh6bS0xMDQgMEgxNTJ2LTcyYzAtMzkuNyAzMi4zLTcyIDcyLTcyczcyIDMyLjMgNzIgNzJ2NzJ6IiBjbGFzcz0iIj48L3BhdGg+PC9zdmc+';
            var imgUnlock =
              'data:image/svg+xml;base64,PHN2ZyB4bWxucz0naHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmcnIHdpZHRoPScxMCcgaGVpZ2h0PScxMCcgdmlld0JveD0iMCAwIDQ0OCA1MTIiPjxwYXRoIGZpbGw9IiMwMDAiIGQ9Ik00MDAgMjU2SDE1MlYxNTIuOWMwLTM5LjYgMzEuNy03Mi41IDcxLjMtNzIuOSA0MC0uNCA3Mi43IDMyLjEgNzIuNyA3MnYxNmMwIDEzLjMgMTAuNyAyNCAyNCAyNGgzMmMxMy4zIDAgMjQtMTAuNyAyNC0yNHYtMTZDMzc2IDY4IDMwNy41LS4zIDIyMy41IDAgMTM5LjUuMyA3MiA2OS41IDcyIDE1My41VjI1Nkg0OGMtMjYuNSAwLTQ4IDIxLjUtNDggNDh2MTYwYzAgMjYuNSAyMS41IDQ4IDQ4IDQ4aDM1MmMyNi41IDAgNDgtMjEuNSA0OC00OFYzMDRjMC0yNi41LTIxLjUtNDgtNDgtNDh6IiBjbGFzcz0iIj48L3BhdGg+PC9zdmc+';
            var authIcon = authInt > 0 ? imgLock : imgUnlock;
            var authType = 'OPEN';
            if (authInt === 1) {
              authType = 'WEP';
            } else if (authInt === 2) {
              authType = 'WPA/PSK';
            } else if (authInt === 3) {
              authType = 'WPA2/PSK';
            } else if (authInt === 4) {
              authType = 'WPA/WPA2/PSK';
            } else if (authInt === 5) {
              authType = 'WPA2/ENT';
            }
            opt.title = authIcon;
            opt.innerHTML =
              Wifi.rssiToStrength(net.rssi) +
              '% - ' +
              net.ssid +
              ' (' +
              authType +
              ')';
            opt.value = net.ssid;
            netSelect.appendChild(opt);
            Wifi.SSIDs.push(net.ssid);
            Wifi.Networks.push(net);
          });
        } else {
          Wifi.Scanning.hide();
          Wifi.Error.show('No networks found');
        }
        Wifi.Buttons._all.scan.enable();
      }
    );
  },
  Error: {},
  Info: {},
  Scanning: {},
  Creds: {},
  rssiToStrength: function(rssi) {
    if (rssi === 0 || rssi <= -100) {
      quality = 0;
    } else if (rssi >= -50) {
      quality = 100;
    } else {
      quality = 2 * (rssi + 100);
    }

    return quality;
  },
  rpcCall: function(method, rpc, msg, data, callback) {
    httpRequest = new XMLHttpRequest();

    httpRequest.onreadystatechange = function() {
      if (httpRequest.readyState !== XMLHttpRequest.DONE) {
        console.log(
          'rpcCall httpRequest readyState is NOT done!',
          httpRequest.readyState
        );
        return false;
      }

      if (httpRequest.status !== 200) {
        console.log('rpcCall httpRequest status is NOT 200!', httpRequest);

        if (httpRequest.responseText && httpRequest.responseText.length > 0) {
          Wifi.Error.show(
            'Error from device ( ' +
              httpRequest.responseText +
              ' ) -- Please try again'
          );
          callback(true);
        } else {
          callback(false);
        }
        return;
      }
      console.log('responseText', httpRequest.responseText);
      var httpResponse = JSON.parse(httpRequest.responseText);
      console.log('httpResponse', httpResponse);

      callback(httpResponse);
    };
    httpRequest.open(method, '/rpc/' + rpc);
    httpRequest.setRequestHeader('Content-Type', 'application/json');
    httpRequest.send(JSON.stringify(data));
  },
  selectNetwork: function(network) {
    console.log(network);
    var userwrapper = document.getElementById('wuserwrapper');
    var passwrapper = document.getElementById('wpasswrapper');
    userwrapper.style.display = 'none';
    passwrapper.style.display = 'none';
    Wifi.Buttons._all.test.disable();
    Wifi.Buttons._all.save.disable();
    var found = false;
    Wifi.Networks.forEach(function(net) {
      if (net.ssid !== network) {
        return;
      }
      var auth = parseInt(net.auth);
      if (auth === 0) {
        Wifi.Creds.hide();
      } else if (auth === 5) {
        Wifi.Creds.show();
        userwrapper.style.display = 'block';
        passwrapper.style.display = 'block';
      } else {
        Wifi.Creds.show();
        passwrapper.style.display = 'block';
      }
      found = true;
    });
    if (found) {
      Wifi.Buttons.enableAll();
    }
  },
  Buttons: {
    _proto: function(elID, clickCB) {
      var el = document.getElementById(elID);
      if (el) {
        el.addEventListener('click', clickCB);
      }
      return {
        $el: el,
        preVal: false,
        update: function(msg) {
          this.$el.innerHTML = msg;
        },
        disable: function(msg) {
          if (!this.preVal) {
            this.preVal = this.$el.innerHTML;
          }
          if (msg) {
            this.update(msg);
          }
          this.$el.disabled = true;
        },
        enable: function() {
          if (this.preVal) {
            this.$el.innerHTML = this.preVal;
            this.preVal = false;
          }
          this.$el.disabled = false;
        }
      };
    },
    _all: {},
    _ids: ['scan', 'save', 'test'],
    init: function() {
      for (var i = 0; i < Wifi.Buttons._ids.length; i++) {
        var elID = Wifi.Buttons._ids[i];
        Wifi.Buttons._all[elID] = new Wifi.Buttons._proto(elID, Wifi[elID]);
      }
    },
    enableAll: function() {
      for (var btn in Wifi.Buttons._all) {
        Wifi.Buttons._all[btn].enable();
      }
    },
    disableAll: function(msg) {
      for (var btn in Wifi.Buttons._all) {
        Wifi.Buttons._all[btn].disable(msg);
      }
    }
  },
  save: function() {
    var ssid = document.getElementById('networks').value;
    var user = document.getElementById('wuser').value;
    var pass = document.getElementById('wpass').value;
    if (!ssid || ssid.length < 1) {
      Wifi.Info.hide();
      Wifi.Error.show('You must select a network');
      return;
    }
    Wifi.Buttons.disableAll();
  }
};

document.addEventListener('DOMContentLoaded', Wifi.init);

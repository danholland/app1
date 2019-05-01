var locked = [
  'data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBmaWxsLW9wYWNpdHk9Ii4zIiBkPSJNMTIuMDEgMjEuNDlMMjMuNjQgN2MtLjQ1LS4zNC00LjkzLTQtMTEuNjQtNEM1LjI4IDMgLjgxIDYuNjYuMzYgN2wxMS42MyAxNC40OS4wMS4wMS4wMS0uMDF6Ii8+PHBhdGggZD0iTTAgMGgyNHYyNEgweiIgZmlsbD0ibm9uZSIvPjwvc3ZnPg==',
  'data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBmaWxsPSJub25lIiBkPSJNMCAwaDI0djI0SDBWMHoiLz48cGF0aCBkPSJNMjMgMTZ2LTEuNWMwLTEuNC0xLjEtMi41LTIuNS0yLjVTMTggMTMuMSAxOCAxNC41VjE2Yy0uNSAwLTEgLjUtMSAxdjRjMCAuNS41IDEgMSAxaDVjLjUgMCAxLS41IDEtMXYtNGMwLS41LS41LTEtMS0xem0tMSAwaC0zdi0xLjVjMC0uOC43LTEuNSAxLjUtMS41czEuNS43IDEuNSAxLjVWMTZ6Ii8+PHBhdGggZD0iTTE1LjUgMTQuNWMwLTIuOCAyLjItNSA1LTUgLjQgMCAuNyAwIDEgLjFMMjMuNiA3Yy0uNC0uMy00LjktNC0xMS42LTRDNS4zIDMgLjggNi43LjQgN0wxMiAyMS41bDMuNS00LjN2LTIuN3oiIG9wYWNpdHk9Ii4zIi8+PHBhdGggZD0iTTYuNyAxNC45bDUuMyA2LjYgMy41LTQuM3YtMi42YzAtLjIgMC0uNS4xLS43LS45LS41LTIuMi0uOS0zLjYtLjktMyAwLTUuMSAxLjctNS4zIDEuOXoiLz48L3N2Zz4=',
  'data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBmaWxsPSJub25lIiBkPSJNMCAwaDI0djI0SDBWMHoiLz48cGF0aCBkPSJNMjMgMTZ2LTEuNWMwLTEuNC0xLjEtMi41LTIuNS0yLjVTMTggMTMuMSAxOCAxNC41VjE2Yy0uNSAwLTEgLjUtMSAxdjRjMCAuNS41IDEgMSAxaDVjLjUgMCAxLS41IDEtMXYtNGMwLS41LS41LTEtMS0xem0tMSAwaC0zdi0xLjVjMC0uOC43LTEuNSAxLjUtMS41czEuNS43IDEuNSAxLjVWMTZ6Ii8+PHBhdGggZD0iTTE1LjUgMTQuNWMwLTIuOCAyLjItNSA1LTUgLjQgMCAuNyAwIDEgLjFMMjMuNiA3Yy0uNC0uMy00LjktNC0xMS42LTRDNS4zIDMgLjggNi43LjQgN0wxMiAyMS41bDMuNS00LjN2LTIuN3oiIG9wYWNpdHk9Ii4zIi8+PHBhdGggZD0iTTQuOCAxMi41bDcuMiA5IDMuNS00LjR2LTIuNmMwLTEuMy41LTIuNSAxLjQtMy40QzE1LjYgMTAuNSAxNCAxMCAxMiAxMGMtNC4xIDAtNi44IDIuMi03LjIgMi41eiIvPjwvc3ZnPg==',
  'data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBvcGFjaXR5PSIuMyIgZD0iTTEyIDNDNS4zIDMgLjggNi43LjQgN2wzLjIgMy45TDEyIDIxLjVsMy41LTQuM3YtMi42YzAtMi4yIDEuNC00IDMuMy00LjcuMy0uMS41LS4yLjgtLjIuMy0uMS42LS4xLjktLjEuNCAwIC43IDAgMSAuMUwyMy42IDdjLS40LS4zLTQuOS00LTExLjYtNHoiLz48cGF0aCBmaWxsPSJub25lIiBkPSJNMCAwaDI0djI0SDBWMHoiLz48cGF0aCBkPSJNMjMgMTZ2LTEuNWMwLTEuNC0xLjEtMi41LTIuNS0yLjVTMTggMTMuMSAxOCAxNC41VjE2Yy0uNSAwLTEgLjUtMSAxdjRjMCAuNS41IDEgMSAxaDVjLjUgMCAxLS41IDEtMXYtNGMwLS41LS41LTEtMS0xem0tMSAwaC0zdi0xLjVjMC0uOC43LTEuNSAxLjUtMS41czEuNS43IDEuNSAxLjVWMTZ6bS0xMCA1LjVsMy41LTQuM3YtMi42YzAtMi4yIDEuNC00IDMuMy00LjdDMTcuMyA5IDE0LjkgOCAxMiA4Yy00LjggMC04IDIuNi04LjUgMi45Ii8+PC9zdmc+',
  'data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBmaWxsPSJub25lIiBkPSJNMCAwaDI0djI0SDBWMHoiLz48cGF0aCBkPSJNMjMgMTZ2LTEuNWMwLTEuNC0xLjEtMi41LTIuNS0yLjVTMTggMTMuMSAxOCAxNC41VjE2Yy0uNSAwLTEgLjUtMSAxdjRjMCAuNS41IDEgMSAxaDVjLjUgMCAxLS41IDEtMXYtNGMwLS41LS41LTEtMS0xem0tMSAwaC0zdi0xLjVjMC0uOC43LTEuNSAxLjUtMS41czEuNS43IDEuNSAxLjVWMTZ6bS02LjUtMS41YzAtMi44IDIuMi01IDUtNSAuNCAwIC43IDAgMSAuMUwyMy42IDdjLS40LS4zLTQuOS00LTExLjYtNEM1LjMgMyAuOCA2LjcuNCA3TDEyIDIxLjVsMy41LTQuNHYtMi42eiIvPjwvc3ZnPg=='
];
var unlocked = [
  'data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBmaWxsLW9wYWNpdHk9Ii4zIiBkPSJNMTIuMDEgMjEuNDlMMjMuNjQgN2MtLjQ1LS4zNC00LjkzLTQtMTEuNjQtNEM1LjI4IDMgLjgxIDYuNjYuMzYgN2wxMS42MyAxNC40OS4wMS4wMS4wMS0uMDF6Ii8+PHBhdGggZD0iTTAgMGgyNHYyNEgweiIgZmlsbD0ibm9uZSIvPjwvc3ZnPg==',
  'data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBmaWxsLW9wYWNpdHk9Ii4zIiBkPSJNMTIuMDEgMjEuNDlMMjMuNjQgN2MtLjQ1LS4zNC00LjkzLTQtMTEuNjQtNEM1LjI4IDMgLjgxIDYuNjYuMzYgN2wxMS42MyAxNC40OS4wMS4wMS4wMS0uMDF6Ii8+PHBhdGggZD0iTTAgMGgyNHYyNEgweiIgZmlsbD0ibm9uZSIvPjxwYXRoIGQ9Ik02LjY3IDE0Ljg2TDEyIDIxLjQ5di4wMWwuMDEtLjAxIDUuMzMtNi42M0MxNy4wNiAxNC42NSAxNS4wMyAxMyAxMiAxM3MtNS4wNiAxLjY1LTUuMzMgMS44NnoiLz48L3N2Zz4=',
  'data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBmaWxsLW9wYWNpdHk9Ii4zIiBkPSJNMTIuMDEgMjEuNDlMMjMuNjQgN2MtLjQ1LS4zNC00LjkzLTQtMTEuNjQtNEM1LjI4IDMgLjgxIDYuNjYuMzYgN2wxMS42MyAxNC40OS4wMS4wMS4wMS0uMDF6Ii8+PHBhdGggZD0iTTAgMGgyNHYyNEgweiIgZmlsbD0ibm9uZSIvPjxwYXRoIGQ9Ik00Ljc5IDEyLjUybDcuMiA4Ljk4SDEybC4wMS0uMDEgNy4yLTguOThDMTguODUgMTIuMjQgMTYuMSAxMCAxMiAxMHMtNi44NSAyLjI0LTcuMjEgMi41MnoiLz48L3N2Zz4=',
  'data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBmaWxsLW9wYWNpdHk9Ii4zIiBkPSJNMTIuMDEgMjEuNDlMMjMuNjQgN2MtLjQ1LS4zNC00LjkzLTQtMTEuNjQtNEM1LjI4IDMgLjgxIDYuNjYuMzYgN2wxMS42MyAxNC40OS4wMS4wMS4wMS0uMDF6Ii8+PHBhdGggZD0iTTAgMGgyNHYyNEgweiIgZmlsbD0ibm9uZSIvPjxwYXRoIGQ9Ik0zLjUzIDEwLjk1bDguNDYgMTAuNTQuMDEuMDEuMDEtLjAxIDguNDYtMTAuNTRDMjAuMDQgMTAuNjIgMTYuODEgOCAxMiA4Yy00LjgxIDAtOC4wNCAyLjYyLTguNDcgMi45NXoiLz48L3N2Zz4=',
  'data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0Ij48cGF0aCBkPSJNMTIuMDEgMjEuNDlMMjMuNjQgN2MtLjQ1LS4zNC00LjkzLTQtMTEuNjQtNEM1LjI4IDMgLjgxIDYuNjYuMzYgN2wxMS42MyAxNC40OS4wMS4wMS4wMS0uMDF6Ii8+PHBhdGggZD0iTTAgMGgyNHYyNEgweiIgZmlsbD0ibm9uZSIvPjwvc3ZnPg=='
];

var Wifi = {
  SSIDs: [],
  currentSSID: null,
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
    Wifi.List = new Wifi._msg_proto('networkswrapper');
    Wifi.Progress = new Wifi._msg_proto('wifiprogress');
    // Modal close functionality
    document.onclick = function(e) {
      if (e.target == Wifi.Creds.$el) {
        Wifi.Creds.hide();
      }
    };

    Wifi.scan();
  },
  scan: function() {
    Wifi.Info.hide();
    Wifi.Error.hide();
    Wifi.Creds.hide();
    Wifi.List.hide();
    Wifi.Buttons.disableAll();
    Wifi.Scanning.show();
    Wifi.Progress.show();
    Wifi.rpcCall(
      'POST',
      'Wifi.Scan',
      'Scanning for wireless networks...',
      false,
      function(resp) {
        if (resp && resp.length > 0) {
          Wifi.SSIDs = [];
          Wifi.Networks = [];
          Wifi.Scanning.hide();
          Wifi.Progress.hide();
          var netList = document.getElementById('networklist');
          netList.innerHTML = '';
          resp.forEach(function(net) {
            if (Wifi.SSIDs.indexOf(net.ssid) > -1) {
              return;
            }
            var item = document.createElement('li');
            var strength = Wifi.rssiToStrength(net.rssi);
            var i = Math.round((strength / 100) * 4);
            var authInt = parseInt(net.auth);
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

            var authIcon = authInt > 0 ? locked[i] : unlocked[i];
            item.innerHTML =
              '<img src="' +
              authIcon +
              '" class="list-view-icon" />' +
              net.ssid;
            item.onclick = function() {
              Wifi.selectNetwork(net.ssid);
            };
            netList.appendChild(item);

            Wifi.SSIDs.push(net.ssid);
            Wifi.Networks.push(net);
          });
          Wifi.List.show();
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
  List: {},
  Progress: {},
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
      // var httpResponse = JSON.parse(httpRequest.responseText);
      // console.log('httpResponse', httpResponse);

      callback(JSON.parse(httpRequest.responseText));
    };
    httpRequest.open(method, '/rpc/' + rpc);
    httpRequest.setRequestHeader('Content-Type', 'application/json');
    httpRequest.send(JSON.stringify(data));
  },
  selectNetwork: function(network) {
    console.log(network);
    Wifi.currentSSID = network;
    var userwrapper = document.getElementById('wuserwrapper');
    var passwrapper = document.getElementById('wpasswrapper');
    document.getElementById('wuser').value = '';
    document.getElementById('wpass').value = '';
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
  },
  test: function() {
    var ssid = Wifi.currentSSID;
    var user = document.getElementById('wuser').value;
    var pass = document.getElementById('wpass').value;
    if (!ssid || ssid.length < 1) {
      Wifi.Info.hide();
      Wifi.Error.show('You must select a network');
      return;
    }
    if (!user || user.length < 1) {
      user = '';
    }
    Wifi.Buttons.disableAll();
    Wifi.rpcCall(
      'POST',
      'Wifi.Test',
      'Attempting to connect...',
      { ssid: ssid, pass: '$uperHer0e$', user: user },
      function(resp) {
        console.log(resp);
        return;
      }
    );
  }
};

document.addEventListener('DOMContentLoaded', Wifi.init);

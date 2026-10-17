import assert from 'node:assert/strict';
import { readFileSync } from 'node:fs';
import { test } from 'node:test';
import vm from 'node:vm';

const source = readFileSync('skins/Belchertown/js/belchertown.js.tmpl', 'utf8');

function functionSlice(startMarker, endMarker) {
  const start = source.indexOf(startMarker);
  const end = source.indexOf(endMarker, start);
  assert.ok(start >= 0 && end > start, 'expected function boundaries');
  return source.slice(start, end);
}

test('MQTT decodes one packet once and passes its parsed object onward', () => {
  let decodes = 0;
  let received;
  const context = {
    mqtt_message_count: 0,
    mqtt_payload: '',
    belchertown_debug() {},
    update_current_wx(value) { received = value; },
    jQuery: {
      parseJSON(payload) { decodes += 1; return JSON.parse(payload); }
    },
    console: { error() {} }
  };
  vm.runInNewContext(
    functionSlice('function onMessageArrived(message) {', 'function refreshHooks()'),
    context
  );
  context.onMessageArrived({
    payloadString: '{"outTemp":23.4}',
    destinationName: 'weewx/loop'
  });
  assert.equal(decodes, 1);
  assert.equal(received.outTemp, 23.4);
  assert.strictEqual(context.mqtt_payload, received);
  assert.equal(context.mqtt_message_count, 1);
});

test('invalid MQTT packet is caught without running the UI updater', () => {
  let received = 0;
  let logged = 0;
  const context = {
    mqtt_message_count: 0,
    mqtt_payload: '',
    belchertown_debug() {},
    update_current_wx() { received += 1; },
    jQuery: { parseJSON: JSON.parse },
    console: { error() { logged += 1; } }
  };
  vm.runInNewContext(
    functionSlice('function onMessageArrived(message) {', 'function refreshHooks()'),
    context
  );
  assert.doesNotThrow(() => context.onMessageArrived({
    payloadString: '{', destinationName: 'weewx/loop'
  }));
  assert.equal(received, 0);
  assert.equal(logged, 1);
});

test('chart initialization runs only when Highcharts is available', () => {
  const raw = functionSlice(
    'if (typeof Highcharts !== "undefined") {',
    'function showChart('
  );
  const init = raw
    .replaceAll('$highcharts_timezoneoffset', '0')
    .replaceAll('$highcharts_decimal', '.')
    .replaceAll('$highcharts_thousands', ',');
  const moment = {
    months: () => [], monthsShort: () => [],
    weekdays: () => [], weekdaysShort: () => []
  };
  assert.doesNotThrow(() => vm.runInNewContext(init, { moment }));
  let options;
  vm.runInNewContext(init, {
    moment,
    Highcharts: { setOptions(value) { options = value; } }
  });
  assert.equal(options.global.timezoneOffset, 0);
  assert.equal(options.lang.decimalPoint, '.');
});

test('browser assets use a stable release version and chart scripts are scoped', () => {
  const header = readFileSync('skins/Belchertown/header.html.tmpl', 'utf8');
  const config = readFileSync('skins/Belchertown/skin.conf', 'utf8');
  assert.ok(!header.includes('int( time.time() )'));
  assert.equal((header.match(/\?ver=\$Extras\.asset_version/g) || []).length, 5);
  assert.match(config, /^\s*asset_version\s*=/m);
  assert.match(header, /\$page == "graphs" or \(\$page == "home"/);
  assert.match(header, /\(\$page == "pi" or \$page == "home" or \$page == "kiosk"\) and/);
});

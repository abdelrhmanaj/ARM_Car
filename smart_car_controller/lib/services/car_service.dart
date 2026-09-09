import 'dart:convert';
import 'dart:io';

import '../models/car_state.dart';

/// Transport boundary for the ESP32. Replace these methods with an HTTP or WebSocket implementation later.
class CarService {
  CarService._internal();
  static final CarService _shared = CarService._internal();
  factory CarService() => _shared;

  String baseUrl = 'http://192.168.4.1';

  Future<void> sendDirection(DriveDirection direction) async {
    await _safePost('/api/command', {'type': direction == DriveDirection.stop ? 'stop' : 'move', 'direction': direction.name});
  }

  Future<void> sendSpeed(double speed) async {
    await _safePost('/api/command', {'type': 'speed', 'value': speed.round()});
  }

  Future<void> emergencyStop() async {
    await _safePost('/api/command', {'type': 'stop'});
  }

  Future<void> sendHeadlight(bool enabled) async {
    await _safePost('/api/command', {'type': 'light', 'state': enabled});
  }

  Future<void> sendHorn(bool enabled) async {
    await _safePost('/api/command', {'type': 'horn', 'state': enabled});
  }

  Future<bool> testConnection(String ipAddress) async {
    final oldUrl = baseUrl;
    baseUrl = 'http://$ipAddress';
    try {
      final response = await _get('/api/health');
      return response.statusCode == 200;
    } catch (_) {
      return false;
    } finally {
      baseUrl = oldUrl;
    }
  }

  Future<CarState?> readState() async {
    try {
      final response = await _get('/api/state');
      if (response.statusCode != 200) return null;
      final json = jsonDecode(await response.transform(utf8.decoder).join()) as Map<String, dynamic>;
      final dist = (json['distance'] as num?)?.toDouble() ?? 0;
      return CarState(
        distance: dist,
        leftDistance: (json['leftDistance'] as num?)?.toDouble() ?? dist,
        rightDistance: (json['rightDistance'] as num?)?.toDouble() ?? dist,
        mode: DriveMode.autoScan,
      );
    } catch (_) {
      return null;
    }
  }

  Future<HttpClientResponse> _post(String path, Map<String, Object> body) async {
    final client = HttpClient()..connectionTimeout = const Duration(seconds: 2);
    final request = await client.postUrl(Uri.parse('$baseUrl$path'));
    final payload = jsonEncode(body);
    request.headers.contentType = ContentType.json;
    request.contentLength = utf8.encode(payload).length;
    request.write(payload);
    final response = await request.close();
    await response.drain<void>();
    client.close();
    return response;
  }

  Future<void> _safePost(String path, Map<String, Object> body) async {
    try {
      final response = await _post(path, body);
      if (response.statusCode >= 400) return;
    } catch (_) {
      // Keep the UI responsive while the car is offline.
    }
  }

  Future<HttpClientResponse> _get(String path) async {
    final client = HttpClient()..connectionTimeout = const Duration(seconds: 2);
    final response = await client.getUrl(Uri.parse('$baseUrl$path')).then((request) => request.close());
    client.close();
    return response;
  }
}

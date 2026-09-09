enum DriveDirection { stop, forward, backward, left, right }
enum DriveMode { autoScan }

class CarState {
  const CarState({
    this.direction = DriveDirection.stop,
    this.speed = 100,
    this.distance = 42,
    this.leftDistance = 42,
    this.rightDistance = 42,
    this.mode = DriveMode.autoScan,
    this.connected = true,
    this.headlight = false,
    this.horn = false,
  });

  final DriveDirection direction;
  final double speed;
  final double distance;
  final double leftDistance;
  final double rightDistance;
  final DriveMode mode;
  final bool connected;
  final bool headlight;
  final bool horn;
}

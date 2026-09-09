import 'dart:async';
import 'package:flutter/material.dart';

import '../models/car_state.dart';
import '../services/car_service.dart';
import '../widgets/dashboard_card.dart';
import '../widgets/radar_view.dart';
import '../widgets/status_pill.dart';

class ControlScreen extends StatefulWidget {
  const ControlScreen({super.key});

  @override
  State<ControlScreen> createState() => _ControlScreenState();
}

class _ControlScreenState extends State<ControlScreen>
    with SingleTickerProviderStateMixin {
  final service = CarService();
  late final AnimationController radarSweep = AnimationController(
    vsync: this,
    duration: const Duration(seconds: 3),
  )..repeat();

  DriveDirection direction = DriveDirection.stop;
  double distance = 42;
  bool headlightOn = false;
  bool hornActive = false;
  bool isConnected = true;

  Timer? telemetryTimer;
  Timer? _holdTimer;

  @override
  void initState() {
    super.initState();
    telemetryTimer = Timer.periodic(const Duration(milliseconds: 350), (_) async {
      final state = await service.readState();
      if (!mounted) return;
      if (state != null) {
        setState(() {
          distance = state.distance;
          isConnected = true;
        });
      } else {
        if (isConnected) {
          setState(() => isConnected = false);
        }
      }
    });
  }

  @override
  void dispose() {
    radarSweep.dispose();
    telemetryTimer?.cancel();
    _holdTimer?.cancel();
    super.dispose();
  }

  void startDriving(DriveDirection value) {
    _holdTimer?.cancel();
    setState(() => direction = value);
    service.sendDirection(value);
    _holdTimer = Timer.periodic(const Duration(milliseconds: 150), (_) {
      service.sendDirection(value);
    });
  }

  void stopDriving() {
    _holdTimer?.cancel();
    _holdTimer = null;
    setState(() => direction = DriveDirection.stop);
    service.sendDirection(DriveDirection.stop);
  }

  void toggleHeadlight() {
    final next = !headlightOn;
    setState(() => headlightOn = next);
    service.sendHeadlight(next);
  }

  void startHorn() {
    if (!hornActive) {
      setState(() => hornActive = true);
      service.sendHorn(true);
    }
  }

  void stopHorn() {
    if (hornActive) {
      setState(() => hornActive = false);
      service.sendHorn(false);
    }
  }

  Color get proximityColor {
    if (distance <= 0) return const Color(0xFF637994);
    if (distance < 18) return const Color(0xFFFF2A55);
    if (distance < 35) return const Color(0xFFFFB300);
    return const Color(0xFF00E5FF);
  }

  String get proximityStatus {
    if (distance <= 0) return 'NO TARGET';
    if (distance < 18) return 'DANGER: OBSTACLE IMMINENT';
    if (distance < 35) return 'CAUTION: OBJECT DETECTED';
    return 'PATH CLEAR';
  }

  @override
  Widget build(BuildContext context) {
    final alertColor = proximityColor;
    final alertText = proximityStatus;

    return AnimatedBuilder(
      animation: radarSweep,
      builder: (context, _) {
        final scanAngle = 20 + (radarSweep.value * 140);
        return Scaffold(
          backgroundColor: const Color(0xFF070C15),
          body: SafeArea(
            child: ListView(
              padding: const EdgeInsets.symmetric(horizontal: 18, vertical: 12),
              children: [
                // Top Cockpit Header
                Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: [
                    Row(
                      children: [
                        Container(
                          width: 38,
                          height: 38,
                          decoration: BoxDecoration(
                            gradient: const LinearGradient(
                              colors: [Color(0xFF00E5FF), Color(0xFF0072FF)],
                              begin: Alignment.topLeft,
                              end: Alignment.bottomRight,
                            ),
                            borderRadius: BorderRadius.circular(10),
                            boxShadow: [
                              BoxShadow(
                                color: const Color(0xFF00E5FF).withValues(alpha: 0.35),
                                blurRadius: 10,
                              ),
                            ],
                          ),
                          child: const Icon(Icons.directions_car_filled_rounded, color: Colors.black, size: 22),
                        ),
                        const SizedBox(width: 12),
                        Column(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: [
                            Text(
                              'SMART COCKPIT',
                              style: TextStyle(
                                color: Theme.of(context).colorScheme.primary,
                                fontSize: 9,
                                fontWeight: FontWeight.w900,
                                letterSpacing: 2,
                              ),
                            ),
                            const Text(
                              'BlackPill 4WD',
                              style: TextStyle(
                                color: Colors.white,
                                fontSize: 18,
                                fontWeight: FontWeight.w800,
                              ),
                            ),
                          ],
                        ),
                      ],
                    ),
                    StatusPill(connected: isConnected),
                  ],
                ),

                const SizedBox(height: 16),

                // Central Tactical Radar & Distance HUD
                DashboardCard(
                  padding: const EdgeInsets.all(16),
                  child: Column(
                    children: [
                      // Sub-header with mode and proximity status
                      Row(
                        mainAxisAlignment: MainAxisAlignment.spaceBetween,
                        children: [
                          Row(
                            children: [
                              Container(
                                width: 8,
                                height: 8,
                                decoration: BoxDecoration(
                                  color: alertColor,
                                  shape: BoxShape.circle,
                                  boxShadow: [
                                    BoxShadow(
                                      color: alertColor.withValues(alpha: 0.6),
                                      blurRadius: 6,
                                      spreadRadius: 1,
                                    ),
                                  ],
                                ),
                              ),
                              const SizedBox(width: 8),
                              Text(
                                alertText,
                                style: TextStyle(
                                  color: alertColor,
                                  fontSize: 10,
                                  fontWeight: FontWeight.w800,
                                  letterSpacing: 1.2,
                                ),
                              ),
                            ],
                          ),
                          Container(
                            padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 3),
                            decoration: BoxDecoration(
                              color: const Color(0xFF132034),
                              borderRadius: BorderRadius.circular(6),
                              border: Border.all(color: const Color(0xFF22354F)),
                            ),
                            child: const Text(
                              'PA0/PA1 ULTRASONIC',
                              style: TextStyle(
                                color: Color(0xFF7E98B9),
                                fontSize: 9,
                                fontWeight: FontWeight.bold,
                                letterSpacing: 0.8,
                              ),
                            ),
                          ),
                        ],
                      ),

                      const SizedBox(height: 12),

                      // Radar View
                      RadarView(angle: scanAngle, distance: distance),

                      const SizedBox(height: 12),

                      // Large Telemetry Display
                      Container(
                        padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 10),
                        decoration: BoxDecoration(
                          color: const Color(0xFF0C1625),
                          borderRadius: BorderRadius.circular(14),
                          border: Border.all(color: alertColor.withValues(alpha: 0.25)),
                        ),
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.spaceBetween,
                          children: [
                            Column(
                              crossAxisAlignment: CrossAxisAlignment.start,
                              children: [
                                const Text(
                                  'FRONT DISTANCE',
                                  style: TextStyle(
                                    color: Color(0xFF758CA9),
                                    fontSize: 9,
                                    fontWeight: FontWeight.bold,
                                    letterSpacing: 1.2,
                                  ),
                                ),
                                const SizedBox(height: 2),
                                RichText(
                                  text: TextSpan(
                                    children: [
                                      TextSpan(
                                        text: distance.round().toString(),
                                        style: TextStyle(
                                          color: alertColor,
                                          fontSize: 32,
                                          fontWeight: FontWeight.w900,
                                          letterSpacing: -1,
                                        ),
                                      ),
                                      const TextSpan(
                                        text: ' CM',
                                        style: TextStyle(
                                          color: Color(0xFF8DA3BF),
                                          fontSize: 14,
                                          fontWeight: FontWeight.w700,
                                        ),
                                      ),
                                    ],
                                  ),
                                ),
                              ],
                            ),

                            // Gear / Reverse indicator
                            Container(
                              padding: const EdgeInsets.symmetric(horizontal: 14, vertical: 8),
                              decoration: BoxDecoration(
                                color: direction == DriveDirection.backward
                                    ? const Color(0xFFFF2A55).withValues(alpha: 0.18)
                                    : const Color(0xFF132034),
                                borderRadius: BorderRadius.circular(10),
                                border: Border.all(
                                  color: direction == DriveDirection.backward
                                      ? const Color(0xFFFF2A55)
                                      : const Color(0xFF243954),
                                  width: direction == DriveDirection.backward ? 1.5 : 1,
                                ),
                              ),
                              child: Column(
                                children: [
                                  Text(
                                    direction == DriveDirection.backward
                                        ? '🔴 REVERSE'
                                        : direction == DriveDirection.forward
                                            ? '▲ DRIVE'
                                            : direction == DriveDirection.left
                                                ? '↺ SPIN L'
                                                : direction == DriveDirection.right
                                                    ? '↻ SPIN R'
                                                    : '■ PARK',
                                    style: TextStyle(
                                      color: direction == DriveDirection.backward
                                          ? const Color(0xFFFF4D71)
                                          : direction == DriveDirection.stop
                                              ? const Color(0xFF758CA9)
                                              : const Color(0xFF00E5FF),
                                      fontSize: 11,
                                      fontWeight: FontWeight.w900,
                                      letterSpacing: 1,
                                    ),
                                  ),
                                  if (direction == DriveDirection.backward)
                                    const Text(
                                      'RED LAMP ON',
                                      style: TextStyle(
                                        color: Color(0xFFFF7E98),
                                        fontSize: 8,
                                        fontWeight: FontWeight.bold,
                                      ),
                                    ),
                                ],
                              ),
                            ),
                          ],
                        ),
                      ),
                    ],
                  ),
                ),

                const SizedBox(height: 14),

                // Quick Cockpit Controls: Headlight & Horn
                Row(
                  children: [
                    // Headlight Button
                    Expanded(
                      child: GestureDetector(
                        onTap: toggleHeadlight,
                        child: AnimatedContainer(
                          duration: const Duration(milliseconds: 200),
                          padding: const EdgeInsets.symmetric(vertical: 14, horizontal: 12),
                          decoration: BoxDecoration(
                            gradient: headlightOn
                                ? const LinearGradient(
                                    colors: [Color(0xFF005E7A), Color(0xFF0095B6)],
                                    begin: Alignment.topLeft,
                                    end: Alignment.bottomRight,
                                  )
                                : const LinearGradient(
                                    colors: [Color(0xFF101B2A), Color(0xFF142236)],
                                  ),
                            borderRadius: BorderRadius.circular(16),
                            border: Border.all(
                              color: headlightOn ? const Color(0xFF00E5FF) : const Color(0xFF22374E),
                              width: headlightOn ? 1.8 : 1,
                            ),
                            boxShadow: headlightOn
                                ? [
                                    BoxShadow(
                                      color: const Color(0xFF00E5FF).withValues(alpha: 0.35),
                                      blurRadius: 14,
                                      spreadRadius: 1,
                                    ),
                                  ]
                                : [],
                          ),
                          child: Row(
                            mainAxisAlignment: MainAxisAlignment.center,
                            children: [
                              Icon(
                                headlightOn ? Icons.lightbulb_rounded : Icons.lightbulb_outline_rounded,
                                color: headlightOn ? const Color(0xFFE0FFFF) : const Color(0xFF728AA9),
                                size: 26,
                              ),
                              const SizedBox(width: 10),
                              Column(
                                crossAxisAlignment: CrossAxisAlignment.start,
                                children: [
                                  Text(
                                    headlightOn ? 'LIGHT: ON' : 'LIGHT: OFF',
                                    style: TextStyle(
                                      color: headlightOn ? Colors.white : const Color(0xFFA5B8D0),
                                      fontSize: 12,
                                      fontWeight: FontWeight.w900,
                                      letterSpacing: 0.8,
                                    ),
                                  ),
                                  const Text(
                                    'Headlight (PA4)',
                                    style: TextStyle(
                                      color: Color(0xFF637C9B),
                                      fontSize: 9,
                                      fontWeight: FontWeight.w600,
                                    ),
                                  ),
                                ],
                              ),
                            ],
                          ),
                        ),
                      ),
                    ),

                    const SizedBox(width: 10),

                    // Horn / Buzzer Button (Hold to honk)
                    Expanded(
                      child: Listener(
                        onPointerDown: (_) => startHorn(),
                        onPointerUp: (_) => stopHorn(),
                        onPointerCancel: (_) => stopHorn(),
                        child: AnimatedContainer(
                          duration: const Duration(milliseconds: 150),
                          padding: const EdgeInsets.symmetric(vertical: 14, horizontal: 12),
                          decoration: BoxDecoration(
                            gradient: hornActive
                                ? const LinearGradient(
                                    colors: [Color(0xFF8A4600), Color(0xFFFF8F00)],
                                    begin: Alignment.topLeft,
                                    end: Alignment.bottomRight,
                                  )
                                : const LinearGradient(
                                    colors: [Color(0xFF101B2A), Color(0xFF142236)],
                                  ),
                            borderRadius: BorderRadius.circular(16),
                            border: Border.all(
                              color: hornActive ? const Color(0xFFFFB300) : const Color(0xFF22374E),
                              width: hornActive ? 1.8 : 1,
                            ),
                            boxShadow: hornActive
                                ? [
                                    BoxShadow(
                                      color: const Color(0xFFFFB300).withValues(alpha: 0.4),
                                      blurRadius: 14,
                                      spreadRadius: 1,
                                    ),
                                  ]
                                : [],
                          ),
                          child: Row(
                            mainAxisAlignment: MainAxisAlignment.center,
                            children: [
                              Icon(
                                hornActive ? Icons.volume_up_rounded : Icons.volume_down_rounded,
                                color: hornActive ? Colors.white : const Color(0xFF728AA9),
                                size: 26,
                              ),
                              const SizedBox(width: 10),
                              Column(
                                crossAxisAlignment: CrossAxisAlignment.start,
                                children: [
                                  Text(
                                    hornActive ? 'HONKING!' : 'HORN / بازر',
                                    style: TextStyle(
                                      color: hornActive ? Colors.white : const Color(0xFFA5B8D0),
                                      fontSize: 12,
                                      fontWeight: FontWeight.w900,
                                      letterSpacing: 0.8,
                                    ),
                                  ),
                                  const Text(
                                    'Buzzer (PB5)',
                                    style: TextStyle(
                                      color: Color(0xFF637C9B),
                                      fontSize: 9,
                                      fontWeight: FontWeight.w600,
                                    ),
                                  ),
                                ],
                              ),
                            ],
                          ),
                        ),
                      ),
                    ),
                  ],
                ),

                const SizedBox(height: 16),

                // Precision Driving D-Pad
                DashboardCard(
                  padding: const EdgeInsets.symmetric(vertical: 16, horizontal: 12),
                  child: Column(
                    children: [
                      // Forward Button
                      _TactileDriveButton(
                        icon: Icons.keyboard_arrow_up_rounded,
                        label: 'FORWARD',
                        active: direction == DriveDirection.forward,
                        activeColor: const Color(0xFF00E5FF),
                        onPress: () => startDriving(DriveDirection.forward),
                        onRelease: stopDriving,
                      ),

                      const SizedBox(height: 8),

                      // Middle Row: Left - Brake - Right
                      Row(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: [
                          _TactileDriveButton(
                            icon: Icons.rotate_left_rounded,
                            label: 'SPIN LEFT',
                            active: direction == DriveDirection.left,
                            activeColor: const Color(0xFFFFB300),
                            onPress: () => startDriving(DriveDirection.left),
                            onRelease: stopDriving,
                          ),
                          const SizedBox(width: 12),
                          _TactileDriveButton(
                            icon: Icons.stop_circle_rounded,
                            label: 'E-BRAKE',
                            danger: true,
                            onTap: () {
                              stopDriving();
                              service.emergencyStop();
                            },
                          ),
                          const SizedBox(width: 12),
                          _TactileDriveButton(
                            icon: Icons.rotate_right_rounded,
                            label: 'SPIN RIGHT',
                            active: direction == DriveDirection.right,
                            activeColor: const Color(0xFFFFB300),
                            onPress: () => startDriving(DriveDirection.right),
                            onRelease: stopDriving,
                          ),
                        ],
                      ),

                      const SizedBox(height: 8),

                      // Backward Button (with red reverse glow)
                      _TactileDriveButton(
                        icon: Icons.keyboard_arrow_down_rounded,
                        label: 'REVERSE',
                        reverseTheme: true,
                        active: direction == DriveDirection.backward,
                        activeColor: const Color(0xFFFF2A55),
                        onPress: () => startDriving(DriveDirection.backward),
                        onRelease: stopDriving,
                      ),
                    ],
                  ),
                ),

                const SizedBox(height: 12),

                // Footer tip
                Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Icon(Icons.touch_app_rounded, size: 14, color: Colors.blueGrey.shade400),
                    const SizedBox(width: 6),
                    const Text(
                      'Hold button to drive • Release to brake immediately',
                      style: TextStyle(color: Color(0xFF627793), fontSize: 11, fontWeight: FontWeight.w600),
                    ),
                  ],
                ),
                const SizedBox(height: 10),
              ],
            ),
          ),
        );
      },
    );
  }
}

class _TactileDriveButton extends StatelessWidget {
  const _TactileDriveButton({
    required this.icon,
    required this.label,
    this.active = false,
    this.activeColor = const Color(0xFF00E5FF),
    this.danger = false,
    this.reverseTheme = false,
    this.onPress,
    this.onRelease,
    this.onTap,
  });

  final IconData icon;
  final String label;
  final bool active;
  final Color activeColor;
  final bool danger;
  final bool reverseTheme;
  final VoidCallback? onPress;
  final VoidCallback? onRelease;
  final VoidCallback? onTap;

  @override
  Widget build(BuildContext context) {
    Color bg = const Color(0xFF111E2E);
    Color border = const Color(0xFF1E3149);
    Color textCol = const Color(0xFF8AA1BE);
    Color iconCol = const Color(0xFF00E5FF);

    if (danger) {
      bg = const Color(0xFF2A1016);
      border = const Color(0xFFFF2A55).withValues(alpha: 0.5);
      iconCol = const Color(0xFFFF4D71);
      textCol = const Color(0xFFFFA2B4);
    } else if (reverseTheme && !active) {
      bg = const Color(0xFF181827);
      border = const Color(0xFF382334);
      iconCol = const Color(0xFFFF6685);
      textCol = const Color(0xFFB58EA0);
    }

    if (active) {
      bg = activeColor.withValues(alpha: 0.25);
      border = activeColor;
      iconCol = Colors.white;
      textCol = Colors.white;
    }

    return Listener(
      onPointerDown: (_) {
        if (onPress != null) {
          onPress!();
        } else if (onTap != null) {
          onTap!();
        }
      },
      onPointerUp: (_) => onRelease?.call(),
      onPointerCancel: (_) => onRelease?.call(),
      child: AnimatedContainer(
        duration: const Duration(milliseconds: 100),
        width: 86,
        height: 72,
        decoration: BoxDecoration(
          color: bg,
          borderRadius: BorderRadius.circular(18),
          border: Border.all(color: border, width: active ? 2 : 1.2),
          boxShadow: active
              ? [
                  BoxShadow(
                    color: activeColor.withValues(alpha: 0.45),
                    blurRadius: 14,
                    spreadRadius: 1,
                  ),
                ]
              : [
                  BoxShadow(
                    color: Colors.black.withValues(alpha: 0.25),
                    blurRadius: 4,
                    offset: const Offset(0, 2),
                  ),
                ],
        ),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Icon(icon, size: 34, color: iconCol),
            const SizedBox(height: 2),
            Text(
              label,
              style: TextStyle(
                color: textCol,
                fontSize: 9,
                fontWeight: FontWeight.w900,
                letterSpacing: 0.8,
              ),
            ),
          ],
        ),
      ),
    );
  }
}

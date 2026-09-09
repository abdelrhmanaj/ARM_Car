import 'dart:math' as math;
import 'package:flutter/material.dart';

class RadarView extends StatelessWidget {
  const RadarView({super.key, required this.angle, required this.distance});
  final double angle; final double distance;
  @override Widget build(BuildContext context) => AspectRatio(aspectRatio: 1.28, child: CustomPaint(painter: _RadarPainter(angle, distance)));
}
class _RadarPainter extends CustomPainter {
  _RadarPainter(this.angle, this.distance); final double angle; final double distance;
  @override void paint(Canvas canvas, Size size) {
    final c = Offset(size.width / 2, size.height * .57); final radius = math.min(size.width * .42, size.height * .8); final grid = Paint()..style = PaintingStyle.stroke..strokeWidth = 1..color = const Color(0xFF254056);
    for (var i = 1; i <= 3; i++) {
      canvas.drawCircle(c, radius * i / 3, grid);
    }
    canvas.drawLine(Offset(c.dx - radius, c.dy), Offset(c.dx + radius, c.dy), grid); canvas.drawLine(c, Offset(c.dx, c.dy - radius), grid); canvas.drawArc(Rect.fromCircle(center: c, radius: radius), math.pi, math.pi, false, grid);
    final sweep = (angle - 90) * math.pi / 180; final target = c + Offset(math.cos(sweep), math.sin(sweep)) * radius * math.min(distance / 100, 1);
    canvas.drawLine(c, c + Offset(math.cos(sweep), math.sin(sweep)) * radius, Paint()..color = const Color(0xFF46E4E8).withValues(alpha: .16)..strokeWidth = radius * .38); canvas.drawCircle(target, 5, Paint()..color = const Color(0xFFFFB547)); canvas.drawCircle(c, 3, Paint()..color = const Color(0xFF46E4E8));
  }
  @override bool shouldRepaint(covariant _RadarPainter oldDelegate) => oldDelegate.angle != angle || oldDelegate.distance != distance;
}

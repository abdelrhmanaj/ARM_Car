import 'package:flutter/material.dart';

class StatusPill extends StatelessWidget {
  const StatusPill({super.key, required this.connected});
  final bool connected;
  @override
  Widget build(BuildContext context) {
    final color = connected ? const Color(0xFF46E4E8) : const Color(0xFFFF667D);
    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 8),
      decoration: BoxDecoration(color: color.withValues(alpha: .10), borderRadius: BorderRadius.circular(30), border: Border.all(color: color.withValues(alpha: .28))),
      child: Row(mainAxisSize: MainAxisSize.min, children: [
        Container(width: 7, height: 7, decoration: BoxDecoration(color: color, shape: BoxShape.circle)), const SizedBox(width: 7),
        Text(connected ? 'CONNECTED' : 'DISCONNECTED', style: TextStyle(color: color, fontSize: 10, fontWeight: FontWeight.w800, letterSpacing: 1)),
      ]),
    );
  }
}

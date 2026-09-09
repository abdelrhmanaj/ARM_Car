import 'package:flutter/material.dart';

class DashboardCard extends StatelessWidget {
  const DashboardCard({super.key, required this.child, this.padding = const EdgeInsets.all(16)});
  final Widget child; final EdgeInsets padding;
  @override Widget build(BuildContext context) => Container(
    padding: padding,
    decoration: BoxDecoration(color: const Color(0xFF111A2B), borderRadius: BorderRadius.circular(20), border: Border.all(color: const Color(0xFF1D2B42))),
    child: child,
  );
}

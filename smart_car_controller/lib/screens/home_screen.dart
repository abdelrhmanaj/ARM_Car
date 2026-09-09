import 'package:flutter/material.dart';
import 'control_screen.dart';
import 'settings_screen.dart';

class HomeScreen extends StatefulWidget {
  const HomeScreen({super.key});
  @override State<HomeScreen> createState() => _HomeScreenState();
}
class _HomeScreenState extends State<HomeScreen> {
  int index = 0;
  final pages = const [ControlScreen(), SettingsScreen()];
  @override Widget build(BuildContext context) => Scaffold(
    body: SafeArea(child: IndexedStack(index: index, children: pages)),
    bottomNavigationBar: NavigationBar(
      selectedIndex: index, onDestinationSelected: (value) => setState(() => index = value),
      backgroundColor: const Color(0xFF0D1524), indicatorColor: const Color(0xFF183B4B),
      destinations: const [
        NavigationDestination(icon: Icon(Icons.gamepad_outlined), selectedIcon: Icon(Icons.gamepad), label: 'Control'),
        NavigationDestination(icon: Icon(Icons.tune_outlined), selectedIcon: Icon(Icons.tune), label: 'Settings'),
      ],
    ),
  );
}

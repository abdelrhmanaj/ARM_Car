import 'package:flutter/material.dart';
import '../services/car_service.dart';
import '../widgets/dashboard_card.dart';
import '../widgets/section_title.dart';
import '../widgets/status_pill.dart';

class SettingsScreen extends StatefulWidget {
  const SettingsScreen({super.key});
  @override State<SettingsScreen> createState() => _SettingsScreenState();
}
class _SettingsScreenState extends State<SettingsScreen> {
  final service = CarService();
  final ipController = TextEditingController(text: '192.168.4.1');
  bool connecting = false;
  bool connected = true;
  @override void dispose() { ipController.dispose(); super.dispose(); }
  Future<void> connect() async {
    setState(() => connecting = true); final result = await service.testConnection(ipController.text.trim());
    if (!mounted) return;
    if (result) service.baseUrl = 'http://${ipController.text.trim()}';
    setState(() { connecting = false; connected = result; });
    ScaffoldMessenger.of(context).showSnackBar(SnackBar(content: Text(result ? 'Connection test successful' : 'Could not reach the ESP32')));
  }
  @override Widget build(BuildContext context) => ListView(padding: const EdgeInsets.fromLTRB(20, 18, 20, 28), children: [
    Row(mainAxisAlignment: MainAxisAlignment.spaceBetween, children: [Column(crossAxisAlignment: CrossAxisAlignment.start, children: [Text('SYSTEM CONFIG', style: TextStyle(color: Theme.of(context).colorScheme.primary, fontSize: 10, fontWeight: FontWeight.w800, letterSpacing: 2)), const SizedBox(height: 6), const Text('Settings', style: TextStyle(fontSize: 23, fontWeight: FontWeight.w800))]), const Icon(Icons.settings_outlined, color: Color(0xFF637994))]),
    const SizedBox(height: 28), const SectionTitle('Wi-Fi connection'), const SizedBox(height: 12),
    DashboardCard(child: Column(crossAxisAlignment: CrossAxisAlignment.start, children: [
      const Text('NETWORK TARGET', style: TextStyle(color: Color(0xFF7E91AE), fontSize: 10, fontWeight: FontWeight.bold, letterSpacing: 1)), const SizedBox(height: 10),
      TextField(controller: ipController, keyboardType: TextInputType.number, decoration: const InputDecoration(prefixIcon: Icon(Icons.router_outlined), labelText: 'ESP32 IP address')),
      const SizedBox(height: 14), SizedBox(width: double.infinity, height: 48, child: FilledButton.icon(onPressed: connecting ? null : connect, icon: connecting ? const SizedBox(width: 16, height: 16, child: CircularProgressIndicator(strokeWidth: 2)) : const Icon(Icons.wifi_tethering), label: Text(connecting ? 'TESTING...' : 'TEST CONNECTION'))),
    ])),
    const SizedBox(height: 25), const SectionTitle('Communication status'), const SizedBox(height: 12),
    DashboardCard(child: Column(children: [Row(children: [Container(width: 42, height: 42, decoration: BoxDecoration(color: const Color(0xFF46E4E8).withValues(alpha: .1), borderRadius: BorderRadius.circular(12)), child: const Icon(Icons.wifi_rounded, color: Color(0xFF46E4E8))), const SizedBox(width: 13), const Expanded(child: Column(crossAxisAlignment: CrossAxisAlignment.start, children: [Text('ESP32 controller', style: TextStyle(fontWeight: FontWeight.w700)), SizedBox(height: 4), Text('Wi-Fi transport layer ready', style: TextStyle(color: Color(0xFF7E91AE), fontSize: 11))])), StatusPill(connected: connected)]), const Divider(height: 28, color: Color(0xFF24334B)), const Row(mainAxisAlignment: MainAxisAlignment.spaceBetween, children: [Text('Protocol', style: TextStyle(color: Color(0xFF7E91AE), fontSize: 12)), Text('HTTP / JSON', style: TextStyle(fontSize: 12, fontWeight: FontWeight.bold))])])),
    const SizedBox(height: 25), const SectionTitle('About'), const SizedBox(height: 12),
    DashboardCard(child: const Column(crossAxisAlignment: CrossAxisAlignment.start, children: [Text('SMART CAR CONTROLLER', style: TextStyle(color: Color(0xFF46E4E8), fontWeight: FontWeight.w800, letterSpacing: 1.2)), SizedBox(height: 9), Text('A focused control interface for your Wi-Fi enabled 4WD robot car.', style: TextStyle(color: Color(0xFF9BAAC0), fontSize: 12, height: 1.5)), SizedBox(height: 17), Row(mainAxisAlignment: MainAxisAlignment.spaceBetween, children: [Text('App version', style: TextStyle(color: Color(0xFF7E91AE), fontSize: 12)), Text('1.0.0', style: TextStyle(fontWeight: FontWeight.bold, fontSize: 12))])])),
  ]);
}

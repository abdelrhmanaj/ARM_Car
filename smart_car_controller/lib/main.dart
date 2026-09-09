import 'package:flutter/material.dart';
import 'screens/home_screen.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized();
  runApp(const SmartCarApp());
}

class SmartCarApp extends StatelessWidget {
  const SmartCarApp({super.key});
  @override
  Widget build(BuildContext context) {
    const background = Color(0xFF080D18);
    const surface = Color(0xFF111A2B);
    const cyan = Color(0xFF46E4E8);
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'Smart Car Controller',
      theme: ThemeData(
        useMaterial3: true,
        brightness: Brightness.dark,
        scaffoldBackgroundColor: background,
        colorScheme: ColorScheme.fromSeed(seedColor: cyan, brightness: Brightness.dark, surface: surface).copyWith(primary: cyan, secondary: const Color(0xFFFFB547)),
        fontFamily: 'Arial',
        appBarTheme: const AppBarTheme(backgroundColor: background, surfaceTintColor: Colors.transparent, elevation: 0),
        inputDecorationTheme: const InputDecorationTheme(
          filled: true, fillColor: surface,
          border: OutlineInputBorder(borderRadius: BorderRadius.all(Radius.circular(14)), borderSide: BorderSide.none),
          enabledBorder: OutlineInputBorder(borderRadius: BorderRadius.all(Radius.circular(14)), borderSide: BorderSide(color: Color(0xFF24334B))),
          focusedBorder: OutlineInputBorder(borderRadius: BorderRadius.all(Radius.circular(14)), borderSide: BorderSide(color: cyan)),
        ),
      ),
      home: const HomeScreen(),
    );
  }
}

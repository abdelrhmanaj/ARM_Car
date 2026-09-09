import 'package:flutter/material.dart';

class SectionTitle extends StatelessWidget {
  const SectionTitle(this.title, {super.key, this.trailing});
  final String title;
  final Widget? trailing;
  @override
  Widget build(BuildContext context) => Row(mainAxisAlignment: MainAxisAlignment.spaceBetween, children: [
    Text(title.toUpperCase(), style: const TextStyle(color: Color(0xFF7E91AE), fontSize: 11, fontWeight: FontWeight.w800, letterSpacing: 1.5)),
    ?trailing,
  ]);
}

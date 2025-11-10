import 'package:flutter/material.dart';

class SettingsScreen extends StatelessWidget {
  const SettingsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Settings'),
        backgroundColor: const Color(0xFF1E1A3B),
      ),
      body: ListView(
        children: [
          SwitchListTile(
            title: const Text('Notification reminders'),
            value: true,
            onChanged: (_) {},
          ),
          SwitchListTile(
            title: const Text('Email Updates'),
            value: true,
            onChanged: (_) {},
          ),
        ],
      ),
    );
  }
}

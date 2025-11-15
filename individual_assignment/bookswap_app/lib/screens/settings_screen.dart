import 'package:flutter/material.dart';
import 'package:firebase_auth/firebase_auth.dart';

class SettingsScreen extends StatelessWidget {
  const SettingsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final user = FirebaseAuth.instance.currentUser;

    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          const Icon(Icons.person, size: 80, color: Colors.white),
          const SizedBox(height: 20),
          Text(
            user?.email ?? "Unknown User",
            style: const TextStyle(color: Colors.white, fontSize: 18),
          ),
          const SizedBox(height: 20),
          ElevatedButton(
            style: ElevatedButton.styleFrom(backgroundColor: Colors.redAccent),
            onPressed: () {
              FirebaseAuth.instance.signOut();
            },
            child: const Text("Logout"),
          ),
        ],
      ),
    );
  }
}




// import 'package:flutter/material.dart';

// class SettingsScreen extends StatelessWidget {
//   const SettingsScreen({super.key});

//   @override
//   Widget build(BuildContext context) {
//     // return Scaffold(
//     //   appBar: AppBar(
//     //     title: const Text('Settings'),
//     //     backgroundColor: const Color(0xFF1E1A3B),
//     //   ),
//       body: ListView(
//         children: [
//           SwitchListTile(
//             title: const Text('Notification reminders'),
//             value: true,
//             onChanged: (_) {},
//           ),
//           SwitchListTile(
//             title: const Text('Email Updates'),
//             value: true,
//             onChanged: (_) {},
//           ),
//         ],
//       ),
//     );
//   }
// }

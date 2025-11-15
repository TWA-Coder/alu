// import 'package:cloud_firestore/cloud_firestore.dart';
// import 'package:firebase_auth/firebase_auth.dart';
// import 'package:flutter/material.dart';
// import 'chat_screen.dart'; 

// class ChatsListScreen extends StatelessWidget {
//   const ChatsListScreen({super.key});

//   @override
//   Widget build(BuildContext context) {
//     final currentUserId = FirebaseAuth.instance.currentUser!.uid;

//     return Scaffold(
//       backgroundColor: const Color(0xFF1E1A3B),
//       appBar: AppBar(
//         backgroundColor: const Color(0xFF1E1A3B),
//         title: const Text("Chats"),
//       ),

//       // --------------- REALTIME FIRESTORE STREAM ----------------
//       body: StreamBuilder<QuerySnapshot>(
//         stream: FirebaseFirestore.instance
//             .collection('chats')
//             .where('user1', isEqualTo: currentUserId)
//             .snapshots(),

//         builder: (context, snapshot) {
//           if (!snapshot.hasData) {
//             return const Center(child: CircularProgressIndicator());
//           }

//           final chats = snapshot.data!.docs;

//           if (chats.isEmpty) {
//             return const Center(
//               child: Text(
//                 "No chats yet",
//                 style: TextStyle(color: Colors.white70, fontSize: 18),
//               ),
//             );
//           }

//           return ListView.builder(
//             padding: const EdgeInsets.all(16),
//             itemCount: chats.length,
//             itemBuilder: (context, index) {
//               final chat = chats[index];
//               final chatId = chat.id;

//               final otherUserId = chat['user1'] == currentUserId
//                   ? chat['user2']
//                   : chat['user1'];

//               final lastMessage = chat['lastMessage'] ?? "";

//               return FutureBuilder<DocumentSnapshot>(
//                 future: FirebaseFirestore.instance
//                     .collection("users")
//                     .doc(otherUserId)
//                     .get(),

//                 builder: (context, userSnap) {
//                   if (!userSnap.hasData) {
//                     return const ListTile(
//                       title: Text("Loading...", style: TextStyle(color: Colors.white)),
//                     );
//                   }

//                   final userData = userSnap.data!;
//                   final username = userData['name'] ?? "User";
//                   final avatarUrl = userData['profileImage'];

//                   return ListTile(
//                     contentPadding: const EdgeInsets.symmetric(vertical: 8),
//                     leading: CircleAvatar(
//                       backgroundColor: Colors.white12,
//                       backgroundImage:
//                           avatarUrl != null ? NetworkImage(avatarUrl) : null,
//                       child: avatarUrl == null
//                           ? const Icon(Icons.person, color: Colors.white)
//                           : null,
//                     ),
//                     title: Text(
//                       username,
//                       style: const TextStyle(color: Colors.white),
//                     ),
//                     subtitle: Text(
//                       lastMessage,
//                       style: const TextStyle(color: Colors.white70),
//                       maxLines: 1,
//                       overflow: TextOverflow.ellipsis,
//                     ),
//                     trailing: const Icon(Icons.chevron_right, color: Colors.white70),

//                     onTap: () {
//                       Navigator.push(
//                         context,
//                         MaterialPageRoute(
//                           builder: (_) => chat_screen(chatId: chatId),
//                         ),
//                       );
//                     },
//                   );
//                 },
//               );
//             },
//           );
//         },
//       ),
//     );
//   }
// }





import 'package:flutter/material.dart';

class ChatsListScreen extends StatelessWidget {
  const ChatsListScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Chats'),
        backgroundColor: const Color(0xFF1E1A3B),
      ),
      body: ListView(
        padding: const EdgeInsets.all(16),
        children: [
          ListTile(
            leading: const CircleAvatar(child: Icon(Icons.person)),
            title: const Text('Alice'),
            subtitle: const Text('Hi, are you interested in finding...'),
            onTap: () {},
          ),
        ],
      ),
    );
  }
}

import 'package:flutter/material.dart';
import 'post_book_screen.dart'; 

class MyListingsScreen extends StatelessWidget {
  const MyListingsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('My Listings'),
        backgroundColor: const Color(0xFF1E1A3B),
      ),
      body: const Center(
        child: Text('Your posted books will appear here'),
      ),
      floatingActionButton: FloatingActionButton(
        backgroundColor: const Color(0xFFFFC107),
        onPressed: () {
          Navigator.push(
            context,
            MaterialPageRoute(builder: (_) => const PostBookScreen()),
          );
        },
        child: const Icon(Icons.add),
      ),
    );
  }
}


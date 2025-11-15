import 'package:flutter/material.dart';
import 'package:firebase_auth/firebase_auth.dart';
import '../models/book.dart';
import '../services/book_service.dart';
import '../models/swap.dart';
import '../services/swap_service.dart';

class BrowseListingsScreen extends StatelessWidget {
  const BrowseListingsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final bookService = BookService();
    final currentUser = FirebaseAuth.instance.currentUser!;

    return Scaffold(
      appBar: AppBar(
        title: const Text('Browse Listings'),
        backgroundColor: const Color(0xFF1E1A3B),
      ),
      body: StreamBuilder<List<Book>>(
        stream: bookService.getBooks(),
        builder: (context, snapshot) {
          if (snapshot.connectionState == ConnectionState.waiting) {
            return const Center(child: CircularProgressIndicator());
          }
          if (!snapshot.hasData || snapshot.data!.isEmpty) {
            return const Center(child: Text('No books posted yet.'));
          }

          final books = snapshot.data!;
          return ListView.builder(
            padding: const EdgeInsets.all(16),
            itemCount: books.length,
            itemBuilder: (context, index) {
              final book = books[index];
              final isOwner = currentUser.uid == book.userId;

              return Card(
                shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(16)),
                elevation: 4,
                margin: const EdgeInsets.only(bottom: 16),
                child: ListTile(
                  leading:
                      const Icon(Icons.menu_book, color: Color(0xFF1E1A3B)),
                  title: Text(book.title,
                      style: const TextStyle(fontWeight: FontWeight.bold)),
                  subtitle:
                      Text('By ${book.author}\n${book.condition} • ${book.swapFor}'),
                  trailing: !isOwner
                      ? ElevatedButton(
                          child: const Text('Swap'),
                          onPressed: () async {
                            final swap = Swap(
                              id: '', // Firestore auto ID
                              bookId: book.id,
                              fromUserId: currentUser.uid,
                              toUserId: book.userId,
                              status: SwapStatus.pending,
                            );

                            await SwapService().createSwap(swap);

                            ScaffoldMessenger.of(context).showSnackBar(
                              const SnackBar(
                                  content: Text('Swap offer sent!')),
                            );
                          },
                        )
                      : null,
                ),
              );
            },
          );
        },
      ),
    );
  }
}


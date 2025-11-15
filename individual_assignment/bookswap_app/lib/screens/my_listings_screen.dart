import 'package:flutter/material.dart';
import '../models/book.dart';
import '../services/book_service.dart';
import 'post_book_screen.dart'; 

class MyListingsScreen extends StatelessWidget {
  const MyListingsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final bookService = BookService();

    return Scaffold(
      appBar: AppBar(
        title: const Text('My Listings'),
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
              return Card(
                shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(16)),
                elevation: 4,
                margin: const EdgeInsets.only(bottom: 16),
                child: ListTile(
                  leading:
                      const Icon(Icons.menu_book, color: Color(0xFF1E1A3B)),
                  title: Text(book.title,
                      style:
                          const TextStyle(fontWeight: FontWeight.bold)),
                  subtitle: Text(
                      'By ${book.author}\n${book.condition} • ${book.swapFor}'),
                ),
              );
            },
          );
        },
      ),
      floatingActionButton: FloatingActionButton(
        backgroundColor: const Color(0xFF1E1A3B),
        child: const Icon(Icons.add, color: Colors.white),
        onPressed: () {
          Navigator.push(
            context,
            MaterialPageRoute(builder: (_) => const PostBookScreen()),
          );
        },
      ),
    );
  }
}







// import 'package:flutter/material.dart';
// import 'post_book_screen.dart'; 

// class MyListingsScreen extends StatelessWidget {
//   const MyListingsScreen({super.key});

//   @override
//   Widget build(BuildContext context) {
//     // return Scaffold(
//     //   appBar: AppBar(
//     //     title: const Text('My Listings'),
//     //     backgroundColor: const Color(0xFF1E1A3B),
//     //   ),
//       body: const Center(
//         child: Text('Your posted books will appear here'),
//       ),
//       floatingActionButton: FloatingActionButton(
//         backgroundColor: const Color(0xFFFFC107),
//         onPressed: () {
//           Navigator.push(
//             context,
//             MaterialPageRoute(builder: (_) => const PostBookScreen()),
//           );
//         },
//         child: const Icon(Icons.add),
//       ),
//   }
// }


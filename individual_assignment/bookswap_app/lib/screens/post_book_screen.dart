import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import '../models/book.dart';
import '../services/book_service.dart';

class PostBookScreen extends StatefulWidget {
  const PostBookScreen({super.key});

  @override
  State<PostBookScreen> createState() => _PostBookScreenState();
}

class _PostBookScreenState extends State<PostBookScreen> {
  final _formKey = GlobalKey<FormState>();
  final _titleController = TextEditingController();
  final _authorController = TextEditingController();
  final _swapForController = TextEditingController();
  String _condition = 'New';
  final BookService _bookService = BookService();

  bool _isLoading = false;

  void _postBook() async {
    if (!_formKey.currentState!.validate()) return;

    setState(() => _isLoading = true);

    final book = Book(
      id: '',
      title: _titleController.text.trim(),
      author: _authorController.text.trim(),
      condition: _condition,
      swapFor: _swapForController.text.trim(),
      userId: 'demo_user', // TODO: replace with FirebaseAuth user.uid
      postedAt: DateTime.now(),
    );

    try {
      await _bookService.addBook(book);
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('Book posted successfully!')),
      );
      Navigator.pop(context);
    } catch (e) {
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text('Error: $e')),
      );
    }

    setState(() => _isLoading = false);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Post a Book'),
        backgroundColor: const Color(0xFF1E1A3B),
      ),
      body: Padding(
        padding: const EdgeInsets.all(20),
        child: Form(
          key: _formKey,
          child: Column(
            children: [
              TextFormField(
                controller: _titleController,
                decoration: const InputDecoration(labelText: 'Book Title'),
                validator: (v) => v!.isEmpty ? 'Enter a book title' : null,
              ),
              const SizedBox(height: 12),
              TextFormField(
                controller: _authorController,
                decoration: const InputDecoration(labelText: 'Author'),
                validator: (v) => v!.isEmpty ? 'Enter the author name' : null,
              ),
              const SizedBox(height: 12),
              TextFormField(
                controller: _swapForController,
                decoration: const InputDecoration(labelText: 'Swap For'),
              ),
              const SizedBox(height: 20),
              const Align(
                alignment: Alignment.centerLeft,
                child: Text('Condition:', style: TextStyle(fontSize: 16, fontWeight: FontWeight.w500)),
              ),
              Wrap(
                spacing: 10,
                children: ['New', 'Like New', 'Good', 'Used']
                    .map((cond) => ChoiceChip(
                          label: Text(cond),
                          selected: _condition == cond,
                          onSelected: (_) => setState(() => _condition = cond),
                          selectedColor: const Color(0xFFFFC107),
                        ))
                    .toList(),
              ),
              const SizedBox(height: 30),
              ElevatedButton(
                onPressed: _isLoading ? null : _postBook,
                style: ElevatedButton.styleFrom(
                  backgroundColor: const Color(0xFFFFC107),
                  minimumSize: const Size(double.infinity, 48),
                  shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
                ),
                child: _isLoading
                    ? const CircularProgressIndicator(color: Colors.black)
                    : const Text('Post', style: TextStyle(color: Colors.black, fontSize: 18)),
              ),
            ],
          ),
        ),
      ),
    );
  }
}

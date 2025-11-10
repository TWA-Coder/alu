import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'firebase_options.dart';
import 'services/book_service.dart';
//import 'services/auth_service.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'BookSwap',
      home: TestAuth(), // change to your first screen
    );
  }
}

class TestAuth extends StatefulWidget {
  const TestAuth({super.key});

  @override
  State<TestAuth> createState() => _TestAuthState();
}
class _TestAuthState extends State<TestAuth> {
  //final AuthService _authService = AuthService();

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("Test Auth")),
      body: Center(child: Text("Welcome to BookSwap")),
    );
  }
}
class BrowseListings extends StatelessWidget {
  final bookService = BookService();

  @override
  Widget build(BuildContext context) {
    return StreamBuilder(
      stream: bookService.browseListings(),
      builder: (context, snapshot) {
        if (!snapshot.hasData) return const Center(child: CircularProgressIndicator());
        final docs = snapshot.data!.docs;
        return ListView.builder(
          itemCount: docs.length,
          itemBuilder: (context, i) {
            final d = docs[i].data() as Map<String, dynamic>;
            return ListTile(
              title: Text(d['title']),
              subtitle: Text(d['author']),
              trailing: Text(d['condition']),
            );
          },
        );
      },
    );
  }
}


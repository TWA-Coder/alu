import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'firebase_options.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );
  runApp(const MyApp());
}

class TestAuth extends StatefulWidget {
  const TestAuth({super.key});
  @override
  State<TestAuth> createState() => _TestAuthState();
}

class _TestAuthState extends State<TestAuth> {
  final auth = AuthService();
  final emailController = TextEditingController();
  final passController = TextEditingController();
  final nameController = TextEditingController();

  String message = '';

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Auth Test')),
      body: Padding(
        padding: const EdgeInsets.all(20),
        child: Column(
          children: [
            TextField(controller: nameController, decoration: const InputDecoration(labelText: 'Name')),
            TextField(controller: emailController, decoration: const InputDecoration(labelText: 'Email')),
            TextField(controller: passController, decoration: const InputDecoration(labelText: 'Password'), obscureText: true),
            const SizedBox(height: 10),
            ElevatedButton(
              onPressed: () async {
                try {
                  await auth.signUp(emailController.text, passController.text, nameController.text);
                  setState(() => message = "User created! Check your email for verification.");
                } catch (e) {
                  setState(() => message = e.toString());
                }
              },
              child: const Text('Sign Up'),
            ),
            ElevatedButton(
              onPressed: () async {
                try {
                  await auth.login(emailController.text, passController.text);
                  setState(() => message = "Logged in!");
                } catch (e) {
                  setState(() => message = e.toString());
                }
              },
              child: const Text('Login'),
            ),
            Text(message),
          ],
        ),
      ),
    );
  }
}

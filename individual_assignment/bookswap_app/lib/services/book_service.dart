import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:firebase_auth/firebase_auth.dart';


class BookService {
  final CollectionReference booksCollection =
      FirebaseFirestore.instance.collection('books');

  Stream<QuerySnapshot> getBooks() {
    return booksCollection.snapshots();
  }

  Future<void> addBook(Map<String, dynamic> bookData) async {
    await booksCollection.add(bookData);
  }
}


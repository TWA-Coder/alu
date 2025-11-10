import 'package:cloud_firestore/cloud_firestore.dart';
import '../models/book.dart';

class BookService {
  final CollectionReference booksRef =
      FirebaseFirestore.instance.collection('books');

  Future<void> addBook(Book book) async {
    await booksRef.add(book.toMap());
  }

  Stream<List<Book>> getBooks() {
    return booksRef.orderBy('postedAt', descending: true).snapshots().map(
      (snapshot) {
        return snapshot.docs
            .map((doc) => Book.fromMap(doc.id, doc.data() as Map<String, dynamic>))
            .toList();
      },
    );
  }
}



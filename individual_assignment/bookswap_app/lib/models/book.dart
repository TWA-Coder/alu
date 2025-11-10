class Book {
  final String id;
  final String title;
  final String author;
  final String condition;
  final String swapFor;
  final String userId;
  final DateTime postedAt;

  Book({
    required this.id,
    required this.title,
    required this.author,
    required this.condition,
    required this.swapFor,
    required this.userId,
    required this.postedAt,
  });

  Map<String, dynamic> toMap() {
    return {
      'title': title,
      'author': author,
      'condition': condition,
      'swapFor': swapFor,
      'userId': userId,
      'postedAt': postedAt.toIso8601String(),
    };
  }

  factory Book.fromMap(String id, Map<String, dynamic> data) {
    return Book(
      id: id,
      title: data['title'] ?? '',
      author: data['author'] ?? '',
      condition: data['condition'] ?? '',
      swapFor: data['swapFor'] ?? '',
      userId: data['userId'] ?? '',
      postedAt: DateTime.tryParse(data['postedAt'] ?? '') ?? DateTime.now(),
    );
  }
}

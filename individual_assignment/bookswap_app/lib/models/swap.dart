enum SwapStatus { pending, accepted, rejected }

class Swap {
  final String id;
  final String bookId;
  final String fromUserId; // User making the offer
  final String toUserId;   // Owner of the book
  final SwapStatus status;

  Swap({
    required this.id,
    required this.bookId,
    required this.fromUserId,
    required this.toUserId,
    this.status = SwapStatus.pending,
  });

  Map<String, dynamic> toMap() => {
        'bookId': bookId,
        'fromUserId': fromUserId,
        'toUserId': toUserId,
        'status': status.toString().split('.').last,
      };

  factory Swap.fromMap(String id, Map<String, dynamic> map) {
    return Swap(
      id: id,
      bookId: map['bookId'],
      fromUserId: map['fromUserId'],
      toUserId: map['toUserId'],
      status: SwapStatus.values.firstWhere(
          (e) => e.toString().split('.').last == map['status']),
    );
  }
}

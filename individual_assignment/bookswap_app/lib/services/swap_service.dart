import 'package:cloud_firestore/cloud_firestore.dart';
import '../models/swap.dart';

class SwapService {
  final swapsRef = FirebaseFirestore.instance.collection('swaps');

  Stream<List<Swap>> getSwapsForUser(String userId) {
    return swapsRef
        .where('toUserId', isEqualTo: userId)
        .snapshots()
        .map((snap) =>
            snap.docs.map((doc) => Swap.fromMap(doc.id, doc.data())).toList());
  }

  Future<void> createSwap(Swap swap) async {
    await swapsRef.add(swap.toMap());
  }

  Future<void> updateSwapStatus(String swapId, SwapStatus status) async {
    await swapsRef.doc(swapId).update({'status': status.toString().split('.').last});
  }
}

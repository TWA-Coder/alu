import sys

try:
    import vibration
except ImportError:
    print("Error: The 'vibration' C extension module could not be found.")
    print("Please build it first using: python setup.py build_ext --inplace")
    sys.exit(1)

def test_happy_path_list():
    print("--- Test: Happy Path (List) ---")
    data = [1.2, 2.4, 3.6, 4.8, 6.0, 7.2]
    
    print(f"Data: {data}")
    print(f"Peak to peak: {vibration.peak_to_peak(data):.4f} (Expected: 6.0)")
    print(f"RMS: {vibration.rms(data):.4f}")
    print(f"Std Dev (Sample): {vibration.std_dev(data):.4f}")
    print(f"Above threshold (4.0): {vibration.above_threshold(data, 4.0)}")
    
    summary = vibration.summary(data)
    print(f"Summary Dictionary: {summary}")
    print()

def test_happy_path_tuple():
    print("--- Test: Happy Path (Tuple) ---")
    data = (10.0, 15.0, -5.0, 0.0)
    
    print(f"Data: {data}")
    print(f"Peak to peak: {vibration.peak_to_peak(data):.4f} (Expected: 20.0)")
    print(f"Std Dev (Sample): {vibration.std_dev(data):.4f}")
    print()

def test_edge_cases():
    print("--- Test: Edge Cases & Validation ---")
    
    # 1. Empty List
    empty = []
    print("Testing Empty List:")
    try:
        vibration.rms(empty)
    except ValueError as e:
        print(f"  [SUCCESS] Caught ValueError: {e}")

    # 2. Too few elements for Std Dev
    single_item = [5.5]
    print("Testing Std Dev on single element:")
    try:
        vibration.std_dev(single_item)
    except ValueError as e:
        print(f"  [SUCCESS] Caught ValueError: {e}")

    # 3. Invalid types (Strings instead of Floats)
    invalid_data = [1.1, 2.2, "3.3", 4.4]
    print("Testing Invalid Inner Types (Strings):")
    try:
        vibration.summary(invalid_data)
    except TypeError as e:
        print(f"  [SUCCESS] Caught TypeError: {e}")

    # 4. Invalid sequence type (Set or Dict instead of List/Tuple)
    invalid_seq = {1.1, 2.2, 3.3}
    print("Testing Invalid Sequence (Set):")
    try:
        vibration.peak_to_peak(invalid_seq)
    except TypeError as e:
        print(f"  [SUCCESS] Caught TypeError: {e}")

if __name__ == "__main__":
    print("========================================")
    print(" VIBRATION ANALYTICS C EXTENSION TESTER ")
    print("========================================\n")
    
    test_happy_path_list()
    test_happy_path_tuple()
    test_edge_cases()

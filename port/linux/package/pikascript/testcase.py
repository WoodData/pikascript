import _testcase


def test_tuple_nested():
    student_records = ((12345, "Alice"), (67890, "Bob"))
    _testcase.test_tuple_nested(student_records)


def test_class_init():
    t = _testcase.test()
    print(t.environ)

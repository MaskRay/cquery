union Foo {
  int a : 5;
  bool b : 3;
};

Foo f;

void act(Foo*) {
  f.a = 3;
}

/*
// TODO: instantiations on Foo should include parameter?

OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 8501689086387244262,
      "short_name": "Foo",
      "detailed_name": "Foo",
      "kind": 10,
      "definition_spelling": "1:7-1:10",
      "definition_extent": "1:1-4:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0, 1],
      "instances": [2],
      "uses": ["1:7-1:10", "6:1-6:4", "8:10-8:13"]
    }],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 13982179977217945200,
      "short_name": "act",
      "detailed_name": "void act(Foo *)",
      "kind": 12,
      "declarations": [],
      "definition_spelling": "8:6-8:9",
      "definition_extent": "8:1-10:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 9529311430721959843,
      "short_name": "a",
      "detailed_name": "int Foo::a",
      "definition_spelling": "2:7-2:8",
      "definition_extent": "2:3-2:12",
      "declaring_type": 0,
      "kind": 14,
      "uses": ["2:7-2:8", "9:5-9:6"]
    }, {
      "id": 1,
      "usr": 8804696910588009104,
      "short_name": "b",
      "detailed_name": "bool Foo::b",
      "definition_spelling": "3:8-3:9",
      "definition_extent": "3:3-3:13",
      "declaring_type": 0,
      "kind": 14,
      "uses": ["3:8-3:9"]
    }, {
      "id": 2,
      "usr": 2933643612409209903,
      "short_name": "f",
      "detailed_name": "Foo f",
      "definition_spelling": "6:5-6:6",
      "definition_extent": "6:1-6:6",
      "variable_type": 0,
      "kind": 13,
      "uses": ["6:5-6:6", "9:3-9:4"]
    }]
}
*/

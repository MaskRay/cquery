class Foo {
  void foo() {}
};

/*
OUTPUT:
{
  "types": [{
      "id": 0,
      "short_name": "Foo",
      "qualified_name": "Foo",
      "declaration": null,
      "definition": "tests/method_inline_declaration.cc:1:7",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "uses": []
    }],
  "functions": [{
      "id": 0,
      "short_name": "foo",
      "qualified_name": "Foo::foo",
      "declaration": null,
      "definition": "tests/method_inline_declaration.cc:2:8",
      "declaring_type": 0,
      "base": null,
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": [],
      "uses": []
    }],
  "variables": []
}
*/
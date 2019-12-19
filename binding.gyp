{
  "targets": [
    {
      "target_name": "AbortInspector",
      "sources": [
        "src/trap.cc"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "include"
      ],
       "libraries": [ "-lunwind -lunwind-generic", "-L'<!(pwd)/lib'", "-Wl,-rpath,<!(pwd)/lib"]
    }
  ]
}
